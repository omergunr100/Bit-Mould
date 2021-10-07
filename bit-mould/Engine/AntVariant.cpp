#include "AntVariant.h"

// Config
// Base stats increases tied to strongDirection
const float Weights::spreadBias = Config::GetAsFloat("SD_SPREAD");
const float Weights::aggressivenessBias = Config::GetAsFloat("SD_AGGRESSIVENESS");
const float Weights::strengthBias = Config::GetAsFloat("SD_STRENGTH");

// Colors
const bool Weights::mix = Config::GetAsBool("COLORS_MIX"); // Whether to mix the colors of nearby cells each turn
const int Weights::ownWeightMult = Config::GetAsInteger("COLORS_SELF_WEIGHT"); // Weight multiplier of the cell's own color in the mixing
// Stat decrease/increase based on actions
const float Weights::spreadDecOnSuccess = Config::GetAsFloat("STAT_SPREAD_DEC");
const float Weights::spreadIncOnFail = Config::GetAsFloat("STAT_SPREAD_INC");
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

void AntVariant::microTurn(AntVariant* AntPointer, Color* sumAll, int* counter, const int& direction)
{
	if (AntPointer->m_id == SpecialId::PlaceHolder) {
		return;
	}
	else if (AntPointer->m_id == SpecialId::Empty) {
		if (Spread(direction)) {
			m_spreadChance -= Weights::spreadDecOnSuccess;
			*AntPointer = AntVariant(*this);
			// For color logic
			*counter++;
			*sumAll += m_color;
		}
		else {
			m_spreadChance += Weights::spreadIncOnFail;
			// Not considering empty cell for color
		}
	}
	else if (this->m_id == AntPointer->m_id) {
		*AntPointer = AntVariant(*this);
		// For color logic
		*counter++;
		*sumAll += AntPointer->m_color;
	}
	else if (this->RollAggression(direction)) {
		if (this->Attack(*AntPointer, direction)) {
			*AntPointer = AntVariant(*this);
			// For color logic
			*counter++;
			*sumAll += m_color;
		}
		else {
			// For color logic
			*counter++;
			*sumAll += AntPointer->m_color;
		}
	}
	else {
		// For color logic
		*counter++;
		*sumAll += AntPointer->m_color;
	}
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

std::vector<AntVariant*> AntVariant::PlayTurn(const std::vector<AntVariant>& neighbors)
{
	std::vector<AntVariant*> ResultVect;
	ResultVect.resize(9);
	Color* sumAll = new Color();
	int* counter = new int(0);
	for (int i = 0; i < neighbors.size(); i++) {
		if (neighbors[i].m_id == SpecialId::PlaceHolder) {
			ResultVect[i] = new AntVariant(neighbors[i]);
		}
		else if (neighbors[i].m_id == SpecialId::Empty) {
			if (Spread(i)) {
				m_spreadChance -= Weights::spreadDecOnSuccess;
				ResultVect[i] = new AntVariant(*this);
				// For color logic
				*counter += 1;
				*sumAll += m_color;
			}
			else {
				m_spreadChance += Weights::spreadIncOnFail;
				ResultVect[i] = new AntVariant(neighbors[i]);
				// Not considering empty cell for color
			}
		}
		else if (this->m_id == neighbors[i].m_id) {
			ResultVect[i] = new AntVariant(*this);
			// For color logic
			*counter += 1;
			*sumAll += neighbors[i].m_color;
		}
		else if (this->RollAggression(i)) {
			if (this->Attack(neighbors[i], i)) {
				ResultVect[i] = new AntVariant(*this);
				// For color logic
				*counter += 1;
				*sumAll += m_color;
			}
			else {
				ResultVect[i] = new AntVariant(neighbors[i]);
				// For color logic
				*counter += 1;
				*sumAll += neighbors[i].m_color;
			}
		}
		else {
			ResultVect[i] = new AntVariant(neighbors[i]);
			// For color logic
			*counter += 1;
			*sumAll += neighbors[i].m_color;
		}
	}
	if (Weights::mix) {
		m_color.r = (sumAll->r + m_color.r*(Weights::ownWeightMult - 1)) / (*counter + Weights::ownWeightMult - 1);
		m_color.g = (sumAll->g + m_color.g*(Weights::ownWeightMult - 1)) / (*counter + Weights::ownWeightMult - 1);
		m_color.b = (sumAll->b + m_color.b*(Weights::ownWeightMult - 1)) / (*counter + Weights::ownWeightMult - 1);
	}
	delete sumAll, counter;
	return ResultVect;
}