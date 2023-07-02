#ifndef _ANY_
#define _ANY_
#include <typeinfo>
#include "type_traits.hpp"
#include "except.h"
#include "defined.h"

MGL_START

class any {
public:
    // 构造函数
    //any() = default;
    any() : content(0) {}
    //
    template <typename ValueType>
    any(const ValueType& value)
        : content(
            new holder<typename remove_cv<typename decay<const ValueType>::type>::type>(
                value)) {}
    //
    any(const any& other)
        : content(other.content ? other.content->clone() : 0) {}

    any(any&& other) noexcept : content(other.content) { other.content = 0; }

    template <typename ValueType>
    any(ValueType&& value,
        disable_if_t<is_same<any&, ValueType>::value>* = 0,
        disable_if_t<is_const<ValueType>::value>* = 0)
        : content(new holder<typename decay<ValueType>::type>(
            static_cast<ValueType&&>(value))) {}

    ~any() noexcept { delete content; }

    any& swap(any& rhs) noexcept {
        placeholder* tmp = content;
        content = rhs.content;
        rhs.content = tmp;
        return *this;
    }

    any& operator=(const any& rhs) {
        any(rhs).swap(*this);
        return *this;
    }
    // move
    any& operator=(any&& rhs) noexcept {
        rhs.swap(*this);
        any().swap(rhs);
        return *this;
    }

    template <typename ValueType>
    any& operator=(ValueType&& rhs) {
        any(static_cast<ValueType&&>(rhs)).swap(*this);
        return *this;
    }

    bool empty() const noexcept { return !content; }

    const std::type_info& type() const noexcept {
        return content ? content->type() : typeid(void);
    }

    void clear() noexcept { any().swap(*this); }

private:
    class placeholder {
    public:
        virtual ~placeholder() {}

    public:
        virtual const std::type_info& type() const noexcept = 0;
        virtual placeholder* clone() const = 0;
    };

    template <typename ValueType>
    class holder final : public placeholder {
    public:
        holder(const ValueType& value) : held(value) {}
        holder(ValueType&& value) : held(static_cast<ValueType&&>(value)) {}

    public:
        const std::type_info& type() const noexcept { return typeid(held); }
        placeholder* clone() const { return new holder(held); }

    public:
        ValueType held;

    private:
        holder& operator=(const holder&);
    };

    template <typename ValueType>
    friend ValueType* any_cast(any*) noexcept;
    template <typename ValueType>
    friend ValueType* unsafe_any_cast(any*) noexcept;

    placeholder* content;
};
inline void swap(any& lhs, any& rhs) noexcept { lhs.swap(rhs); }
// any_cast
template <typename ValueType>
ValueType* any_cast(any* operand) noexcept {
    return operand && operand->type() == typeid(ValueType)
        ? addressof(static_cast<
            any::holder<remove_cv_t<ValueType>> *>(
                operand->content)
            ->held)
        : 0;
}

template <typename ValueType>
ValueType* any_cast(const any* operand) noexcept {
    return any_cast<ValueType>(const_cast<any*>(operand));
}

template <typename ValueType>
ValueType any_cast(any& operand) {
    typedef typename remove_reference<ValueType>::type nonref;
    nonref* result = any_cast<nonref>(addressof(operand));
    if (!result) throw bad_any_cast("bad_any_cast error");
    typedef conditional_t<is_reference<ValueType>::value, ValueType,
        add_reference_t<ValueType>>
        ref_type;
    return static_cast<ref_type>(*result);
}

template <typename ValueType>
inline ValueType any_cast(const any& operand) {
    typedef remove_reference_t<ValueType> nonref;
    return any_cast<const nonref&>(const_cast<any&>(operand));
}

template <typename ValueType>
inline ValueType any_cast(any&& operand) {
    return any_cast<ValueType>(operand);
}

template <typename ValueType>
inline ValueType* unsafe_any_cast(any* operand) noexcept {
    return &static_cast<any::holder<ValueType> *>(operand->content)->held;
}

template <typename ValueType>
inline const ValueType* unsafe_any_cast(const any* operand) noexcept {
    return unsafe_any_cast<ValueType>(const_cast<any*>(operand));
}

MGL_END
#endif