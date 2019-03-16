﻿#pragma once

#include <iostream>		//cout
#include <iomanip>		//setw
#include <windows.h>	//WinApi header
#include <algorithm>    // std::random_shuffle
#include <random>		//random_device, sort, random_shuffle
#include <functional>	//Bind

#include "MMortgage.h"
#include "MTrade.h"
#include "Net.h"

HANDLE  hConsole;
Data Data_Info;

int y = 0;
int x = 120;

std::vector<std::string> StuffAndThings;

#pragma region Nerual Network
//33 inputs, 10 outputs. 
//std::vector<unsigned int> topology = { 34,22,11 };
std::vector<unsigned int> topology = { 16,11,11 };
//4 network, 1 for each player.
Net network[4];
std::vector<double> InputValues;
std::vector<double> ResultValues;
std::vector<double> TargetValues;
std::vector<double> Winning_TargetValues = { 1,1,1,1,1, 1,1,1,1,1 };
std::vector<double> Losing_TargetValues = { 0,0,0,0,0, 0,0,0,0,0 };


void feedForwardAI(PlayerInfo players[], int p) {
	InputValues = {
				(double)players[p].Money,
				(double)players[p].TimesAroundBoard,
				(double)players[p].TotalAmountOfHouses,
				(double)players[p].TotalAmountOfHotels,
				(double)players[p].TotalAssetValue,
				(double)players[p].Made_iT[0],
				(double)players[p].Made_iT[1],
				(double)players[p].Made_iT[2],
				(double)players[p].Made_iT[3],
				(double)players[p].Made_iT[4],
				(double)players[p].Made_iT[5],
				(double)players[p].Made_iT[6],
				(double)players[p].Made_iT[7],
				(double)players[p].Made_iT[8],
				(double)players[p].Made_iT[9]

				////Land Made: 28
				////First Side: 6
				//(double)players[p].Land_Made[1],
				//(double)players[p].Land_Made[3],
				//(double)players[p].Land_Made[5],//Second Half
				//(double)players[p].Land_Made[6],
				//(double)players[p].Land_Made[8],
				//(double)players[p].Land_Made[9],
				////Second Side: 8
				//(double)players[p].Land_Made[11],
				//(double)players[p].Land_Made[12],
				//(double)players[p].Land_Made[13],
				//(double)players[p].Land_Made[14],
				//(double)players[p].Land_Made[15],//Second Half
				//(double)players[p].Land_Made[16],
				//(double)players[p].Land_Made[18],
				//(double)players[p].Land_Made[19],
				////Third Side: 8
				//(double)players[p].Land_Made[21],
				//(double)players[p].Land_Made[23],
				//(double)players[p].Land_Made[24],
				//(double)players[p].Land_Made[25],//Second Half
				//(double)players[p].Land_Made[26],
				//(double)players[p].Land_Made[27],
				//(double)players[p].Land_Made[28],
				//(double)players[p].Land_Made[29],
				////Forth Side: 6
				//(double)players[p].Land_Made[31],
				//(double)players[p].Land_Made[32],
				//(double)players[p].Land_Made[34],
				//(double)players[p].Land_Made[35],//Second Half
				//(double)players[p].Land_Made[37],
				//(double)players[p].Land_Made[39]
	};
	network[p].feedForward(InputValues);
	network[p].getResults(ResultValues);
}

void backPropagationAI(PlayerInfo players[], int p, bool Good) {
	if (!Good) { network[p].backPropagation(Losing_TargetValues); }
	else { network[p].backPropagation(Winning_TargetValues); }

	//TargetValues = {
	//	(double)players[i].Townships[0],
	//	(double)players[i].Townships[1],
	//	(double)players[i].Townships[2],
	//	(double)players[i].Townships[3],
	//	(double)players[i].Townships[4],
	//	(double)players[i].Townships[5],
	//	(double)players[i].Townships[6],
	//	(double)players[i].Townships[7],
	//	(double)players[i].Townships[8],
	//	(double)players[i].Townships[9]
	//};
	//network[p].backPropagation(TargetValues);
}

#pragma endregion

#pragma region Data Functions

void ChangeColour(int i) {
	// colors are [0=black, 1=blue,  2=green,  3 = Aqua,  4 = Red,  5 = Purple,  6 = Yellow,  7 = White]
	// then light [8=Gray , 9=blue, 10=green, 11 = Aqua, 12 = Red, 13 = Purple, 14 = Yellow, 15 = Bright White]
	// colorattribute = foreground + (background * 16)
	switch (i) {
	case  0: SetConsoleTextAttribute(hConsole, 7); break;
	case  1: SetConsoleTextAttribute(hConsole, 80); break; //Purple
	case  2: SetConsoleTextAttribute(hConsole, 2); break;
	case  3: SetConsoleTextAttribute(hConsole, 80); break; //Purple
	case  4: SetConsoleTextAttribute(hConsole, 4); break;
	case  5: SetConsoleTextAttribute(hConsole, 112); break; //White
	case  6: SetConsoleTextAttribute(hConsole, 176); break; //Light Aqua
	case  7: SetConsoleTextAttribute(hConsole, 10); break;
	case  8: SetConsoleTextAttribute(hConsole, 176); break; //Light Aqua
	case  9: SetConsoleTextAttribute(hConsole, 176); break; //Light Aqua
	case 10: SetConsoleTextAttribute(hConsole, 2); break;
	case 11: SetConsoleTextAttribute(hConsole, 208); break; //Light Purple
	case 12: SetConsoleTextAttribute(hConsole, 128); break; //Grey
	case 13: SetConsoleTextAttribute(hConsole, 208); break; //Light Purple
	case 14: SetConsoleTextAttribute(hConsole, 208); break; //Light Purple
	case 15: SetConsoleTextAttribute(hConsole, 112); break; //White
	case 16: SetConsoleTextAttribute(hConsole, 96); break; //Yellow
	case 17: SetConsoleTextAttribute(hConsole, 2); break;
	case 18: SetConsoleTextAttribute(hConsole, 96); break; //Yellow
	case 19: SetConsoleTextAttribute(hConsole, 96); break; //Yellow
	case 20: SetConsoleTextAttribute(hConsole, 128); break; //Grey
	case 21: SetConsoleTextAttribute(hConsole, 192); break; //Light Red
	case 22: SetConsoleTextAttribute(hConsole, 10); break;
	case 23: SetConsoleTextAttribute(hConsole, 192); break; //Light Red
	case 24: SetConsoleTextAttribute(hConsole, 192); break; //Light Red
	case 25: SetConsoleTextAttribute(hConsole, 112); break; //White
	case 26: SetConsoleTextAttribute(hConsole, 224); break; //Light Yellow
	case 27: SetConsoleTextAttribute(hConsole, 224); break; //Light Yellow
	case 28: SetConsoleTextAttribute(hConsole, 128); break; //Grey
	case 29: SetConsoleTextAttribute(hConsole, 224); break; //Light Yellow
	case 30: SetConsoleTextAttribute(hConsole, 12); break;
	case 31: SetConsoleTextAttribute(hConsole, 160); break; //Light Green
	case 32: SetConsoleTextAttribute(hConsole, 160); break; //Light Green
	case 33: SetConsoleTextAttribute(hConsole, 2); break;
	case 34: SetConsoleTextAttribute(hConsole, 160); break; //Light Green
	case 35: SetConsoleTextAttribute(hConsole, 112); break; //White
	case 36: SetConsoleTextAttribute(hConsole, 10); break;
	case 37: SetConsoleTextAttribute(hConsole, 144); break; //Light Blue
	case 38: SetConsoleTextAttribute(hConsole, 4); break;
	case 39: SetConsoleTextAttribute(hConsole, 144); break; //Light Blue
	case 40: SetConsoleTextAttribute(hConsole, 12); break;

	default: SetConsoleTextAttribute(hConsole, 7); break;
	}
}

void GetData() {

	std::string Output = "";
	std::string ADD;
	std::cout << std::setw(28) << "Average over " << Games << " Games"<< std::endl;

	ADD = std::to_string(GetAverageVector(Data_Info.Rolls)); padTo(ADD, 5); Output.append("["+ADD+"] ");
	std::cout << std::setw(28) << "[Rolls] " << Output << std::endl; Output = "";
	for (int i = 0; i < PlayerNum; i++) { ADD = std::to_string(GetAverageVector(Data_Info.Players[i].Money));				 padTo(ADD, 5); Output.append("["+ADD+"] "); }
	std::cout << std::setw(28) << "[Money] " << Output << std::endl; Output = "";
	for (int i = 0; i < PlayerNum; i++) { ADD = std::to_string(GetAverageVector(Data_Info.Players[i].Houses));				 padTo(ADD, 5); Output.append("["+ADD+"] "); }
	std::cout << std::setw(28) << "[Houses] " << Output << std::endl; Output = "";
	for (int i = 0; i < PlayerNum; i++) { ADD = std::to_string(GetAverageVector(Data_Info.Players[i].Hotels));				 padTo(ADD, 5); Output.append("["+ADD+"] "); }
	std::cout << std::setw(28) << "[Hotels] " << Output << std::endl; Output = "";
	for (int i = 0; i < PlayerNum; i++) { ADD = std::to_string(GetAverageVector(Data_Info.Players[i].AssetValue));			 padTo(ADD, 5); Output.append("["+ADD+"] "); }
	std::cout << std::setw(28) << "[Assets] " << Output << std::endl; Output = "";
	for (int i = 0; i < PlayerNum; i++) { ADD = std::to_string(GetAverageVector(Data_Info.Players[i].LocationsLandedOn));	 padTo(ADD, 5); Output.append("["+ADD+"] "); }
	std::cout << std::setw(28) << "[Locations Landed On] " << Output << std::endl; Output = "";
	for (int i = 0; i < PlayerNum; i++) { ADD = std::to_string(GetAverageVector(Data_Info.Players[i].TimesAroundBoard));	 padTo(ADD, 5); Output.append("["+ADD+"] "); }
	std::cout << std::setw(28) << "[Times Around Board] " << Output << std::endl; Output = "";
	for (int i = 0; i < PlayerNum; i++) { ADD = std::to_string(GetAverageVector(Data_Info.Players[i].DiedAt));				 padTo(ADD, 5); Output.append("["+ADD+"] "); }
	std::cout << std::setw(28) << "[Died At Roll #] " << Output << std::endl; Output = "";
	std::cout << std::endl;
	//Place
	for (int j = PlayerNum; j > 0; j--) {
		for (int i = 0; i < PlayerNum; i++) { ADD = std::to_string(GetAmountOfNum(Data_Info.Players[i].Place, j));			 padTo(ADD, 5); Output.append("["+ADD+"] "); }
		switch (j) {
			case 1:
				std::cout << std::setw(28) << "[Times in 1st] " << Output << std::endl; Output = "";
				break;
			case 2:
				std::cout << std::setw(28) << "[Times in 2nd] " << Output << std::endl; Output = "";
				break;
			case 3:
				std::cout << std::setw(28) << "[Times in 3rd] " << Output << std::endl; Output = "";
				break;
			default: 
				std::cout << std::setw(28) << "[Times in " << std::to_string(j) << "th] " << Output << std::endl; Output = "";
				break;
		}
	}


	//Data

	//Title
	for (unsigned int i = 0; i < PlayerNum; i++) {
		std::cout << std::setw(22) << "Name" << "[" << std::setw(3) << ":" << std::setw(6) << "Landed" << ":" << std::setw(7) << ":" << std::setw(6) << "Owned" << ":" << std::setw(8) << "] "; std::cout << "| ";
	}
	std::cout << std::endl;
	
	std::vector<float> Landed = { 0,0,0,0 };
	std::vector<float> Owned = { 0,0,0,0 };
	for (unsigned int i = 0; i < 41; i++) {
		for (int p = 0; p < PlayerNum; p++) {
			Landed[p] += Data_Info.Players[p].Squares[i];
			Owned[p] += Data_Info.Players[p].Land[i];
		}
	}
	//Display Stats
	for (unsigned int i = 0; i < 41; i++) {
		for (int p = 0; p < PlayerNum; p++) {
			ChangeColour(i);
			std::cout << std::fixed << std::setw(22) << StreetNames[i] << "[" << std::setw(2) << i << ":";
			std::cout << std::fixed << std::setw(6) << Data_Info.Players[p].Squares[i] << ":" << std::setw(6) << std::setprecision(4) << (Data_Info.Players[p].Squares[i] / Landed[p]) << ":";
			if (Owned[p] == 0) { std::cout << std::fixed << std::setw(6) << Data_Info.Players[p].Land[i] << ":" << std::setw(6) << std::setprecision(4) << (0) << "] "; }
			else { std::cout << std::fixed << std::setw(6) << Data_Info.Players[p].Land[i] << ":" << std::setw(6) << std::setprecision(4) << (Data_Info.Players[p].Land[i] / Owned[p]) << "] "; }
			ChangeColour(0);
			std::cout << "| ";
			if (p == PlayerNum - 1) { std::cout << std::endl; }
		}
	}
	

}
void DisplayStats(PlayerInfo(&players)[PlayerNum], int y) {
	std::cout << std::endl;
	y += 55;

	//Title
	for (unsigned int i = 0; i < PlayerNum; i++) {
		std::cout << std::setw(22) << "Name" << "[" << std::setw(2) << " #] " << std::setw(9) << "[   Landed] " << std::setw(7) << "[Owned] " << "| ";
	}
	std::cout << std::endl;
	//Display Stats
	for (unsigned int i = 0; i < 41; i++) {
		for (int p = 0; p < PlayerNum; p++) {
			ChangeColour(i);
			std::cout << std::fixed << std::setw(22) << StreetNames[i] << "[" << std::setw(2) << i << "] ";
			std::cout << std::fixed << "[" << std::setw(2) << players[p].Squares[i] << ":" << std::setw(6) << std::setprecision(4) << (players[p].Squares[i] / (float)RollsPerGame) << "] ";
			std::cout << std::fixed << "[" << std::setw(2) << players[p].Land[i] << ":" << std::setw(2) << players[p].LandMortgaged[i] << "] ";
			ChangeColour(0);
			std::cout << "| ";
			if (p == PlayerNum - 1) { std::cout << std::endl; }
		}
	}
	for (int p = 0; p < PlayerNum; p++) { std::cout << std::setw(28) << "Locations Landed" << "[" << std::setw(6) << players[p].TotalLocationsLandedOn << "] "; }
	std::cout << std::endl;
	for (int p = 0; p < PlayerNum; p++) { std::cout << std::setw(28) << "Times Around Board" << "[" << std::setw(6) << players[p].TimesAroundBoard << "] "; }
	std::cout << std::endl;
	for (int p = 0; p < PlayerNum; p++) { std::cout << std::setw(28) << "Amount Of Houses " << "[" << std::setw(6) << players[p].TotalAmountOfHouses << "] "; }
	std::cout << std::endl;
	for (int p = 0; p < PlayerNum; p++) { std::cout << std::setw(28) << "Amount Of Hotels " << "[" << std::setw(6) << players[p].TotalAmountOfHotels << "] "; }
	std::cout << std::endl;
	for (int p = 0; p < PlayerNum; p++) { std::cout << std::setw(28) << "Money " << "[" << std::setw(6) << players[p].Money << "] "; }
	std::cout << std::endl;
	for (int p = 0; p < PlayerNum; p++) { std::cout << std::setw(28) << "Total Asset Value " << "[" << std::setw(6) << players[p].TotalAssetValue << "] "; }
	std::cout << std::endl;
	for (int p = 0; p < PlayerNum; p++) { std::cout << std::setw(28) << "Died at roll " << "[" << std::setw(6) << players[p].DiedAt << "] "; }
	std::cout << std::endl;
	std::cout << std::setw(28) << "Number of Rolls in Game " << "[" << std::setw(6) << Data_Info.Rolls[Data_Info.Rolls.size()-1] << "] ";
	std::cout << std::endl;

	for (int i = 0; i < 10; i++) { std::cout << PiT_Max[i] << ","; } std::cout << std::endl;
	for (int p = 0; p < PlayerNum; p++) {
		for (int i = 0; i < 10; i++) { 
			if (players[p].PiT[i] == PiT_Max[i]) { SetConsoleTextAttribute(hConsole, 160); }//Light Green
			else if (players[p].PiT[i] == 0) { SetConsoleTextAttribute(hConsole, 192); }//Light Red
			else if (players[p].PiT[i] != 0) { SetConsoleTextAttribute(hConsole, 224); }//Light Yellow
			std::cout << players[p].PiT[i];
			SetConsoleTextAttribute(hConsole, 7);
			std::cout << ",";
		}
		std::cout << std::endl;
	}
}
void ColourString(std::string& Output) {
	while (Output.length() > 4) {
		std::size_t f0;
		std::size_t f1 = Output.find("[");
		std::size_t f2 = Output.find("]");
		std::size_t f3 = Output.find(":");
		std::size_t f4 = Output.find("$");
		if (f2 > f3) { f0 = f3; }
		else if (f2 > f4) { f0 = f4; }
		else { f0 = f2; }
		char Buffer[48];

		if (((f0 - f1) + 1) < sizeof(Buffer) && f1 != std::string::npos) {
			std::size_t length = Output.copy(Buffer, (f0 - f1) + 1, f1);
			Buffer[length] = '\0';
			if (length != std::string::npos) {
				std::string SBuffer = Buffer;
				if (SBuffer == "[P:") {
					char Buffer2[4];
					std::size_t length2 = Output.copy(Buffer2, 2, f0 + 1);
					Buffer2[length2] = '\0';
					if (length != std::string::npos) {
						int Number = std::stoi((std::string)Buffer2);
						if (Number < 99 && Number >= 0) { ChangeColour(Number); }
					}
				}
				else if (SBuffer == "[$")				 { SetConsoleTextAttribute(hConsole, 10); }
				else if (SBuffer == "[-$")				 { SetConsoleTextAttribute(hConsole, 12); }
				else if (SBuffer == "[Passed GO]")		 { SetConsoleTextAttribute(hConsole,  2); }
				else if (SBuffer == "[Move to]")		 { SetConsoleTextAttribute(hConsole,  2); }
				else if (SBuffer == "[Free Pass]")		 { SetConsoleTextAttribute(hConsole,  2); }
				else if (SBuffer == "[Free Jail Pass]")	 { SetConsoleTextAttribute(hConsole,  2); }
				else if (SBuffer == "[C:")				 { SetConsoleTextAttribute(hConsole, 10); }
				else if (SBuffer == "[M:")				 { SetConsoleTextAttribute(hConsole,  4); }
				else if (SBuffer == "[UM:")				 { SetConsoleTextAttribute(hConsole,  2); }
				else if (SBuffer == "[SH:")				 { SetConsoleTextAttribute(hConsole,  4); }
				else if (SBuffer == "[Trade:")			 { SetConsoleTextAttribute(hConsole,112); }
				else if (SBuffer == "[ERROR:")			 { SetConsoleTextAttribute(hConsole,206); }//224
				else if (SBuffer == "[R:")				 { SetConsoleTextAttribute(hConsole,  7); }

				//Doubles
				else if (SBuffer == "[1|1]")			 { SetConsoleTextAttribute(hConsole, 12); }
				else if (SBuffer == "[2|2]")			 { SetConsoleTextAttribute(hConsole, 12); }
				else if (SBuffer == "[3|3]")			 { SetConsoleTextAttribute(hConsole, 12); }
				else if (SBuffer == "[4|4]")			 { SetConsoleTextAttribute(hConsole, 12); }
				else if (SBuffer == "[5|5]")			 { SetConsoleTextAttribute(hConsole, 12); }
				else if (SBuffer == "[6|6]")			 { SetConsoleTextAttribute(hConsole, 12); }

				for (int i = 0; i < 41; i++) {
					std::string ADD = StreetNames[i];
					if (SBuffer == "[" + ADD + "]") { ChangeColour(i); }
					padTo(ADD, 24);
					if (SBuffer == "[" + ADD + "]") { ChangeColour(i); }
				}
			}

			//Print out whatever is in the brackets[]
			length = Output.copy(Buffer, (f2-f1)+1, f1);
			Buffer[length] = '\0';
			if (length != std::string::npos) {
				std::cout << (std::string)Buffer;
				Output.replace(f1, f2+1, "");
			}
			//Change colour back
			ChangeColour(0);
		}
		else { Output.replace(f1, f2 + 1, ""); }
	}
	ChangeColour(0);
	std::cout << "|";
	Output = "";
}
void SetPlayerStats(PlayerInfo(&players)[PlayerNum]) {
	//Reset Values
	for (int p = 0; p < PlayerNum; p++) {
		players[p].AI = true;
		//Temporary per-game
		players[p].OldPosition = 0;	 //Old Player position
		players[p].position = 0;	 //Player position
		players[p].Doubles = 0;		 //Amount of doubles in a row
		players[p].InJail = 0;		 //Amount of turns the player is in jail for (3, 2, 1, 0)
		players[p].JailPass = 0;	 //Amount of (get out of jail free) passes
		players[p].Money = 1500;
		//Data 
		//Land the player owns and the stage it is at
		players[p].Land = {
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0 };
		players[p].LandMortgaged = {
			false,false,false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,false,false,
			false,false,false,false,false,false,false,false,false,false,
			false };
		for (int i = 0; i < 10; i++) { players[p].Townships[i] = 0; }
		for (int i = 0; i < 10; i++) { players[p].PiT[i] = 0; }
		players[p].TotalAmountOfHouses = 0;
		players[p].TotalAmountOfHotels = 0;
		players[p].TotalAssetValue = 0;
		//Count how many times the player landed on each square
		players[p].Squares = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0 };
		players[p].TotalLocationsLandedOn = 0;
		players[p].TimesAroundBoard = 0;

		players[p].isDead = false;
		players[p].DiedAt = 0;

		players[p].Land_Made = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0 };
		for (int i = 0; i < 10; i++) { players[p].Made_iT[i] = 0; }
	}

	for (int i = 0; i < 40; i++) { Data_Info.LandOwnerShip[i] = -1; }
}

#pragma endregion

#pragma region other

void CalculateCard(PlayerInfo(&players)[PlayerNum], int p, std::string card, int& AIP, std::string& Output) {

	std::string ADD;
	if (card == "-1") {}
	//Starts with a Letter
	else if ((card[0] >= 'A' && card[0] <= 'Z') || (card[0] >= 'a' && card[0] <= 'z')) {
		//Go Back 3 Spaces
		if (card == "B") {
			players[p].Squares[players[p].position] += 1;//Add a point to the Location you landed on
			players[p].TotalLocationsLandedOn += 1;
			players[p].OldPosition = players[p].OldPosition - 3;
			players[p].position = players[p].position - 3;
			ADD = std::to_string(players[p].position); padTo(ADD, 2); Output.append("[Go Back 3] [P:" + ADD + "] ");
			Output.append("[" + StreetNames[players[p].position] + "] ");
		}
		//Get out of Jail Free card
		else if (card == "F") {
			players[p].JailPass += 1;
			Output.append("[Free Jail Pass] ");
		}
		//Go to Jail
		else if (card == "J") {
			players[p].Squares[players[p].position] += 1;//Add a point to the Location you landed on
			players[p].TotalLocationsLandedOn += 1;
			players[p].position = 40; //Jail
			Output.append("[Go to Jail] ");
			if (players[p].JailPass == 0) { players[p].InJail = 3; }
			else {
				players[p].position = 10; //Just Visiting Jail
				players[p].JailPass -= 1;
				Output.append("[Free Pass] ");
			}
			players[p].Doubles = 0; //Reset doubles
			ADD = std::to_string(players[p].position); padTo(ADD, 2); Output.append("[P:" + ADD + "] ");
		}
		//Advance token to the nearest Railroad
		else if (card == "R") {
			players[p].Squares[players[p].position] += 1;//Add a point to the Location you landed on
			players[p].TotalLocationsLandedOn += 1;
			while (players[p].position != 5 && players[p].position != 15 && players[p].position != 25 && players[p].position != 35) { players[p].position = (players[p].position + 1) % 40; }
			ADD = std::to_string(players[p].position); padTo(ADD, 2); Output.append("[->Railroad] [P:" + ADD + "] ");
			Output.append("[" + StreetNames[players[p].position] + "] ");
		}
		//Advance token to nearest Utility
		else if (card == "U") {
			players[p].Squares[players[p].position] += 1;//Add a point to the Location you landed on
			players[p].TotalLocationsLandedOn += 1;
			while (players[p].position != 12 && players[p].position != 28) { players[p].position = (players[p].position + 1) % 40; }
			ADD = std::to_string(players[p].position); padTo(ADD, 2); Output.append("[->Utility] [P:" + ADD + "] ");
			Output.append("[" + StreetNames[players[p].position] + "] ");
		}

		//Player Collects Money from other players
		else if (card[0] == 'c') {
			card.erase(0, 1);
			Output.append("[Collect $" + card + " from each player] ");
			for (int j = 0; j < PlayerNum; j++) {
				if (j != p && !players[j].isDead) { players[j].Money -= std::stoi(card); players[p].Money += std::stoi(card); }
			}
			Output.append("[$" + std::to_string(players[p].Money) + "] ");
		}
		//Player Pays Money to other players
		else if (card[0] == 'p') {
			card.erase(0, 1);
			Output.append("[Pay each player  $" + card + "] ");
			for (int j = 0; j < PlayerNum; j++) {
				if (j != p && !players[j].isDead) { players[j].Money += std::stoi(card); players[p].Money -= std::stoi(card); }
			}
			Output.append("[$" + std::to_string(players[p].Money) + "] ");
		}

		//Make general repairs on all your property � For each house pay $25 � For each hotel $100 
		else if (card == "hr") {
			int Pay = (players[p].TotalAmountOfHouses * 25) + (players[p].TotalAmountOfHotels * 100);
			Output.append("[Pay for house repairs ");
			ADD = "$" + std::to_string(Pay); padTo(ADD, 6); Output.append(ADD + "] ");
			players[p].Money -= Pay;
			Output.append("[$" + std::to_string(players[p].Money) + "] ");
		}
		//You are assessed for street repairs � $40 per house � $115 per hotel
		else if (card == "sr") {
			int Pay = (players[p].TotalAmountOfHouses * 40) + (players[p].TotalAmountOfHotels * 115);
			Output.append("[Pay for street repairs ");
			ADD = "$" + std::to_string(Pay); padTo(ADD, 6); Output.append(ADD + "] ");
			players[p].Money -= Pay;
			Output.append("[$" + std::to_string(players[p].Money) + "] ");
		}
		//Letter ERROR
		else { Output.append("[ERROR: Card-ABC] "); }
	}
	//Player Loses Money
	else if (card[0] == '-') {
		card.erase(0, 1);
		players[p].Money -= std::stoi(card);
		AIP += std::stoi(card);
		Output.append("[-$" + card + "] ");
		Output.append("[$" + std::to_string(players[p].Money) + "] ");
	}
	//Player Gains Money
	else if (card[0] == '$') {
		card.erase(0, 1);
		players[p].Money += std::stoi(card);
		Output.append("[+$" + card + "] ");
		Output.append("[$" + std::to_string(players[p].Money) + "] ");
	}
	//Go to card Location OR ERROR
	else {
		int CardValue = -1;
		//Make sure it's a number
		if (card[0] >= '0' && card[0] <= '9') {
			CardValue = std::stoi(card);
			//Go to card Location
			if (CardValue >= 0 && CardValue <= 40) {
				players[p].Squares[players[p].position] += 1;//Add a point to the Location you landed on
				players[p].TotalLocationsLandedOn += 1;
				players[p].position = CardValue;
				ADD = std::to_string(players[p].position); padTo(ADD, 2); Output.append("[Move to] [P:" + ADD + "] ");
				Output.append("[" + StreetNames[players[p].position] + "] ");
			}
			//Number ERROR
			else { Output.append("[ERROR: Card-#] "); }
		}
		//ERROR
		else { Output.append("[ERROR: Card] "); }
	}


	/*
	//You have been elected Chairman of the Board � Pay each player $50
	else if (card == "p50") {
		Output.append("[Pay each player $50] ");
		//players[p].Money -= (50*PlayerNum);
		for (int j = 0; j < PlayerNum; j++) {
			if (j != p && !players[j].isDead) { players[j].Money += 50; players[p].Money -= 50; }
		}
		Output.append("[$"+std::to_string(players[p].Money)+"] ");
	}
	//It is your birthday - Collect $10 from each player //A player could end up with a negative value and have to mortgage on there turn.
	else if (card == "c10") {
		Output.append("[Collect $10 from each player] ");
		//players[p].Money += (10*PlayerNum);
		for (int j = 0; j < PlayerNum; j++) {
			if (j != p && !players[j].isDead) { players[j].Money -= 10; players[p].Money += 10; }
		}
		Output.append("[$" + std::to_string(players[p].Money) + "] ");
	}
	//Grand Opera Night {Opening} � Collect $50 from every player for opening night seats //A player could end up with a negative value and have to mortgage on there turn.
	else if (card == "c50") {
		Output.append("[Collect $50 from each player] ");
		//players[p].Money += (50*PlayerNum);
		for (int j = 0; j < PlayerNum; j++) {
			if (j != p && !players[j].isDead) { players[j].Money -= 50; players[p].Money += 50; }
		}
		Output.append("[$" + std::to_string(players[p].Money) + "] ");
	}

	//Player Loses Money
	//Pay poor tax of $15
	else if (card == "-15") { Output.append("[-$15] "); players[p].Money -= 15; AmountInParking += 15; }
	//Pay fees of $20 //Not in use
	else if (card == "-20") { Output.append("[-$20] "); players[p].Money -= 20; AmountInParking += 20; }
	//Pay fees of $25 //Not in use
	else if (card == "-25") { Output.append("[-$25] "); players[p].Money -= 25; AmountInParking += 25; }
	//Doctor"s fees {fee} � Pay $50
	else if (card == "-50") { Output.append("[-$50] "); players[p].Money -= 50; AmountInParking += 50; }
	//Pay hospital fees of $100
	else if (card == "-100") { Output.append("[-$100] "); players[p].Money -= 100; AmountInParking += 100; }
	//Pay school fees of $150
	else if (card == "-150") { Output.append("[-$150] "); players[p].Money -= 150; AmountInParking += 150; }
	//Pay fees of $200 //Not in use
	else if (card == "-200") { Output.append("[-$200] "); players[p].Money -= 200; AmountInParking += 200; }

	//Player Gains Money
	//You have won second prize in a beauty contest � Collect $10
	else if (card == "$10") { Output.append("[$10] "); players[p].Money += 10; }
	//Income tax refund � Collect $20
	else if (card == "$20") { Output.append("[$20] "); players[p].Money += 20; }
	//Receive $25 consultancy fee
	else if (card == "$25") { Output.append("[$25] "); players[p].Money += 25; }
	//Bank pays you dividend of $50  //From sale of stock you get $50
	else if (card == "$50") { Output.append("[$50] "); players[p].Money += 50; }
	//You have won a crossword competition - Collect $100 //Holiday {Xmas} Fund matures - Collect $100 //Life insurance matures � Collect $100 //You inherit $100
	else if (card == "$100") { Output.append("[$100] "); players[p].Money += 100; }
	//Your building {and} loan matures � Collect $150
	else if (card == "$150") { Output.append("[$150] "); players[p].Money += 150; }
	//Bank error in your favor - Collect $200
	else if (card == "$200") { Output.append("[$200] "); players[p].Money += 200; }

	//Error happened
	else { Output.append("[...] "); }
	*/
}

int IsDead(PlayerInfo(&players)[PlayerNum], int p, std::string& Output) {
	std::string ADD;
	if (players[p].Money <= 0) {
		MortgageProperty(players, p, Output);
		//FindBuyer(Data_Info, players, p, Output);
		for (int i = 0; i < 41; i++) {
			if (players[p].Land[i] > 0) { Data_Info.LandOwnerShip[i] = -1; }
		}
		ADD = std::to_string(players[p].Money); padTo(ADD, 2); Output.append("[$" + ADD + "] ");
		if (players[p].Money <= 0) { return 2; }
		else { return 1; }
	}
	else { return 0; }
}



//bool Monopoly(PlayerInfo players[]) {
//	/*
//		If the Player has to pay another Player money while not on their turn,
//		and falls bellow $0 they will not die until their turn
//		to give them a chance to save themselves.
//	*/
//
//	//Game
//	while (rolls < (RollsPerGame*PlayerNum)) {
//		if (1 >= (PlayerNum - AmountDead)) { break; }
//
//		//Display the roll number.
//		if (Display) {
//			COORD xy = { (0 * x), y };
//			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
//			ADD = (std::to_string((int)ceil(rolls*0.25f))); padTo(ADD, 3); Output.append("[R:" + ADD + "] ");
//		}
//	}
//	return true;
//}

class MonopolyGame
{
public:
	MonopolyGame();
	~MonopolyGame();
	PlayerInfo players[PlayerNum];

	void StartGame();//PlayerInfo(&players)[PlayerNum]
	void EndGame();
	void Move();
	int RollDice();

	int AmountInParking;
	int AmountDead;
	int rolls;
	std::string Output;
	std::string ADD;
	int CurrentPlayer;
private:
	std::vector<std::string> chest;
	std::vector<std::string> chance;
	std::default_random_engine generator;
};

MonopolyGame::MonopolyGame() {
	CurrentPlayer = 0;
	AmountInParking = 0;
	AmountDead = 0;
	rolls = 0;
	Output = "";
	ADD = "";
	for (int p = 0; p < PlayerNum; p++) { players[p] = PlayerInfo(); }
}
MonopolyGame::~MonopolyGame() {
}
int MonopolyGame::RollDice() {
	std::uniform_int_distribution<int> distribution(0, 35);
	return distribution(generator);
}
void MonopolyGame::StartGame() {

	CurrentPlayer = 0;
	//Reset and shuffle the decks
	chest = master_chest; //Community Chest
	std::random_shuffle(chest.begin(), chest.end()); //Shuffle the deck
	chance = master_chance; //Chance
	std::random_shuffle(chance.begin(), chance.end()); //Shuffle the deck
	AmountInParking = 0;
	AmountDead = 0;
	rolls = 0;

	//Blank out data
	for (int p = 0; p < PlayerNum; p++){ players[p] = PlayerInfo(); }
	
	SetPlayerStats(players);

	Output = "";
}
void MonopolyGame::EndGame() {


	CalculateAmountOfHouses(players);
	CalculateTotalAssetValue(players);

	Data_Info.Rolls.push_back((rolls / PlayerNum));

	//Last player Alive
	for (int p = 0; p < PlayerNum; p++) {
		if (!players[p].isDead) {
			players[p].DiedAt = (int)(rolls / PlayerNum);
			Data_Info.Players[p].Place.push_back(1);

			if (DisplaySpecificRounds) {
				//Show Certain Rounds. Specific 
				if (players[p].DiedAt == SpecificRound) {
					int i = 0;
					while (i < (players[p].DiedAt*PlayerNum)) {
						for (int pp = 0; pp < PlayerNum; pp++) {
							COORD xy = { (pp * x), y };
							SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
							if (pp == PlayerNum - 1) { y += 1; }
							ColourString(StuffAndThings[i]);
							i++;
						}
					}
					DisplayStats(players, y);
				}
				//StuffAndThings.clear();
			}


			//Feed the neural network with the winning player scores. 
			//This will be sent twice.
			feedForwardAI(players, p);
			backPropagationAI(players, p, true);
		}
	}
	StuffAndThings.clear();

	//Get the last player to die.
	int highest = 0;
	for (int i = 0; i < PlayerNum; i++) {
		if (players[i].DiedAt > highest) { highest = players[i].DiedAt; }
	}

	for (int i = 0; i < PlayerNum; i++) {
		if (players[i].Money < 0) { Data_Info.Players[i].Money.push_back(0); }
		else { Data_Info.Players[i].Money.push_back(players[i].Money); }
		Data_Info.Players[i].Houses.push_back(players[i].TotalAmountOfHouses);
		Data_Info.Players[i].Hotels.push_back(players[i].TotalAmountOfHotels);
		Data_Info.Players[i].AssetValue.push_back(players[i].TotalAssetValue);
		Data_Info.Players[i].LocationsLandedOn.push_back(players[i].TotalLocationsLandedOn);
		Data_Info.Players[i].TimesAroundBoard.push_back(players[i].TimesAroundBoard);
		Data_Info.Players[i].DiedAt.push_back(players[i].DiedAt);

		for (unsigned int j = 0; j < 41; j++) {
			Data_Info.Players[i].Squares[j] += players[i].Squares[j];
			Data_Info.Players[i].Land[j] += players[i].Land[j];

			//Adds to the total the land has made.
			Data_Info.Players[i].Land_Made[j] += players[i].Land_Made[j];
		}
		for (unsigned int j = 0; j < 10; j++) {
			//Adds to the total the township has made.
			Data_Info.Players[i].Made_iT[j] += players[i].Made_iT[j];
		}


		//Feed the neural network with all the players scores.
		//If the player was in the top 2, expect best target values.
		//If in the bottom 2, expect the worst target values.
		feedForwardAI(players, i);
		//The 2 last players did good.
		if (players[i].DiedAt == 0 || players[i].DiedAt == highest) { backPropagationAI(players, i, true); }
		else { backPropagationAI(players, i, false); }

	}
}
void MonopolyGame::Move() {

	Output = "";

	int p = CurrentPlayer;
	if (!players[p].isDead) {

		if (Display) {
			COORD xy = { (p * x), y };
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
			if (p == PlayerNum - 1) { y += 1; }
			else if (players[PlayerNum - 1].isDead) {
				if (p == PlayerNum - 2) { y += 1; }
				else if (players[PlayerNum - 2].isDead) {
					if (p == PlayerNum - 3) { y += 1; }
				}
			}
		}

		ADD = std::to_string(players[p].Money); padTo(ADD, 5); Output.append("[$" + ADD + "] ");


		players[p].OldPosition = players[p].position;
		//Get a random dice roll
		int DiceRoll = RollDice();

		ADD = std::to_string(players[p].position); padTo(ADD, 2); Output.append("[P:" + ADD + "] ");
		Output.append("[" + std::to_string((int)std::floor(DiceRoll / 6) + 1) + "|" + std::to_string(rollvalues[DiceRoll] - ((int)std::floor(DiceRoll / 6) + 1)) + "] ");
		ADD = std::to_string(rollvalues[DiceRoll]); padTo(ADD, 2); Output.append("[" + ADD + "] ");


		bool IsDouble = false;
		//Checks for a double
		for (int i = 0; i < 6; i++) { if (DiceRoll == DoubleIndex[i]) { IsDouble = true; } }

		//Rolls a double to get out of Jail
		if (IsDouble && players[p].InJail > 0) {
			players[p].InJail = 0;
			players[p].position = 10;
			ADD = std::to_string(players[p].position); padTo(ADD, 2); Output.append("[P:" + ADD + "] ");
			ADD = "Out of Jail"; padTo(ADD, 24); Output.append("[" + ADD + "] ");
		}
		//Add one to the Doubles Counter
		else if (IsDouble) { players[p].Doubles += 1; }
		//Reset doubles
		else { players[p].Doubles = 0; }

		//If in jail do nothing
		if (players[p].InJail > 1) {
			players[p].InJail -= 1;
			ADD = std::to_string(players[p].position); padTo(ADD, 2); Output.append("[P:" + ADD + "] ");
			ADD = StreetNames[players[p].position]; padTo(ADD, 24); Output.append("[" + ADD + "] ");
			players[p].Squares[players[p].position] += 1;
			players[p].TotalLocationsLandedOn += 1;
			//Need something for paying to get out of jail
			//Check if player wants to leave by paying JailFee
			//Check if AI wants to leave by paying JailFee
		}
		//If the amount of doubles in a row is equal to or greater then 3, go to jail
		else if (players[p].Doubles >= 3) {
			players[p].position = 40; //Jail
			if (players[p].JailPass == 0) {
				players[p].InJail = 3;
				ADD = std::to_string(players[p].position); padTo(ADD, 2); Output.append("[P:" + ADD + "] ");
				ADD = "3 Doubles: Go to Jail"; padTo(ADD, 24); Output.append("[" + ADD + "] ");
				players[p].Squares[players[p].position] -= 1;
				players[p].TotalLocationsLandedOn -= 1;
			}
			else {
				players[p].position = 10; //Just Visiting Jail
				players[p].JailPass -= 1;
				ADD = std::to_string(players[p].position); padTo(ADD, 2); Output.append("[P:" + ADD + "] ");
				ADD = "3 Doubles: Go to Jail"; padTo(ADD, 24); Output.append("[" + ADD + "] ");
				Output.append("[Free Pass] ");
				players[p].Squares[players[p].position] -= 1;
				players[p].TotalLocationsLandedOn -= 1;
			}
			players[p].Doubles = 0; //Reset doubles
		}
		//Continue
		else {

			//If it is the players last day in jail
			if (players[p].InJail == 1) {
				players[p].InJail -= 1;
				//No longer in Jail so set position to 10
				players[p].position = 10;
				players[p].Money -= JailFee;

				ADD = "Free"; Output.append("[" + ADD + "] ");
			}
			//Roll the dice
			players[p].position = ((players[p].position + rollvalues[DiceRoll]) % 40);
			ADD = std::to_string(players[p].position); padTo(ADD, 2); Output.append("[P:" + ADD + "] ");
			ADD = StreetNames[players[p].position]; padTo(ADD, 24); Output.append("[" + ADD + "] ");

			//Calculate Rent payment
			CalculateRentPayment(Data_Info, players, p, rollvalues[DiceRoll], Output);
			//Calculate if the player should buy the land
			CalculateLandPayment(Data_Info, players, p, Output);

			int OldRentPosition = players[p].position;

			std::string card = "-1";
			//Chance
			if (players[p].position == 7 || players[p].position == 22 || players[p].position == 36) {
				card = chance[0]; //Get the value of the card on the top of the deck
				chance.erase(chance.begin()); //Remove the card from the deck
											  //If there is not more cards, reset the deck and shuffle
				if (chance.size() == 0) {
					chance = master_chance; //Reset the deck
					std::random_shuffle(chance.begin(), chance.end()); //Shuffle the deck
				}
			}
			//Community Chest
			if (players[p].position == 2 || players[p].position == 17 || players[p].position == 33) {
				card = chest[0]; //Get the value of the card on the top of the deck
				chest.erase(chest.begin()); //Remove the card from the deck
				if (chest.size() == 0) { //If there is not more cards, reset the deck and shuffle
					chest = master_chest; //Reset the deck
					std::random_shuffle(chest.begin(), chest.end()); //Shuffle the deck
				}
			}
			CalculateCard(players, p, card, AmountInParking, Output);

			//Go to jail
			if (players[p].position == 30) {
				players[p].position = 40; //Jail
				if (players[p].JailPass == 0) { players[p].InJail = 3; }
				else {
					players[p].position = 10; //Just Visiting Jail
					players[p].JailPass -= 1;
					Output.append("[Free Pass] ");
				}
				players[p].Doubles = 0; //Reset doubles
				//Add a point to the (Go to Jail) Location you landed on
				players[p].Squares[30] += 1;
				players[p].TotalLocationsLandedOn += 1;
			}
			//Income Tax
			else if (players[p].position == 4) {
				if (200 < (int)(players[p].TotalAssetValue*0.1)) {
					players[p].Money -= 200;
					Output.append("[-$200] ");
					AmountInParking += 200;
				}
				else {
					int temp = (int)(players[p].TotalAssetValue*0.1);
					players[p].Money -= temp;
					Output.append("[-$" + std::to_string(temp) + "] ");
					AmountInParking += temp;
				}
			}
			//Luxury Tax
			else if (players[p].position == 38) {
				players[p].Money -= 75;
				Output.append("[-$75] ");
				AmountInParking += 75;
			}
			//Free Parking
			else if (FreeParkingRule && players[p].position == 20) {
				players[p].Money += AmountInParking;
				Output.append("[$" + std::to_string(AmountInParking) + "] ");
				AmountInParking = 0;
			}
			//Recalculate Rent payment or If the player should buy the land
			if (OldRentPosition != players[p].position && players[p].position != 40) {
				CalculateRentPayment(Data_Info, players, p, rollvalues[DiceRoll], Output);
				CalculateLandPayment(Data_Info, players, p, Output);
			}

		}

		//Check if Player passed Go
		if (players[p].position < players[p].OldPosition && players[p].position != 10 && players[p].position != 40 && players[p].OldPosition != 10 && players[p].OldPosition != 40) {
			players[p].Money += 200;
			players[p].TimesAroundBoard += 1;
			Output.append("[Passed GO] ");
		}

		//End Position (not including jail)
		if (players[p].position >= 0 && players[p].position < 40) {
			players[p].Squares[players[p].position] += 1;
			players[p].TotalLocationsLandedOn += 1;
		}

		//Player is Dead
		int PlayerState = IsDead(players, p, Output);
		if (PlayerState == 2) {
			//Lost
			Data_Info.Players[p].Place.push_back(PlayerNum - AmountDead);
			players[p].isDead = true;
			players[p].DiedAt = (rolls / PlayerNum);
			AmountDead += 1;
			for (int l = 0; l < 40; l++) {
				if (Data_Info.LandOwnerShip[l] == p) { Data_Info.LandOwnerShip[l] = -1; }
			}

			Output.append("[Died at roll: " + std::to_string((rolls / PlayerNum)) + "]");
		}
		else if (PlayerState == 1) {
			//Sold stuff to stay alive, so do nothing
		}
		else if (PlayerState == 0) {
			if (TradePastRound < (rolls / PlayerNum)) { Trade(Data_Info, players, p, Output); }
			if (UpgradePastRound < (rolls / PlayerNum)) { UpgradeTownship(players, p, Output); }
			if (UnMortgagePastRound < (rolls / PlayerNum)) { UnMortgageProperty(players, p, Output); }
		}


		//What will this do?
		feedForwardAI(players, p);
		ResultValues;
		//if (Display) { ColourString(Output); }
	}
	
	if (Display) { ColourString(Output); }
	if (DisplaySpecificRounds) { StuffAndThings.push_back(Output); }
	rolls += 1;

	//Move to the next player
	CurrentPlayer++;
	if (CurrentPlayer % 4 == 0) { CurrentPlayer = 0; }
}


#pragma endregion


