#pragma once


#include <vector>



// Function to generate random numbers in given range  
float Rand(float LOW, float HIGH) {
	return  (LOW + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HIGH - LOW))));
}
float transferFunction(float x) {
	//tanh - output range [-1...1]
	return tanh(x);
}
float transferFunctionDerivative(float x) {
	//tanh Derivative
	return (1.0f - (x*x));
}


enum Features {
	ID,
	fitness,
	Money,
	TotalAmountOfHouses,
	TotalAmountOfHotels,
	TotalAssetValue,
	TotalLocationsLandedOn,
	TimesAroundBoard,
	COUNT
};


struct Individual {
	float Info[Features::COUNT];
	//int ID = -1;
	//int fitness = 0;
	//int Money = 0;
	//unsigned int TotalAmountOfHouses = 0;
	//unsigned int TotalAmountOfHotels = 0;
	//unsigned int TotalAssetValue = 0;
	//unsigned int TotalLocationsLandedOn = 0;
	//unsigned int TimesAroundBoard = 0;
	std::vector<float> TimesLanedOn = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0 };
	std::vector<float> MoneyMade = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0 };
};
// Class representing individual in population 
class Population {
public:
	float MutationRate = 0.1f;
	int PopulationSize = 200;
	Population();
	Population(int size);
	std::vector<Individual> _Population;
	std::vector<Individual> _Elite; //Best
	void Calculate_Fitness();
	void Create_Next_Population();
};
Population::Population() {
	_Population.clear();
	Individual blank;
	for (unsigned short int  i = 0; i < PopulationSize; i++) {
		blank.Info[Features::ID] = i;
		_Population.push_back(blank);
	}
}
Population::Population(int size) {
	PopulationSize = size;
	Individual blank;
	for (unsigned short int i = 0; i < PopulationSize; i++) {
		blank.Info[Features::ID] = i;
		_Population.push_back(blank);
	}
}
void Population::Calculate_Fitness() {
	_Elite.clear();
	for (unsigned short int i = 0; i < PopulationSize; i++) {
		_Population[i].Info[Features::fitness] = 1;
		//Calculate fitness here
		//
		//
		//Shorter or Longer Games?
		//More or Less Money?
		//More or Less Assets?

		//Should they buy Properties
		//Should they buy Townships
		//Should they buy Houses
		//Should they buy Hotels

		//Should they trade Properties
	}
}
void Population::Create_Next_Population() {
	float Inherit2 = (1 - MutationRate);
	float Inherit1 = (Inherit2 * 0.5f);
	std::vector<Individual> NewPopulation;

	for (unsigned short int P1 = 0; P1 < _Elite.size(); P1++) {
		_Elite[P1];
		int P2 = Rand(0, _Elite.size());
		Individual child;

		for (unsigned short int i = 0; i < Features::COUNT; i++) {
			float p = Rand(0, 1);
			if (p < Inherit1) { child.Info[i] = _Elite[P1].Info[i]; }
			else if (p < Inherit2) { child.Info[i] = _Elite[P2].Info[i]; }
			else {}
		}

		NewPopulation.push_back(child);
	}
	
	_Population = NewPopulation;
}


// Overloading < operator 
bool operator < (const Individual &ind1, const Individual &ind2) {
	return ind1.Info[Features::fitness] < ind2.Info[Features::fitness];
}