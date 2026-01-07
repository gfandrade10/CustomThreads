#pragma once

//C++ libs
#include <chrono>
#include <random>
#include <type_traits>

//fmt 3rs party
#include <fmt/base.h>

namespace TimeChecker
{
    extern std::random_device rd_s;
    extern uint64_t seed_s;

    template<typename Callable>
    fmt::enable_if_t<std::is_invocable_v<Callable, size_t>, float>
    ElapsedTime(Callable&& func, size_t NofArgs)
    {
        auto Start = std::chrono::steady_clock::now();
        func(NofArgs);
        auto End = std::chrono::steady_clock::now();
        
        long Duration = std::chrono::duration_cast<std::chrono::microseconds>(End-Start).count();
        return Duration/1000.0f;
    }

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

    void FillArrayUnoptimized(const size_t& N);
    void FillArrayOptimized(const size_t& N);
    void FillArrayCXXThread(const size_t& N);    
};
