#include "circle.h"

#include <cmath>
#include <cstdio>

void Circle::move(double dx, double dy) {
  center.move(dx, dy);
}

double Circle::intersectionArea(const Circle & otherCircle) {
  double pi = 3.1415926535;
  double d = center.distanceFrom(otherCircle.center);
  if (d >= radius + otherCircle.radius) {
    return 0;
  }
  else if (d <= std::abs(radius - otherCircle.radius)) {
    if (radius <= otherCircle.radius) {
      return pi * radius * radius;
    }
    else {
      return pi * otherCircle.radius * otherCircle.radius;
    }
  }
  else {
    double temp =
        (radius * radius - otherCircle.radius * otherCircle.radius + d * d) / (2 * d);
    double c1 = radius * radius * std::acos(temp / radius);
    double c2 = otherCircle.radius * otherCircle.radius *
                std::acos((d - temp) / otherCircle.radius);
    double t1 = temp * std::sqrt(radius * radius - temp * temp);
    double t2 = (d - temp) * std::sqrt(radius * radius - temp * temp);

    return c1 + c2 - t1 - t2;
  }
}
