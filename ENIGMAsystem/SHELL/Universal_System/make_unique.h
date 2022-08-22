// c++11 is missing make_unique so I copied an implementation here
// feel free to remove this if we ever move to c++14

#include <memory>

#if (__cplusplus < 201402L)

// note: this implementation does not disable this overload for array types

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#else
    using std::make_unique;
#endif