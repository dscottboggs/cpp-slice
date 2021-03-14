# Slice

A single-header implementation of a simple "slice" data-type. That is, a
pointer with associated size. Provides an iterator implementation, bounds-
checked `operator[]` implementation, and more.

This data-type lies somewhere between a `std::array` and a `std::vector`, is
useful in embedded applications, and is inspired by Go's
[`slice`](https://blog.golang.org/slices-intro), Crystal's
[`Slice`](https://crystal-lang.org/api/0.36.1/Slice.html), Nim's
[`Slice`](https://nim-lang.org/docs/tut1.html#advanced-types-slices), or Rust's
[`std::slice`](https://doc.rust-lang.org/std/slice/) data-types. It offers a bounds-checked view into a borrowed in-memory buffer.

## Examples

### Creating a slice

```c++
char buffer[0x1000];
slice::Slice<char> mySlice(buffer, 0x1000);
```

### Accessing an element of the slice

```c++
memset(buffer, 0, 0x1000);
mySlice[15] == 0; // true
```

### Setting a value

```c++
mySlice[8] = 64;
mySlice[8] == 64; // true
```

### Getting a new slice from an existing slice

```c++
auto subSlice = mySlice[{100, 200}];
subSlice.length() // 100
subSlice[0] = 101;
mySlice[100] == 101; // true
```

### Iteration

```c++
// given some buffer
int intBuf[10];
// filled with data
for(int i = 0; i < 10; ++i)
  intBuf[i] = i;
// take a slice of that buffer
slice::Slice<int> intSlice(intBuf, 10);
// the data is present
int i = 0;
for(auto v : intSlice)
  REQUIRE(v == i++);
// double each value
for(auto &v : intSlice)
  v *= 2;
i = 0;
for(auto v : intSlice)
  REQUIRE(v == ((i++) * 2));
```
