// All content copyright (c) Allan Deutsch 2017. All rights reserved.
#include <type_traits>
#include <utility>

namespace ftl {

  // Iterators
  template<typename T>
  struct has_begin {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::iterator{ std::declval<T>().begin() })>;
    template<typename C> static auto test(void*)->check<decltype(typename T::iterator{ std::declval<const T>().begin() })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_end {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::iterator{ std::declval<T>().end() })>;
    template<typename C> static auto test(void*)->check<decltype(typename T::iterator{ std::declval<const T>().end() })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_rbegin {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::reverse_iterator{ std::declval<T>().rbegin() })>;
    template<typename C> static auto test(void*)->check<decltype(typename T::reverse_iterator{ std::declval<const T>().rbegin() })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_rend {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::reverse_iterator{ std::declval<T>().rend() })>;
    template<typename C> static auto test(void*)->check<decltype(typename T::reverse_iterator{ std::declval<const T>().rend() })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_cbegin {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::const_iterator{ std::declval<T>().cbegin() })>;
    template<typename C> static auto test(void*)->check<decltype(typename T::const_iterator{ std::declval<const T>().cbegin() })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_cend {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::const_iterator{ std::declval<T>().cend() })>;
    template<typename C> static auto test(void*)->check<decltype(typename T::const_iterator{ std::declval<const T>().cend() })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_crbegin {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::const_reverse_iterator{ std::declval<T>().crbegin() })>;
    template<typename C> static auto test(void*)->check<decltype(typename T::const_reverse_iterator{ std::declval<const T>().crbegin() })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_crend {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::const_reverse_iterator{ std::declval<T>().crend() })>;
    template<typename C> static auto test(void*)->check<decltype(typename T::const_reverse_iterator{ std::declval<const T>().crend() })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  // Capacity
  template<typename T>
  struct has_size {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::size_type{ std::declval<T>().size() })>;
    template<typename C> static auto test(void*)->check<decltype(typename T::size_type{ std::declval<const T>().size() })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_max_size {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::size_type{ std::declval<T>().max_size() })>;
    template<typename C> static auto test(void*)->check<decltype(typename T::size_type{ std::declval<const T>().max_size() })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_capacity {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::size_type{ std::declval<T>().capacity() })>;
    template<typename C> static auto test(void*)->check<decltype(typename T::size_type{ std::declval<const T>().capacity() })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_resize {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(std::declval<T>().resize(std::declval<typename T::size_type>()))>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_resize_fill {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)
      ->check<decltype(std::declval<C>().resize(std::declval<T::size_type>(), std::declval<typename T::value_type>()))>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_empty {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(bool{ std::declval<T>().empty() })>;
    template<typename C> static auto test(void*)->check<decltype(bool{ std::declval<const T>().empty() })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_reserve {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(std::declval<C>().reserve(std::declval<typename T::size_type>()))>;
    template<typename C> static auto test(void*)->check<decltype(std::declval<const C>().reserve(std::declval<typename T::size_type>()))>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_shrink_to_fit {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(std::declval<C>().shrink_to_fit())>;
    template<typename C> static auto test(void*)->check<decltype(std::declval<const C>().shrink_to_fit())>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  // Element Access

  template<typename T>
  struct has_index_operator {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::reference{ std::declval<T>()[std::declval<typename T::size_type>()] })>;
    //template<typename C> static auto test(void*)->check<decltype(typename T::const_reference{ std::declval<const T>()[std::declval<typename T::size_type>()] })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_map_index_operator {
  private:
    template<typename C> struct check : std::true_type { using type = void; };
    template<typename C, class U = void>
    struct has_mapped_type {
      static constexpr bool value = false;
    };
    template<typename C>
    struct has_mapped_type<C, typename check<typename C::mapped_type>::value > {
      static constexpr bool value = true;
      using type = typename C::mapped_type;
    };
    template<typename C = T> static auto test(int)->check< std::enable_if_t<has_mapped_type<C>::value, void> >;
    //template<typename C> static auto test(void*)->check<decltype(typename T::mapped_type{ std::declval<const T>()[std::declval<const typename T::key_type>()] })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_at {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::reference{ std::declval<T>().at(std::declval<typename T::size_type>()) })>;
    //template<typename C> static auto test(void*)->check<decltype(typename T::const_reference{ std::declval<const T>().at(std::declval<typename T::size_type>()) })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_front {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::reference{ std::declval<T>().front() })>;
    //template<typename C> static auto test(void*)->check<decltype(typename T::const_reference{ std::declval<const T>().front() })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_back {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::reference{ std::declval<T>().back() })>;
    //template<typename C> static auto test(void*)->check<decltype(typename T::const_reference{ std::declval<const T>().back() })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_clear {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(std::declval<T>().clear())>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_data {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::pointer{ std::declval<C>().data() })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_assign_range {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(std::declval<C>().assign(std::declval<typename T::iterator>(), std::declval<typename T::iterator>()))>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_assign_fill {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(std::declval<C>().assign(typename T::size_type{}, std::declval<typename T::value_type>()))>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_assign_il {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(std::declval<C>().assign(std::declval<std::initializer_list<typename T::value_type>>()))>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_push_back {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(std::declval<C>().push_back(std::declval<const typename T::value_type&>()))>;
    template<typename C> static auto test(void *)->check<decltype(std::declval<C>().push_back(std::move(std::declval<typename T::value_type>())))>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_pop_back {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(std::declval<C>().pop_back())>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_insert {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::iterator{ std::declval<C>().insert(std::declval<typename T::const_iterator>(), std::declval<const typename T::value_type&>()) })>;
    template<typename C> static auto test(void*)->check<decltype(typename T::iterator{ std::declval<C>().insert(std::declval<typename T::const_iterator>(), std::declval<typename T::value_type&&>()) })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_insert_n {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::iterator{ std::declval<C>().insert(std::declval<typename T::const_iterator>(), std::declval<typename T::size_type>(), std::declval<const typename T::value_type&>()) })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_insert_range {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::iterator{ std::declval<C>().insert(std::declval<typename T::const_iterator>(), std::declval<typename T::iterator>(), std::declval<typename T::iterator>()) })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_insert_il {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::iterator{ std::declval<C>().insert(std::declval<typename T::const_iterator>(), std::initializer_list<typename T::value_type>()) })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_erase {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::iterator{ std::declval<C>().erase(std::declval<typename T::iterator>()) })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_erase_range {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(typename T::iterator{ std::declval<C>().erase(std::declval<typename T::iterator>(), std::declval<typename T::iterator>()) })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_emplace {
  private:
    struct anonymous_1 {};
    struct anonymous_2 {};
    struct anonymous_3 {};
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)
      ->check<decltype(typename T::iterator{
      std::declval<C>().emplace(std::declval<typename T::iterator>()
      , std::declval<anonymous_1&&>()
        , std::declval<anonymous_2&&>()
        , std::declval<anonymous_3&&>())
    })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };
  template<typename T>
  struct has_emplace_back {
  private:
    struct anonymous_1 {};
    struct anonymous_2 {};
    struct anonymous_3 {};
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)
      ->check<decltype(typename T::iterator{
      std::declval<C>().emplace_back(std::declval<typename T::iterator>()
      , std::declval<anonymous_1&&>()
        , std::declval<anonymous_2&&>()
        , std::declval<anonymous_3&&>())
    })>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };

  template<typename T>
  struct has_swap {
  private:
    template<typename> struct check : std::true_type {};
    template<typename C> static auto test(int)->check<decltype(std::declval<C>().swap(std::declval<T>()))>;
    template<class> static auto test(long)->std::false_type;
    template<typename C> struct verify : decltype(test<C>(0)){};
  public:
    static constexpr bool value{ verify<T>() };
  };

  // composite traits
  template<typename T>
  struct has_iterable_range {
    static constexpr bool value{ (has_begin<T>::value && has_end<T>::value)
      || (has_cbegin<T>::value && has_cend<T>::value)
      || (has_rbegin<T>::value && has_rend<T>::value)
      || (has_crbegin<T>::value && has_crend<T>::value) };
  };



} // namespace ftl