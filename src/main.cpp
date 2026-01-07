
//C++ std headers
#include <cstddef>

//2rd Party Libs
#include <fmt/base.h>
#include <fmt/core.h>

//My Libs
#include "TimeChecker.h"
#include "pcg.h"

int main()
{
    using namespace TimeChecker;
    constexpr size_t N = 100'000'000;
    fmt::println("\n-- Thread Benchmark with {} elements --\n", N);

    float TimeUnop = CalcAverage(FillArrayUnoptimized, N);
    float TimeOpt = CalcAverage(FillArrayOptimized, N);
    float TimeSTLThread = CalcAverage(FillArrayCXXThread, N);
    float TimeOMPwPCG = CalcAverage(FillMultiOMPwPCG32, N);
    float TimeOMPwPXoshiro = CalcAverage(FillMultiOMPwXoshiro, N);

    fmt::println("\nElapsed Time Unoptimized: {:.3f}ms", TimeUnop);
    fmt::println("Elapsed Time Optimized OpenMP: {:.3f}ms", TimeOpt);
    fmt::println("Elapsed Time Optimized std::thread: {:.3f}ms", TimeSTLThread);
    fmt::println("Elapsed Time Optimized OpenMP + pcg32: {:.3f}ms", TimeOMPwPCG);
    fmt::println("Elapsed Time Optimized OpenMP + Xoshiro256: {:.3f}ms\n", TimeOMPwPXoshiro);
}
