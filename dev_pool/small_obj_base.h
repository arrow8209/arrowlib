#pragma once
#include <assert.h>
#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <stdint.h>
#include <vector>
#include "chunk.h"
#include "fixed_allocator.h"
#include "small_obj_allocator.h"

namespace Arrow2
{
namespace Other
{

class SmallObjectBase
{

protected:
    inline SmallObjectBase(void) {}
    inline SmallObjectBase(const SmallObjectBase&) {}
    inline SmallObjectBase& operator=(const SmallObjectBase&)
    {
        return *this;
    }
    inline ~SmallObjectBase() {}

public:
        /// Throwing single-object new throws bad_alloc when allocation fails.
#ifdef _MSC_VER
        /// @note MSVC complains about non-empty exception specification lists.
        static void * operator new ( std::size_t size )
#else
        static void * operator new ( std::size_t size ) throw ( std::bad_alloc )
#endif
        {
            return AppSmallObjAllocator::Instance()->Allocate( size, true );
        }

        /// Non-throwing single-object new returns NULL if allocation fails.
        static void * operator new ( std::size_t size, const std::nothrow_t & ) throw ()
        {
            return AppSmallObjAllocator::Instance()->Allocate(size, false);
        }

        /// Placement single-object new merely calls global placement new.
        inline static void * operator new ( std::size_t size, void * place )
        {
            return ::operator new( size, place );
        }

        /// Single-object delete.
        static void operator delete ( void * p, std::size_t size ) throw ()
        {
            AppSmallObjAllocator::Instance()->Deallocate(p, size);
        }

        /** Non-throwing single-object delete is only called when nothrow
         new operator is used, and the constructor throws an exception.
         */
        static void operator delete ( void * p, const std::nothrow_t & ) throw()
        {
            AppSmallObjAllocator::Instance()->Deallocate(p);
        }

        /// Placement single-object delete merely calls global placement delete.
        inline static void operator delete ( void * p, void * place )
        {
            ::operator delete ( p, place );
        }
};

}
}