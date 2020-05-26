#include <cmath>
#include <future>
#include <iostream>
#include <photons/application.hpp>

using namespace std;

namespace photons {

application::application() : window{{500, 500}, "Photons"}, sys{10000} {
  view.setCenter(0, 0);

  photons::generate_random(sys, rng);
}

void application::resize(int w, int h) {
  view.setSize(w, h);
  view.zoom(fov / h);
  window.setView(view);
}

void application::render() {
  const float radius = fov / 500;
  sf::CircleShape circle{radius};
  circle.setOrigin(radius, radius);

  for (int i = 0; i < sys.size(); ++i) {
    circle.setPosition(sys.pos_x[i], sys.pos_y[i]);
    circle.setFillColor(sf::Color(255 * sys.weights[i], 255 * sys.weights[i],
                                  255 * sys.weights[i], 128));
    window.draw(circle);
  }
}

void application::execute() {
  bool update = false;
  bool pause = false;
  bool draw = true;
  sf::Vector2i old_mouse{};

  constexpr float fps_bound = 1.0;
  auto old_time = std::chrono::high_resolution_clock::now();
  auto frames = 0;

  while (window.isOpen()) {
    const auto mouse = sf::Mouse::getPosition(window);

    sf::Event event{};
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::Resized:
          resize(event.size.width, event.size.height);
          break;

        case sf::Event::MouseWheelMoved:
          fov *= exp(-event.mouseWheel.delta * 0.05f);
          fov = clamp(fov, 1e-6f, 1000.f);
          update = true;
          break;

        case sf::Event::KeyPressed:
          switch (event.key.code) {
            case sf::Keyboard::Escape:
              window.close();
              break;

            case sf::Keyboard::Space:
              pause = !pause;
              break;

            case sf::Keyboard::R:
              photons::generate_random(sys, rng);
              break;

            case sf::Keyboard::D:
              draw = !draw;
              break;
          }
          break;
      }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      const auto mouse_move =
          window.mapPixelToCoords(mouse) - window.mapPixelToCoords(old_mouse);
      view.move(-mouse_move);
      update = true;
    }

    if (update) {
      resize(window.getSize().x, window.getSize().y);
      update = false;
    }

    if (!pause) {
      // photons::optics::advance(sys, rng);
      // photons::optics_cache::advance(sys, vrng);
      photons::phase_function::advance(sys, rng);
      // const auto start = std::chrono::high_resolution_clock::now();
      // for (int i = 0; i < 1000; ++i) {
      //   // photons::phase_function::advance(sys, rng);
      //   photons::phase_function_avx::advance(sys, rng);
      //   // photons::phase_function_avx_prng::advance(sys, vrng);
      // }
      // const auto end = std::chrono::high_resolution_clock::now();
      // const auto time = std::chrono::duration<float>(end - start).count();
      // std::cout << time << "\n";
    }

    window.clear();
    if (draw) render();
    window.display();

    old_mouse = mouse;

    ++frames;
    const auto current_time = std::chrono::high_resolution_clock::now();
    const auto time =
        std::chrono::duration<float>(current_time - old_time).count();
    // if (time > fps_bound) {
    //   std::cout << frames / time << " FPS\t\tframe time = " << time / frames
    //             << " s\n";
    //   frames = 0;
    //   old_time = current_time;
    // }
  }
}

}  // namespace photons