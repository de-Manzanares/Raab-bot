/**
* @file Vector.hpp
 * debug build bounds-checking vector
 */

#pragma once

#include <cassert>
#include <vector>

template <typename T> class Vector : public std::vector<T> {
 public:
  using reference       = std::vector<T>::reference;
  using const_reference = std::vector<T>::const_reference;
  using size_type       = std::vector<T>::size_type;

  constexpr reference operator[](size_type pos)
  {
    assert(pos < this->size());
    return std::vector<T>::operator[](pos);
  }

  constexpr const_reference operator[](size_type pos) const
  {
    assert(pos < this->size());
    return std::vector<T>::operator[](pos);
  }
};
