#pragma once
#include<string>

namespace Config {
	struct General {
		static const std::string configPath;
		static const std::FILE* config;
		static const std::string logPath;
		static const std::FILE* log;

		void Parse();

		General() = delete;
		General(const General& other) = delete;
		~General() = delete;
	};

	struct AntWeights{
		// Base stats increases tied to strongDirection
		static const float spreadBias;
		static const float aggressivenessBias;
		static const float strengthBias;

		// Colors
		static const bool mix; // Whether to mix the colors of nearby cells each turn
		static const int ownWeightMult; // Weight multiplier of the cell's own color in the mixing

		// Stat decrease/increase based on actions
		static const float spreadDecOnSuccess;
		static const float spreadIncOnFail;

		void Parse();

		AntWeights() = delete;
		AntWeights(const AntWeights& other) = delete;
		~AntWeights() = delete;
	};
	
	struct GameParameters {
		static const int width;
		static const int height;
		static const int players;
		static const int turns;
		static const int scaleWidth;
		static const int scaleHeight;

		void Parse();

		GameParameters() = delete;
		GameParameters(const GameParameters& other) = delete;
		~GameParameters() = delete;
	};
};