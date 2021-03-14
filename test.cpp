#include "slice.hpp"
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("const Slice::slice() and const Slice::operator[]") {
  // some buffer to slice
  unsigned char arr[10];
  // populate arr
  for (unsigned char i = 0; i < 10; ++i)
    arr[i] = i;
  // initialize the slice
  const slice::Bytes slice(arr, 10);
  // take slice by size
  const auto subj1 = slice.slice(5);
  REQUIRE(subj1.length() == 5);
  for (unsigned char i = 0; i < 5; ++i)
    REQUIRE(subj1[i] == i);
  const auto subj2 = slice.slice(3, 8);
  for (unsigned char i = 0, v = 3; v < 8; ++i, ++v)
    REQUIRE(subj2[i] == v);
  const auto subj3 = slice.sliceFrom(4);
  for (unsigned char i = 0, v = 4; v < 10; ++i, ++v)
    REQUIRE(subj3[i] == v);
}

TEST_CASE("Slice::operator[] when a 'range' is passed") {
  // some buffer to slice
  unsigned char arr[10];
  // populate arr
  for (unsigned char i = 0; i < 10; ++i)
    arr[i] = i;
  // initialize the slice
  const slice::Bytes slice(arr, 10);
  // Take a slice by the [{}] operator
  const auto subj4 = slice[{3, 8}];
  for (unsigned char i = 0, v = 3; v < 8; ++i, ++v)
    REQUIRE(subj4[i] == v);
}
TEST_CASE("Slice::operator[]= for setting a value") {
  // some buffer to slice
  unsigned char arr[10];
  // populate arr
  for (unsigned char i = 0; i < 10; ++i)
    arr[i] = i;
  // initialize the slice
  slice::Bytes slice(arr, 10);
  // set value 3 to 6
  slice[2] = 6;
  for (size_t i = 0; i < 10; i++)
    if (i == 2)
      REQUIRE(slice[i] == 6);
    else
      REQUIRE(slice[i] == i);
}
TEST_CASE("double each value of a mutable Slice") {
  // given some buffer
  int intBuf[10];
  // filled with data
  for (int i = 0; i < 10; ++i)
    intBuf[i] = i;
  // take a slice of that buffer
  slice::Slice<int> intSlice(intBuf, 10);
  for (auto& v : intSlice)
    v *= 2;
  int i = 0;
  for (auto v : intSlice)
    REQUIRE(v == ((i++) * 2));
  // also, the original buffer reflects the changes
  for (i = 0; i < 10; ++i)
    REQUIRE(intBuf[i] == (i * 2));
}