#pragma once
#include "Color.h"
#include "Randomness.h"
#include <vector>

struct Weights {
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
};

class AntVariant {
public:
	enum SpecialId {
		Empty = -2, PlaceHolder = -1
	};
	static int EmptyCells;
	static int CreatedInstances;
	static int LastId;

private:
	int m_id;
	Color m_color;
	float m_spreadChance;
	float m_aggressiveness;
	float m_strength;
	int m_strongDirection;
	//int m_isNull;

public:
	AntVariant();
	AntVariant(const int& id);
	AntVariant(const AntVariant& other);
	AntVariant(const Color& color, const float& spreadChance, const float& aggressiveness, const float& strength, const int& strongDirection);
	~AntVariant();

	int GetId();
	Color GetColor();
	static AntVariant GetRandom();	
	std::vector<AntVariant> PlayTurn(const std::vector<AntVariant>& neighbors);

private:
	AntVariant(const int& id, const Color& color, const float& spreadChance, const float& aggressiveness, const float& strength, const int& strongDirection);
	bool Spread(const int& direction);
	bool RollAggression(const int& direction);
	bool Attack(const AntVariant& neighbor, const int& direction);
};
