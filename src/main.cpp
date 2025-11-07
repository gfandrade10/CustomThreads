//2rd Party Libs
#include <fmt/base.h>
#include <fmt/core.h>

//My Libs
#include "TimeChecker.h"

int main()
{
    using namespace TimeChecker;
    constexpr size_t N = 100'000'000;
    fmt::println("\n-- Benchmark with {} elements --\n", N);

    float TimeUnop = ElapsedTime(FillArrayUnoptimized, N);
    float TimeOpt = ElapsedTime(FillArrayOptimized, N);
    float TimeSTLThread = ElapsedTime(FillArrayCXXThread, N);

    fmt::println("");
    fmt::println("Elapsed Time Unoptimized: {:.3f}ms", TimeUnop);
    fmt::println("Elapsed Time Optimized OpenMP: {:.3f}ms", TimeOpt);
    fmt::println("Elapsed Time Optimized std::thread: {:.3f}ms\n", TimeSTLThread);
}
