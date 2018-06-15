#pragma once
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdint>

// uncomment this to completely drop audio
// #define NO_AUDIO

// uncomment this to replace PCG with Mersenne Twister from std
// #define NO_PCG

// uncomment this to replace PLF list with list from std
// #define NO_PLF

namespace fw {
	namespace cf {
		// Sets window size and if it's fullscreen
		constexpr std::size_t width{1920};
		constexpr std::size_t height{1080};
		constexpr bool fullscreen{true};
		
		// Sets frames per second
		constexpr float fps{60.0f};

		// Sets how often new fireworks are generated in frames (lower means more often)
		constexpr std::size_t every_n_frames{15};

		// Sets particle size
		constexpr float particle_size{10.0f};
		
		// Sets minimum and maximum particle speed
		constexpr float min_speed{20.0f};
		constexpr float max_speed{40.0f};

		// Sets minimum and maximum particle time to live is seconds (until it explodes or disppears)
		constexpr float min_ttl{0.5f};
		constexpr float max_ttl{1.0f};

		// Sets probability of next (second, third, ...) stage from corona particles
		constexpr double next_stage_prob{0.1};

		// Sets minimum and maximum corona (outburst) particles
		constexpr std::uint8_t min_corona_parts{5};
		constexpr std::uint8_t max_corona_parts{10};

		// Sets gravitational acceleration (pixels / frame^2)
		constexpr float gravity_accel{3.0f};

		// Sets minimum and maximum colour component (RGB) saturation (brightness)
		constexpr std::uint8_t min_col_sat{50};
		constexpr std::uint8_t max_col_sat{255};

		// Sets maximum colour dim (alpha)
		constexpr std::uint8_t max_col_dim{200};

		#ifndef NO_AUDIO
		// Sets sound volume
		constexpr float volume{50.0f};

		// Sets number of sound files used, only change if you add or remove sounds, make sure the files follow the naming pattern
		constexpr std::size_t sound_files{5};
		#endif
	}
}

#endif