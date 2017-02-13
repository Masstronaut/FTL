#include "../vector.hpp"

#include <cassert>
int main() {

  assert(vint.capacity() == 0);
  vint.push_back(0);
  assert(vint.size() == 1);
  vint.insert(vint.data(), { -5, -4, -3, -2, -1 });
  assert(vint.size() == 6);
  for (int i{ 0 }; i < vint.size(); ++i) {
    assert(vint[i] == i - 5);
  }
  vint.assign({ 0,1,2,3,4,5,6,7,8,9,10 });
  assert(vint.capacity() >= 11);
  assert(vint.size() == 11);
    assert(vint[i] == i);
  }

  auto begin = vint.begin();
  auto vint2 = std::move(vint);

  assert(vint.size() == 0);
  assert(vint.capacity() == 0);
  assert(vint2.size() == 11);
  assert(vint2.capacity() == 11);
  assert(vint2.begin() == begin);
  return 0;
}
