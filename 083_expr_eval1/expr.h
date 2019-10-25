#ifndef __T_EXPR_H___
#define __T_EXPR_H___

#include <assert.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Expression {
 public:
  Expression() {}
  virtual std::string toString() const = 0;
  virtual ~Expression() {}
};

class NumExpression : public Expression {
  long number;

 public:
  NumExpression(long n) : number(n) {}
  virtual std::string toString() const {
    std::stringstream ans;
    ans << number;
    return ans.str();
  }
  virtual ~NumExpression() {}
};

class PlusExpression : public Expression {
  Expression * lhs;
  Expression * rhs;

 public:
  PlusExpression(Expression * lhs, Expression * rhs) : lhs(lhs), rhs(rhs) {}
  virtual std::string toString() const {
    std::stringstream ans;
    ans << "(" << lhs->toString() << " + " << rhs->toString() << ")";
    return ans.str();
  }
  virtual ~PlusExpression() {
    delete lhs;
    delete rhs;
  }
};

#endif
