#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#include <cmath>
#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "functions.hpp"
#include "system.hpp"

int main(int argc, char ** argv)
{
  sf::RenderWindow window(sf::VideoMode(800U, 600U), "PHYSICS");
  window.setFramerateLimit(60);

  sf::Vector2f min_point(  0.0f,   0.0f);
  sf::Vector2f max_point(775.0f, 575.0f);

  const auto r = 2.5f;

  std::vector < System::particle_t > particles;

//  std::vector<std::vector<bool>> fill_matrix{ {true, true}, {true, true}, {true, true} };
//  std::vector<std::vector<bool>> fill_matrix{ {true, true} };
  std::vector<std::vector<bool>> fill_matrix(6, std::vector<bool>(4, true));
//  fill_matrix[0][0] = false;
//  fill_matrix[0][3] = false;
//  fill_matrix[4][0] = false;
//  fill_matrix[4][3] = false;

  System system(min_point, max_point, fill_matrix);

  sf::Event event{};

  while (window.isOpen())
  {
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }

    system.update();

    const auto push_force = 0.4f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
      system.push(sf::Vector2f(0.0f, -push_force));
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
      system.push(sf::Vector2f(0.0f, push_force));
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
      system.push(sf::Vector2f(-push_force, 0.0f));
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
      system.push(sf::Vector2f(push_force, 0.0f));
    }

    window.clear();


    for (int i{}; i < system.links().size(); ++i){
      auto [p1, p2] = system.link(i).particles();

//      auto rect_init = sf::Vector2f(length(sf::Vector2f(p1->position()-p2->position())), 1);
//      auto line_dir = p2->position()-p1->position();
//      sf::RectangleShape line(rect_init);
//      line.setPosition(p1->position());
//      auto angle = std::acos((rect_init.x*line_dir.x+rect_init.y*line_dir.y)/ length(line_dir)/ length(rect_init));
//      std::cout << angle << '\n';
//      line.rotate(angle*180/3.1415);
//      window.draw(line);

      sf::Vertex line[] = {
          sf::Vertex(p1->position() + 2.0f*sf::Vector2f(r, r)),
          sf::Vertex(p2->position() + 2.0f*sf::Vector2f(r, r))
      };

      window.draw(line, 2, sf::Lines);
    }

    for (auto i = 0U; i < system.particles().size(); ++i)
    {


      sf::CircleShape circle(2.0f * r);

      circle.setPosition(system.particle(i)->position() + sf::Vector2f(r, r));

      circle.setFillColor(sf::Color::Red);

      window.draw(circle);
    }

    window.display();
  }

  // system("pause");

  return EXIT_SUCCESS;
}

