#include <functional>

template <typename Fx, typename Arg>
class task_impl
{
public:
    template <typename Lam> 
    explicit task_impl(Lam lambda) noexcept;

    template <typename Lam> 
    auto then(Lam lambda) const noexcept;

    Fx run() const;

private:
    const std::function<Fx(Arg)> func;
};

template <typename Fx, typename Arg>
template <typename Lam> 
task_impl<Fx, Arg>::task_impl(Lam lambda) noexcept : func{lambda} {}

template <typename Fx, typename Arg>
template <typename Lam>
auto task_impl<Fx, Arg>::then(Lam lambda) const noexcept
{
    return task_impl<decltype(lambda(Fx{})), Arg>{
        [this, lambda](const Arg& a) { 
            return lambda(func(a)); 
        }
    };
}

template <typename Fx, typename Arg>
Fx task_impl<Fx, Arg>::run() const
{
    return func(Arg{});
}

template<typename Fx, typename Lam>
auto task(Lam lambda) noexcept
{
    struct unit {};

    return task_impl<Fx, unit>{[lambda](auto) { return lambda(); }};
}
