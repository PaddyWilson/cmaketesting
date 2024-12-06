#pragma once

#include <random>
#include <unordered_map>

namespace UUID {
	static std::random_device randomDevice;
	static std::mt19937_64 s_Engine(randomDevice());
	static std::uniform_int_distribution<uint64_t> uniformDistribution;

	uint64_t Generate();
}