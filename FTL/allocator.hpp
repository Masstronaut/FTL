// All content copyright (C) Allan Deutsch 2017. All rights reserved.

#pragma once

#include <limits> // numeric_limits
#include <type_traits> // std::false_type
#include <cstddef> // ptrdiff_t
#include <utility> // forward
#include <cstring> // memset, size_t
namespace ftl {

  // Allocator interface:
  // This is the default allocator. It fulfills the minimum interface requirements of an allocator.
  // If you wish to write a custom allocator, it must have at least these type aliases and member functions.
  // It does not need to derive from this class.
  template<typename T>
  class default_allocator {
  public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_pointer = const T *;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = decltype(pointer{} -pointer{});
    template<typename Type>
    using rebind = default_allocator<Type>;
    using propagate_on_container_move_assignment = std::false_type;

    default_allocator() noexcept {}
    default_allocator(const default_allocator<T> &alloc) noexcept { *this = alloc; }
    template<class U>
    default_allocator(const default_allocator<U> &alloc) noexcept;
    ~default_allocator() {}

    pointer address(reference x) const noexcept { return &x; }
    const_pointer address(const_reference x) const noexcept { return &x; }
    pointer allocate(size_type n, const void * hint = 0) {
      // the default allocator uses the system new and delete implementations.
      // Therefore it can't do anything with the hint pointer.
      (void)hint; // unused
      return reinterpret_cast<pointer>(::new char[n * sizeof(value_type)]);
    }
    void deallocate(pointer p, size_type n) {
      (void)n; // unused
      ::delete[] p;
    }
    size_type max_size() const noexcept { return ::std::numeric_limits<unsigned>::max(); }
    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
      ::new (p) U(::std::forward<Args>(args)...);
    }
    template<class U>
    void destroy(U* p) {
      p->~U();
    }
  };

  // This allocator takes the element count as a template parameter
  // It then reserves that much stack space to dish out allocations from
  template<typename T, std::size_t N>
  class linear_stack_allocator {
  public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_pointer = const T *;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = ::std::ptrdiff_t;
    template<typename Type>
    using rebind = linear_stack_allocator<Type, N>;
    using propagate_on_container_move_assignment = ::std::false_type;

    linear_stack_allocator() noexcept = default;
    linear_stack_allocator(const linear_stack_allocator<T, N> &alloc) noexcept {}
    ~linear_stack_allocator() = default;

    pointer address(reference x) const noexcept {
      return reinterpret_cast<pointer>(&reinterpret_cast<char&>(x));
    }
    const_pointer address(const_reference x) const noexcept {
      return reinterpret_cast<const_pointer>(&reinterpret_cast<const char&>(x));
    }
    pointer allocate(size_type n, const void * hint = 0) {
      // Since this is a linear allocator, the hint is ignored.
      assert(max_size() >= m_index + n);
      size_t alloc_index{ m_index };
      m_index += n;
      return reinterpret_cast<pointer>(m_storage) + alloc_index;
    }
    void deallocate(pointer p, size_type n) {
      if (p != reinterpret_cast<pointer>(m_storage) - n) {
        return;
      }
      else {
        m_index -= n;
        ::std::memset(m_storage + m_index, 0, n * sizeof(value_type));
      }
    }
    size_type max_size() const noexcept { return N; }
    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
      ::new (p) U(::std::forward<Args>(args)...);
    }
    template<class U>
    void destroy(U* p) {
      p->~U();
    }
  private:
    char m_storage[N * sizeof(value_type)]{ 0 };
    size_t m_index{ 0 };
  };

} // namespace ftl
