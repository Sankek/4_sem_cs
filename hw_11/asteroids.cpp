#include <iostream>
#include <list>
#include <memory>
#include <random>

#include <SFML/Graphics.hpp>

#include "constants.h"
#include "Animation.h"
#include "Entity.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "Player.h"


bool isCollide(const std::shared_ptr < Entity >& a, const std::shared_ptr < Entity >&  b)
{
  return (b->x() - a->x()) * (b->x() - a->x()) +
      (b->y() - a->y()) * (b->y() - a->y()) <
      (a->r() + b->r()) * (a->r() + b->r());
}


class Game{
 public:
  Game() :
    sExplosion(t3, 0, 0, 256, 256, 48, 0.5),
    sRock(t4, 0, 0, 64, 64, 16, 0.2),
    sRock_small(t6, 0, 0, 64, 64, 16, 0.2),
    sBullet(t5, 0, 0, 32, 64, 16, 0.8),
    sPlayer(t1, 40, 0, 40, 40, 1, 0),
    sPlayer_go(t1, 40, 40, 40, 40, 1, 0),
    sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5)
  {
    t1.loadFromFile("../images/spaceship.png");
    t2.loadFromFile("../images/background.jpg");
    t3.loadFromFile("../images/explosions/type_C.png");
    t4.loadFromFile("../images/rock.png");
    t5.loadFromFile("../images/fire_blue.png");
    t6.loadFromFile("../images/rock_small.png");
    t7.loadFromFile("../images/explosions/type_B.png");

    t1.setSmooth(true);
    t2.setSmooth(true);

    if (!font.loadFromFile("../Font/ITCBLKAD.ttf"))
    {
      std::cerr << "font loading problem occured\n";
    }

    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Red);
    text.setPosition(10, 10);
  }

  void run(){
    sf::RenderWindow app(sf::VideoMode(constants::kWidth, constants::kHeight), "Asteroids!");
    app.setFramerateLimit(60);

    sf::Sprite background(t2);

    std::list< std::shared_ptr < Entity > > entities;

    std::uniform_int_distribution<int> uniform_int_distribution(0, constants::kRandMax);

    for (int i = 0; i < max_num_rocks; i++)
    {
      auto a = std::make_shared < Asteroid >(rng);

      entities.push_back(a);

      a->settings(sRock, uniform_int_distribution(rng) % constants::kWidth,
                            uniform_int_distribution(rng) % constants::kHeight,
                              uniform_int_distribution(rng) % 360, big_rock_radius);

    }

    auto p = std::make_shared < Player >();
    p->settings(sPlayer, constants::kWidth / 2, constants::kHeight / 2, 0, player_radius);
    entities.push_back(p);

    ///// game cycle ////
    while (app.isOpen() && p->is_alive())
    {
      sf::Event event {};
      while (app.pollEvent(event))
      {
        if (event.type == sf::Event::Closed)
          app.close();

        if (event.type == sf::Event::KeyPressed)
          if (event.key.code == sf::Keyboard::Space)
          {
            auto b = std::make_shared < Bullet >();
            b->settings(sBullet, p->x(), p->y(), p->angle(), bullet_radius);
            entities.push_back(b);
          }
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) p->angle() += 3;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  p->angle() -= 3;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) p->thrust() = true;
      else p->thrust() = false;

      for (const auto a : entities)
        for (const auto b : entities)
        {
          if (a->name() == constants::EntityName::Asteroid && b->name() == constants::EntityName::Bullet)
            if (isCollide(a, b))
            {
              a->life() = false;
              b->life() = false;

              auto e = std::make_shared < Entity >();
              e->settings(sExplosion, a->x(), a->y());
              e->name() = constants::EntityName::Explosion;
              entities.push_back(e);


              for (int i = 0; i < num_small_rocks_after_crash; i++)
              {
                if (a->r() == small_rock_radius){
                  continue;
                }
                auto new_e = std::make_shared < Asteroid >(rng);
                new_e->settings(sRock_small, a->x(), a->y(), uniform_int_distribution(rng) % 360, small_rock_radius);
                entities.push_back(new_e);
              }

              p->score_increase();

            }

          if (a->name() == constants::EntityName::Player && b->name() == constants::EntityName::Asteroid)
            if (isCollide(a, b))
            {
              b->life() = false;

              auto e = std::make_shared < Entity >();
              e->settings(sExplosion_ship, a->x(), a->y());
              e->name() = constants::EntityName::Explosion;
              entities.push_back(e);

              p->settings(sPlayer, constants::kWidth / 2, constants::kHeight / 2, 0, player_radius);
              p->dx() = 0; p->dy() = 0;
              p->lives()--;
            }
        }

      if (p->thrust()){
        p->animation() = sPlayer_go;
      }
      else{
        p->animation() = sPlayer;
      }

      for (const auto& e : entities) {
        if (e->name() == constants::EntityName::Explosion) {
          if (e->animation().isEnd()) {
            e->life() = false;
          }
        }
      }

      if (uniform_int_distribution(rng) % 150 == 0)
      {
        auto a = std::make_shared < Asteroid >(rng);
        a->settings(sRock,
                    0,
                    uniform_int_distribution(rng) % constants::kHeight,
                    uniform_int_distribution(rng) % 360,
                    big_rock_radius);
        entities.push_back(a);
      }

      for (auto i = entities.begin(); i != entities.end();)
      {
        std::shared_ptr < Entity > e = *i;

        e->update();
        e->animation().update();

        if (!e->life()){
          { i = entities.erase(i); e.reset(); }
        }
        else{
          i++;
        }
      }

      //////draw//////
      app.draw(background);
      for (const auto& i : entities) i->draw(app);
      text.setString("score: " + std::to_string(p->score()) + "\nLifes: " + std::to_string(p->lives()));
      app.draw(text);
      app.display();
    }

  }
 private:
  sf::Texture t1, t2, t3, t4, t5, t6, t7;
  Animation sExplosion, sRock, sRock_small, sBullet, sPlayer, sPlayer_go, sExplosion_ship;
  sf::Font font;
  sf::Text text;
  std::random_device random_device{};
  std::mt19937 rng{random_device()};
  const unsigned int max_num_rocks = 15U;
  const float big_rock_radius = 25.0f;
  const float small_rock_radius = 15.0f;
  const float player_radius = 20.0f;
  const float bullet_radius = 10.0f;
  const unsigned int num_small_rocks_after_crash = 2U;
};


int main()
{
  Game asteroids;

  asteroids.run();

  return 0;
}