#pragma once

#include "MConstants.h"

#pragma region Trading

bool MajorityPropertyHolderT(PlayerInfo(&players)[PlayerNum], int p, int t) {
	//(player) has half of all [Properties] in that [Township]
	if (players[p].PiT[t] >= ceil(PiT_Max[t] * 0.5)) { return true; }
	else { return false; }
}
bool MajorityPropertyHolderL(PlayerInfo(&players)[PlayerNum], int p, int l) {
	//(player) has half of all [Properties] in that [Township]
	if (players[p].PiT[GetTownshipFromProperty(l)] >= ceil(PiT_Max[GetTownshipFromProperty(l)] * 0.5)) { return true; }
	else { return false; }
}
bool MinorityPropertyHolderT(PlayerInfo(&players)[PlayerNum], int p, int t) {
	//(player) has less then half of all [Properties] in that [Township]
	if (players[p].PiT[t] < ceil(PiT_Max[t] * 0.5) && players[p].PiT[t] > 0) { return true; }
	else { return false; }
}
bool MinorityPropertyHolderL(PlayerInfo(&players)[PlayerNum], int p, int l) {
	//(player) has less then half of all [Properties] in that [Township]
	if (players[p].PiT[GetTownshipFromProperty(l)] < ceil(PiT_Max[GetTownshipFromProperty(l)] * 0.5) && players[p].PiT[GetTownshipFromProperty(l)] > 0) { return true; }
	else { return false; }
}

int PriceChart(PlayerInfo(&players)[PlayerNum], int buyer, int seller, int l, bool Index[4]) {
	int value = 1;

	//Player One could finish a [Township]
	if (MajorityPropertyHolderL(players, buyer, l)) { value += 3; } //Player Two charges a lot more.
	else if (MinorityPropertyHolderL(players, buyer, l)) { value += 2; } //Player Two charges more.
	//Player One could buy a [Township]
	else if (MajorityPropertyHolderL(players, seller, l)) { value += 1; } //Player Two charges more.
	else if (MinorityPropertyHolderL(players, seller, l)) { value += 0; } //Player Two charges default.

	//All four combinations
	if (Index[0] && Index[1] && Index[2] && Index[3]) { value += 4; } //Player One, has everything. //Player Two charges a lot.
	else if (!Index[0] && !Index[1] && !Index[2] && !Index[3]) { value += 0; } //Player Two, has everything.
	//All one combinations
	else if (!Index[0] && Index[1] && Index[2] && Index[3]) { value += 0; } //Player One, less [Money]		 but more of everything else.
	else if (Index[0] && !Index[1] && Index[2] && Index[3]) { value += 3; } //Player One, less [Township]	 but more of everything else.
	else if (Index[0] && Index[1] && !Index[2] && Index[3]) { value += 3; } //Player One, less [Properties]	 but more of everything else.
	else if (Index[0] && Index[1] && Index[2] && !Index[3]) { value += 3; } //Player One, less [Houses]		 but more of everything else.
	//All three combinations
	else if (Index[0] && !Index[1] && !Index[2] && !Index[3]) { value += 2; } //Player One, more [Money]		 but less of everything else.
	else if (!Index[0] && Index[1] && !Index[2] && !Index[3]) { value += 1; } //Player One, more [Township]	 but less of everything else.
	else if (!Index[0] && !Index[1] && Index[2] && !Index[3]) { value += 1; } //Player One, more [Properties]	 but less of everything else.
	else if (!Index[0] && !Index[1] && !Index[2] && Index[3]) { value += 1; } //Player One, more [Houses]		 but less of everything else.
	//All two combinations
	else if (!Index[0] && !Index[1] && Index[2] && Index[3]) { value += 0; } //Player One, less [Money]		[Township]		//Player Two, more [Properties]	[Houses]	
	else if (!Index[0] && Index[1] && !Index[2] && Index[3]) { value += 0; } //Player One, less [Money]		[Properties]	//Player Two, more [Township]	[Houses]	
	else if (Index[0] && !Index[1] && !Index[2] && Index[3]) { value += 2; } //Player One, less [Township]	[Properties]	//Player Two, more [Money]		[Houses]	
	else if (!Index[0] && Index[1] && Index[2] && !Index[3]) { value += 0; } //Player One, less [Money]		[Houses]		//Player Two, more [Township]	[Properties]
	else if (Index[0] && !Index[1] && Index[2] && !Index[3]) { value += 2; } //Player One, less [Township]	[Houses]		//Player Two, more [Money]		[Properties]
	else if (Index[0] && Index[1] && !Index[2] && !Index[3]) { value += 2; } //Player One, less [Properties]	[Houses]		//Player Two, more [Money]		[Township]	

	return value;

	/*
	//All four combinations
		//Player One, has everything.
		//Player Two, has everything.
	//All one combinations
		//Player One, less [Money]		 but more of everything else.
		//Player One, less [Township]	 but more of everything else.
		//Player One, less [Properties]	 but more of everything else.
		//Player One, less [Houses]		 but more of everything else.
	//All three combinations
		//Player Two, less [Money]		 but more of everything else.
		//Player Two, less [Township]	 but more of everything else.
		//Player Two, less [Properties]	 but more of everything else.
		//Player Two, less [Houses]		 but more of everything else.
	//All two combinations
		//Player One, less [Money]		[Township]		//Player Two, more [Properties]	[Houses]
		//Player One, less [Money]		[Properties]	//Player Two, more [Township]	[Houses]
		//Player One, less [Township]	[Properties]	//Player Two, more [Money]		[Houses]
		//Player One, less [Money]		[Houses]		//Player Two, more [Township]	[Properties]
		//Player One, less [Township]	[Houses]		//Player Two, more [Money]		[Properties]
		//Player One, less [Properties]	[Houses]		//Player Two, more [Money]		[Township]
	*/
}

void Trade_Buy(Data& Data_Info, PlayerInfo(&players)[PlayerNum], int buyer, int seller, int space, int price) {

	Data_Info.LandOwnerShip[space] = buyer; //Look up table, to see who owns what

	players[buyer].Money -= price;
	players[buyer].Land[space] = 1; //Counts as owning it (no houses yet)
	players[buyer].PiT[GetTownshipFromProperty(space)] += 1;
	CalculateAmountOfHouses(players, buyer);
	CheckIfTownship(Data_Info, players, buyer); //Checks if player has a group of all the same colours


	players[seller].Money += price;
	players[seller].Land[space] = 0;
	players[seller].PiT[GetTownshipFromProperty(space)] -= 1;
	CalculateAmountOfHouses(players, seller);
	CheckIfTownship(Data_Info, players, seller); //Checks if player has a group of all the same colours
}
void AIRequestTrade(Data& Data_Info, PlayerInfo(&players)[PlayerNum], int buyer, int seller, int space, std::string& Output) {
	if (players[seller].AI) {

		//buyer, seller, player with the upper-hand
		int PlayerIndex[2][4];
		bool Index[4];
		//Amount of [Money] a player owns over the amount of the [Property] Price
		PlayerIndex[0][0] = (players[buyer].Money / PropertyPrice[space]);
		PlayerIndex[1][0] = (players[seller].Money / PropertyPrice[space]);
		//Amount of [Township] a player owns
		for (int i = 0; i < 10; i++) {
			if (players[buyer].Townships[i] == 1) { PlayerIndex[0][1] += 1; }
			if (players[seller].Townships[i] == 1) { PlayerIndex[1][1] += 1; }
		}
		//[Properties] & [Houses]
		for (int i = 0; i < 40; i++) {
			if (players[buyer].Land[i] > 0) {
				//Amount of [Properties] a player owns
				PlayerIndex[0][2] += 1;
				//Amount of [Houses] a player owns
				PlayerIndex[0][3] += players[buyer].Land[i];
			}
			if (players[seller].Land[i] > 0) {
				//Amount of [Properties] a player owns
				PlayerIndex[1][2] += 1;
				//Amount of [Houses] a player owns
				PlayerIndex[1][3] += players[seller].Land[i];
			}
		}


		//What player has the upper-hand [Money]
		if (PlayerIndex[0][0] == PlayerIndex[1][0]) {
			if ((players[buyer].Money / PropertyPrice[space]) > (players[seller].Money / PropertyPrice[space])) { Index[0] = true; }
			else { Index[0] = false; }
		}
		else if (PlayerIndex[0][0] > PlayerIndex[1][0]) { Index[0] = true; }
		else { Index[0] = false; }
		//What player has the upper-hand [Township]
		if (PlayerIndex[0][1] > PlayerIndex[1][1]) { Index[1] = true; }
		else { Index[1] = false; }
		//What player has the upper-hand [Properties]
		if (PlayerIndex[0][2] > PlayerIndex[1][2]) { Index[2] = true; }
		else { Index[2] = false; }
		//What player has the upper-hand [Houses]
		if (PlayerIndex[0][3] > PlayerIndex[1][3]) { Index[3] = true; }
		else { Index[3] = false; }


		int price = (int)(PriceChart(players, buyer, seller, space, Index)*0.25f*PropertyPrice[space]);

		//Charge Player One as much money as possible
		if (players[buyer].Money > price) {
			Output.append("[Trade:" + std::to_string(space) + "| P" + std::to_string(buyer) + " -P" + std::to_string(seller) + "] ");
			Output.append("[$" + std::to_string(PropertyPrice[space]) + "|" + std::to_string(price) + "] ");
			Trade_Buy(Data_Info, players, buyer, seller, space, price);
		}
		//All but $5 of Player Ones money
		else if (players[buyer].Money < price && players[buyer].Money > PropertyPrice[space]) {
			Output.append("[Trade:" + std::to_string(space) + "| P" + std::to_string(buyer) + " -P" + std::to_string(seller) + "] ");
			Output.append("[$" + std::to_string(PropertyPrice[space]) + "|" + std::to_string(players[buyer].Money - 5) + "] ");
			Trade_Buy(Data_Info, players, buyer, seller, space, players[buyer].Money - 5);
		}
		//Simple way of buying the [Properties]
		else if (players[buyer].Money > PropertyPrice[space]) {
			Output.append("[Trade:" + std::to_string(space) + "| P" + std::to_string(buyer) + " -P" + std::to_string(seller) + "] ");
			Trade_Buy(Data_Info, players, buyer, seller, space, PropertyPrice[space]);
		}

	}
	//AI trading with a human
	else {}
}
void PlayerRequestTrade(Data& Data_Info, PlayerInfo(&players)[PlayerNum], std::string& Output, int space1, int space2, int value1 = 0, int value2 = 0) {

}
void Trade(Data& Data_Info, PlayerInfo(&players)[PlayerNum], int p, std::string& Output) {

	if (players[p].Money <= 0) { return; }

	for (int i = 0; i < 10; i++) {
		//(player) has a full [Township]
		if (players[p].PiT[i] == PiT_Max[i]) {}
		//(player) has no [Properties] in that [Township]
		else if (players[p].PiT[i] == 0) {}
		//(player) has half of all [Properties] in that [Township]
		else if (MajorityPropertyHolderT(players, p, i)) {
			std::vector<int> properties = GetPropertiesInTownship(i);
			for (int l = 0; l < properties.size(); l++) {
				//Check to see if someone owns the missing property; Try to buy it
				int Owner = Data_Info.LandOwnerShip[properties[l]];
				if (p != Owner && Owner != -1) { AIRequestTrade(Data_Info, players, p, Owner, properties[l], Output); }
			}
		}
		//(player) has less then half of all [Properties] in that [Township]
		else {}
	}

	//If (player) has the majority of [Properties] in a [Township]; Try to get the remaining [Property]

	//If (player) has a lot of money and [Townships]; Try to get more [Properties]

}
//Player p is trying to sell his properties to not Die
void FindBuyer(Data& Data_Info, PlayerInfo(&players)[PlayerNum], int p, std::string& Output) {

	if (players[p].Money > 0) { return; }

	if (players[p].AI) {

		//(player two) has half of all [Properties] in that [Township]
		for (int pp = 0; pp < PlayerNum; pp++) {
			if (players[pp].Money > 0 && pp != p && players[pp].AI) {
				for (int i = 0; i < 10; i++) {
					//(player two) has a full [Township]; won't help (player one)
					if (players[pp].PiT[i] == PiT_Max[i]) {}
					//(player two) has half of all [Properties] in that [Township]
					else if (MajorityPropertyHolderT(players, pp, i)) {
						std::vector<int> properties = GetPropertiesInTownship(i);
						for (int l = 0; l < properties.size(); l++) {
							//Check to see if (player one) owns the missing property; Try to buy it
							int Owner = Data_Info.LandOwnerShip[properties[l]];
							if (p == Owner && pp != Owner && Owner != -1) { AIRequestTrade(Data_Info, players, pp, p, properties[l], Output); }
							if (players[p].Money > 0) { return; }
						}
					}
				}
			}
		}
		//(player two) has less then half of all [Properties] in that [Township]
		for (int pp = 0; pp < PlayerNum; pp++) {
			if (players[pp].Money > 0 && pp != p && players[pp].AI) {
				for (int i = 0; i < 10; i++) {
					//(player two) has a full [Township]; won't help (player one)
					if (players[pp].PiT[i] == PiT_Max[i]) {}
					//(player two) has less then half of all [Properties] in that [Township]
					else if (MinorityPropertyHolderT(players, pp, i)) {
						std::vector<int> properties = GetPropertiesInTownship(i);
						for (int l = 0; l < properties.size(); l++) {
							//Check to see if (player one) owns the missing property; Try to buy it
							int Owner = Data_Info.LandOwnerShip[properties[l]];
							if (p == Owner && pp != Owner && Owner != -1) { AIRequestTrade(Data_Info, players, pp, p, properties[l], Output); }
							if (players[p].Money > 0) { return; }
						}
					}
				}
			}
		}
		//(player one) has a full [Township]
		for (int pp = 0; pp < PlayerNum; pp++) {
			if (players[pp].Money > 0 && pp != p && players[pp].AI) {
				for (int i = 0; i < 10; i++) {
					//(player two) has a full [Township]; won't help (player one)
					if (players[pp].PiT[i] == PiT_Max[i]) {}
					//(player one) has a full [Township]
					else if (players[p].PiT[i] == PiT_Max[i]) {
						std::vector<int> properties = GetPropertiesInTownship(i);
						for (int l = 0; l < properties.size(); l++) {
							//Try to buy the [Properties]
							if (pp != p && p != -1) { AIRequestTrade(Data_Info, players, pp, p, properties[l], Output); }
							if (players[p].Money > 0) { return; }
						}
					}
				}
			}
		}
		//(player two) has no [Properties] in that [Township]
		for (int pp = 0; pp < PlayerNum; pp++) {
			if (players[pp].Money > 0 && pp != p && players[pp].AI) {
				for (int i = 0; i < 10; i++) {
					//(player two) has a full [Township]; won't help (player one)
					if (players[pp].PiT[i] == PiT_Max[i]) {}
					//(player two) has no [Properties] in that [Township]
					else if (players[pp].PiT[i] == 0) {
						std::vector<int> properties = GetPropertiesInTownship(i);
						for (int l = 0; l < properties.size(); l++) {
							//Check to see if (player one) owns the missing property; Try to buy it
							int Owner = Data_Info.LandOwnerShip[properties[l]];
							if (p == Owner && pp != Owner && Owner != -1) { AIRequestTrade(Data_Info, players, pp, p, properties[l], Output); }
							if (players[p].Money > 0) { return; }
						}
					}
				}
			}
		}

		Output.append("[No Trade] ");
	}
	//Human Decides
	else {}
}

#pragma endregion