/**
 * Simple comparison of algorithms with conditions and branch-free algorithms.
 *
 * THE PROGRAM IS RELEASED UNDER THE WTF PUBLIC LICENSE 
 *          http://www.wtfpl.net/txt/copying/
 *
 * Functions to compete: sign, abs, min and max (signed and unsigned).
 *   Suffix '0' - Functions with condition.
 *   Suffix '1' - Branch free functions.
 *
 * Compilation is correct in 
 *   - VC++ 2015 
 *   - GCC 4.8.1 (From MinGW).
 *
 * Artem M. Karavaev, 13.04.2016 (http://zealcomputing.ru)
 * `````````````````  ``````````  ```````````````````````
 */

#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstdint>

// Testing function 'F' with time duration variable 'D'
#define TEST(F, D)      \
  begin = get_now();    \
  a = 0;                \
  do {                  \
    a = 19993*a+1;      \
    s+=(u32)F;          \
  } while (a != 0);   \
  end = get_now();      \
  auto D = end - begin;   

// Output working time of -0's and -1's functions
#define OUT(S, N) output_vs (S, N##0_time - empty, N##1_time - empty);



typedef int32_t  i32;
typedef uint32_t u32;
typedef int8_t   sign_t;

const u32 SHIFT = 31;

using namespace std;



// Some helpers

static chrono::high_resolution_clock::time_point get_now () {
  return chrono::high_resolution_clock::now();
}

static void output_vs (const char * str, const chrono::high_resolution_clock::duration &duration0, const chrono::high_resolution_clock::duration &duration1) {
  cout << str << ": " << fixed << setprecision(2) << chrono::duration_cast <chrono::duration<double>> (duration0).count() << " vs "
                      << fixed << setprecision(2) << chrono::duration_cast <chrono::duration<double>> (duration1).count() << endl;
}



// Sign functions

static sign_t sign0 (i32 a) {
  if (a>0)  return +1;
  if (a<0)  return -1;
  return 0;
}

static sign_t sign1 (i32 a) {
  return (a >> SHIFT) | ((u32)(-a) >> SHIFT);
}



// Abs functions

static u32 abs0 (i32 a) {
  if (a<0)  return -a;
  return a;
}

static u32 abs1 (i32 a) {
 const i32 b = a >> SHIFT; 
 return (a+b) ^ b;
}



// Max/min functions

static i32 mini0 (i32 a, i32 b) {
  return a>b ? b:a;
}

static i32 maxi0 (i32 a, i32 b) {
  return a<b ? b:a;
}

static i32 mini1 (i32 a, i32 b) {
  i32 d = a-b;
  return a - (d&(~(d^((a^b)&(d^a))) >> SHIFT));
}

static i32 maxi1 (i32 a, i32 b) {
  i32 d = a-b;
  return b + (d&(~(d^((a^b)&(d^a))) >> SHIFT));
}

static u32 minu0 (u32 a, u32 b) {
  return a>b ? b:a;
}

static u32 maxu0 (u32 a, u32 b) {
  return a<b ? b:a;
}

static u32 minu1 (u32 a, u32 b) {
  u32 d = a-b;
  return a - (d&~(int((~a&b)|(~(a^b)&d)) >> SHIFT));
}

static u32 maxu1 (u32 a, u32 b) {
  u32 d = a-b;
  return b + (d&~(int((~a&b)|(~(a^b)&d)) >> SHIFT));
}



int main() {

  u32 a, s = 0;
  chrono::high_resolution_clock::time_point begin, end;

  // Empty loop
  TEST (a, empty)

  // Testing the functions
  TEST (sign0(i32(a)), sign0_time)
  TEST (sign1(i32(a)), sign1_time)

  TEST (abs0(i32(a)), abs0_time)
  TEST (abs1(i32(a)), abs1_time)

  TEST (mini0(i32(a), i32(~a)), mini0_time)
  TEST (maxi0(i32(a), i32(~a)), maxi0_time)  
  TEST (mini1(i32(a), i32(~a)), mini1_time)
  TEST (maxi1(i32(a), i32(~a)), maxi1_time)
  
  TEST (minu0(u32(a), u32(~a)), minu0_time)
  TEST (maxu0(u32(a), u32(~a)), maxu0_time)  
  TEST (minu1(u32(a), u32(~a)), minu1_time)
  TEST (maxu1(u32(a), u32(~a)), maxu1_time)

  // Output the result
  OUT ("sign", sign)
  OUT (" abs", abs)
  OUT ("mini", mini)
  OUT ("maxi", maxi)
  OUT ("minu", minu)
  OUT ("maxu", maxu)

  // Output 's' to avoid aggressive optimization (removing useless loops).
  // ('volatile' is not a good way to achieve the same effect).
  cerr << s << endl;
  
  return 0;
}
