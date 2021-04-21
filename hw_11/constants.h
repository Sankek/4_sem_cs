#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <SFML/Graphics.hpp>

namespace constants{

inline const unsigned int kWidth = sf::VideoMode::getDesktopMode().width;
inline const unsigned int kHeight = sf::VideoMode::getDesktopMode().height;
inline const float kDegToRad = 0.017453f;
inline const auto kRandMax = RAND_MAX;

enum EntityName{
  Asteroid,
  Bullet,
  Player,
  Explosion
};

}

#endif //CONSTANTS_H_
