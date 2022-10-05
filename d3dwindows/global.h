#pragma once
#include <memory>

#define DECLARE_PRIVATE(ClassName) \
    class Private; \
    std::unique_ptr<ClassName::Private> p_;

#define MAKE_PRIVATRE(ClassName, ...) \
    p_{ MakePrivate<ClassName::Private>(## __VA_ARGS__)}

template<class T, class ...Args>
auto MakePrivate(Args&& ...args) 
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

#define DEFINE_MAKE_UNIQUE_PTR(ClassName) \
template<class ...Args> \
auto Make##ClassName(Args&& ...args) \
{ \
    return std::make_unique<ClassName>(std::forward<Args>(args)...); \
}

#define DISABLE_COPY(Class) \
    Class(const Class&) = delete;\
    Class& operator=(const Class&) = delete;

#define DISABLE_MOVE(Class) \
    Class(Class&&) = delete; \
    Class& operator=(Class&&) = delete;

#define DISABLE_COPY_MOVE(Class) \
    DISABLE_COPY(Class) \
    DISABLE_MOVE(Class)
