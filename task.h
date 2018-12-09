#include <functional>
#include <variant>

namespace laserpants
{
    template <typename Fx, typename Arg>
    class task_impl
    {
    public:
        template <typename Lam>
        explicit task_impl(Lam lambda) noexcept;

        virtual ~task_impl() = default;

        template <typename Lam>
        auto then(Lam lambda) const noexcept;

        template <typename Lam>
        auto then_catch(Lam lambda) const noexcept;

        Fx run(const Arg& x = Arg{}) const;

    private:
        const std::function<Fx(Arg)> func;
    };

    template <typename Fx, typename Arg>
    template <typename Lam>
    task_impl<Fx, Arg>::task_impl(Lam lambda) noexcept : func{lambda}
    {
    }

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
    template <typename Lam>
    auto task_impl<Fx, Arg>::then_catch(Lam lambda) const noexcept
    {
        return task_impl<Fx, Arg>{
            [this, lambda](const Arg& a) {
                try {
                    return func(a);
                } catch(const std::exception& e) {
                    return lambda(e);
                }
            }
        };
    }

    template <typename Fx, typename Arg>
    Fx task_impl<Fx, Arg>::run(const Arg& x) const
    {
        return func(x);
    }

    template <typename Fx>
    class task : public task_impl<Fx, std::monostate>
    {
    public:
        template <typename Lam>
        explicit task(Lam lambda);

        task(const task&) = delete;
        task operator=(const task&) = delete;
    };

    template <typename Fx>
    template <typename Lam>
    task<Fx>::task(Lam lambda)
      : task_impl<Fx, std::monostate>{[lambda](auto) { return lambda(); }}
    {
    }
}
