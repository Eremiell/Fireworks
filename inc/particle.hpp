#pragma once
#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <random>
#include <chrono>
#include <cstdint>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "config.hpp"

#ifndef NO_PCG
#include <pcg_random.hpp>
#endif

#ifndef NO_PLF
#include <plf_list.h>
#else
#include <list>
#endif

#ifndef NO_AUDIO
#include <array>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#endif

namespace fw {
	class Particle {
		public:
			#ifndef NO_AUDIO
			static void init();
			static void prune();
			#endif
			Particle();
			Particle(const sf::Vector2<float> &position, const sf::Color &colour, bool explosive);
			void integrate(float dt);
			void draw(sf::RenderWindow &window);
			bool is_done();
			#ifndef NO_PLF
			void explode(plf::list<Particle> &particles);
			#else
			void explode(std::list<Particle> &particles);
			#endif
		private:
			float ttl;
			float age;
			float speed;
			float angle;
			float gravity;
			bool explosive;
			sf::RectangleShape visual;
			#ifndef NO_AUDIO
			static std::array<sf::SoundBuffer, 5> buffers;
			#ifndef NO_PLF
			static plf::list<sf::Sound> sounds;
			#else
			static std::list<sf::Sound> sounds;
			#endif
			#endif
			#ifndef NO_PCG
			static pcg64 prg;
			#else
			static std::mt19937_64 prg;
			#endif
			static std::uniform_real_distribution<float> spd_dist;
			static std::uniform_real_distribution<float> ttl_dist;
			static std::uniform_real_distribution<float> rad_dist;
			static std::uniform_real_distribution<float> pos_dist;
			static std::uniform_int_distribution<std::uint8_t> col_dist;
			static std::uniform_int_distribution<std::uint8_t> cor_dist;
			#ifndef NO_AUDIO
			static std::uniform_int_distribution<std::uint8_t> snd_dist;
			#endif
			static std::bernoulli_distribution stage_dist;
	};
}

#endif