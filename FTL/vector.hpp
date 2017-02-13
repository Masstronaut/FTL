// All content copyright (C) Allan Deutsch 2017. All rights reserved.

#pragma once

#include <limits> // needed for allocator::max_size
#include <iterator> // std::reverse_iterator<>
#include <utility> // std::distance
#include <memory>


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
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  template<typename Type>
  using rebind = default_allocator<Type>;
  using propagate_on_container_move_assignment = std::false_type;

  default_allocator() noexcept {}
  default_allocator(const default_allocator<T> &alloc) noexcept {}
  template<class U>
  default_allocator(const default_allocator<U> &alloc) noexcept;
  ~default_allocator() {}

  pointer address(reference x) const noexcept { return &x; }
  const_pointer address(const_reference x) const noexcept { return &x; }
  pointer allocate(size_type n, const void * hint = 0) {
    // the default allocator uses the system new and delete implementations.
    // Therefore it can't do anything with the hint pointer.
    return reinterpret_cast<pointer>(new char[n * sizeof(value_type)]);
  }
  void deallocate(pointer p, size_type n) { delete[] p; }
  size_type max_size() const noexcept { return std::numeric_limits<unsigned>::max(); }
  template<typename U, typename... Args>
  void construct(U* p, Args&&... args) {
    new (p) U(std::forward<Args>(args)...);
  }
  template<class U>
  void destroy(U* p) {
    p->~U();
  }
};


// vector implementation with std::vector parity
template<typename T, typename Alloc = default_allocator<T>>
class vector {
public:
  // type aliases
  using size_type = size_t;
  using allocator_type = Alloc;
  using value_type = T;
  using iterator = T*;
  using const_iterator = const T *;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // constructors
  // default
  vector();
  explicit vector(const allocator_type& alloc);
  // fill
  explicit vector(size_type n, const allocator_type& alloc = allocator_type{});
  vector(size_type n, const value_type& val, const allocator_type& alloc = allocator_type{});
  // range
  template<typename InputIterator>
  vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type{});
  // copy
  vector(const vector<T, Alloc> &other);
  vector(const vector<T, Alloc>& other, const allocator_type& alloc);
  // move
  vector(vector<T, Alloc> &&other);
  vector(vector<T, Alloc> &&other, const allocator_type& alloc);
  // initializer list
  vector(std::initializer_list<value_type> il, const allocator_type& alloc = allocator_type{});

  virtual ~vector();

  // assignment
  vector<T, Alloc>& operator=(const vector<T, Alloc> &other);
  vector<T, Alloc>& operator=(vector<T, Alloc> &&other);
  vector<T, Alloc>& operator=(std::initializer_list<value_type> il);

  // iterators
  iterator begin() const noexcept;
  iterator end() const noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;
  reverse_iterator rbegin() const noexcept;
  reverse_iterator rend() const noexcept;
  const_reverse_iterator crbegin() const noexcept;
  const_reverse_iterator crend() const noexcept;

  // Modifiers
  void push_back(const T &data);
  void pop_back();
  template<typename... Args>
  void emplace_back(Args&&... args);

  template<typename InputIterator>
  void assign(InputIterator first, InputIterator last);
  void assign(size_type n, const value_type &val);
  void assign(std::initializer_list<value_type> il);

  iterator insert(const_iterator position, const value_type &val);
  iterator insert(const_iterator position, size_type n, const value_type &val);
  template<typename InputIterator>
  iterator insert(const_iterator position, InputIterator first, InputIterator last);
  iterator insert(const_iterator position, value_type &&val);
  iterator insert(const_iterator position, std::initializer_list<value_type> il);

  iterator erase(const_iterator position);
  iterator erase(const_iterator first, const_iterator last);

  template<typename Vector>
  void swap(Vector &other);

  void clear() noexcept;

  template<typename... Args>
  iterator emplace(const_iterator position, Args&&... args);

  // element access
  reference front() const;
  reference back() const;
  pointer data() const noexcept;
  pointer at(size_type n) noexcept;
  const_pointer at(size_type n) const noexcept;
  reference operator[](size_type n);
  const_reference operator[](size_type n) const;

  // capacity
  size_type size() const noexcept;
  size_type capacity() const noexcept;
  size_type max_size() const noexcept;
  bool empty() const noexcept;
  void resize(size_type elements);
  void reserve(size_type elements);
  void shrink_to_fit();

  // allocator
  allocator_type get_allocator() const noexcept;

protected:
  vector(iterator Begin, iterator End, size_type capacity);

  virtual void grow();
  bool full() const noexcept;
  //@@ALLAN: come up with a better name and change it
  void make_capacity();


  pointer m_begin{ nullptr };
  pointer m_end{ nullptr };
  size_type m_capacity{ 0u };
  allocator_type m_alloc{};
};

// constructors
// default
template<typename T, typename Alloc>
vector<T, Alloc>::vector() { }
template<typename T, typename Alloc>
vector<T, Alloc>::vector(const allocator_type& alloc) 
  : m_alloc(alloc) {
}
// fill
template<typename T, typename Alloc>
vector<T, Alloc>::vector(size_type n, const allocator_type& alloc)
  : m_alloc(alloc) {
  reserve(n);
}
template<typename T, typename Alloc>
vector<T, Alloc>::vector(size_type n, const value_type& val, const allocator_type& alloc)
  : m_alloc(alloc) {
  assign(n, val);
}
// range
template<typename T, typename Alloc>
template<typename InputIterator>
vector<T, Alloc>::vector(InputIterator first, InputIterator last, const allocator_type& alloc)
  : m_alloc(alloc) {
  assign(first, last);
}
// copy
template<typename T, typename Alloc>
vector<T, Alloc>::vector(const vector<T, Alloc> &other) 
  : m_alloc(other.get_allocator()) {
  assign(other.begin(), other.end());
}

template<typename T, typename Alloc>
vector<T, Alloc>::vector(const vector<T, Alloc>& other, const allocator_type& alloc)
  : m_alloc(alloc) {
  assign(other.begin(), other.end());
}
// move
template<typename T, typename Alloc>
vector<T, Alloc>::vector(vector<T, Alloc> &&other)
  : m_begin(other.m_begin)
  , m_end(other.m_end)
  , m_capacity(other.m_capacity) {
  other.m_begin = nullptr;
  other.m_end = nullptr;
  other.m_capacity = 0;
}
template<typename T, typename Alloc>
vector<T, Alloc>::vector(vector<T, Alloc> &&other, const allocator_type& alloc)
  : m_begin(other.begin())
  , m_end(other.end())
  , m_capacity(other.capacity())
  , m_alloc(alloc) {
  other.m_begin = other.m_end = nullptr;
  other.m_capacity = 0u;
}
// initializer list
template<typename T, typename Alloc>
vector<T, Alloc>::vector(std::initializer_list<value_type> il, const allocator_type& alloc)
  : m_alloc(alloc) {
  reserve(il.size());
  assign(il);
}

template<typename T, typename Alloc>
vector<T, Alloc>::~vector() {
  clear();
  delete[] m_begin;
}

// assignment
template<typename T, typename Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc> &other) {
  clear();
  assign(other.begin(), other.end());
  return *this;
}
template<typename T, typename Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(vector<T, Alloc> &&other) {
  clear();
  m_begin = other.m_begin;
  m_end = other.m_end;
  m_capacity = other.m_capacity;
  other.m_begin = nullptr;
  other.m_end = nullptr;
  other.m_capacity = 0;
  return *this;
}
template<typename T, typename Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(std::initializer_list<T> il) {
  clear();
  assign(il);
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::size_type vector<T, Alloc>::size() const noexcept {
  return m_end - m_begin;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::size_type vector<T, Alloc>::capacity() const noexcept {
  return m_capacity;
}

template<typename T, typename Alloc>
bool vector<T, Alloc>::full() const noexcept {
  return static_cast<unsigned long long>(std::distance(m_begin, m_end) ) >= m_capacity;
}

template<typename T, typename Alloc>
void vector<T, Alloc>::make_capacity() {
  if (full()) grow();
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::size_type vector<T, Alloc>::max_size() const noexcept {
  return m_alloc.max_size();
}
template<typename T, typename Alloc>
bool vector<T, Alloc>::empty() const noexcept {
  return m_begin == m_end;
}

template<typename T, typename Alloc>
void vector<T, Alloc>::resize(size_type elements) {
  if (capacity < elements) {
    reserve(elements);
    for (size_type i{ size() }; i < elements; ++i) {
      emplace_back();
    }
  }
  else {
    while (size() > elements) {
      pop_back();
    }
  }
}

template<typename T, typename Alloc>
void vector<T, Alloc>::reserve(size_type elements) {
  if (capacity() >= elements) return;

  pointer new_buffer = m_alloc.allocate(elements);
  pointer temp_begin = begin(), temp_end = end();
  m_begin = m_end = new_buffer;
  size_type old_capacity{ capacity() };
  m_capacity = elements;
  assign(temp_begin, temp_end);

  for (auto it{ temp_begin }; it != temp_end; ++it) {
    m_alloc.destroy(&*it);
  }
  m_alloc.deallocate(temp_begin, old_capacity);
}
template<typename T, typename Alloc>
void vector<T, Alloc>::shrink_to_fit() {
  // The behavior of this function is implementation defined.
  // This implementation chooses to avoid reallocating into a smaller space.
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::begin() const noexcept {
  return m_begin;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::end() const noexcept {
  return m_end;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_iterator vector<T, Alloc>::cbegin() const noexcept {
  return m_begin;
}
template<typename T, typename Alloc>
typename vector<T, Alloc>::const_iterator vector<T, Alloc>::cend() const noexcept {
  return m_end;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::reverse_iterator vector<T, Alloc>::rbegin() const noexcept {
  return reverse_iterator{ m_end - 1 };
}
template<typename T, typename Alloc>
typename vector<T, Alloc>::reverse_iterator vector<T, Alloc>::rend() const noexcept {
  return reverse_iterator{ m_begin - 1 };
}
template<typename T, typename Alloc>
typename vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::crbegin() const noexcept {
  return const_reverse_iterator{ m_end - 1 };
}
template<typename T, typename Alloc>
typename vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::crend() const noexcept {
  return const_reverse_iterator{ m_begin - 1 };
}

template<typename T, typename Alloc>
void vector<T, Alloc>::pop_back() {
  m_alloc.destroy(--m_end);
}

template<typename T, typename Alloc>
void vector<T, Alloc>::push_back(const T &data) {
  make_capacity();
  m_alloc.construct(m_end++, data);
}


template<typename T, typename Alloc>
template<typename... Args>
void vector<T, Alloc>::emplace_back(Args&&... args) {
  make_capacity();
  m_alloc.construct(m_end++, std::forward<Args>(args)...);
}





template<typename T, typename Alloc>
template<typename InputIterator>
void vector<T, Alloc>::assign(InputIterator first, InputIterator last) {
  clear();
  reserve( static_cast<size_type>( std::distance(first, last) ) );
  for (auto it{ first }; it != last; ++it) {
    emplace_back(*it);
  }
}

template<typename T, typename Alloc>
void vector<T, Alloc>::assign(size_type n, const value_type &val) {
  clear();
  reserve(n);
  for (size_type i{ 0 }; i < n; ++i) {
    push_back(val);
  }
}

template<typename T, typename Alloc>
void vector<T, Alloc>::assign(std::initializer_list<typename vector<T, Alloc>::value_type> il) {
  clear();
  reserve(il.size());
  for (auto it{ std::begin(il) }; it != std::end(il); ++it) {
    push_back(*it);
  }
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const_iterator position, const value_type &val) {
  iterator it{ const_cast<iterator>(position) };
  push_back(val);
  std::rotate(it, end() - 1, end());
  return it;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const_iterator position, size_type n, const value_type &val) {
  iterator it{ const_cast<iterator>(position) }, n_first{ end() };
  for (size_type i{ 0 }; i < n; ++i) {
    push_back(val);
  }
  std::rotate(it, n_first, end());
  return it;
}

template<typename T, typename Alloc>
template<typename InputIterator>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const_iterator position, InputIterator first, InputIterator last) {
  iterator it{ const_cast<iterator>(position) }, n_first{ end() };
  for (auto it{ first }; it != last; ++it) {
    push_back(*it);
  }
  std::rotate(it, n_first, end());
  return it;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const_iterator position, value_type &&val) {
  iterator it{ const_cast<iterator>(position) };
  emplace_back(val);
  std::rotate(it, end() - 1, end());
  return it;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const_iterator position, std::initializer_list<value_type> il) {
  return insert(position, std::begin(il), std::end(il));
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(const_iterator position) {
  iterator follow{ const_cast<iterator>(position) };
  iterator lead{ follow + 1 };
  for (; lead != end(); ++follow, ++lead) {
    m_alloc.destroy(follow);
    m_alloc.construct(follow, std::move(*lead));
  }
  pop_back();
  return const_cast<iterator>(position);
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(const_iterator first, const_iterator last) {
  iterator follow{ const_cast<iterator>(first) };
  iterator lead{ const_cast<iterator>(last) };

  for (; lead != end(); ++lead, ++follow) {
    m_alloc.destroy(follow);
    m_alloc.construct(follow, std::move(*lead));
  }
  iterator new_end{ follow };
  for (; follow != end(); ++follow) {
    m_alloc.destroy(follow);
  }
  m_end = new_end;
}

template<typename T, typename Alloc>
template<typename Vector>
void vector<T, Alloc>::swap(Vector &other) {
  static_assert(std::is_same<value_type, typename Vector::value_type>::value, "Swapping the elements of two vectors requires that they have the same value type.");
  // @@ALLAN: redo this correctly for any vector type
  T *temp_begin = begin(), *temp_end = end();
  size_type temp_capacity{ m_capacity };
  m_begin = other.m_begin;
  m_end = other.m_end;
  m_capacity = other.m_capacity;

  other.m_begin = temp_begin;
  other.m_end = temp_end;
  other.capacity = temp_capacity;
}

template<typename T, typename Alloc>
void vector<T, Alloc>::clear() noexcept {
  while (size()) pop_back();
}

template<typename T, typename Alloc>
template<typename... Args>
typename vector<T, Alloc>::iterator vector<T, Alloc>::emplace(const_iterator position, Args&&... args) {
  iterator it{ const_cast<iterator>(position) }, n_first{ end() };
  emplace_back(std::forward<Args>(args)...);
  std::rotate(it, n_first, end());
  return it;
}

template<typename T, typename Alloc>
void vector<T, Alloc>::grow() {
  size_type new_capacity{ (capacity() != 0) ? (capacity() * 2) : 10 };
  reserve(new_capacity);
}




template<typename T, typename Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::front() const {
  return *m_begin;
}
template<typename T, typename Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::back() const {
  return *(m_end - 1);
}
template<typename T, typename Alloc>
typename vector<T, Alloc>::pointer vector<T, Alloc>::data() const noexcept {
  return m_begin;
}
template<typename T, typename Alloc>
typename vector<T, Alloc>::pointer vector<T, Alloc>::at(size_type n) noexcept {
  if (m_begin + n < m_end) return m_begin + n;
  else return nullptr;
}
template<typename T, typename Alloc>
typename vector<T, Alloc>::const_iterator vector<T, Alloc>::at(size_type n) const noexcept {
  if (m_begin + n < m_end) return m_begin + n;
  else return nullptr;
}
template<typename T, typename Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::operator[](size_type n) {
  return *(m_begin + n);
}
template<typename T, typename Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::operator[](size_type n) const {
  return *(m_begin + n);
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::allocator_type vector<T, Alloc>::get_allocator() const noexcept {
  return m_alloc;
}




}
