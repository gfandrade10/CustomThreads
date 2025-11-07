#pragma once

#include <functional>

namespace TimeChecker
{
    float ElapsedTime(std::function<void(size_t)> func, size_t NofArgs);
    void FillArrayUnoptimized(const size_t& N);
    void FillArrayOptimized(const size_t& N);
    void FillArrayCXXThread(const size_t& N);    
};
