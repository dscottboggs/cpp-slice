#pragma once
#include <cstddef>
#include <cstring>

#include <stdexcept>
#include <utility>
#include <functional>
#include <algorithm>

#include <fmt/format.h>

namespace slice {
  using std::get;
  template <typename T> class Slice {
    T* _ptr;
    size_t _length;

    size_t validateIndex(long int index) const {
      if (index < 0)
        index += _length;
      if (index >= _length or index < 0)
        throw new OutOfBounds(index, _length);
      return index;
    }

  public:
    class OutOfBounds : public std::runtime_error {
      long int _requested_idx;
      size_t _length;

    public:
      OutOfBounds(int r, size_t l)
        : _requested_idx(r), _length(l),
        std::runtime_error(fmt::format(
          "requested index {} was larger than the slice of size {}", r,
          l)) {}
      long int requested_idx() const { return _requested_idx; }
      size_t length() const { return _length; }
    };
    // constructors
    explicit Slice(T* ptr, size_t size) : _ptr(ptr), _length(size) {}
    Slice(Slice&) = default;
    Slice& operator=(Slice& it) {
      return { it._ptr, it._length };
    }
    Slice(Slice&&) = default;
    ~Slice() = default;
    // iteration
    typedef T* iterator;
    typedef const T* const_iterator;
    iterator begin() noexcept { return _ptr; }
    iterator end() noexcept { return _ptr + _length; }
    const_iterator begin() const noexcept { return _ptr; }
    const_iterator end() const noexcept { return _ptr + _length; }

    // slicing
    Slice<T> slice(long int size) const { return slice(0, size); }
    Slice<T> slice(long int start, long int end) const {
      while (end < 0)
        end = _length + end;
      if ((end - start) > _length) throw new OutOfBounds(end - start, _length - start);
      return Slice<T>(_ptr + start, end - start);
    }
    Slice<T> sliceFrom(long int offset) const {
      return slice(offset, _length);
    }
    Slice<T>& operator[](std::tuple<long int, long int> range) {
      return slice(get<0>(range), get<1>(range));
    }
    Slice<T> operator[](std::tuple<long int, long int> range) const {
      return slice(get<0>(range), get<1>(range));
    }

    // value access
    T& operator[](long int index) { return _ptr[validateIndex(index)]; }
    T operator[](long int index) const { return _ptr[validateIndex(index)]; }

    // length
    inline size_t length() const {
      return _length;
    }

    // Access to the underlying buffer
    inline T* ptr() { return _ptr; }
    inline const T* ptr() const { return _ptr; }
    T** operator&() { return &_ptr; }
    const T** operator&() const { return &_ptr; }

    // simulate a pointer, why not?
    const T& operator*() const {
      return (*this)[0];
    }
    T& operator*() { return (*this)[0]; }

    // comparison
    constexpr bool operator==(const Slice<T>& other) const {
      return std::equal(begin(), end(), other.begin());
    }

  };
  using Bytes = Slice<unsigned char>;
} // namespace slice
