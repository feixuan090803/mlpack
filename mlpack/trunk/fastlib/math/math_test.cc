/***
 * @file math_test.cc
 * @author Ryan Curtin
 *
 * Tests for everything in the math:: namespace.
 */
#include "range.h"
#include "math_lib.h"

#define BOOST_TEST_MODULE Math_Test
#include <boost/test/unit_test.hpp>

//using namespace mlpack;
//using namespace mlpack::math;
using namespace math;

/***
 * Verify that the empty constructor creates an empty range.
 */
BOOST_AUTO_TEST_CASE(RangeEmptyConstructor) {
  Range x = Range();

  // Just verify that it is empty.
  BOOST_REQUIRE_GT(x.lo, x.hi);
}

/***
 * Verify that the point constructor correctly creates a range that is just a
 * point.
 */
BOOST_AUTO_TEST_CASE(RangePointConstructor) {
  Range x(10.0);

  BOOST_REQUIRE_CLOSE(x.lo, x.hi, 1e-25);
  BOOST_REQUIRE_SMALL(x.width(), 1e-5);
  BOOST_REQUIRE_CLOSE(x.lo, 10.0, 1e-25);
  BOOST_REQUIRE_CLOSE(x.hi, 10.0, 1e-25);
}

/***
 * Verify that the range constructor correctly creates the range.
 */
BOOST_AUTO_TEST_CASE(RangeConstructor) {
  Range x(0.5, 5.5);

  BOOST_REQUIRE_CLOSE(x.lo, 0.5, 1e-25);
  BOOST_REQUIRE_CLOSE(x.hi, 5.5, 1e-25);
}

/***
 * Test that we get the width correct.
 */
BOOST_AUTO_TEST_CASE(RangeWidth) {
  Range x(0.0, 10.0);

  BOOST_REQUIRE_CLOSE(x.width(), 10.0, 1e-20);

  // Make it empty.
  x.hi = 0.0;

  BOOST_REQUIRE_SMALL(x.width(), 1e-5);

  // Make it negative.
  x.hi = -2.0;

  BOOST_REQUIRE_SMALL(x.width(), 1e-5);

  // Just one more test.
  x.lo = -5.2;
  x.hi = 5.2;

  BOOST_REQUIRE_CLOSE(x.width(), 10.4, 1e-5);
}

/**
 * Test that we get the midpoint correct.
 */
BOOST_AUTO_TEST_CASE(RangeMidpoint) {
  Range x(0.0, 10.0);

  BOOST_REQUIRE_CLOSE(x.mid(), 5.0, 1e-5);

  x.lo = -5.0;

  BOOST_REQUIRE_CLOSE(x.mid(), 2.5, 1e-5);
}

/***
 * Test that we can expand to include other ranges correctly.
 */
BOOST_AUTO_TEST_CASE(RangeIncludeOther) {
  // We need to test both |= and |.
  // We have three cases: non-overlapping; overlapping; equivalent, and then a
  // couple permutations (switch left with right and make sure it still works).
  Range x(0.0, 2.0);
  Range y(3.0, 5.0);

  Range z(0.0, 2.0); // Used for operator|=().
  Range w;
  z |= y;
  w = x | y;

  BOOST_REQUIRE_SMALL(z.lo, 1e-5);
  BOOST_REQUIRE_CLOSE(z.hi, 5.0, 1e-5);
  BOOST_REQUIRE_SMALL(w.lo, 1e-5);
  BOOST_REQUIRE_CLOSE(w.hi, 5.0, 1e-5);

  // Switch operator precedence.
  z = y;
  z |= x;
  w = y | x;

  BOOST_REQUIRE_SMALL(z.lo, 1e-5);
  BOOST_REQUIRE_CLOSE(z.hi, 5.0, 1e-5);
  BOOST_REQUIRE_SMALL(w.lo, 1e-5);
  BOOST_REQUIRE_CLOSE(w.hi, 5.0, 1e-5);

  // Now make them overlapping.
  x = Range(0.0, 3.5);
  y = Range(3.0, 4.0);

  z = x;
  z |= y;
  w = x | y;

  BOOST_REQUIRE_SMALL(z.lo, 1e-5);
  BOOST_REQUIRE_CLOSE(z.hi, 4.0, 1e-5);
  BOOST_REQUIRE_SMALL(w.lo, 1e-5);
  BOOST_REQUIRE_CLOSE(w.hi, 4.0, 1e-5);

  // Switch operator precedence.
  z = y;
  z |= x;
  w = y | x;

  BOOST_REQUIRE_SMALL(z.lo, 1e-5);
  BOOST_REQUIRE_CLOSE(z.hi, 4.0, 1e-5);
  BOOST_REQUIRE_SMALL(w.lo, 1e-5);
  BOOST_REQUIRE_CLOSE(w.hi, 4.0, 1e-5);

  // Now the equivalent case.
  x = Range(0.0, 2.0);
  y = Range(0.0, 2.0);

  z = x;
  z |= y;
  w = x | y;

  BOOST_REQUIRE_SMALL(z.lo, 1e-5);
  BOOST_REQUIRE_CLOSE(z.hi, 2.0, 1e-5);
  BOOST_REQUIRE_SMALL(w.lo, 1e-5);
  BOOST_REQUIRE_CLOSE(w.hi, 2.0, 1e-5);

  z = y;
  z |= x;
  w = y | x;

  BOOST_REQUIRE_SMALL(z.lo, 1e-5);
  BOOST_REQUIRE_CLOSE(z.hi, 2.0, 1e-5);
  BOOST_REQUIRE_SMALL(w.lo, 1e-5);
  BOOST_REQUIRE_CLOSE(w.hi, 2.0, 1e-5);
}

/***
 * Test that we can 'and' ranges correctly.
 */
BOOST_AUTO_TEST_CASE(RangeIntersectOther) {
  // We need to test both &= and &.
  // We have three cases: non-overlapping, overlapping; equivalent, and then a
  // couple permutations (switch left with right and make sure it still works).
  Range x(0.0, 2.0);
  Range y(3.0, 5.0);

  Range z(0.0, 2.0);
  Range w;
  z &= y;
  w = x & y;

  BOOST_REQUIRE_SMALL(z.width(), 1e-5);
  BOOST_REQUIRE_SMALL(w.width(), 1e-5);

  // Reverse operator precedence.
  z = y;
  z &= x;
  w = y & x;

  BOOST_REQUIRE_SMALL(z.width(), 1e-5);
  BOOST_REQUIRE_SMALL(w.width(), 1e-5);

  // Now make them overlapping.
  x = Range(0.0, 3.5);
  y = Range(3.0, 4.0);

  z = x;
  z &= y;
  w = x & y;

  BOOST_REQUIRE_CLOSE(z.lo, 3.0, 1e-5);
  BOOST_REQUIRE_CLOSE(z.hi, 3.5, 1e-5);
  BOOST_REQUIRE_CLOSE(w.lo, 3.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.hi, 3.5, 1e-5);

  // Reverse operator precedence.
  z = y;
  z &= x;
  w = y & x;

  BOOST_REQUIRE_CLOSE(z.lo, 3.0, 1e-5);
  BOOST_REQUIRE_CLOSE(z.hi, 3.5, 1e-5);
  BOOST_REQUIRE_CLOSE(w.lo, 3.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.hi, 3.5, 1e-5);

  // Now make them equivalent.
  x = Range(2.0, 4.0);
  y = Range(2.0, 4.0);

  z = x;
  z &= y;
  w = x & y;

  BOOST_REQUIRE_CLOSE(z.lo, 2.0, 1e-5);
  BOOST_REQUIRE_CLOSE(z.hi, 4.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.lo, 2.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.hi, 4.0, 1e-5);
}

/***
 * Test multiplication of a range with a double.
 */
BOOST_AUTO_TEST_CASE(RangeMultiply) {
  // We need to test both * and *=, as well as both cases of *.
  // We'll try with a couple of numbers: -1, 0, 2.
  // And we'll have a couple of cases for bounds: strictly less than zero;
  // including zero; and strictly greater than zero.
  //
  // So, nine total cases.
  Range x(-5.0, -3.0);
  Range y(-5.0, -3.0);
  Range z;
  Range w;

  y *= -1.0;
  z = x * -1.0;
  w = -1.0 * x;

  BOOST_REQUIRE_CLOSE(y.lo, 3.0, 1e-5);
  BOOST_REQUIRE_CLOSE(y.hi, 5.0, 1e-5);
  BOOST_REQUIRE_CLOSE(z.lo, 3.0, 1e-5);
  BOOST_REQUIRE_CLOSE(z.hi, 5.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.lo, 3.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.hi, 5.0, 1e-5);

  y = x;
  y *= 0.0;
  z = x * 0.0;
  w = 0.0 * x;

  BOOST_REQUIRE_SMALL(y.lo, 1e-5);
  BOOST_REQUIRE_SMALL(y.hi, 1e-5);
  BOOST_REQUIRE_SMALL(z.lo, 1e-5);
  BOOST_REQUIRE_SMALL(z.hi, 1e-5);
  BOOST_REQUIRE_SMALL(w.lo, 1e-5);
  BOOST_REQUIRE_SMALL(w.hi, 1e-5);

  y = x;
  y *= 2.0;
  z = x * 2.0;
  w = 2.0 * x;

  BOOST_REQUIRE_CLOSE(y.lo, -10.0, 1e-5);
  BOOST_REQUIRE_CLOSE(y.hi, -6.0, 1e-5);
  BOOST_REQUIRE_CLOSE(z.lo, -10.0, 1e-5);
  BOOST_REQUIRE_CLOSE(z.hi, -6.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.lo, -10.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.hi, -6.0, 1e-5);

  x = Range(-2.0, 2.0);
  y = x;

  y *= -1.0;
  z = x * -1.0;
  w = -1.0 * x;

  BOOST_REQUIRE_CLOSE(y.lo, -2.0, 1e-5);
  BOOST_REQUIRE_CLOSE(y.hi, 2.0, 1e-5);
  BOOST_REQUIRE_CLOSE(z.lo, -2.0, 1e-5);
  BOOST_REQUIRE_CLOSE(z.hi, 2.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.lo, -2.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.hi, 2.0, 1e-5);

  y = x;
  y *= 0.0;
  z = x * 0.0;
  w = 0.0 * x;

  BOOST_REQUIRE_SMALL(y.lo, 1e-5);
  BOOST_REQUIRE_SMALL(y.hi, 1e-5);
  BOOST_REQUIRE_SMALL(z.lo, 1e-5);
  BOOST_REQUIRE_SMALL(z.hi, 1e-5);
  BOOST_REQUIRE_SMALL(w.lo, 1e-5);
  BOOST_REQUIRE_SMALL(w.hi, 1e-5);

  y = x;
  y *= 2.0;
  z = x * 2.0;
  w = 2.0 * x;

  BOOST_REQUIRE_CLOSE(y.lo, -4.0, 1e-5);
  BOOST_REQUIRE_CLOSE(y.hi, 4.0, 1e-5);
  BOOST_REQUIRE_CLOSE(z.lo, -4.0, 1e-5);
  BOOST_REQUIRE_CLOSE(z.hi, 4.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.lo, -4.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.hi, 4.0, 1e-5);

  x = Range(3.0, 5.0);

  y = x;
  y *= -1.0;
  z = x * -1.0;
  w = -1.0 * x;

  BOOST_REQUIRE_CLOSE(y.lo, -5.0, 1e-5);
  BOOST_REQUIRE_CLOSE(y.hi, -3.0, 1e-5);
  BOOST_REQUIRE_CLOSE(z.lo, -5.0, 1e-5);
  BOOST_REQUIRE_CLOSE(z.hi, -3.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.lo, -5.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.hi, -3.0, 1e-5);

  y = x;
  y *= 0.0;
  z = x * 0.0;
  w = 0.0 * x;

  BOOST_REQUIRE_SMALL(y.lo, 1e-5);
  BOOST_REQUIRE_SMALL(y.hi, 1e-5);
  BOOST_REQUIRE_SMALL(z.lo, 1e-5);
  BOOST_REQUIRE_SMALL(z.hi, 1e-5);
  BOOST_REQUIRE_SMALL(w.lo, 1e-5);
  BOOST_REQUIRE_SMALL(w.hi, 1e-5);

  y = x;
  y *= 2.0;
  z = x * 2.0;
  w = 2.0 * x;

  BOOST_REQUIRE_CLOSE(y.lo, 6.0, 1e-5);
  BOOST_REQUIRE_CLOSE(y.hi, 10.0, 1e-5);
  BOOST_REQUIRE_CLOSE(z.lo, 6.0, 1e-5);
  BOOST_REQUIRE_CLOSE(z.hi, 10.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.lo, 6.0, 1e-5);
  BOOST_REQUIRE_CLOSE(w.hi, 10.0, 1e-5);
}

/***
 * Test equality operator.
 */
BOOST_AUTO_TEST_CASE(RangeEquality) {
  // Three cases: non-overlapping, overlapping, equivalent.  We should also
  // consider empty ranges, which are not necessarily equal...
  Range x(0.0, 2.0);
  Range y(3.0, 5.0);

  // These are odd calls, but we don't want to use operator!= here.
  BOOST_REQUIRE_EQUAL((x == y), false);
  BOOST_REQUIRE_EQUAL((y == x), false);

  y = Range(1.0, 3.0);

  BOOST_REQUIRE_EQUAL((x == y), false);
  BOOST_REQUIRE_EQUAL((y == x), false);

  y = Range(0.0, 2.0);

  BOOST_REQUIRE_EQUAL((x == y), true);
  BOOST_REQUIRE_EQUAL((y == x), true);

  x = Range(1.0, -1.0); // Empty.
  y = Range(1.0, -1.0); // Also empty.

  BOOST_REQUIRE_EQUAL((x == y), true);
  BOOST_REQUIRE_EQUAL((y == x), true);

  // No need to test what it does if the empty ranges are different "ranges"
  // because we are not forcing behavior for that.
}

/***
 * Test inequality operator.
 */
BOOST_AUTO_TEST_CASE(RangeInequality) {
  // We will use the same three cases as the RangeEquality test.
  Range x(0.0, 2.0);
  Range y(3.0, 5.0);

  // Again, odd calls, but we want to force use of operator!=.
  BOOST_REQUIRE_EQUAL((x != y), true);
  BOOST_REQUIRE_EQUAL((y != x), true);

  y = Range(1.0, 3.0);

  BOOST_REQUIRE_EQUAL((x != y), true);
  BOOST_REQUIRE_EQUAL((y != x), true);

  y = Range(0.0, 2.0);

  BOOST_REQUIRE_EQUAL((x != y), false);
  BOOST_REQUIRE_EQUAL((y != x), false);

  x = Range(1.0, -1.0); // Empty.
  y = Range(1.0, -1.0); // Also empty.

  BOOST_REQUIRE_EQUAL((x != y), false);
  BOOST_REQUIRE_EQUAL((y != x), false);
}

/***
 * Test strict less-than operator.
 */
BOOST_AUTO_TEST_CASE(RangeStrictLessThan) {
  // Three cases: non-overlapping, overlapping, and equivalent.
  Range x(0.0, 2.0);
  Range y(3.0, 5.0);

  BOOST_REQUIRE_EQUAL((x < y), true);
  BOOST_REQUIRE_EQUAL((y < x), false);

  y = Range(1.0, 3.0);

  BOOST_REQUIRE_EQUAL((x < y), false);
  BOOST_REQUIRE_EQUAL((y < x), false);

  y = Range(0.0, 2.0);

  BOOST_REQUIRE_EQUAL((x < y), false);
  BOOST_REQUIRE_EQUAL((y < x), false);
}

/***
 * Test strict greater-than operator.
 */
BOOST_AUTO_TEST_CASE(RangeStrictGreaterThan) {
  // Three cases: non-overlapping, overlapping, and equivalent.
  Range x(0.0, 2.0);
  Range y(3.0, 5.0);

  BOOST_REQUIRE_EQUAL((x > y), false);
  BOOST_REQUIRE_EQUAL((y > x), true);

  y = Range(1.0, 3.0);

  BOOST_REQUIRE_EQUAL((x > y), false);
  BOOST_REQUIRE_EQUAL((y > x), false);

  y = Range(0.0, 2.0);

  BOOST_REQUIRE_EQUAL((x > y), false);
  BOOST_REQUIRE_EQUAL((y > x), false);
}

/***
 * Test the Contains() operator.
 */
BOOST_AUTO_TEST_CASE(RangeContains) {
  // We have three Range cases: strictly less than 0; overlapping 0; and
  // strictly greater than 0.  Then the numbers we check can be the same three
  // cases, including one greater than and one less than the range.  This should
  // be about 15 total cases.
  Range x(-2.0, -1.0);

  BOOST_REQUIRE(!x.Contains(-3.0));
  BOOST_REQUIRE(x.Contains(-2.0));
  BOOST_REQUIRE(x.Contains(-1.5));
  BOOST_REQUIRE(x.Contains(-1.0));
  BOOST_REQUIRE(!x.Contains(-0.5));
  BOOST_REQUIRE(!x.Contains(0.0));
  BOOST_REQUIRE(!x.Contains(1.0));

  x = Range(-1.0, 1.0);

  BOOST_REQUIRE(!x.Contains(-2.0));
  BOOST_REQUIRE(x.Contains(-1.0));
  BOOST_REQUIRE(x.Contains(0.0));
  BOOST_REQUIRE(x.Contains(1.0));
  BOOST_REQUIRE(!x.Contains(2.0));

  x = Range(1.0, 2.0);

  BOOST_REQUIRE(!x.Contains(-1.0));
  BOOST_REQUIRE(!x.Contains(0.0));
  BOOST_REQUIRE(!x.Contains(0.5));
  BOOST_REQUIRE(x.Contains(1.0));
  BOOST_REQUIRE(x.Contains(1.5));
  BOOST_REQUIRE(x.Contains(2.0));
  BOOST_REQUIRE(!x.Contains(2.5));

  // Now let's try it on an empty range.
  x = Range();

  BOOST_REQUIRE(!x.Contains(-10.0));
  BOOST_REQUIRE(!x.Contains(0.0));
  BOOST_REQUIRE(!x.Contains(10.0));

  // And an infinite range.
  x = Range(-DBL_MAX, DBL_MAX);

  BOOST_REQUIRE(x.Contains(-10.0));
  BOOST_REQUIRE(x.Contains(0.0));
  BOOST_REQUIRE(x.Contains(10.0));
}