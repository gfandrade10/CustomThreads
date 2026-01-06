
//C++ std headers
#include <cstddef>
#include <type_traits>

//2rd Party Libs
#include <fmt/base.h>
#include <fmt/core.h>

//My Libs
#include "TimeChecker.h"
#include "pcg.h"

template<typename Callable>
std::enable_if_t<std::is_same_v<std::invoke_result_t<Callable, size_t>, void>, float>
CalcAverage(Callable&& f, size_t num)
{
    constexpr size_t runs = 10;
    float sum = 0.0f;

    for(size_t i = 0; i < runs; ++i)
        sum += TimeChecker::ElapsedTime(f,num);

    return sum/static_cast<float>(runs);
}

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
