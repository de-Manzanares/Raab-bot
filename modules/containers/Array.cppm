module;

#include <array>
#include <cassert>

export module Array;

export template <typename T, std::size_t N> struct Array : std::array<T, N> {
  using reference       = std::array<T, N>::reference;
  using const_reference = std::array<T, N>::const_reference;
  using size_type       = std::array<T, N>::size_type;

  constexpr reference operator[](size_type pos)
  {
    assert(pos < this->size());
    return std::array<T, N>::operator[](pos);
  }

  constexpr const_reference operator[](size_type pos) const
  {
    assert(pos < this->size());
    return std::array<T, N>::operator[](pos);
  }
};
