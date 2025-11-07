//CXX Libs
#include <random>
#include <chrono>
#include <functional>
#include <cstddef>
#include <random>
#include <thread>
#include <vector>

//3rd Party Libs
#include <fmt/base.h>
#include <fmt/core.h>
#include <omp.h>

//My Libs
#include "TimeChecker.h"

namespace TimeChecker
{
    static std::random_device rd_s;

    float ElapsedTime(std::function<void(size_t)> func, size_t NofArgs)
    {
        auto Start = std::chrono::steady_clock::now();
        std::invoke(func, NofArgs);
        auto End = std::chrono::steady_clock::now();
        
        long Duration = std::chrono::duration_cast<std::chrono::microseconds>(End-Start).count();
        return Duration/1000.0f;
    }

    void FillArrayUnoptimized(const size_t& N)
    {
        std::vector<int> vec;
        vec.reserve(N);
        std::mt19937 seed(rd_s());
        std::uniform_int_distribution<int> gen(0,100);
        std::uniform_int_distribution<int> last_pick(0, N-1);

        for(size_t i = 0; i < N; ++i)
            vec.push_back(gen(seed));

        for(size_t i = 0; i < N; ++i)
            vec[i] *= vec[i];

        int lp = last_pick(seed);
        fmt::println("Element number {}: {}", lp + 1, vec[lp]);
    }

    void FillArrayOptimized(const size_t& N)
    {
        std::vector<int> vec(N);

        //First OMP Block
        #pragma omp parallel
        {
            std::mt19937 seed(rd_s() + omp_get_thread_num());
            std::uniform_int_distribution<int> gen(0,100);
            
            #pragma omp for
            for(size_t i = 0; i < N; ++i)
                vec[i] = gen(seed);
        }

        //Second OMP Block
        #pragma omp parallel for
        for(size_t i = 0; i < N; ++i)
            vec[i] *= vec[i];
        

        std::mt19937 seed(rd_s());
        std::uniform_int_distribution<int> last_pick(0, N-1);

        int lp = last_pick(seed);
        fmt::println("Element number {}: {}", lp + 1, vec[lp]);
    }

    void FillArrayCXXThread(const size_t& N)
    {
        const unsigned num_of_threads = std::thread::hardware_concurrency();
        std::vector<int> vec(N);

        const auto mem_blocks = [N, num_of_threads]() ->auto
        {
            std::vector<std::pair<size_t, size_t>> mb(num_of_threads);
            mb[0] = {0, (1000/num_of_threads * N)/1000};

            for(size_t i = 1; i < num_of_threads; ++i)
            {
                mb[i].first = mb[i-1].second + 1;
                if(i == num_of_threads - 1) mb[i].second = N;
                else mb[i].second = ((1000 * (i+1))/num_of_threads * N) / 1000;
            }
            return mb;
        }();

        auto thread_arr_gen = [&vec, &mem_blocks](size_t id) ->void 
        {
            std::mt19937 seed(rd_s() + id);
            std::uniform_int_distribution<int> gen(0,100);
            for(size_t i = mem_blocks[id].first; i < mem_blocks[id].second; ++i)
                vec[i] = gen(seed);
        };

        auto thread_arr_sqr = [&vec, &mem_blocks](size_t id) ->void 
        {
            for(size_t i = mem_blocks[id].first; i < mem_blocks[id].second; ++i)
                vec[i] *= vec[i];
        };

        std::vector<std::thread> threads_gen, threads_sqr;
        threads_gen.reserve(num_of_threads);
        threads_sqr.reserve(num_of_threads);

        //arr gen
        for(size_t i = 0; i < num_of_threads; ++i)
            threads_gen.emplace_back(std::thread(thread_arr_gen, i));

        for(size_t i = 0; i < num_of_threads; ++i)
            threads_gen[i].join();

        //arr square
        for(size_t i = 0; i < num_of_threads; ++i)
            threads_sqr.emplace_back(std::thread(thread_arr_sqr, i));

        for(size_t i = 0; i < num_of_threads; ++i)
            threads_sqr[i].join();
        
        std::mt19937 seed(rd_s());
        std::uniform_int_distribution<int> last_pick(0, N-1);
        
        int lp = last_pick(seed);
        fmt::println("Element number {}: {}", lp + 1, vec[lp]);
    }
};
