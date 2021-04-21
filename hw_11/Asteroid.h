#ifndef ASTEROID_H_
#define ASTEROID_H_

#include "constants.h"

class Asteroid : public Entity
{
 public:
  Asteroid(std::mt19937 & rng)
  {
    std::uniform_int_distribution<int> uniform_int_distribution(-4, 4);
    dx() = static_cast<float>(uniform_int_distribution(rng));
    dy() = static_cast<float>(uniform_int_distribution(rng));
    name() = constants::EntityName::Asteroid;
  }
 private:
  void update() override
  {
    x() += dx();
    y() += dy();

    if (x() > constants::kWidth) x() = 0;  if (x() < 0) x() = constants::kWidth;
    if (y() > constants::kHeight) y() = 0;  if (y() < 0) y() = constants::kHeight;
  }
};

#endif //ASTEROID_H_
