#include "fireworks.hpp"
#include <iterator>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "config.hpp"

#ifdef DEBUG
#include <iostream>
#endif

namespace fw {
	Fireworks::Fireworks() : window{sf::VideoMode{cf::width, cf::height}, u8"Fireworks", cf::fullscreen?sf::Style::Fullscreen:sf::Style::Titlebar|sf::Style::Close} {
		window.setKeyRepeatEnabled(false);
	}

	int Fireworks::run() {
		static sf::Clock timer;
		static constexpr float dt{1.0f/cf::fps};
		static float accumulator;
		static unsigned long long t{0ull};
		static bool running{true};
		#ifndef NO_AUDIO
		Particle::init(); // loads explosion sounds
		#endif
		while (running) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					running = false;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))) {
				running = false;
			}
			accumulator += timer.restart().asSeconds();
			while (accumulator >= dt) {
				accumulator -= dt;
				++t;
				if (!(t % cf::every_n_frames)) {
					this->particles.emplace_back();
				}
				for (auto it{this->particles.begin()}; it != this->particles.end(); std::advance(it, 1)) {
					it->integrate(dt);
					if (it->is_done()) {
						it->explode(this->particles);
						it = this->particles.erase(it);
						std::advance(it, -1);
					}
				}
				#ifndef NO_AUDIO
				Particle::prune(); // removes finished sounds
				#endif
			}
			window.clear();
			for (auto &particle : this->particles) {
				particle.draw(this->window);
			}
			window.display();
			#ifdef DEBUG
			std::cerr << u8"Particles: " << this->particles.size() << '\n';
			#endif
		}
		window.close();
		return 0;
	}
}