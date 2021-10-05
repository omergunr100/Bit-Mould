#include "AntVariant.h"
#include <iostream>

using namespace std;

// Config
// Base stats increases tied to strongDirection
const float Weights::spreadBias = 0.15f;
const float Weights::aggressivenessBias = 0.15f;
const float Weights::strengthBias = 0.15f;
// Colors
const bool Weights::mix = true; // Whether to mix the colors of nearby cells each turn
const int Weights::ownWeightMult = 3; // Weight multiplier of the cell's own color in the mixing
// Stat decrease/increase based on actions
const float Weights::spreadDecOnSuccess = 0.1f;
const float Weights::spreadIncOnFail = 0.05f;
//End of config


// Initializing static variables
int AntVariant::EmptyCells = 0;
int AntVariant::CreatedInstances = 0;
int AntVariant::LastId = 0;

AntVariant::AntVariant()
	:m_id(SpecialId::Empty),
	m_color{ 0,0,0 },
	m_spreadChance(0.0f),
	m_aggressiveness(0.0f),
	m_strength(0.0f),
	m_strongDirection(4)
{
	EmptyCells++;
	CreatedInstances++;
}

AntVariant::AntVariant(const int & id)
	:m_id(-1),
	m_color{ 0,0,0 },
	m_spreadChance(0.0f),
	m_aggressiveness(0.0f),
	m_strength(0.0f),
	m_strongDirection(4)
{
}

AntVariant::AntVariant(const AntVariant & other)
	:m_id(other.m_id),
	m_color(other.m_color),
	m_spreadChance(other.m_spreadChance),
	m_aggressiveness(other.m_aggressiveness),
	m_strength(other.m_strength),
	m_strongDirection(other.m_strongDirection)
{
	CreatedInstances++;
	if (m_id == SpecialId::Empty)
		EmptyCells++;
}

AntVariant::AntVariant(const int & id, const Color & color, const float & spreadChance, const float & aggressiveness, const float & strength, const int & strongDirection)
	:m_id(id),
	m_color(color),
	m_spreadChance(spreadChance),
	m_aggressiveness(aggressiveness),
	m_strength(strength),
	m_strongDirection(strongDirection)
{
	CreatedInstances++;
}

bool AntVariant::Spread(const int & direction)
{
	if (Randomness::dist_1_0() <= m_spreadChance + static_cast<float>(direction == m_strongDirection)*Weights::spreadBias) {
		EmptyCells--;
		return true;
	}
	return false;
}

AntVariant::AntVariant(const Color & color, const float & spreadChance, const float & aggressiveness, const float & strength, const int & strongDirection)
	:m_color(color),
	m_spreadChance(spreadChance),
	m_aggressiveness(aggressiveness),
	m_strength(strength),
	m_strongDirection(strongDirection)
{
	m_id = LastId++;
	CreatedInstances++;
}

AntVariant::~AntVariant()
{
}

int AntVariant::GetId()
{
	return m_id;
}

Color AntVariant::GetColor()
{
	return m_color;
}

AntVariant AntVariant::GetRandom()
{
	Color color;
	color.r = Randomness::dist_1_0();
	color.g = Randomness::dist_1_0();
	color.b = Randomness::dist_1_0();
	float spreadChance = Randomness::dist_1_0();
	float aggressiveness = Randomness::dist_1_0();
	float strength = Randomness::dist_1_0();
	int strongDirection = Randomness::dist_8_0();
	return AntVariant(color, spreadChance, aggressiveness, strength, strongDirection);
}

bool AntVariant::RollAggression(const int & direction)
{
	if (Randomness::dist_1_0() <= m_aggressiveness + static_cast<float>(direction == m_strongDirection)*Weights::aggressivenessBias) {
		return true;
	}
	return false;
}

bool AntVariant::Attack(const AntVariant & neighbor, const int & direction)
{
	if (m_strength + static_cast<float>(m_strongDirection == direction)*Weights::strengthBias > neighbor.m_strength + static_cast<float>(neighbor.m_strongDirection == (8 - direction))*Weights::strengthBias) {
		return true;
	}
	return false;
}

vector<AntVariant> AntVariant::PlayTurn(const std::vector<AntVariant>& neighbors)
{
	vector<AntVariant> ResultVect;
	ResultVect.resize(9);
	float r = 0, g = 0, b = 0;
	int counter = 0;
	for (int i = 0; i < neighbors.size(); i++) {
		//cout << "Neighbor " << i << endl;
		if (neighbors[i].m_id == SpecialId::PlaceHolder) {
			ResultVect[i] = neighbors[i];
		}
		else if (neighbors[i].m_id == SpecialId::Empty) {
			if (Spread(i)) {
				m_spreadChance -= Weights::spreadDecOnSuccess;
				ResultVect[i] = AntVariant(*this);
				// For color logic
				counter++;
				r += m_color.r;
				g += m_color.g;
				b += m_color.b;
			}
			else {
				m_spreadChance += Weights::spreadIncOnFail;
				ResultVect[i] = neighbors[i];
				// Not considering empty cell for color
			}
		}
		else if (this->m_id == neighbors[i].m_id) {
			ResultVect[i] = AntVariant(*this);
			// For color logic
			counter++;
			r += neighbors[i].m_color.r;
			g += neighbors[i].m_color.g;
			b += neighbors[i].m_color.b;
		}
		else if (this->RollAggression(i)) {
			if (this->Attack(neighbors[i], i)) {
				ResultVect[i] = AntVariant(*this);
				// For color logic
				counter++;
				r += m_color.r;
				g += m_color.g;
				b += m_color.b;
			}
			else {
				ResultVect[i] = neighbors[i];
				// For color logic
				counter++;
				r += neighbors[i].m_color.r;
				g += neighbors[i].m_color.g;
				b += neighbors[i].m_color.b;
			}
		}
		else {
			ResultVect[i] = neighbors[i];
			// For color logic
			counter++;
			r += neighbors[i].m_color.r;
			g += neighbors[i].m_color.g;
			b += neighbors[i].m_color.b;
		}
	}
	if (Weights::mix) {
		m_color.r = (r + m_color.r*(Weights::ownWeightMult - 1)) / (counter + Weights::ownWeightMult - 1);
		m_color.g = (g + m_color.g*(Weights::ownWeightMult - 1)) / (counter + Weights::ownWeightMult - 1);
		m_color.b = (b + m_color.b*(Weights::ownWeightMult - 1)) / (counter + Weights::ownWeightMult - 1);
	}
	return ResultVect;
}