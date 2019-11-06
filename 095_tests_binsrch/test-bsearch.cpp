#include <math.h>

#include <cstdio>
#include <cstdlib>

#include "function.h"

int binarySearchForZero(Function<int, int> * f, int low, int high);

class SinFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 10000000 * (sin(arg / 100000.0) - 0.5); }
};

class pos_const_function : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 5; }
};

class neg_const_function : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return -2; }
};

class linear__function : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return arg; }
};

class CountedIntFn : public Function<int, int> {
 protected:
  unsigned remaining;
  Function<int, int> * f;
  const char * mesg;

 public:
  CountedIntFn(unsigned n, Function<int, int> * fn, const char * m) :
      remaining(n),
      f(fn),
      mesg(m) {}
  virtual int invoke(int arg) {
    if (remaining == 0) {
      fprintf(stderr, "Too many function invocations in %s\n", mesg);
      exit(EXIT_FAILURE);
    }
    remaining--;
    return f->invoke(arg);
  }
};

void check(Function<int, int> * f,
           int low,
           int high,
           int expected_ans,
           const char * mesg) {
  int invok;

  if (high > low) {
    invok = log(high - low) / log(2) + 1;
  }
  else {
    invok = 1;
  }

  CountedIntFn * func = new CountedIntFn(invok, f, mesg);

  int answer = binarySearchForZero(func, low, high);

  if (answer != expected_ans) {
    fprintf(stderr, "Error for %s!\n", mesg);
    exit(EXIT_FAILURE);
  }
}

int main(void) {
  SinFunction sinf;
  pos_const_function pf;
  neg_const_function nf;
  linear__function lf;

  check(&sinf, 0, 150000, 52359, "sin function!");
  check(&pf, -2, 5, -2, "positive constant function!");
  check(&nf, -3, 5, 4, "negative constant function!");
  check(&lf, -3, 5, 0, "linear function!");
  check(&lf, -10, -1, -2, "negative linear function!");
  check(&lf, 1, 999, 1, "positive linear function!");
  check(&lf, 5, 5, 5, "Range!");
}
