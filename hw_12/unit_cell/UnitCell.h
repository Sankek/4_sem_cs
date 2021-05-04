#pragma once

#include <vector>

#include "link.hpp"
#include "particle.hpp"

#include <iostream>

/*
 *  unit cell structure:
 *  *---*
 *  |\ /|
 *  | * |
 *  |/ \|
 *  *---*
 */

struct Cell{
  using particle_t = std::shared_ptr<Particle>;
  using cell_t = std::shared_ptr<Cell>;
  cell_t left_cell, up_cell, down_cell, right_cell;
  particle_t center_particle, ul_particle, ur_particle, dl_particle, dr_particle;
  bool ul_particle_owner, ur_particle_owner, dl_particle_owner, dr_particle_owner;
  bool left_link_owner, right_link_owner, up_link_owner, down_link_owner;
};

class UnitCell {
 public:
  using particle_t = std::shared_ptr<Particle>;

  const float cell_size = 100.0f;
  const sf::Vector2f unit_cell_position = sf::Vector2f(100.0f, 100.0f);
  const float stiffness = 0.3f;
  const float particle_radius = 2.5f;
  const sf::Vector2f gravity_force = sf::Vector2f(0.0f, 0.2f);

  explicit UnitCell(const std::vector<std::vector<bool>>& fill_matrix): fill_matrix_(fill_matrix){
    Initialize();
  };

  auto get_particles() const{ return particles_; }
  auto get_links() const{ return links_; }
  
 private:
  void Initialize();

  const std::vector<std::vector<bool>>& fill_matrix_;
  std::vector<std::vector<Cell>> cells_;
  std::vector<Link> links_;
  std::vector<particle_t> particles_;
};
