#include "arrow/arrow.h"

void TestLoadLib()
{
    Arrow::Other::DynamicLib lib;
    lib.LoadLib("./libtestlib1.so");

    lib.call<uint32_t>("Init", "asbd");
}