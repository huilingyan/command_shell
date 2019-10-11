#include "vector.hpp"

#include <cmath>
#include <cstdio>

/* write your class implementation in this file
 */
/*class Vector2D {
  private:
  double x;
  double y;

 public:
  void initVector(double init_x, double init_y);
  double getMagnitude() const;
  Vector2D operator+(const Vector2D & rhs) const;
  Vector2D & operator+=(const Vector2D & rhs);
  double dot(const Vector2D & rhs) const;
  void print() const;
};

Vector v1 = <1.00, 2.00> and has magnitude 2.24
Vector v2 = <-1.50, 3.00> and has magnitude 3.35
v1 + v2 = <-0.50, 5.00>
v1 += v2 = <-0.50, 5.00>
Now v1 = <-0.50, 5.00>
v1 . v2 = 15.75

*/
void Vector2D::initVector(double init_x, double init_y) {
  x = init_x;
  y = init_y;
}

double Vector2D::getMagnitude() const {
  return std::sqrt(x * x + y * y);
}

Vector2D Vector2D::operator+(const Vector2D & rhs) const {
  Vector2D v_add;
  v_add.x = x + rhs.x;
  v_add.y = y + rhs.y;

  return v_add;
}

Vector2D & Vector2D::operator+=(const Vector2D & rhs) {
  x += rhs.x;
  y += rhs.y;

  return *this;
}

double Vector2D::dot(const Vector2D & rhs) const {
  return x * rhs.x + y * rhs.y;
}

void Vector2D::print() const {
  std::printf("<%.2f, %.2f>", this->x, this->y);
}
