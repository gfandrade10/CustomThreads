
//C++ std headers
#include <cstdint>
#include <vector>
#include <random>

//3rd party libs
#include <pcg_random.hpp>
#include <pcg_extras.hpp>
#include <xoshiro-cpp/XoshiroCpp.hpp>
#include <fmt/core.h>
#include <omp.h>

//my libs
#include "pcg.h"
#include "TimeChecker.h"

void FillMultiOMPwPCG32(const size_t& N)
{
    std::vector<int> vec(N);
    uint64_t seed;
    pcg_extras::seed_seq_from<std::random_device> seed_source;
    
    seed_source.generate(&seed, &seed + 1);

    //First OMP Block
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        uint64_t stream = static_cast<uint64_t>(thread_id);
        pcg32 rng(seed, stream);
        
        #pragma omp for schedule(static) nowait
        for(size_t i = 0; i < N; ++i)
            vec[i] = rng() % 101;
        
        #pragma omp for schedule(static)
        for(size_t i = 0; i < N; ++i)
            vec[i] *= vec[i];
    }
    
    pcg32 last_rng (TimeChecker::rd_s());
    int lp = last_rng() % N;

    fmt::println("Element number {}: {}", lp + 1, vec[lp]);
}

void FillMultiOMPwXoshiro(const size_t& N)
{
    std::vector<int> vec(N);
    uint64_t global_seed = std::random_device{}();

    //First OMP Block
    #pragma omp parallel
    {
        XoshiroCpp::SplitMix64 sm(global_seed + omp_get_thread_num());
        const XoshiroCpp::Xoshiro256PlusPlus::state_type state {
            sm(), sm(), sm(), sm()
        };

        XoshiroCpp::Xoshiro256PlusPlus rng(state);
        
        #pragma omp for schedule(static) nowait
        for(size_t i = 0; i < N; ++i)
            vec[i] = rng() % 101;
        
        #pragma omp for schedule(static)
        for(size_t i = 0; i < N; ++i)
            vec[i] *= vec[i];
    }
    
    XoshiroCpp::Xoshiro256PlusPlus last_rng (TimeChecker::rd_s());
    int lp = last_rng() % N;

    fmt::println("Element number {}: {}", lp + 1, vec[lp]);
}
