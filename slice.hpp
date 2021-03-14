#pragma once
#include <cstddef>
#include <fmt/format.h>
#include <stdexcept>
#include <utility>

namespace slice {
  using std::get;
  template <typename T> class Slice {
    T* _ptr;
    size_t _length;

    size_t validateIndex(size_t index) const {
      while (index < 0)
        index += _length;
      if (index >= _length)
        throw new OutOfBounds(index, _length);
      return index;
    }

  public:
    class OutOfBounds : public std::runtime_error {
      size_t _requested_idx;
      size_t _length;

    public:
      OutOfBounds(size_t r, size_t l)
        : _requested_idx(r), _length(l),
        std::runtime_error(fmt::format(
          "requested index {} was larger than the slice of size {}", r,
          l)) {}
      size_t requested_idx() const { return _requested_idx; }
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
    iterator begin() { return _ptr; }
    iterator end() { return _ptr + _length; }
    const_iterator begin() const { return _ptr; }
    const_iterator end() const { return _ptr + _length; }
    // slicing
    Slice<T> slice(size_t size) const { return slice(0, size); }
    Slice<T> slice(size_t start, size_t end) const {
      while (end < 0)
        end = _length + end;
      if ((end - start) > _length) throw new OutOfBounds(end - start, _length - start);
      return Slice<T>(_ptr + start, end - start);
    }
    Slice<T> sliceFrom(size_t offset) const {
      return slice(offset, _length);
    }
    Slice<T>& operator[](std::tuple<size_t, size_t> range) {
      return slice(get<0>(range), get<1>(range));
    }
    Slice<T> operator[](std::tuple<size_t, size_t> range) const {
      return slice(get<0>(range), get<1>(range));
    }
    // value access
    T& operator[](size_t index) { return _ptr[validateIndex(index)]; }
    T operator[](size_t index) const { return _ptr[validateIndex(index)]; }
    // length
    inline size_t length() const {
      return _length;
    }
    // simulate a pointer, why not?
    const T& operator*() const {
      return (*this)[0];
    }
  };
  using Bytes = Slice<unsigned char>;
} // namespace slice
