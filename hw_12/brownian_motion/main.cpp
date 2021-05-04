#include <chrono>
#include <ctime>
#include <cstdint>
#include <vector>
#include <random>

#include <SFML/Graphics.hpp>
#include <iostream>

struct Particle {
    float x;
    float y;
};

int main(int argc, char ** argv)
{
    const int n_particles = 1000;
    const int window_height = 480;
    const int window_width = 480;
    const int num_cells = 10;
    const int cell_size = window_height / num_cells;
    const auto mean_particles_in_cell = static_cast < double > (n_particles) * cell_size *
            cell_size / window_width / window_height;

    sf::Color color(0x4B077FFF);


    std::random_device random_device{};
    std::mt19937 rng{random_device()};
    static std::uniform_int_distribution <> init_distr_x {0, window_width};
    static std::uniform_int_distribution <> init_distr_y {0, window_height};

    static std::normal_distribution <> norm_x {0, 1};
    static std::normal_distribution <> norm_y {0, 1};

    std::vector < Particle > particles (n_particles);
    std::vector < std::vector < std::size_t > > cells (window_height / cell_size,
                                               std::vector < std::size_t > (window_width / cell_size, 0));
    for (auto & particle: particles)
    {
        particle.x = static_cast <float> (init_distr_x(rng));
        particle.y = static_cast <float> (init_distr_y(rng));
    }

    sf::CircleShape circle(2.0f);
    circle.setFillColor(sf::Color::Red);

    sf::RectangleShape cell({cell_size, cell_size});

    sf::RenderWindow application(sf::VideoMode(window_height, window_width), "Brownian_motion");
    application.setFramerateLimit(60);
    while (application.isOpen())
    {
        sf::Event event{};

        while (application.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                application.close();
            }
        }
        application.clear(sf::Color::White);

        for (auto i = 0U; i < cells.size(); ++i)
        {
            for (auto j = 0U; j < cells.at(i).size(); ++j)
            {
                cell.setPosition(i*cell_size, j*cell_size);
                auto transparency = 128 + 127 * (static_cast < double > (cells[i][j] - mean_particles_in_cell)) / mean_particles_in_cell;
                transparency = (transparency > 255) ? 255 : transparency;
                color.a = transparency;
                cell.setFillColor(color);
                application.draw(cell);
            }
        }

        for (auto & i: cells)
        {
            for (auto & j: i)
            {
                j = 0;
            }
        }

        for (auto & particle: particles)
        {
            auto dx = static_cast <float> (norm_x(rng));
            auto dy = static_cast <float> (norm_y(rng));
            auto new_x = particle.x + dx;
            auto new_y = particle.y + dy;
            if (new_x > window_width || new_x < 0)
            {
                new_x = particle.x - dx;
            }
            if (new_y > window_height || new_y < 0)
            {
                new_y = particle.y - dy;
            }
            particle.x = new_x;
            particle.y = new_y;

            circle.setPosition(particle.x, particle.y);
            cells.at(particle.x / cell_size).at(particle.y / cell_size) += 1;

            application.draw(circle);
        }
        application.display();
    }


    return EXIT_SUCCESS;
}