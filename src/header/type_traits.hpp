#ifndef _TEMPLATE_TYPE_TRAITS_
#define _TEMPLATE_TYPE_TRAITS_
#include "defined.h"
MGL_START
// 类型标识
template <typename T>
struct type_identity {
    using type = T;
};
// 完整定义任意类型常量
template <typename T, T v>
struct integral_constant {
    static constexpr T value = v;    // 常量值
    using value_type = T;            // 值类型
    using type = integral_constant;  // 类型
                                     // 类型转换
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};
// 定义bool常量
template <bool B>
struct bool_constant : integral_constant<bool, B> {};
// bool常量true，false
struct true_type : bool_constant<true> {};
struct false_type : bool_constant<false> {};
/**
 * @brief conditional
 * @note 通用条件选择
 */
template <bool B, typename T, typename F>
struct conditional : type_identity<T> {};
template <typename T, typename F>
struct conditional<false, T, F> : type_identity<F> {};
template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

/**
 * @brief is reference
 * @note 推断一个类型是否为引用
 */
template <typename T>
struct is_reference : false_type {};
template <typename T>
struct is_reference<T &> : true_type {};
template <typename T>
struct is_reference<T &&> : true_type {};
#if __cplusplus > 201703L
template <typename T>
inline constexpr bool is_reference_v = is_reference<T>::value;
#else
template <typename T>
constexpr bool is_reference_v = is_reference<T>::value;
#endif
/**
 * @brief remove reference
 * @note 移除一个类型的引用
 */
template <typename T>
struct remove_reference : type_identity<T> {};
template <typename T>
struct remove_reference<T &> : type_identity<T> {};
template <typename T>
struct remove_reference<T &&> : type_identity<T> {};
template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

/**
 * @brief is same
 * @note 判断两个类型是否相同
 */
template <typename T, typename U>
struct is_same : false_type {};
template <typename T>
struct is_same<T, T> : true_type {};
#if __cplusplus > 201703L
template <typename T, typename U>
inline constexpr bool is_same_v = is_same<T, U>::value;
#else
template <typename T, typename U>
constexpr bool is_same_v = is_same<T, U>::value;
#endif

/**
 * @brief is one of
 * @note 判断一个类型是否在一些类型里
 */
template <typename T, typename U, typename... R>
struct is_one_of
    : bool_constant<is_one_of<T, U>::value || is_one_of<T, R...>::value> {};
template <typename T, typename U>
struct is_one_of<T, U> : is_same<T, U> {};
#if false
template <typename T, typename U, typename... R>
struct is_one_of
    : conditional_t<is_same_v<T, U>, true_type, is_one_of<T, R...>> {};
template <typename T, typename U>
struct is_one_of<T, U> : conditional_t<is_same_v<T, U>, true_type, false_type> {
};
#endif
#if __cplusplus > 201703L
template <typename T, typename... R>
inline constexpr bool is_one_of_v = is_one_of<T, R...>::value;
#else
template <typename T, typename... R>
constexpr bool is_one_of_v = is_one_of<T, R...>::value;
#endif

/**
 * @brief is instantiation of
 * @note 判断某个类型是否为一个模板类型的实例化
 */
template <typename Inst, template <typename...> typename Tmpl>
struct is_instantiation_of : false_type {};
template <template <typename...> typename Tmpl, typename... Args>
struct is_instantiation_of<Tmpl<Args...>, Tmpl> : true_type {};
#if __cplusplus > 201703L
template <typename Inst, template <typename...> typename Tmpl>
inline constexpr bool is_instantiation_of_v =
    is_instantiation_of<Inst, Tmpl>::value;
#else
template <typename Inst, template <typename...> typename Tmpl>
constexpr bool is_instantiation_of_v = is_instantiation_of<Inst, Tmpl>::value;
#endif

/**
 * @brief rank
 * @note 返回数组的维度
 */
template <typename T>
struct rank : integral_constant<size_t, 0> {};
template <typename T>
struct rank<T[]> : integral_constant<size_t, rank<T>::value + 1> {};
template <typename T, size_t N>
struct rank<T[N]> : integral_constant<size_t, rank<T>::value + 1> {};
#if __cplusplus > 201703L
template <typename T>
inline constexpr size_t rank_v = rank<T>::value;
#else
template <typename T>
constexpr size_t rank_v = rank<T>::value;
#endif

/**
 * @brief extent
 * @note 获取数组第N维的大小
 */
template <typename T, unsigned N = 0>
struct extent : integral_constant<size_t, 0> {};
template <typename T>
struct extent<T[], 0> : integral_constant<size_t, 0> {};
template <typename T, unsigned N>
struct extent<T[], N> : extent<T, N - 1> {};
template <typename T, size_t I>
struct extent<T[I], 0> : integral_constant<size_t, I> {};
template <typename T, size_t I, unsigned N>
struct extent<T[I], N> : extent<T, N - 1> {};
#if __cplusplus > 201703L
template <typename T>
inline constexpr size_t extent_v = extent<T>::value;
#else
template <typename T>
constexpr size_t extent_v = extent<T>::value;
#endif

/**
 * @brief enable_if
 * @note 如果bool为true，则返回T本身，否则什么也不做
 */
template <bool, typename T = void>
struct enable_if : type_identity<T> {};
template <typename T>
struct enable_if<false, T> {};
template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

template <bool, typename T = void>
struct disable_if : type_identity<T> {};
template <typename T>
struct disable_if<true, T> {};
template <bool B, typename T = void>
using disable_if_t = typename disable_if<B, T>::type;

/**
 * @brief always true
 */
template <typename... Args>
struct always_true : true_type {};
template <typename... Args>
constexpr bool always_true_v = always_true<Args...>::value;

/**
 * @brief void_t
 */
template <typename... T>
struct make_void {
    using type = void;
};
template <typename... T>
using void_t = typename make_void<T...>::type;

/**
 * @brief is_detected
 */
template <typename, template <typename...> class Op, typename... T>
struct is_detected_impl : false_type {};
template <template <typename...> class Op, typename... T>
struct is_detected_impl<void_t<Op<T...>>, Op, T...> : true_type {};
template <template <typename...> class Op, typename... T>
using is_detected = is_detected_impl<void, Op, T...>;

/**
 * @brief return lvalue type
 */
template <typename T>
type_identity<T &> return_lvalue_type(int);
template <typename T>
type_identity<T> return_lvalue_type(...);
template <typename T>
/**
 * @brief return rvalue type
 */
type_identity<T &&> return_rvalue_type(int);
template <typename T>
type_identity<T> return_rvalue_type(...);

/**
 * @brief add lvalue reference
 * @note 添加左值引用
 */
template <typename T>
struct add_lvalue_reference : decltype(return_lvalue_type<T>(0)) {};
template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
/**
 * @brief add lvalue reference
 * @note 添加右值引用
 */
template <typename T>
struct add_rvalue_reference : decltype(return_rvalue_type<T>(0)) {};
template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

/**
 * @brief add_reference
 * @note 将一个类型转为引用
 */
template <typename T>
struct add_reference_impl : add_lvalue_reference<T> {};
template <typename T>
struct add_reference_impl<T &&> : add_rvalue_reference<T> {};
template <class T>
struct add_reference {
    typedef typename add_reference_impl<T>::type type;
};
template <class T>
struct add_reference<T &> {
    typedef T &type;
};
template <>
struct add_reference<void> {
    typedef void type;
};
template <>
struct add_reference<const void> {
    typedef const void type;
};
template <>
struct add_reference<const volatile void> {
    typedef const volatile void type;
};
template <>
struct add_reference<volatile void> {
    typedef volatile void type;
};
template <class T>
using add_reference_t = typename add_reference<T>::type;

/**
 * @brief declval
 */
template <typename T>
add_rvalue_reference_t<T> declval() noexcept;

/**
 * @brief is copy operator
 * @note 判断一个类型是否能拷贝赋值
 */
template <typename T>
using copy_assign_t = decltype(declval<T &>() = declval<T const &>());
template <typename T, typename = void>
struct is_copy_assignable : false_type {};
template <typename T>
struct is_copy_assignable<T, void_t<copy_assign_t<T>>> : true_type {};

/**
 *@brief is move operator
 *@note 判断一个类型是否能移动赋值
 */
template <typename T>
using move_assign_t = decltype(declval<T &>() = declval<T &&>());
template <typename T, typename = void>
struct is_move_assignable : false_type {};
template <typename T>
struct is_move_assignable<T, void_t<move_assign_t<T>>> : true_type {};

/**
 *@brief is_integer
 *@note 判断一个类型是否是整数类型
 */
template <typename T>
struct is_integer : false_type {};
template <>
struct is_integer<int> : true_type {};
template <typename T>
constexpr bool is_integer_v = is_integer<T>::value;

/**
 *@brief is_float
 *@note 判断一个类型是否是浮点类型
 */
template <typename T>
struct is_float : false_type {};
template <>
struct is_float<float> : true_type {};
template <typename T>
constexpr bool is_float_v = is_float<T>::value;

/**
 *@brief is_long
 *@note 判断一个类型是否是long类型
 */
template <typename T>
struct is_long : false_type {};
template <>
struct is_long<long> : true_type {};
template <typename T>
constexpr bool is_long_v = is_long<T>::value;

/**
 *@brief is_double
 *@note 判断一个类型是否是double类型
 */
template <typename T>
struct is_double : false_type {};
template <>
struct is_double<double> : true_type {};
template <typename T>
constexpr bool is_double_v = is_double<T>::value;

/**
 *@brief is_char
 *@note 判断一个类型是否是char类型
 */
template <typename T>
struct is_char : false_type {};
template <>
struct is_char<char> : true_type {};
template <typename T>
constexpr bool is_char_v = is_char<T>::value;

/**
 *@brief is_short
 *@note 判断一个类型是否是short类型
 */
template <typename T>
struct is_short : false_type {};
template <>
struct is_short<short> : true_type {};
template <typename T>
constexpr bool is_short_v = is_short<T>::value;

/**
 *@brief is_longlong
 *@note 判断一个类型是否是long long类型
 */
template <typename T>
struct is_longlong : false_type {};
template <>
struct is_longlong<long long> : true_type {};
template <typename T>
constexpr bool is_longlong_v = is_longlong<T>::value;

/**
 *@brief is_pointer
 *@note 判断一个类型是否为指针
 */
template <typename T>
struct is_pointer : false_type {};
template <typename T>
struct is_pointer<T *> : true_type {};
template <typename T>
constexpr bool is_pointer_v = is_pointer<T>::value;

/**
 * @brief remove const
 * @note 移除const限定符
 */
template <typename T>
struct remove_const : type_identity<T> {};
template <typename T>
struct remove_const<const T> : type_identity<T> {};
template <typename T>
using remove_const_t = typename remove_const<T>::type;

/**
 * @brief remove volatile
 * @note 移除volatile限定符
 */
template <typename T>
struct remove_volatile : type_identity<T> {};
template <typename T>
struct remove_volatile<volatile T> : type_identity<T> {};
template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

/**
 * @brief remove_cv
 * @note 移除cv限定符
 */
template <class T>
struct remove_cv {
    typedef T type;
};
template <class T>
struct remove_cv<T const> {
    typedef T type;
};
template <class T>
struct remove_cv<T volatile> {
    typedef T type;
};
template <class T>
struct remove_cv<T const volatile> {
    typedef T type;
};
template <class T, unsigned long long N>
struct remove_cv<T const [N]> { typedef T type[N]; };
template <class T, unsigned long long N>
struct remove_cv<T const volatile [N]> { typedef T type[N]; };
template <class T, unsigned long long N>
struct remove_cv<T volatile [N]> { typedef T type[N]; };
template <class T>
struct remove_cv<T const[]> {
    typedef T type[];
};
template <class T>
struct remove_cv<T const volatile[]> {
    typedef T type[];
};
template <class T>
struct remove_cv<T volatile[]> {
    typedef T type[];
};
template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

/**
 * @brief decay
 * @note 类型退化
 */
template <typename T>
struct decay : remove_cv<T> {};
template <typename T>
struct decay<T[]> : type_identity<T *> {};
template <typename T, size_t N>
struct decay<T[N]> : type_identity<T *> {};
template <typename R, typename... Args>
struct decay<R(Args...)> : type_identity<R (*)(Args...)> {};
template <typename R, typename... Args>
struct decay<R(Args..., ...)> : type_identity<R (*)(Args..., ...)> {};

/**
 * is default construct
 */
/******一种实现方式, 较繁琐
template <typename T>
struct is_default_constructible_helper {
 private:
  template <typename U, typename = decltype(U())>
  static true_type test(void *);
  template <typename>
  static false_type test(...);

 public:
  using type = decltype(test<T>(nullptr));
};
template <typename T>
struct is_default_constructible : is_default_constructible_helper<T>::type {};
template <typename T>
constexpr bool is_default_constructible_v = is_default_constructible<T>::value;
*/
/**
 * @brief is default construct
 * @note 判断是否存在默认构造函数
 */
template <typename T>
using defalut_construct_t = decltype(T());
template <typename T, typename = void>
struct is_default_constructible : false_type {};
template <typename T>
struct is_default_constructible<T, void_t<defalut_construct_t<T>>> : true_type {
};

/**
 * @brief is copy construct
 * @note 判断是否存在拷贝构造函数
 */
template <typename T>
using copy_construct_t = decltype(T(T()));
template <typename T, typename = void>
struct is_copy_constructible : false_type {};
template <typename T>
struct is_copy_constructible<T, void_t<copy_construct_t<T>>> : true_type {};

/**
 * @brief is void type
 * @note 判断是否为void类型
 */
template <typename T>
struct is_void : false_type {};
template <>
struct is_void<void> : true_type {};
/**
 * @brief is array type
 * @note 判断是否时数组类型
 */
template <typename T>
struct is_array : false_type {};
template <typename T>
struct is_array<T[]> : true_type {};
template <typename T, size_t N>
struct is_array<T[N]> : true_type {};

/**
 * @brief is function pointer
 * @note 判断是否是函数指针
 */
template <typename F>
struct is_function : false_type {};
template <typename F, typename... Args>
struct is_function<F(Args...)> : true_type {};
template <typename F, typename... Args>
struct is_function<F(Args..., ...)> : true_type {};

/**
 * @brief is convertible type from -> type to
 * @note 判断一个类型是否能转为另一个类型
 */
template <typename T>
void aux(T);
template <typename FROM, typename TO>
using convertible_t = decltype(aux<TO>(declval<FROM>()));
template <typename FROM, typename TO, typename = void,
          bool = is_void<TO>::value || is_array<TO>::value ||
                 is_function<TO>::value>
struct is_convertible
    : integral_constant<bool, is_void<FROM>::value && is_void<TO>::value> {};
template <typename FROM, typename TO>
struct is_convertible<FROM, TO, void_t<convertible_t<FROM, TO>>, false>
    : true_type {};
/**
 * @brief 判断类是否存在某个类型
 *
 */
#define HAS_TYPE(MemType)                    \
    template <typename, typename = void_t<>> \
    struct has_##MemType : false_type {};    \
    template <typename T>                    \
    struct has_##MemType<T, void_t<typename T::##MemType>> : true_type {};
/**
 * @brief 判断类是否有指定成员
 */
#define HAS_MEMBER(Member)                   \
    template <typename, typename = void_t<>> \
    struct has_##Member : false_type {};     \
    template <typename T>                    \
    struct has_##Member<T, void_t<decltype(&T::##Member)>> : true_type {};
/**
 * @brief 判断类是否有给定方法
 *
 */
#define HAS_METHOD(Method)                                            \
    template <typename, typename = void_t<>>                          \
    struct has_##Method : false_type {};                              \
    template <typename T>                                             \
    struct has_##Method<T, void_t<decltype(declval<T>().##Method())>> \
        : true_type {};

/**
 * @brief is const
 * 判断类型是否是const的
 */
template <class T>
struct is_const : public false_type {};
template <class T>
struct is_const<T const> : public true_type {};
template <class T, unsigned long long N>
struct is_const<T const[N]> : public true_type {};
template <class T>
struct is_const<T const[]> : public true_type{};

/**
 * @brief 获取对象的地址
 *
 */
template <class T>
inline T *addressof(T &obj) {
    return static_cast<T *>(static_cast<void *>(
        const_cast<char *>(&reinterpret_cast<const volatile char &>(obj))));
}

MGL_END
#endif