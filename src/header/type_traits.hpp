#ifndef __TEMPLATE_TYPE_TRAITS__
#define __TEMPLATE_TYPE_TRAITS__
#if defined(MGL_START)
MGL_START
#endif
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
/**
 * index sequence
 */
template <typename T, T... Index>
struct integer_sequence {
    using value_type = T;
    static constexpr size_t size() noexcept { return sizeof...(Index); }
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
template <class T>
constexpr bool is_const_v = is_const<T>::value;

/**
 * @brief is reference
 * @note 推断一个类型是否为引用
 */
template <typename T>
struct is_reference : false_type {};
template <typename T>
struct is_reference<T&> : true_type {};
template <typename T>
struct is_reference<T&&> : true_type {};
#if __cplusplus >= 201703L
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
struct remove_reference<T&> : type_identity<T> {};
template <typename T>
struct remove_reference<T&&> : type_identity<T> {};
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
#if __cplusplus >= 201703L
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
#if __cplusplus >= 201703L
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
#if __cplusplus >= 201703L
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
#if __cplusplus >= 201703L
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
#if __cplusplus >= 201703L
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
type_identity<T&> return_lvalue_type(int);
template <typename T>
type_identity<T> return_lvalue_type(...);
/**
 * @brief return rvalue type
 */
template <typename T>
type_identity<T&&> return_rvalue_type(int);
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
struct add_reference_impl<T&&> : add_rvalue_reference<T> {};
template <class T>
struct add_reference {
    typedef typename add_reference_impl<T>::type type;
};
template <class T>
struct add_reference<T&> {
    typedef T& type;
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

template <typename T1, typename T2>
using assign_t = decltype(declval<T1>() = declval<T2>());
template <typename T1, typename T2, typename = void>
struct is_assignable : false_type {};
template <typename T1, typename T2>
struct is_assignable<T1, T2, void_t<assign_t<T1, T2>>> : true_type {};

/**
 * @brief is copy operator
 * @note 判断一个类型是否能拷贝赋值
 */
template <typename T>
struct is_copy_assignable : is_assignable<T, T const&> {};

/**
 *@brief is move operator
 *@note 判断一个类型是否能移动赋值
 */
template <typename T>
struct is_move_assignable : is_assignable<T, T&&> {};

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
struct is_pointer<T*> : true_type {};
template <typename T>
struct is_pointer<T* const> : true_type {};
template <typename T>
struct is_pointer<T* volatile> : true_type {};
template <typename T>
struct is_pointer<T* const volatile> : true_type {};
template <typename T>
constexpr bool is_pointer_v = is_pointer<T>::value;
/**
 * is member pointer
 */
template <class T>
struct is_member_pointer_helper : false_type {};
template <class T, class U>
struct is_member_pointer_helper<T U::*> : true_type {};
template <class T>
struct is_member_pointer
    : is_member_pointer_helper<typename remove_cv<T>::type> {};

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

template <typename T>
using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;

/**
 * @brief decay
 * @note 类型退化
 */
template <typename T>
struct decay : remove_cv<T> {};
template <typename T>
struct decay<T[]> : type_identity<T*> {};
template <typename T, size_t N>
struct decay<T[N]> : type_identity<T*> {};
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
 * 测试使用指定参数类型时是否可构造类型。
 */
template <typename T, typename... Args>
using construct_t = decltype(T(declval<Args>()...));
template <typename, typename T, typename... Args>
struct is_constructible_impl : false_type {};
template <typename T, typename... Args>
struct is_constructible_impl<void_t<construct_t<T, Args...>>, T, Args...>
    : true_type {};
template <typename T, typename... Args>
struct is_constructible : is_constructible_impl<void, T, Args...> {};
template <typename T, typename... Args>
constexpr bool is_constructible_v = is_constructible<T, Args...>::value;
/**
 * @brief is default construct
 * @note 判断是否存在默认构造函数
 */
template <typename T>
struct is_default_constructible : is_constructible<T> {};
template <typename T>
constexpr bool is_default_constructible_v = is_default_constructible<T>::value;
/**
 * @brief is copy construct
 * @note 判断是否存在拷贝构造函数
 */
template <typename T>
struct is_copy_constructible : is_constructible<T, const T&> {};
template <typename T>
constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;
/**
 * 判断是否存在移动构造函数
 */
template <typename T>
struct is_move_constructible : is_constructible<T, T&&> {};
template <typename T>
constexpr bool is_move_constructible_v = is_move_constructible<T>::value;
/**
 * 判断一个构造函数是否会抛出异常
 */
template <typename T, typename... Args>
constexpr bool nothrow_construct_v = noexcept(T(declval<Args>()...));
template <typename, typename T, typename... Args>
struct is_nothrow_constructible : false_type {};
template <typename T, typename... Args>
struct is_nothrow_constructible<
    enable_if<nothrow_construct_v<T, Args...>, void>, T, Args...> : true_type {
};
template <typename T, typename... Args>
constexpr bool is_nothrow_constructible_v =
    is_nothrow_constructible<T, Args...>::value;

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
template <typename T>
struct is_function : integral_constant<bool, !is_const<const T>::value &&
                                                 is_reference<T>::value> {};
/**
 *
 */
template <class T>
struct is_member_function_pointer_helper : false_type {};
template <class T, class U>
struct is_member_function_pointer_helper<T U::*> : is_function<T> {};
template <class T>
struct is_member_function_pointer
    : is_member_function_pointer_helper<typename remove_cv<T>::type> {};
/**
 * is member object pointer
 */
template <class T>
struct is_member_object_pointer
    : integral_constant<bool, is_member_pointer<T>::value &&
                                  !is_member_function_pointer<T>::value> {};
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
 * @brief 获取对象的地址
 *
 */
template <class T>
inline T* addressof(T& obj) {
    return static_cast<T*>(static_cast<void*>(
        const_cast<char*>(&reinterpret_cast<const volatile char&>(obj))));
}
/*=====================invoke result============================*/
namespace detail {
template <class T>
struct is_reference_wrapper : false_type {};
template <class U>
struct is_reference_wrapper<reference_wrapper<U>> : true_type {};

template <class T>
struct invoke_impl {
    template <class F, class... Args>
    static auto call(F&& f, Args&&... args)
        -> decltype(forward<F>(f)(forward<Args>(args)...));
};

template <class B, class MT>
struct invoke_impl<MT B::*> {
    template <class T, class Td = typename decay<T>::type,
              class = typename enable_if<is_base_of<B, Td>::value>::type>
    static auto get(T&& t) -> T&&;

    template <class T, class Td = typename decay<T>::type,
              class = typename enable_if<is_reference_wrapper<Td>::value>::type>
    static auto get(T&& t) -> decltype(t.get());

    template <
        class T, class Td = typename decay<T>::type,
        class = typename enable_if<!is_base_of<B, Td>::value>::type,
        class = typename enable_if<!is_reference_wrapper<Td>::value>::type>
    static auto get(T&& t) -> decltype(*forward<T>(t));

    template <class T, class... Args, class MT1,
              class = typename enable_if<is_function<MT1>::value>::type>
    static auto call(MT1 B::*pmf, T&& t, Args&&... args)
        -> decltype((invoke_impl::get(forward<T>(t)).*
                     pmf)(forward<Args>(args)...));

    template <class T>
    static auto call(MT B::*pmd, T&& t)
        -> decltype(invoke_impl::get(forward<T>(t)).*pmd);
};

template <class F, class... Args, class Fd = typename decay<F>::type>
auto INVOKE(F&& f, Args&&... args)
    -> decltype(invoke_impl<Fd>::call(forward<F>(f), forward<Args>(args)...));

template <typename AlwaysVoid, typename, typename...>
struct invoke_result {};
template <typename F, typename... Args>
struct invoke_result<decltype(void(INVOKE(declval<F>(), declval<Args>()...))),
                     F, Args...> {
    using type = decltype(INVOKE(declval<F>(), declval<Args>()...));
};
}  // namespace detail
/**
 * 获取某个函数的返回值类型
 */
template <typename T>
struct result_of;
template <typename Func, typename... Args>
struct result_of<Func(Args...)> : detail::invoke_result<void, F, Args...> {};

template <class F, class... ArgTypes>
struct invoke_result : detail::invoke_result<void, F, ArgTypes...> {};
/*=========================invoke===============================*/
// 普通函数
struct _InvokeFunc {
    template <typename _Callable, typename... _Types>
    static auto _Call(_Callable&& obj, _Types&&... args) {
        return obj(forward<_Types>(args)...);
    }
};
// 成员函数
struct _InvokeMemFunc {
    template <typename _Callable, typename _Obj, typename... _Types>
    static auto _Call(_Callable&& fn, _Obj&& obj, _Types&&... argv)
        -> decltype((obj->*fn)(std::forward<_Types>(argv)...)) {
        return (obj->*fn)(std::forward<_Types>(argv)...);
    }
    template <typename _Callable, typename _Obj, typename... _Types>
    static auto _Call(_Callable&& fn, _Obj&& obj, _Types&&... argv)
        -> decltype((obj.*fn)(std::forward<_Types>(argv)...)) {
        return (obj.*fn)(std::forward<_Types>(argv)...);
    }
};
// 调用成员变量
struct _InvokeMemObj {
    template <typename _Callable, typename _Obj>
    static auto _Call(_Callable&& fn, _Obj&& obj) -> decltype((obj->*fn)) {
        return (obj->*fn);
    }
    template <typename _Callable, typename _Obj>
    static auto _Call(_Callable&& fn, _Obj&& obj) -> decltype((obj.*fn)) {
        return (obj.*fn);
    }
};
///////////////
template <typename _Callable, typename _FirstTy,
          typename _Decayed = typename decay<_Callable>::type,
          bool _Is_MemFun = is_member_function_pointer<_Decayed>::value,
          bool _Is_MemObj = is_member_object_pointer<_Decayed>::value>
struct _Invoke1;
// 成员函数
template <typename _Callable, typename _FirstTy, typename _Decayed>
struct _Invoke1<_Callable, _FirstTy, _Decayed, true, false> : _InvokeMemFunc {};
// 成员变量
template <typename _Callable, typename _FirstTy, typename _Decayed>
struct _Invoke1<_Callable, _FirstTy, _Decayed, false, true> : _InvokeMemObj {};
// 普通函数
template <typename _Callable, typename _FirstTy, typename _Decayed>
struct _Invoke1<_Callable, _FirstTy, _Decayed, false, false> : _InvokeFunction {
};
// 本层先把无参数的直接筛选出来了
template <typename _Callable, typename... _Types>
struct _Invoke;
// 无参数，必定是一个普通函数
template <typename _Callable>
struct _Invoke<_Callable> : _InvokeFunction {};
// 有一个或多个参数，可能是普通函数，成员函数，数据成员
template <typename _Callable, typename _FirstTy, typename... _Types>
struct _Invoke<_Callable, _FirstTy, _Types...> : _Invoke1<_Callable, _FirstTy> {
};
/**
 * invoke
 */
template <typename _Callable, typename... _Types>
auto invoke(_Callable&& obj, _Types&&... argv) {
    return _Invoke<_Callable, _Types...>::_Call(forward<_Callable>(obj),
                                                forward<_Types>(argv)...);
}
/*================================================================*/
/**
 * move
 */
template <typename T>
constexpr remove_reference_t<T>&& move(T&& arg) noexcept {
    return static_cast<remove_reference_t<T>&&>(arg);
}
/**
 * forward
 */
template <typename T>
constexpr T&& forward(remove_reference_t<T>& arg) noexcept {
    return static_cast<remove_reference_t<T>&&>(arg);
}
template <typename T>
constexpr T&& forward(remove_reference_t<T>&& arg) noexcept {
    return static_cast<remove_reference_t<T>&&>(arg);
}
/**
 * add const
 */
template <typename T>
struct add_const : type_identity<T const> {};
template <typename T>
using add_const_t = typename add_const<T>::type;
/**
 * add volatile
 */
template <typename T>
struct add_volatile : type_identity<volatile T> {};
template <typename T>
using add_volatile_t = typename add_volatile<T>::type;
/**
 * add const volatile
 */
template <typename T>
struct add_cv : type_identity<volatile const T> {};
template <typename T>
using add_cv_t = typename add_cv<T>::type;

/**
 * always_false
 */
template <typename>
constexpr bool always_false = false;
/**
 * always_true
 */
template <typename>
constexpr bool always_true = true;
/**
 * index sequence
 */
template <int... N>
struct index_sequence {};
/**
 * make index sequence
 */
template <int N, int... M>
struct make_index_sequence : public make_index_sequence<N - 1, N - 1, M...> {};
template <int... M>
struct make_index_sequence<0, M...> : public index_sequence<M...> {};
/**
 * index sequence for
 */
template <typename... Types>
using index_sequence_for = make_index_sequence<sizeof...(Types)>;

/*==================reference_wrapper=====================*/
namespace detail {
template <class T>
constexpr T& fun(T& t) noexcept {
    return t;
}
template <class T>
void fun(T&&) = delete;
}  // namespace detail

template <class T>
class reference_wrapper {
  public:
    typedef T type;
    template <class U,
              class = decltype(detail::fun<T>(declval<U>()),
                               enable_if_t<!is_same_v<reference_wrapper,
                                                      remove_cvref_t<U>>>())>
    constexpr reference_wrapper(U&& u) noexcept(
        noexcept(detail::fun<T>(forward<U>(u))))
        : _ptr(addressof(detail::fun<T>(forward<U>(u)))) {}
    reference_wrapper(const reference_wrapper&) noexcept = default;

    reference_wrapper& operator=(const reference_wrapper&) noexcept = default;

    constexpr operator T&() const noexcept { return *_ptr; }
    constexpr T& get() const noexcept { return *_ptr; }

    template <class... Args>
    constexpr typename invoke_result<T&, Args...>::type operator()(
        Args&&... args) const {
        return invoke(get(), forward<Args>(args)...);
    }

  private:
    T* _ptr;
};
template <class T>
inline reference_wrapper<T> ref(T& t) noexcept {
    return reference_wrapper<T>(t);
}
template <class T>
inline reference_wrapper<T> ref(reference_wrapper<T> t) noexcept {
    return ref(t.get());
}
/*============================================================*/

template <class _Ty>
struct _Unrefwrap_helper {};

template <class _Ty>
struct _Unrefwrap_helper<reference_wrapper<_Ty>> {
    using type = _Ty&;
};

template <class _Ty>
using unrefwrap_t = typename _Unrefwrap_helper<decay_t<_Ty>>::type;

#if defined(MGL_END)
MGL_END
#endif
#endif