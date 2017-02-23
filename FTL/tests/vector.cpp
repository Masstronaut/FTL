// All content copyright (c) Allan Deutsch 2017. All rights reserved.
#include "../container_traits.hpp"
#include "../vector.hpp"

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <cassert>
#include <iostream>
namespace ftl {

  struct container_test_base {
    void execute() {
      // do setup stuff
      test_impl();
      // do cleanup stuff
    }
    virtual void test_impl() = 0;
    virtual ~container_test_base() = default;
  };
  template<typename T>
  struct container_test : container_test_base {
    virtual void test_impl() override {
      test_begin<T>();

      test_end<T>();
      test_cbegin<T>();
      test_cend<T>();
      test_rbegin<T>();
      test_rend<T>();
      test_crbegin<T>();
      test_crend<T>();
      test_size<T>();
      test_max_size<T>();
      test_resize<T>();
      test_resize_fill<T>();
      test_capacity<T>();
      test_empty<T>();
      test_reserve<T>();
      test_shrink_to_fit<T>();
      test_index_operator<T>();
      test_map_index_operator<T>();
      test_at<T>();
      test_front<T>();
      test_back<T>();
      test_data<T>();
      test_clear<T>();
      test_assign_range<T>();
      test_assign_fill<T>();
      test_assign_il<T>();
      test_push_back<T>();
      test_pop_back<T>();
      test_insert<T>();
      test_insert_n<T>();
      test_insert_range<T>();
      test_insert_il<T>();
      test_erase<T>();
      test_erase_range<T>();
      test_emplace<T>();
      test_emplace_back<T>();
      test_swap<T>();


    }
    T& add_n_elements(T& container, size_t n = 10) {
      if (::ftl::has_assign_fill<T>::value) {
        container.assign(n, typename T::value_type{});
        return container;
      }
      else if (::ftl::has_push_back<T>::value) {
        for (unsigned i{ 0 }; i < 100; ++i) {
          container.push_back(typename T::value_type{});
        }
        return container;
      }
      else if (::ftl::has_assign_range<T>::value) {
        std::vector<typename T::value_type> other(n, typename T::value_type{});
      }
      assert(false && "Failed to find an interface with which n elements could be added to the container.");
    }

    // sample body for tests that are relevant but not implemented yet.
    // std::cout << #TEST " is an available interface for " << typeid(C).name() << ".\n"; 
#define CONTAINER_TEST_DECL( TEST ) template<typename C> void test_##TEST(std::enable_if_t<!::ftl::has_##TEST<C>::value, int> = 0) { std::cout << #TEST " is NOT an available interface for " << typeid(T).name() << ".\n"; }  \
template<typename C> void test_##TEST(std::enable_if_t<::ftl::has_##TEST<C>::value, unsigned> = 0) 


    CONTAINER_TEST_DECL(begin) {
      T container;
      auto it = container.begin();
      assert(it == container.end());
    }
    CONTAINER_TEST_DECL(end) {
      T container;
      auto it = container.end();
      assert(it == container.begin());
    }
    CONTAINER_TEST_DECL(cbegin) {
      T container;
      auto it = container.cbegin();
      assert(it == container.cend());
    }
    CONTAINER_TEST_DECL(cend) {
      T container;
      auto it = container.cend();
      assert(it == container.cbegin());
    }
    CONTAINER_TEST_DECL(rbegin) {
      T container;
      auto it = container.rbegin();
      assert(it == container.rend());
    }
    CONTAINER_TEST_DECL(rend) {
      T container;
      auto it = container.rend();
      assert(it == container.rbegin());
    }
    CONTAINER_TEST_DECL(crbegin) {
      T container;
      auto it = container.crbegin();
      assert(it == container.crend());
    }
    CONTAINER_TEST_DECL(crend) {
      T container;
      auto it = container.rend();
      assert(it == container.rbegin());
    }
    CONTAINER_TEST_DECL(size) {
      T container;
      assert(container.size() == 0 && "Size failed, returning a non-zero value for an empty container.");
      const typename T::size_type element_step_size{ 10 };
      if (::ftl::has_max_size<T>::value) {
        if (container.max_size() < element_step_size * 100) {
          for (typename T::size_type i{ 1 }; i <= container.max_size(); ++i) {
            add_n_elements(container, 1);
            assert(container.size() == i);
          }
        }
      }
      else {
        for (int i{ 1 }; i <= 100; ++i) {
          add_n_elements(container, element_step_size);
          assert(container.size() == i * element_step_size);
        }
      }
    }
    CONTAINER_TEST_DECL(max_size) {
      T container;
      assert(container.max_size() <= container.get_allocator().max_size());
    }
    CONTAINER_TEST_DECL(empty) {
      T container;
      assert(container.empty());
      add_n_elements(container, 1);
      assert(!container.empty());
    }
    CONTAINER_TEST_DECL(capacity) {
      T container;
      assert(container.capacity() == 0);
    }
    CONTAINER_TEST_DECL(resize) {}
    CONTAINER_TEST_DECL(resize_fill) {}
    CONTAINER_TEST_DECL(reserve) {}
    CONTAINER_TEST_DECL(shrink_to_fit) {}

    CONTAINER_TEST_DECL(index_operator) {}
    CONTAINER_TEST_DECL(map_index_operator) {}
    CONTAINER_TEST_DECL(at) {}
    CONTAINER_TEST_DECL(front) {}
    CONTAINER_TEST_DECL(back) {}
    CONTAINER_TEST_DECL(data) {}
    CONTAINER_TEST_DECL(clear) {}
    CONTAINER_TEST_DECL(assign_range) {}
    CONTAINER_TEST_DECL(assign_fill) {}
    CONTAINER_TEST_DECL(assign_il) {}
    CONTAINER_TEST_DECL(pop_back) {}
    CONTAINER_TEST_DECL(push_back) {}
    CONTAINER_TEST_DECL(insert) {}
    CONTAINER_TEST_DECL(insert_n) {}
    CONTAINER_TEST_DECL(insert_range) {}
    CONTAINER_TEST_DECL(insert_il) {}
    CONTAINER_TEST_DECL(erase) {}
    CONTAINER_TEST_DECL(erase_range) {}
    CONTAINER_TEST_DECL(emplace) {}
    CONTAINER_TEST_DECL(emplace_back) {}
    CONTAINER_TEST_DECL(swap) {}



  };

}

int main() {


  ftl::container_test<std::vector<float>> stdvecfltest;
  stdvecfltest.execute();
  ftl::container_test<ftl::vector<float>> ftlvecfltest;
  ftlvecfltest.execute();







  ftl::vector<int> vint;
  assert(vint.capacity() == 0);
  vint.push_back(0);
  assert(vint.size() == 1);
  vint.insert(vint.data(), { -5, -4, -3, -2, -1 });
  assert(vint.size() == 6);
  for (int i{ 0 }; (unsigned)i < vint.size(); ++i) {
    assert(vint[i] == i - 5);
  }
  vint.assign({ 0,1,2,3,4,5,6,7,8,9,10 });
  assert(vint.capacity() >= 11);
  assert(vint.size() == 11);
  for (unsigned i{ 0 }; i < vint.size(); ++i) {
    assert((unsigned)vint[i] == i);
  }

  auto begin = vint.begin();
  auto vint2 = std::move(vint);

  assert(vint.size() == 0);
  assert(vint.capacity() == 0);
  assert(vint2.size() == 11);
  assert(vint2.capacity() == 11);
  assert(vint2.begin() == begin);

  ftl::inline_vector<float, 24> vfin;
  assert(vfin.capacity() == 24);
  vfin.assign(24, 69.420f);
  for (unsigned i{ 0 }; i < vfin.size(); ++i) {
    assert(vfin[i] == 69.420f);
  }
  for (int i{ 0 }; i < 100; ++i) {
    vfin.push_back((float)i);
  }
  std::vector<int> svint{ 5,4,3,2,1 };

  vint.swap(svint);


  ftl::unordered_vector<int> uiv{ 0,1,2,3,4,5,6,7,8,9 };

  uiv.erase(uiv.begin() + 5, uiv.end() - 2);
  for (int i{ 0 }; i < 5; ++i) {
    assert(uiv[i] == i);
  }
  uiv.erase(uiv.begin());

  return 0;
}