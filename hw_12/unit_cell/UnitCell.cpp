#include "UnitCell.h"

void UnitCell::Initialize(){
  cells_.resize(fill_matrix_.size());
  for (int i{}; i<fill_matrix_.size(); ++i){
    cells_[i].resize(fill_matrix_[i].size());
  }


  for (int m{}; m<cells_.size(); ++m){
    for (int n{}; n<cells_[m].size(); ++n){
      if (fill_matrix_[m][n]){
        auto center_position = sf::Vector2f(m, n)*cell_size + sf::Vector2f(cell_size/2.0f,cell_size/2.0f) + unit_cell_position;

        if (m > 0) {
          cells_[m][n].left_cell = std::make_shared<Cell>(cells_[m - 1][n]);
        }
        if (n > 0) {
          cells_[m][n].up_cell = std::make_shared<Cell>(cells_[m][n - 1]);
        }
        if (m < cells_.size() - 1) {
          cells_[m][n].right_cell = std::make_shared<Cell>(cells_[m + 1][n]);
        }
        if (n < cells_[m].size() - 1) {
          cells_[m][n].down_cell = std::make_shared<Cell>(cells_[m][n + 1]);
        }

        cells_[m][n].center_particle = std::make_shared<Particle>(center_position, center_position,
                                                                  gravity_force, particle_radius);

        if (cells_[m][n].left_cell){
          if (cells_[m][n].left_cell->ur_particle){
            cells_[m][n].ul_particle = cells_[m][n].left_cell->ur_particle;
          }
          if (cells_[m][n].left_cell->dr_particle){
            cells_[m][n].dl_particle = cells_[m][n].left_cell->dr_particle;
          }
        }
        if (cells_[m][n].up_cell){
          if (cells_[m][n].up_cell->dl_particle){
            cells_[m][n].ul_particle = cells_[m][n].up_cell->dl_particle;
          }
          if (cells_[m][n].up_cell->dr_particle){
            cells_[m][n].ur_particle = cells_[m][n].up_cell->dr_particle;
          }
        }
        if (cells_[m][n].right_cell){
          if (cells_[m][n].right_cell->ul_particle){
            cells_[m][n].ur_particle = cells_[m][n].right_cell->ul_particle;
          }
          if (cells_[m][n].right_cell->dl_particle){
            cells_[m][n].dr_particle = cells_[m][n].right_cell->dl_particle;
          }
        }
        if (cells_[m][n].down_cell){
          if (cells_[m][n].down_cell->ul_particle){
            cells_[m][n].dl_particle = cells_[m][n].down_cell->ul_particle;
          }
          if (cells_[m][n].down_cell->ur_particle){
            cells_[m][n].dr_particle = cells_[m][n].down_cell->ur_particle;
          }
        }

        if (m>0 && n>0){
          if (cells_[m-1][n-1].dr_particle){
            cells_[m][n].ul_particle = cells_[m-1][n-1].dr_particle;
          }
        }
        if (m<cells_.size()-1 && n>0){
          if (cells_[m+1][n-1].dl_particle){
            cells_[m][n].ur_particle = cells_[m+1][n-1].dl_particle;
          }
        }
        if (m>0 && n<cells_[m-1].size()-1){
          if (cells_[m-1][n+1].ur_particle){
            cells_[m][n].dl_particle = cells_[m-1][n+1].ur_particle;
          }
        }
        if (m<cells_.size()-1 && n<cells_[m+1].size()-1){
          if (cells_[m+1][n+1].ul_particle){
            cells_[m][n].dr_particle = cells_[m+1][n+1].ul_particle;
          }
        }

        if (!cells_[m][n].ul_particle) {
          auto new_pos = center_position + sf::Vector2f(-cell_size / 2.0f, -cell_size / 2.0f);
          cells_[m][n].ul_particle = std::make_shared<Particle>(new_pos,
                                                                new_pos,
                                                                gravity_force, particle_radius);
          cells_[m][n].ul_particle_owner = true;
        }
        if (!cells_[m][n].dr_particle) {
          auto new_pos = center_position + sf::Vector2f(cell_size / 2.0f, cell_size / 2.0f);
          cells_[m][n].dr_particle = std::make_shared<Particle>(new_pos,
                                                                new_pos,
                                                                gravity_force, particle_radius);
          cells_[m][n].dr_particle_owner = true;
        }
        if (!cells_[m][n].dl_particle) {
          auto new_pos = center_position + sf::Vector2f(-cell_size / 2.0f, cell_size / 2.0f);
          cells_[m][n].dl_particle =
              std::make_shared<Particle>(new_pos,
                                         new_pos,
                                         gravity_force, particle_radius);
          cells_[m][n].dl_particle_owner = true;
        }
        if (!cells_[m][n].ur_particle) {
          auto new_pos = center_position + sf::Vector2f(cell_size / 2.0f, -cell_size / 2.0f);
          cells_[m][n].ur_particle =
              std::make_shared<Particle>(new_pos,
                                         new_pos,
                                         gravity_force, particle_radius);
          cells_[m][n].ur_particle_owner = true;
        }
      }
    }
  }

  for (int m{}; m<cells_.size(); ++m) {
    for (int n{}; n < cells_[m].size(); ++n) {
      if (fill_matrix_[m][n]) {

        particles_.push_back(cells_[m][n].center_particle);
        if (cells_[m][n].ul_particle_owner){
          particles_.push_back(cells_[m][n].ul_particle);
        }
        if (cells_[m][n].ur_particle_owner){
          particles_.push_back(cells_[m][n].ur_particle);
        }
        if (cells_[m][n].dl_particle_owner){
          particles_.push_back(cells_[m][n].dl_particle);
        }
        if (cells_[m][n].dr_particle_owner){
          particles_.push_back(cells_[m][n].dr_particle);
        }

        links_.emplace_back(cells_[m][n].center_particle,
                            cells_[m][n].ul_particle, stiffness);
        links_.emplace_back(cells_[m][n].center_particle,
                            cells_[m][n].ur_particle, stiffness);
        links_.emplace_back(cells_[m][n].center_particle,
                            cells_[m][n].dl_particle, stiffness);
        links_.emplace_back(cells_[m][n].center_particle,
                            cells_[m][n].dr_particle, stiffness);

        if (!cells_[m][n].left_cell || !cells_[m][n].left_cell->right_link_owner){
          links_.emplace_back(cells_[m][n].ul_particle,
                              cells_[m][n].dl_particle, stiffness);
          cells_[m][n].left_link_owner = true;
        }
        if (!cells_[m][n].right_cell || !cells_[m][n].right_cell->left_link_owner){
          links_.emplace_back(cells_[m][n].ur_particle,
                              cells_[m][n].dr_particle, stiffness);
          cells_[m][n].right_link_owner = true;
        }
        if (!cells_[m][n].up_cell || !cells_[m][n].up_cell->down_link_owner){
          links_.emplace_back(cells_[m][n].ul_particle,
                              cells_[m][n].ur_particle, stiffness);
          cells_[m][n].up_link_owner = true;
        }
        if (!cells_[m][n].down_cell || !cells_[m][n].down_cell->up_link_owner){
          links_.emplace_back(cells_[m][n].dl_particle,
                              cells_[m][n].dr_particle, stiffness);
          cells_[m][n].down_link_owner = true;
        }
      }
    }
  }
}