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

/**
 * n-dimensional array of a homogeneous type
 * @note see "Variadic Templates Multidimensional Array Container" at https://tinyurl.com/4fj779wk
 */
export template <typename T, std::size_t... Nx> struct ndArray;

template <typename T, std::size_t N> struct ndArray<T, N> : Array<T, N> {};

template <typename T, std::size_t N, std::size_t... Nx>
struct ndArray<T, N, Nx...> : ndArray<ndArray<T, Nx...>, N> {};
