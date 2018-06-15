#pragma once
#ifndef FIREWORKS_HPP
#define FIREWORKS_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include "particle.hpp"
#include "config.hpp"

#ifndef NO_PLF
#include <plf_list.h>
#else
#include <list>
#endif

namespace fw {
	class Fireworks {
		public:
			Fireworks();
			int run();
		private:
			sf::RenderWindow window;
			#ifndef NO_PLF
			plf::list<Particle> particles;
			#else
			std::list<Particle> particles;
			#endif
	};
}

#endif