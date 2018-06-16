#include "particle.hpp"
#include <cmath>
#include <iterator>

#ifndef NO_AUDIO
#include <SFML/Audio/Listener.hpp>
#endif

#ifdef DEBUG
#include <iostream>
#endif

namespace fw {
	#ifndef NO_AUDIO
	std::array<sf::SoundBuffer, 5> Particle::buffers{};
	#ifndef NO_PLF
	plf::list<sf::Sound> Particle::sounds{};
	#else
	std::list<sf::Sound> Particle::sounds{};
	#endif
	#endif
	#ifndef NO_PCG
	pcg64 Particle::prg{pcg_extras::seed_seq_from<std::random_device>{}};
	#else
	std::mt19937_64 Particle::prg{std::random_device{}()};
	#endif
	std::uniform_real_distribution<float> Particle::spd_dist{cf::min_speed, cf::max_speed};
	std::uniform_real_distribution<float> Particle::ttl_dist{cf::min_ttl, cf::max_ttl};
	std::uniform_real_distribution<float> Particle::rad_dist{0.0f, 6.2831853071795f};
	std::uniform_real_distribution<float> Particle::pos_dist{0.0f, static_cast<float>(cf::width)};
	std::uniform_int_distribution<std::uint8_t> Particle::col_dist{cf::min_col_sat, cf::max_col_sat};
	std::uniform_int_distribution<std::uint8_t> Particle::cor_dist{cf::min_corona_parts, cf::max_corona_parts};
	#ifndef NO_AUDIO
	std::uniform_int_distribution<std::uint8_t> Particle::snd_dist{0, cf::sound_files - 1};
	#endif
	std::bernoulli_distribution Particle::stage_dist{cf::next_stage_prob};

	#ifndef NO_AUDIO
	void Particle::init() {
		sf::Listener::setGlobalVolume(cf::volume);
		for (std::size_t i{0}; i < cf::sound_files; ++i) {
			buffers[i].loadFromFile(u8"firework" + std::to_string(i) + u8".ogg");
		}
		return;
	}

	void Particle::prune() {
		for (auto it{sounds.begin()}; it != sounds.end(); std::advance(it, 1)) {
			if (it->getStatus() == sf::SoundSource::Status::Stopped) {
				it = sounds.erase(it);
				std::advance(it, -1);
			}
		}
		#ifdef DEBUG
		std::cerr << u8"Sounds: " << sounds.size() << '\n';
		#endif
		return;
	}
	#endif

	Particle::Particle() : ttl{ttl_dist(prg)}, age{0.0f}, speed{spd_dist(prg)}, angle{0.0f}, gravity{0.0f}, explosive{true}, visual{sf::Vector2<float>{cf::particle_size, cf::particle_size}} {
		this->visual.setFillColor(sf::Color::White);
		this->visual.setOrigin(cf::particle_size / 2.0f, cf::particle_size / 2.0f);
		this->visual.setPosition(pos_dist(prg), static_cast<float>(cf::height));
	}

	Particle::Particle(const sf::Vector2<float> &position, const sf::Color &colour, bool explosive) : ttl{ttl_dist(prg)}, age{0.0f}, speed{spd_dist(prg)}, angle{rad_dist(prg)}, gravity{0.0f}, explosive{explosive}, visual{sf::Vector2<float>{cf::particle_size, cf::particle_size}} {
		this->visual.setFillColor(colour);
		this->visual.setOrigin(cf::particle_size / 2.0f, cf::particle_size / 2.0f);
		this->visual.setPosition(position);
	}

	void Particle::integrate(float dt) {
		float y_part = std::cos(this->angle);
		float x_part = std::sin(this->angle);
		this->visual.move(this->speed * x_part, -this->speed * y_part);
		this->ttl -= dt;
		this->age += dt;
		this->gravity += this->age * cf::gravity_accel; // accelerate baby!
		this->visual.move(0.0f, this->gravity);
		auto colour = this->visual.getFillColor();
		colour.a = 255 - this->age / (this->age + this->ttl) * cf::max_col_dim; // 255 is full colour, so the alpha needs to be subtracted from it
		this->visual.setFillColor(colour);
		return;
	}

	void Particle::draw(sf::RenderWindow &window) {
		window.draw(this->visual);
		return;
	}

	bool Particle::is_done() {
		return this->ttl <= 0.0f;
	}

	#ifndef NO_PLF
	void Particle::explode(plf::list<Particle> &particles) {
	#else
	void Particle::explode(std::list<Particle> &particles) {
	#endif
		if (this->explosive) {
			auto corona_particles{cor_dist(prg)};
			sf::Color colour{col_dist(prg), col_dist(prg), col_dist(prg)};
			auto expl{stage_dist(prg)};
			for (std::uint8_t i{0}; i < corona_particles; ++i) {
				particles.emplace_back(this->visual.getPosition(), colour, expl);
			}
			#ifndef NO_AUDIO
			sounds.emplace_back(buffers[snd_dist(prg)]).play();
			#endif
		}
		return;
	}
}