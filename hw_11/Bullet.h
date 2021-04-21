#ifndef BULLET_H_
#define BULLET_H_

#include "constants.h"

class Bullet : public Entity
{
 public:
  Bullet() : Entity(constants::EntityName::Bullet){}
 private:
  void  update() override
  {
    dx() = std::cos(angle() * constants::kDegToRad) * 6;
    dy() = std::sin(angle() * constants::kDegToRad) * 6;
    x() += dx();
    y() += dy();

    if (x() > constants::kWidth || x()<0 || y()> constants::kHeight || y() < 0) life() = false;
  }

};

#endif //BULLET_H_
