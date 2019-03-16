#pragma once

#include <vector>		//vector
#include <string>		//string

#pragma region Variables

//Used for CMD output
const bool Display = false;
const bool DisplaySpecificRounds = false;
const int  SpecificRound = 600;

const int TradePastRound = 5;
const int UpgradePastRound = 5;
const int UnMortgagePastRound = 5;
const bool FreeParkingRule = true;
const int JailFee = 50;
const int RollsPerGame = 800; //Total rolls per game
const int Games = 100; //Total amount of games
const int PlayerNum = 4;
//Names of all Streets in order
const std::string StreetNames[] = {
	"GO","Mediterranean Ave.","Community Chest","Baltic Ave.","Income Tax","Reading Railroad","Oriental Ave.","Chance","Vermont Ave.","Connecticut Ave.",
	"Just Visiting Jail","St. Charles Place","Electric Company","States Ave.","Virginia Ave.","Pennsylvania Railroad","St. James Place","Community Chest","Tennessee Ave.","New York Ave.",
	"Free Parking","Kentucky Ave.","Chance","Indiana Ave.","Illinois Ave.","B. & O. Railroad","Atlantic Ave.","Ventnor Ave.","Water Works","Marvin Gardens",
	"Go to Jail","Pacific Ave.","North Carolina Ave.","Community Chest","Pennsylvania Ave.","Short Line Railroad","Chance","Park Place","Luxury Tax","Boardwalk",
	"Jail"
};
const int PropertyPrice[] = { 
	0, 60,  0, 60,  0,   200,100,  0,100,120,
	0,140,150,140,160,   200,180,  0,180,200, 
	0,220,  0,220,240,   200,260,260,150,280, 
	0,300,300,  0,320,   200,  0,350,  0,400 
}; //Mortgage is worth half the price
const int HousePrice[] = {
	0, 50,  0, 50,  0,     0, 50,  0, 50, 50,
	0,100,  0,100,100,     0,100,  0,100,100,
	0,150,  0,150,150,     0,150,150,  0,150,
	0,200,200,  0,200,     0,  0,200,  0,200
}; //Selling is worth half the price
const int PropertyRent[] = { 
	//Zero Houses
	0,   2,   0,   4,   0,   'r',   6,   0,   6,   8,
	0,  10, 'u',  10,  12,   'r',  14,   0,  14,  16,
	0,  18,   0,  18,  20,   'r',  22,  22, 'u',  24,
	0,  26,  26,   0,  28,   'r',   0,  35,   0,  50
	,//One House
	0,  10,   0,  20,   0,   'r',  30,   0,  30,  40,
	0,  50, 'u',  50,  60,   'r',  70,   0,  70,  80,
	0,  90,   0,  90, 100,   'r', 110, 110, 'u', 120,
	0, 130, 130,   0, 150,   'r',   0, 175,   0, 200
	,//Two Houses
	0,  30,   0,  60,   0,   'r',  90,   0,  90, 100,
	0, 150, 'u', 150, 180,   'r', 200,   0, 200, 220,
	0, 250,   0, 250, 300,   'r', 330, 330, 'u', 360,
	0, 390, 390,   0, 450,   'r',   0, 500,   0, 600
	,//Three Houses
	0,  90,   0, 180,   0,   'r', 270,   0, 270, 300,
	0, 450, 'u', 450, 500,   'r', 550,   0, 550, 600,
	0, 700,   0, 700, 750,   'r', 800, 800, 'u', 850,
	0, 900, 900,   0,1000,   'r',   0,1100,   0,1400
	,//Four Houses
	0, 160,   0, 320,   0,   'r', 400,   0, 400, 450,
	0, 625, 'u', 625, 700,   'r', 700,   0, 700, 800,
	0, 875,   0, 875, 925,   'r', 975, 975, 'u',1025,
	0,1100,1100,   0,1200,   'r',   0,1300,   0,1700
	,//One Hotel
	0, 250,   0, 450,   0,   'r', 550,   0, 550, 600,
	0, 750, 'u', 750, 900,   'r', 900,   0, 950,1000,
	0,1050,   0,1050,1100,   'r',1150,1150, 'u',1200,
	0,1275,1275,   0,1400,   'r',   0,1500,   0,2000
};
const int PiT_Max[10] = { 2,3,3,3,3, 3,3,2,2,4 }; //Max number of [Properties] in each [Township]

const int rollvalues[] = { 2, 3, 4, 5, 6, 7, 3, 4, 5, 6, 7, 8, 4, 5, 6, 7, 8, 9, 5, 6, 7, 8, 9, 10, 6, 7, 8, 9, 10, 11, 7, 8, 9, 10, 11, 12 }; //Roll values are values from a six by six grid for all dice rolls
const int DoubleIndex[] = { 0, 7, 14, 21, 28, 35 }; //These are the dice index values for double rolls
const std::vector<std::string> master_chest = { "0", "$200", "-50", "$50", "F", "J", "c50", "$100", "$20", "c10", "$100", "-100", "-150", "$25", "sr", "$10", "$100" }; //Community Chest
const std::vector<std::string> master_chance = { "0", "24", "11", "U", "R", "$50", "F", "B", "J", "hr", "-15", "5", "39", "p50", "$150", "$100" }; //Chance

#pragma endregion

#pragma region Structs

struct PlayerInfo {
	bool AI = true;
	//Temporary per-game
	unsigned int OldPosition = 0;//Old Player position
	unsigned int position = 0;	 //Player position
	unsigned int Doubles = 0;	 //Amount of doubles in a row
	unsigned int InJail = 0;	 //Amount of turns the player is in jail for (3, 2, 1, 0)
	unsigned int JailPass = 0;	 //Amount of (get out of jail free) passes
	int Money = 1500;			 //Amount of money a player has

	//Data 
	//Land the player owns and the stage it is at
	std::vector<int> Land = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0 };
	std::vector<bool> LandMortgaged = {
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false
	};
	//std::vector<int> Townships = {};
	int Townships[10] = { 0,0,0,0,0, 0,0,0,0,0 };
	int PiT[10] = { 0,0,0,0,0, 0,0,0,0,0 }; //[Properties] In [Townships]
	unsigned int TotalAmountOfHouses = 0;
	unsigned int TotalAmountOfHotels = 0;
	unsigned int TotalAssetValue = 0;
	//Count how many times the player landed on each square
	std::vector<int> Squares = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0 };
	unsigned int TotalLocationsLandedOn = 0;
	unsigned int TimesAroundBoard = 0;

	bool isDead = false;
	unsigned int DiedAt = 0;


	//How much a Land has made the player
	std::vector<int> Land_Made = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0 };
	int Made_iT[10] = { 0,0,0,0,0, 0,0,0,0,0 }; //Made In [Townships]
};
struct Data_Player {
	//The end amount of money the player was able to acquire
	std::vector<int> Money;
	//Total amount of Houses the Player was able to acquire
	std::vector<int> Houses;
	//Total amount of Hotels the Player was able to acquire
	std::vector<int> Hotels;
	//Value of all the assets the Player was able to acquire
	std::vector<int> AssetValue;
	//Total amount of locations landed on
	std::vector<int> LocationsLandedOn;
	//Amount of times the Player has made it around the board
	std::vector<int> TimesAroundBoard;
	//Round Player Died at
	std::vector<int> DiedAt;
	//Wins & Losses
	std::vector<int> Place;

	//Count how many times the player landed on each square
	std::vector<int> Squares = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0 };
	//Land the player owns and the stage it is at
	std::vector<int> Land = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0 };


	//How much a Land has made the player
	std::vector<int> Land_Made = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0 };
	int Made_iT[10] = { 0,0,0,0,0, 0,0,0,0,0 }; //Made In [Townships]

};
struct Data {
	std::vector<Data_Player> Players;
	std::vector<int> Rolls;
	int LandOwnerShip[41];// = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,-1,-1, -1 };
};

#pragma endregion

#pragma region Basic Functions
void padTo(std::string &str, const size_t num, const char paddingChar = ' ') {
	if (num > str.size()) { str.insert(0, num - str.size(), paddingChar); }
}
template<typename T>
T GetAverageVector(std::vector<T> vec) {
	T Average = 0.0f;
	for (int i = 0; i < vec.size(); i++) { Average += vec[i]; }
	return (Average * (1.0f / vec.size()));
}
int GetBoolAmount(std::vector<bool> vec) {
	int amount = 0;
	for (int i = 0; i < vec.size(); i++) { if (vec[i] == true) { amount++; }; }
	return amount;
}
template<typename T>
int GetAmountOfNum(std::vector<T> vec, T num) {
	int amount = 0;
	for (int i = 0; i < vec.size(); i++) { if (vec[i] == num) { amount++; }; }
	return amount;
}

#pragma endregion