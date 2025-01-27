
// AI实现的无锁队列

template<typename T>
class LockFreeQueue {
    struct Slot {
        std::atomic<size_t> sequence{0};  // 序列号，用于检测ABA问题
        T data;
    };

    std::atomic<std::vector<std::unique_ptr<Slot[]>>*> buffers_;
    std::atomic<size_t> write_pos_{0};
    std::atomic<size_t> read_pos_{0};
    std::atomic<size_t> capacity_;
    std::atomic<size_t> buffer_size_;
    std::atomic<bool> expanding_{false};

public:
    LockFreeQueue(size_t initial_capacity = 1024)
        : capacity_(initial_capacity), buffer_size_(initial_capacity) {
        auto buffers = new std::vector<std::unique_ptr<Slot[]>>();
        buffers->push_back(std::make_unique<Slot[]>(initial_capacity));
        buffers_.store(buffers, std::memory_order_release);
    }

    bool enqueue(const T& item) {
        while (true) {
            size_t current_write = write_pos_.load(std::memory_order_relaxed);
            size_t current_read = read_pos_.load(std::memory_order_acquire);
            
            if (current_write - current_read >= capacity_.load(std::memory_order_acquire) - 1) {
                if (!expand_capacity()) {
                    std::this_thread::yield();
                    continue;
                }
            }

            // 获取当前buffer size的快照
            size_t current_buffer_size = buffer_size_.load(std::memory_order_acquire);
            size_t buffer_index = (current_write / current_buffer_size) % 
                                 buffers_.load(std::memory_order_acquire)->size();
            size_t slot_index = current_write % current_buffer_size;

            // 检查序列号，防止ABA问题
            Slot& slot = (*buffers_.load(std::memory_order_acquire))[buffer_index][slot_index];
            size_t expected_seq = current_write;
            
            if (slot.sequence.load(std::memory_order_acquire) != expected_seq) {
                continue;  // slot还未准备好，重试
            }

            if (write_pos_.compare_exchange_strong(current_write, current_write + 1,
                                                 std::memory_order_acq_rel)) {
                slot.data = item;
                slot.sequence.store(current_write + 1, std::memory_order_release);
                return true;
            }
        }
    }

    bool dequeue(T& item) {
        while (true) {
            size_t current_read = read_pos_.load(std::memory_order_relaxed);
            size_t current_write = write_pos_.load(std::memory_order_acquire);

            if (current_read >= current_write) {
                return false;  // 队列空
            }

            size_t current_buffer_size = buffer_size_.load(std::memory_order_acquire);
            size_t buffer_index = (current_read / current_buffer_size) % 
                                 buffers_.load(std::memory_order_acquire)->size();
            size_t slot_index = current_read % current_buffer_size;

            Slot& slot = (*buffers_.load(std::memory_order_acquire))[buffer_index][slot_index];
            size_t expected_seq = current_read + 1;
            
            // 等待数据就绪
            while (slot.sequence.load(std::memory_order_acquire) != expected_seq) {
                if (expanding_.load(std::memory_order_acquire)) {
                    std::this_thread::yield();
                    continue;
                }
            }

            if (read_pos_.compare_exchange_strong(current_read, current_read + 1,
                                                std::memory_order_acq_rel)) {
                item = slot.data;
                slot.sequence.store(current_read + capacity_.load(std::memory_order_relaxed),
                                  std::memory_order_release);
                return true;
            }
        }
    }

private:
    bool expand_capacity() {
        bool expected = false;
        if (!expanding_.compare_exchange_strong(expected, true, 
                                              std::memory_order_acquire)) {
            return false;
        }

        size_t old_capacity = capacity_.load(std::memory_order_relaxed);
        size_t new_capacity = old_capacity * 2;
        
        auto new_buffer = std::make_unique<Slot[]>(new_capacity);
        if (!new_buffer) {
            expanding_.store(false, std::memory_order_release);
            return false;
        }

        // 创建新的buffers vector
        auto old_buffers = buffers_.load(std::memory_order_acquire);
        auto new_buffers = new std::vector<std::unique_ptr<Slot[]>>(*old_buffers);
        new_buffers->push_back(std::move(new_buffer));

        // 原子地更新所有相关状态
        buffers_.store(new_buffers, std::memory_order_release);
        capacity_.store(new_capacity, std::memory_order_release);
        buffer_size_.store(new_capacity, std::memory_order_release);

        // 清理旧的buffers
        delete old_buffers;

        expanding_.store(false, std::memory_order_release);
        return true;
    }

    ~LockFreeQueue() {
        delete buffers_.load();
    }
};