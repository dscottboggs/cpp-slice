#pragma once
#include <cstddef>
#include <fmt/format.h>
#include <stdexcept>
#include <utility>
#include <functional>

namespace slice {
  using std::get;
  template <typename T, bool owned = false> class Slice {
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
      size_t requested_idx() const noexcept { return _requested_idx; }
      size_t length() const noexcept { return _length; }
    };
    // constructors
    explicit Slice(typename std::enable_if<not owned, T>* ptr, size_t size) : _ptr(ptr), _length(size) {}
    explicit Slice(typename std::enable_if<owned, T> value, size_t size) : _ptr(new T[size]), _length(size) {
      for (size_t i = 0; i < _length; ++i) _ptr[i] = value;
    }
    explicit Slice(
      typename std::enable_if<owned, size_t> size,
      std::function<T(size_t)> builder
    ) : _ptr(new T[size], _length(size)) {
      for (size_t i = 0; i < _length; ++i)
        _ptr[i] = builder(i);
    }

    Slice(Slice<T>&) = default;
    Slice& operator=(Slice& it) {
      return { it._ptr, it._length };
    }
    Slice(Slice&&) = default;

    // destructor
    ~Slice() {
      if (owned)
        delete _ptr[_length];
    }

    // iteration
    typedef T* iterator;
    typedef const T* const_iterator;
    iterator begin() noexcept { return _ptr; }
    iterator end() noexcept { return _ptr + _length; }
    const_iterator begin() const noexcept { return _ptr; }
    const_iterator end() const noexcept { return _ptr + _length; }
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
    inline size_t length() const noexcept {
      return _length;
    }
    // simulate a pointer, why not?
    T operator*() const {
      return (*this)[0];
    }
    T& operator*() { return (*this)[0]; }
  };
  using Bytes = Slice<unsigned char>;
} // namespace slice
