#ifndef ENTITY_H_
#define ENTITY_H_

#include "constants.h"

class Entity
{
 public:
  auto& animation(){ return anim_; }
  auto& x(){ return x_; }
  auto& y(){ return y_; }
  auto& dx(){ return dx_; }
  auto& dy(){ return dy_; }
  auto& r(){ return r_; }
  auto& angle(){ return angle_; }
  auto& life(){ return life_; }
  auto& name(){ return name_; }

  Entity() : life_(true){}
  Entity(constants::EntityName name) : life_(true), name_(name){}

  void settings(Animation& a, float X, float Y, float Angle = 0.0f, float radius = 1)
  {
    anim_ = a;
    x_ = X; y_ = Y;
    angle_ = Angle;
    r_ = radius;
  }

  virtual void update() {};

  void draw(sf::RenderWindow& app)
  {
    anim_.sprite().setPosition(x_, y_);
    anim_.sprite().setRotation(angle_ + 90);
    app.draw(anim_.sprite());
  }

  virtual ~Entity() = default;

 private:
  Animation anim_;
  float x_{}, y_{}, dx_{}, dy_{}, r_{}, angle_{};
  bool life_;
  constants::EntityName name_;

};

#endif //ENTITY_H_
