#pragma once

#include "MConstants.h"

#pragma region Upgrade & Mortgage

void CalculateAmountOfHouses(PlayerInfo players[]) {
	for (int p = 0; p < PlayerNum; p++) {
		players[p].TotalAmountOfHouses = 0;
		players[p].TotalAmountOfHotels = 0;
		for (unsigned int i = 0; i < 40; i++) {
			if (players[p].Land[i] > 1 && players[p].Land[i] < 6) { players[p].TotalAmountOfHouses += players[p].Land[i]; }
			else if (players[p].Land[i] >= 6 && players[p].Land[i] <= 9) { players[p].TotalAmountOfHotels += (players[p].Land[i] - 5); }
		}
	}
}
void CalculateAmountOfHouses(PlayerInfo players[], int p) {
	players[p].TotalAmountOfHouses = 0;
	players[p].TotalAmountOfHotels = 0;
	for (unsigned int i = 0; i < 40; i++) {
		if (players[p].Land[i] > 1 && players[p].Land[i] < 6) { players[p].TotalAmountOfHouses += players[p].Land[i]; }
		else if (players[p].Land[i] >= 6 && players[p].Land[i] <= 9) { players[p].TotalAmountOfHotels += (players[p].Land[i] - 5); }
	}
}
void CalculateTotalAssetValue(PlayerInfo players[]) {
	//his includes: Total cash on hand, mortgage value of mortgaged properties, printed price of UnMortgaged properties, and printed prices of buildings owned.
	for (int p = 0; p < PlayerNum; p++) {
		int AssetValue = 0;
		AssetValue += players[p].Money; //Total cash on hand
		for (unsigned int i = 0; i < 40; i++) {
			if (players[p].Land[i] > 0) {
				if (players[p].LandMortgaged[i]) { AssetValue += (int)(PropertyPrice[i] * 0.5); } //mortgage value of mortgaged properties
				else {
					AssetValue += PropertyPrice[i]; //printed price of UnMortgaged properties
					AssetValue += (HousePrice[i] * (players[p].Land[i] - 1)); //printed prices of buildings owned
				}
			}
		}
		if (AssetValue > 0) { players[p].TotalAssetValue = AssetValue; }
		else { players[p].TotalAssetValue = 0; }

	}
}

void GetPropertiesInTownship(int zone, std::vector<int>& arr) {
	if (zone == 0) { arr = { 1, 3 }; }	 //Brown
	else if (zone == 1) { arr = { 6, 8, 9 }; }	 //LightBlue
	else if (zone == 2) { arr = { 11,13,14 }; }	 //Pink
	else if (zone == 3) { arr = { 16,18,19 }; }	 //Orange
	else if (zone == 4) { arr = { 21,23,24 }; }	 //Red
	else if (zone == 5) { arr = { 26,27,29 }; }	 //Yellow
	else if (zone == 6) { arr = { 31,32,34 }; }	 //Green
	else if (zone == 7) { arr = { 37,39 }; }	 //Dark Blue
	else if (zone == 8) { arr = { 12,28 }; }	 //Utilities
	else if (zone == 9) { arr = { 5,15,25,35 }; }	 //Railroads
}
std::vector<int> GetPropertiesInTownship(int zone) {
	if (zone == 0) { return { 1, 3 }; }	 //Brown
	else if (zone == 1) { return { 6, 8, 9 }; }	 //LightBlue
	else if (zone == 2) { return { 11,13,14 }; }	 //Pink
	else if (zone == 3) { return { 16,18,19 }; }	 //Orange
	else if (zone == 4) { return { 21,23,24 }; }	 //Red
	else if (zone == 5) { return { 26,27,29 }; }	 //Yellow
	else if (zone == 6) { return { 31,32,34 }; }	 //Green
	else if (zone == 7) { return { 37,39 }; }	 //Dark Blue
	else if (zone == 8) { return { 12,28 }; }	 //Utilities
	else if (zone == 9) { return { 5,15,25,35 }; }	 //Railroads
	else { return { -1 }; }
}
int GetTownshipFromProperty(int zone) {
	if (zone == 1 || zone == 3) { return 0; } //Brown
	else if (zone == 6 || zone == 8 || zone == 9) { return 1; } //Light Blue
	else if (zone == 11 || zone == 13 || zone == 14) { return 2; } //Pink
	else if (zone == 16 || zone == 18 || zone == 19) { return 3; } //Orange
	else if (zone == 21 || zone == 23 || zone == 24) { return 4; } //Red
	else if (zone == 26 || zone == 27 || zone == 29) { return 5; } //Yellow
	else if (zone == 31 || zone == 32 || zone == 34) { return 6; } //Green
	else if (zone == 37 || zone == 39) { return 7; } //Dark Blue
	else if (zone == 12 || zone == 28) { return 8; } //Utilities
	else if (zone == 5 || zone == 15 || zone == 25 || zone == 35) { return 9; } //Railroads
	else { return -1; }
}

void CheckIfTownship(Data &Data_Info, PlayerInfo players[], int p) {
	if (p == Data_Info.LandOwnerShip[1] && p == Data_Info.LandOwnerShip[3]) { players[p].Townships[0] = 1; } //Brown
	if (p == Data_Info.LandOwnerShip[6] && p == Data_Info.LandOwnerShip[8] && p == Data_Info.LandOwnerShip[9]) { players[p].Townships[1] = 1; } //Light Blue
	if (p == Data_Info.LandOwnerShip[11] && p == Data_Info.LandOwnerShip[13] && p == Data_Info.LandOwnerShip[14]) { players[p].Townships[2] = 1; } //Pink
	if (p == Data_Info.LandOwnerShip[16] && p == Data_Info.LandOwnerShip[18] && p == Data_Info.LandOwnerShip[19]) { players[p].Townships[3] = 1; } //Orange
	if (p == Data_Info.LandOwnerShip[21] && p == Data_Info.LandOwnerShip[23] && p == Data_Info.LandOwnerShip[24]) { players[p].Townships[4] = 1; } //Red
	if (p == Data_Info.LandOwnerShip[26] && p == Data_Info.LandOwnerShip[27] && p == Data_Info.LandOwnerShip[29]) { players[p].Townships[5] = 1; } //Yellow
	if (p == Data_Info.LandOwnerShip[31] && p == Data_Info.LandOwnerShip[32] && p == Data_Info.LandOwnerShip[34]) { players[p].Townships[6] = 1; } //Green
	if (p == Data_Info.LandOwnerShip[37] && p == Data_Info.LandOwnerShip[39]) { players[p].Townships[7] = 1; } //Dark Blue
	if (p == Data_Info.LandOwnerShip[12] && p == Data_Info.LandOwnerShip[28]) { players[p].Townships[8] = 1; } //Utilities
	if (p == Data_Info.LandOwnerShip[5] && p == Data_Info.LandOwnerShip[15] && p == Data_Info.LandOwnerShip[25] && p == Data_Info.LandOwnerShip[35]) { players[p].Townships[9] = 1; } //Railroads

	//players[p].Townships.clear();
	//if (p == Data_Info.LandOwnerShip[1]  && p == Data_Info.LandOwnerShip[3]) { players[p].Townships.push_back(0); } //Brown
	//if (p == Data_Info.LandOwnerShip[6]  && p == Data_Info.LandOwnerShip[8]  && p == Data_Info.LandOwnerShip[9])  { players[p].Townships.push_back(1); } //Light Blue
	//if (p == Data_Info.LandOwnerShip[11] && p == Data_Info.LandOwnerShip[13] && p == Data_Info.LandOwnerShip[14]) { players[p].Townships.push_back(2); } //Pink
	//if (p == Data_Info.LandOwnerShip[16] && p == Data_Info.LandOwnerShip[18] && p == Data_Info.LandOwnerShip[19]) { players[p].Townships.push_back(3); } //Orange
	//if (p == Data_Info.LandOwnerShip[21] && p == Data_Info.LandOwnerShip[23] && p == Data_Info.LandOwnerShip[24]) { players[p].Townships.push_back(4); } //Red
	//if (p == Data_Info.LandOwnerShip[26] && p == Data_Info.LandOwnerShip[27] && p == Data_Info.LandOwnerShip[29]) { players[p].Townships.push_back(5); } //Yellow
	//if (p == Data_Info.LandOwnerShip[31] && p == Data_Info.LandOwnerShip[32] && p == Data_Info.LandOwnerShip[34]) { players[p].Townships.push_back(6); } //Green
	//if (p == Data_Info.LandOwnerShip[37] && p == Data_Info.LandOwnerShip[39]) { players[p].Townships.push_back(7); } //Dark Blue
}
bool CanUpgradeTownship(PlayerInfo players[], int p, int zone) {

	double Multiplyer = 2;
	if (zone == 0 && players[p].Money >= ((HousePrice[1] + HousePrice[3]) * Multiplyer)) { return true; } //Brown
	else if (zone == 1 && players[p].Money >= ((HousePrice[6] + HousePrice[8] + HousePrice[9])  * Multiplyer)) { return true; }  //Light Blue
	else if (zone == 2 && players[p].Money >= ((HousePrice[11] + HousePrice[13] + HousePrice[14]) * Multiplyer)) { return true; } //Pink
	else if (zone == 3 && players[p].Money >= ((HousePrice[16] + HousePrice[18] + HousePrice[19]) * Multiplyer)) { return true; } //Orange
	else if (zone == 4 && players[p].Money >= ((HousePrice[21] + HousePrice[23] + HousePrice[24]) * Multiplyer)) { return true; } //Red
	else if (zone == 5 && players[p].Money >= ((HousePrice[26] + HousePrice[27] + HousePrice[29]) * Multiplyer)) { return true; } //Yellow
	else if (zone == 6 && players[p].Money >= ((HousePrice[31] + HousePrice[32] + HousePrice[34]) * Multiplyer)) { return true; } //Green
	else if (zone == 7 && players[p].Money >= ((HousePrice[37] + HousePrice[39]) * Multiplyer)) { return true; } //Dark Blue
	else { return false; }
}
void UpgradeTownship(PlayerInfo players[], int p, std::string& Output) {

	if (players[p].Money <= 0) { return; }
	int Max = 6; //Need to add cases where is goes passed 6 (passed one hotel)
	if (players[p].AI) {
		bool BoughtSomething = true;
		for (; BoughtSomething;) {
			BoughtSomething = false;
			//Only check to (8) as (9 & 10) are [Utilities] & [Railroads] and can't be upgraded
			for (int i = 0; i < 8; i++) {
				int amount = 0;
				if (players[p].Townships[i] == 1) {
					switch (i)
					{
					case 0: if (CanUpgradeTownship(players, p, 0)) {
						if (players[p].Land[1] < Max) { players[p].Land[1] += 1; amount += HousePrice[1]; }
						if (players[p].Land[3] < Max) { players[p].Land[3] += 1; amount += HousePrice[3]; }
						if (amount > 0) {
							players[p].Money -= amount;
							Output.append("[Upgraded z0 $" + std::to_string(amount) + "] ");
							Output.append("[$" + std::to_string(players[p].Money) + "] ");
							CalculateAmountOfHouses(players, p);
							BoughtSomething = true;
						}
					} break; //Brown
					case 1: if (CanUpgradeTownship(players, p, 1)) {
						if (players[p].Land[6] < Max) { players[p].Land[6] += 1; amount += HousePrice[6]; }
						if (players[p].Land[8] < Max) { players[p].Land[8] += 1; amount += HousePrice[8]; }
						if (players[p].Land[9] < Max) { players[p].Land[9] += 1; amount += HousePrice[9]; }
						if (amount > 0) {
							players[p].Money -= amount;
							Output.append("[Upgraded z1 $" + std::to_string(amount) + "] ");
							Output.append("[$" + std::to_string(players[p].Money) + "] ");
							CalculateAmountOfHouses(players, p);
							BoughtSomething = true;
						}
					} break; //Light Blue
					case 2: if (CanUpgradeTownship(players, p, 2)) {
						if (players[p].Land[11] < Max) { players[p].Land[11] += 1; amount += HousePrice[11]; }
						if (players[p].Land[13] < Max) { players[p].Land[13] += 1; amount += HousePrice[13]; }
						if (players[p].Land[14] < Max) { players[p].Land[14] += 1; amount += HousePrice[14]; }
						if (amount > 0) {
							players[p].Money -= amount;
							Output.append("[Upgraded z2 $" + std::to_string(amount) + "] ");
							Output.append("[$" + std::to_string(players[p].Money) + "] ");
							CalculateAmountOfHouses(players, p);
							BoughtSomething = true;
						}
					} break; //Pink
					case 3: if (CanUpgradeTownship(players, p, 3)) {
						if (players[p].Land[16] < Max) { players[p].Land[16] += 1; amount += HousePrice[16]; }
						if (players[p].Land[18] < Max) { players[p].Land[18] += 1; amount += HousePrice[18]; }
						if (players[p].Land[19] < Max) { players[p].Land[19] += 1; amount += HousePrice[19]; }
						if (amount > 0) {
							players[p].Money -= amount;
							Output.append("[Upgraded z3 $" + std::to_string(amount) + "] ");
							Output.append("[$" + std::to_string(players[p].Money) + "] ");
							CalculateAmountOfHouses(players, p);
							BoughtSomething = true;
						}
					} break; //Orange
					case 4: if (CanUpgradeTownship(players, p, 4)) {
						if (players[p].Land[21] < Max) { players[p].Land[21] += 1; amount += HousePrice[21]; }
						if (players[p].Land[23] < Max) { players[p].Land[23] += 1; amount += HousePrice[23]; }
						if (players[p].Land[24] < Max) { players[p].Land[24] += 1; amount += HousePrice[24]; }
						if (amount > 0) {
							players[p].Money -= amount;
							Output.append("[Upgraded z4 $" + std::to_string(amount) + "] ");
							Output.append("[$" + std::to_string(players[p].Money) + "] ");
							CalculateAmountOfHouses(players, p);
							BoughtSomething = true;
						}
					} break; //Red
					case 5: if (CanUpgradeTownship(players, p, 5)) {
						if (players[p].Land[26] < Max) { players[p].Land[26] += 1; amount += HousePrice[26]; }
						if (players[p].Land[27] < Max) { players[p].Land[27] += 1; amount += HousePrice[27]; }
						if (players[p].Land[29] < Max) { players[p].Land[29] += 1; amount += HousePrice[29]; }
						if (amount > 0) {
							players[p].Money -= amount;
							Output.append("[Upgraded z5 $" + std::to_string(amount) + "] ");
							Output.append("[$" + std::to_string(players[p].Money) + "] ");
							CalculateAmountOfHouses(players, p);
							BoughtSomething = true;
						}
					} break; //Yellow
					case 6: if (CanUpgradeTownship(players, p, 6)) {
						if (players[p].Land[31] < Max) { players[p].Land[31] += 1; amount += HousePrice[31]; }
						if (players[p].Land[32] < Max) { players[p].Land[32] += 1; amount += HousePrice[32]; }
						if (players[p].Land[34] < Max) { players[p].Land[34] += 1; amount += HousePrice[34]; }
						if (amount > 0) {
							players[p].Money -= amount;
							Output.append("[Upgraded z6 $" + std::to_string(amount) + "] ");
							Output.append("[$" + std::to_string(players[p].Money) + "] ");
							CalculateAmountOfHouses(players, p);
							BoughtSomething = true;
						}
					} break; //Green
					case 7: if (CanUpgradeTownship(players, p, 7)) {
						if (players[p].Land[37] < Max) { players[p].Land[37] += 1; amount += HousePrice[37]; }
						if (players[p].Land[39] < Max) { players[p].Land[39] += 1; amount += HousePrice[39]; }
						if (amount > 0) {
							players[p].Money -= amount;
							Output.append("[Upgraded z7 $" + std::to_string(amount) + "] ");
							Output.append("[$" + std::to_string(players[p].Money) + "] ");
							CalculateAmountOfHouses(players, p);
							BoughtSomething = true;
						}
					} break; //Dark Blue
					case 8: break; //Utilities
					case 9: break; //Railroads
					default: break;
					}
				}
			}
		}
	}
	//Human Decides
	else {}
}
void CalculateLandPayment(Data& Data_Info, PlayerInfo players[], int p, std::string& Output) {

	if (players[p].Money <= 0) { return; }

	int space = players[p].position;
	int Owner = Data_Info.LandOwnerShip[space];

	//No one owns it
	if (Owner == -1 && PropertyPrice[space] > 0) {
		bool isBuy = false;
		if (players[p].AI) {
			double Multiplyer = 2;
			if (players[p].Money >= (PropertyPrice[space] * Multiplyer)) { isBuy = true; }

			//If buying the Property
			if (isBuy) {
				Output.append("[Bought $" + std::to_string(PropertyPrice[space]) + "] ");
				players[p].Money -= PropertyPrice[space];
				players[p].Land[space] = 1; //Counts as owning it (no houses yet)
				players[p].PiT[GetTownshipFromProperty(space)] += 1;
				Data_Info.LandOwnerShip[space] = p; //Look up table, to see who owns what
				CalculateAmountOfHouses(players, p);
				CheckIfTownship(Data_Info, players, p); //Checks if player has a group of all the same colours
			}
		}
		//Human Decides
		else {}
	}
}
void CalculateRentPayment(Data& Data_Info, PlayerInfo players[], int p, int dice, std::string& Output) {
	int space = players[p].position;
	int Owner = Data_Info.LandOwnerShip[space];

	if (Owner >= 0 && Owner != p) {
		if (!players[Owner].LandMortgaged[space]) {
			int Amount;
			//Utilities
			if (space == 12 || space == 28) {
				Amount = dice;
				if (Owner == Data_Info.LandOwnerShip[12] && Owner == Data_Info.LandOwnerShip[28]) { Amount = Amount * 10; }
				else { Amount = Amount * 4; }
				Output.append("[pt-" + std::to_string(Owner) + " $" + std::to_string(Amount) + "] ");
			}
			//Railroads
			else if (space % 5 == 0 && space % 2 != 0) {
				Amount = 25;
				for (int i = 5; i < 40; i += 10) {
					if (space != i && Owner == Data_Info.LandOwnerShip[i]) { Amount = Amount * 2; }
				}
				Output.append("[pt-" + std::to_string(Owner) + " $" + std::to_string(Amount) + "] ");
			}
			//
			else {
				//Above one hotel
				//if ((players[Owner].Land[space] - 1) > 5) { }
				Amount = (PropertyRent[space + (40 * (players[Owner].Land[space] - 1))]);
				Output.append("[pt-" + std::to_string(Owner) + " $" + std::to_string(Amount) + "] ");
			}
			players[Owner].Money += Amount;
			players[p].Money -= Amount;

			//Adds to the total the land has made.
			players[Owner].Land_Made[space] += Amount;
			//Adds to the total the township has made.
			players[Owner].Made_iT[GetTownshipFromProperty(space)] += Amount;
		}
	}
}

void UnMortgageProperty(PlayerInfo players[], int p, std::string& Output) {
	if (players[p].Money <= 0) { return; }
	if (players[p].AI) {
		std::string ADD;
		double Multiplyer = 6;
		//UnMortgage [Property] in a [Township]
		for (int i = 0; i < 10; i++) {
			//If (player) has [Township] (i)
			if (players[p].Townships[i] == 1) {
				std::vector<int> properties = GetPropertiesInTownship(i);
				for (int j = 0; j < properties.size(); j++) {
					int l = properties[j];
					if (players[p].Money < (PropertyPrice[l] * 0.5)*Multiplyer) { return; }

					//If (player) has [Property] that is in a [Township]; UnMortgage it
					if (i == GetTownshipFromProperty(l) && players[p].Land[l] == 1 && players[p].LandMortgaged[l] == true) {
						ADD = std::to_string(l); padTo(ADD, 2); Output.append("[UM:" + ADD + "] ");
						players[p].LandMortgaged[l] = false;
						players[p].Money -= (int)(PropertyPrice[l] * 0.5);
					}
				}
			}
		}
		//UnMortgage [Property] not in a [Township]
		for (int i = 0; i < 10; i++) {
			//If (player) does not have [Township] (i)
			if (players[p].Townships[i] == 0) {
				std::vector<int> properties = GetPropertiesInTownship(i);
				for (int j = 0; j < properties.size(); j++) {
					int l = properties[j];
					if (players[p].Money < (PropertyPrice[l] * 0.5)*Multiplyer) { return; }

					//If (player) has [Property] that is in a [Township]; UnMortgage it
					if (i == GetTownshipFromProperty(l) && players[p].Land[l] == 1 && players[p].LandMortgaged[l] == true) {
						ADD = std::to_string(l); padTo(ADD, 2); Output.append("[UM:" + ADD + "] ");
						players[p].LandMortgaged[l] = false;
						players[p].Money -= (int)(PropertyPrice[l] * 0.5);
					}
				}
			}
		}
	}
	//Human Decides
	else {}
}
void MortgageProperty(PlayerInfo players[], int p, std::string& Output) {
	if (players[p].AI) {
		std::string ADD;
		//Mortgage [Property] not in a [Township]
		for (int i = 0; i < 10; i++) {
			//If (player) does not have [Township] (i)
			if (players[p].Townships[i] == 0) {
				//Mortgage [Property] not in a [Township]
				std::vector<int> properties = GetPropertiesInTownship(i);
				for (int j = 0; j < properties.size(); j++) {
					int l = properties[j];
					//If (player) has [Property] that is not in a [Township]; Mortgage it
					if (i == GetTownshipFromProperty(l) && players[p].Land[l] == 1 && players[p].LandMortgaged[l] == false) {
						ADD = std::to_string(l); padTo(ADD, 2); Output.append("[M:" + ADD + "] ");
						players[p].LandMortgaged[l] = true;
						players[p].Money += (int)(PropertyPrice[l] * 0.5);
						//Has enough money to continue
						if (players[p].Money > 0) { return; }
					}
					else {}
				}
			}
		}
		//Sell [Houses] in [Townships]
		for (int i = 0; i < 10; i++) {
			if (players[p].Townships[i] == 1) {
				//Sell [Houses] in [Townships]
				std::vector<int> properties = GetPropertiesInTownship(i);
				for (int j = 0; j < properties.size(); j++) {
					int l = properties[j];
					//If (player) only has [Property] in [Townships] and has [Houses] in it; Sell [Houses]
					if (i == GetTownshipFromProperty(l) && players[p].Land[l] > 1 && players[p].LandMortgaged[l] == false) {
						ADD = std::to_string(l); padTo(ADD, 2); Output.append("[SH:" + ADD + "x");
						int k = 0;
						for (; 1 < players[p].Land[l]; k++) {
							//If (player) has [Houses] to Sell; Sell [Houses]
							players[p].Land[l] -= 1;
							players[p].Money += (int)(HousePrice[l] * 0.5);
							//Has enough money to continue
							if (players[p].Money > 0) { Output.append(std::to_string(k) + "] "); return; }
						}
						Output.append(std::to_string(k) + "] ");
					}
					//(player) does not own the [Township] or already Mortgaged or as no [Houses]
					else {}
				}
			}
		}
		//Mortgage [Township]
		for (int i = 0; i < 10; i++) {
			//If (player) has [Township] (i)
			if (players[p].Townships[i] == 1) {
				//Mortgage [Township]
				std::vector<int> properties = GetPropertiesInTownship(i);
				for (int j = 0; j < properties.size(); j++) {
					int l = properties[j];
					//If (player) only has [Property] in a [Township] //No [Houses]... Mortgage [Township]
					if (i == GetTownshipFromProperty(l) && players[p].Land[l] == 1 && players[p].LandMortgaged[l] == false) {
						ADD = std::to_string(l); padTo(ADD, 2); Output.append("[M:" + ADD + "] ");
						players[p].LandMortgaged[l] = true;
						players[p].Money += (int)(PropertyPrice[l] * 0.5);
						//Has enough money to continue
						if (players[p].Money > 0) { return; }
					}
					//(player) does not own the [Township] or already Mortgaged
					else {}
				}
			}
		}

		Output.append("[No Mortgage] ");
	}
	//Human Decides
	else {}
}

#pragma endregion