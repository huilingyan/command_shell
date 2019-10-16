#include "point.h"

class Circle {
  Point center;
  const double radius;

 public:
  Circle(Point center, const double radius) : center(center), radius(radius) {}
  void move(double dx, double dy);
  double intersectionArea(const Circle & otherside);
};
