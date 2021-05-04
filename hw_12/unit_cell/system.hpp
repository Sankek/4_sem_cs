#pragma once

#include <cmath>
#include <memory>
#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>

#include "functions.hpp"
#include "link.hpp"
#include "particle.hpp"
#include "UnitCell.h"

class System {
 public:

  using particle_t = std::shared_ptr<Particle>;

 public:

  explicit System(sf::Vector2f min_point, sf::Vector2f max_point,
                  std::vector<std::vector<bool>> fill_matrix) noexcept:
      m_min_point(min_point), m_max_point(max_point),
      fill_matrix_(std::move(fill_matrix)) {
    initialize();
  }

  ~System() noexcept = default;

 private:

  void initialize();

 public:

  auto particle(std::size_t index) const {
    return m_particles.at(index);
  }

  auto &particles() const noexcept {
    return m_particles;
  }

  auto link(size_t index) const {
    return m_links.at(index);
  }

  auto &links() const noexcept {
    return m_links;
  }

 public:

  void push(sf::Vector2f force) const;

  void update() const;

 private:

  std::vector<std::vector<bool>> fill_matrix_;
  sf::Vector2f m_min_point;
  sf::Vector2f m_max_point;

  std::vector<particle_t> m_particles;

  std::vector<Link> m_links;
};