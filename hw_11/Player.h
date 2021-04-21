#ifndef PLAYER_H_
#define PLAYER_H_

#include "constants.h"
#include "Entity.h"

class Player : public Entity
{
 public:
  auto& thrust(){ return thrust_; }
  auto& lives(){ return lives_; }
  auto& score(){ return score_; }


  Player() : Entity(constants::EntityName::Player), lives_(3), score_(0){}

  bool is_alive() const { return (lives_ > 0); }


  void score_increase() {
    score_++;
  }

 private:
  void update() override
  {
    if (thrust_)
    {
      dx() += std::cos(angle() * constants::kDegToRad) * 0.2f;
      dy() += std::sin(angle() * constants::kDegToRad) * 0.2f;
    }
    else
    {
      dx() *= speed_mult;
      dy() *= speed_mult;
    }

    float speed = std::sqrt(dx() * dx() + dy() * dy());
    if (speed > maxSpeed)
    {
      dx() *= maxSpeed / speed;
      dy() *= maxSpeed / speed;
    }

    x() += dx();
    y() += dy();

    if (x() > constants::kWidth) x() = 0; if (x() < 0) x() = constants::kWidth;
    if (y() > constants::kHeight) y() = 0; if (y() < 0) y() = constants::kHeight;
  }

 private:
  bool thrust_{};
  int lives_{};
  int score_{};
  const float maxSpeed = 15.0;
  const float speed_mult = 0.99;
};

#endif //PLAYER_H_
