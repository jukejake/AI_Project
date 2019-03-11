#pragma once
#include <vector>
#include <array>
#include <tuple>
#include <algorithm>
#include "TimeFunctions.h"

struct Board {
	Board() { }
	//Assign board.
	Board(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8, int v9) {
		state[0] = v1; state[1] = v2; state[2] = v3; state[3] = v4; state[4] = v5; state[5] = v6; state[6] = v7; state[7] = v8; state[8] = v9;
	}
	//Assign board.
	Board(int value[9]) {
		for (unsigned short int i = 0; i < 9; i++) {
			state[i] = value[i];
		}
	}
	//State of the board.
	std::array<int, 9> state { 0,0,0, 0,0,0, 0,0,0 };
	//All valid boards this can become.
	std::vector<int> childs;
	//Where the move was placed on the board.
	int Placement;
	//On what move was this board made (i.e. between 0 and 9)
	int Depth;
	//Who won, 1 == X, 2 == O
	int Won; 
	//What the board is worth i.e. 
	//if it is a winning board early in the game; have a high value relative to who won: 10 or -10
	//if it is a winning board late in the game; have a low value relative to who won: 1 or -1
	int Worth;
	//How many X's and O's are on the board.
	int Xs; int Os;
};

class TicTacToe {
public:
	TicTacToe();
	~TicTacToe();
	//Current board number.
	int CurrentBoard = 0;
	//Holds all possible boards.
	std::vector<Board> Boards;//0=Empty, 1=Player, 2=Opponent
	//The bounds for the boards based on their moves.
	//i.e. first move only has 3 combinations, second move only have 12 combinations.
	std::array<int, 9> Bounds{ 0,0,0, 0,0,0, 0,0,0 };
	//MiniMax that returns only the worth of the max board.
	int minimaxS(int,int,int, bool);
	//MiniMax algorithm that returns the worth and board number of the max board.
	std::pair<int,int> minimaxD(int,int,int, bool);
	//Function to test if there is a duplicate board in a boards children.
	int TestChildren(std::array<int, 9>, int);
	//Transpose a board.
	std::array<int, 9> Transpose(std::array<int, 9>);
	//Flip a board on it's X-axis.
	std::array<int, 9> FlipX(std::array<int, 9>);
	//Flip a board on it's Y-axis.
	std::array<int, 9> FlipY(std::array<int, 9>);
	//Function for the AI to pick the best move it can make, given the current board.
	std::array<int, 9> ChooseMove(std::array<int, 9>,int,int);

	//Function to generate all valid boards.
	void GenerateAllBoards();

private:
	//Array of all wining positions.
	int win[8][3] = {{0, 1, 2},  // Check First row. 
					 {3, 4, 5},  // Check Second Row 
					 {6, 7, 8},  // Check Third Row 
					 {0, 3, 6},  // Check First column 
					 {1, 4, 7},  // Check Second Column 
					 {2, 5, 8},  // Check Third Column 
					 {0, 4, 8},  // Check First Diagonal 
					 {2, 4, 6}}; // Check Second Diagonal 

	//Check to see is a board has won.
	bool CheckWin(Board&, int);

	//Function to test if there is a duplicate board in bounds.
	int TestBoard(std::array<int, 9>, int, int);
	//Function to make all boards within bounds.
	void MakeBoard(int, int, int, int);
	//Function to make link all bounds together.
	//Adds boards as children to their correct parents.
	void LinkBoard(int, int);
};

TicTacToe::TicTacToe(){}

TicTacToe::~TicTacToe(){}

//Check to see if a board has won.
inline bool TicTacToe::CheckWin(Board& board, int state) {
	//Loop through and array containing all win conditions.
	for (int i = 0; i < 8; i++) {
		if (board.state[win[i][0]] == state &&
			board.state[win[i][1]] == state &&
			board.state[win[i][2]] == state)
		{ return true; }
	}
	//return false if it hasn't won.
	return false;
}

//Position = index of the board in vector Boards.
//Outputs Worth.
inline int TicTacToe::minimaxS(int position, int alpha, int beta, bool maximizingPlayer) {
	//Return the worth of the board if it won or tied.
	if (Boards[position].Depth == 8 || Boards[position].Won) {
		return Boards[position].Worth;
	}
	if (maximizingPlayer) {
		int maxEval = -1000; int worth = 0; //INFINITY
		for each (int child in Boards[position].childs) {
			worth = minimaxS(child, alpha, beta, !maximizingPlayer);
			maxEval = std::max(maxEval, worth);
			alpha = std::max(alpha, worth);
			if (beta <= alpha) { break; }
		}
		return maxEval;
	}
	else {
		int minEval = 1000; int worth = 0;//INFINITY
		for each (int child in Boards[position].childs) {
			worth = minimaxS(child, alpha, beta, !maximizingPlayer);
			minEval = std::min(minEval, worth);
			beta = std::min(beta, worth);
			if (beta <= alpha) { break; }
		}
		return minEval;
	}
}
//Position = index of the board in vector Boards.
//Outputs Worth & Position.
inline std::pair<int, int> TicTacToe::minimaxD(int position, int alpha, int beta, bool maximizingPlayer){
	//Return the worth and board number of the board if it won or tied.
	if (Boards[position].Depth == 8 || Boards[position].Won || Boards[position].childs.size() < 1) {
		return std::make_pair(Boards[position].Worth, position);
	}
	if (maximizingPlayer) {
		int maxEval = -1000, worth = 0; 
		int maxPos = 0, pos = 0;
		for each (int child in Boards[position].childs) {
			std::tie(worth, pos) = minimaxD(child, alpha, beta, false);
			if (maxEval < worth) { maxEval = worth; maxPos = pos; }
			alpha = std::max(alpha, worth);
			if (beta <= alpha) { break; }
		}
		return std::make_pair(maxEval, maxPos);
	}
	else {
		int minEval = 1000, worth = 0; int minPos = 0, pos = 0;
		for each (int child in Boards[position].childs) {
			std::tie(worth, pos) = minimaxD(child, alpha, beta, true);
			if (minEval > worth) { minEval = worth; minPos = pos;}
			alpha = std::min(alpha, worth);
			if (beta <= alpha) { break; }
		}
		return std::make_pair(minEval, minPos);
	}
}

//Function for the AI to pick the best move it can make, given the current board.
inline std::array<int, 9> TicTacToe::ChooseMove(std::array<int, 9> CB, int CBN, int p) {

	//Potential new board numbers.
	int NB1 = CBN, NB2 = CBN;
	//Potential new board values.
	int BV1 = -1000, BV2 = 1000;

	//Loop through all the current boards children to get the best outcome.
	for each (int child in Boards[CBN].childs) {
		//Get board value from minimax.
		int bv = minimaxS(child, -1000, 1000, true);
		//Get highest value
		if (BV1 < bv) { BV1 = bv; NB1 = child; }
		//Get lowest value
		if (BV2 > bv) { BV2 = bv; NB2 = child; }
	}
	////Display
	//{
	//	if (CBN < 10) { std::cout << "CB:" << CBN << "__"; }
	//	else if (CBN < 100) { std::cout << "CB:" << CBN << "_"; }
	//	else { std::cout << "CB:" << CBN; }
	//	std::cout << "||__V" << BV1 << "___||__V" << BV2 << "__"  << std::endl;
	//	for (unsigned short int i = 0; i < 3; i++) {
	//		for (unsigned short int j = 0; j < 3; j++) {
	//			std::cout << Boards[CBN].state[(i * 3) + j] << " ";
	//		}
	//		std::cout << "|| ";
	//		for (unsigned short int j = 0; j < 3; j++) {
	//			std::cout << Boards[NB1].state[(i * 3) + j] << " ";
	//		}
	//		std::cout << "|| ";
	//		for (unsigned short int j = 0; j < 3; j++) {
	//			std::cout << Boards[NB2].state[(i * 3) + j] << " ";
	//		}
	//		std::cout << std::endl;
	//	}
	//	std::cout << "-----------------------" << std::endl;
	//}


	int NewBoard = 0;
	//Get the board with the lowest value.
	if (BV2 > BV1) { NewBoard = NB1; }
	else { NewBoard = NB2; }

	//Set current board to a variable that we can modify.
	std::array<int, 9> temp = CB;
	//Set new board to a variable that we can modify.
	std::array<int, 9> temp2 = Boards[NewBoard].state;
	for (unsigned short int i = 0; i < 9; i++) {
		//test new board position
		temp[i] = p;
			 if (temp == temp2)							 { CB[i] = p; }
		else if (temp == FlipY(temp2))					 { CB[i] = p; }
		else if (temp == FlipX(temp2))					 { CB[i] = p; }
		else if (temp == FlipX(FlipY(temp2)))			 { CB[i] = p; }
		else if (temp == Transpose(temp2))				 { CB[i] = p; }
		else if (temp == FlipY(Transpose(temp2)))		 { CB[i] = p; }
		else if (temp == FlipX(Transpose(temp2)))		 { CB[i] = p; }
		else if (temp == FlipX(FlipY(Transpose(temp2)))) { CB[i] = p; }
		//Reset modifiable current board variable.
		temp = CB;
	}
	//Set the current board to the new board.
	CurrentBoard = NewBoard;
	//Return the state of the new board.
	return CB;
}
//Transpose a board.
inline std::array<int, 9> TicTacToe::Transpose(std::array<int, 9> b) {
	std::array<int, 9> b2 = b;
	for (unsigned short int r = 0; r < 3; r++) {
		for (unsigned short int c = r; c < 3; c++) {
			std::swap(b2[((r * 3) + c)], b2[(r + (c * 3))]);
		}
	}
	//Returns the Transposed board.
	return b2;
}
//Flip a board on it's X-axis.
inline std::array<int, 9> TicTacToe::FlipX(std::array<int, 9> board) {
	for (unsigned short int i = 0; i < 3; i++) {
		std::swap(board[(i * 3)], board[((i * 3) + 2)]);
	}
	//Returns the Flipped board.
	return board;
}
//Flip a board on it's Y-axis.
inline std::array<int, 9> TicTacToe::FlipY(std::array<int, 9> board) {
	for (unsigned short int i = 0; i < 3; i++) {
		std::swap(board[i], board[(i + 6)]);
	}
	//Returns the Flipped board.
	return board;
}

//Function to test if there is a duplicate board in bounds.
inline int TicTacToe::TestBoard(std::array<int, 9> b, int begin, int end) {
	//Generate all permutations a board could be. 
	//Done by flipping it, rotating it, and transposing it.
	std::array<int, 9> n = b;
	std::array<int, 9> FY = FlipY(n);
	std::array<int, 9> FX = FlipX(n);
	std::array<int, 9> FXY = FlipX(FY);
	std::array<int, 9> T = Transpose(n);
	std::array<int, 9> TFY = FlipY(T);
	std::array<int, 9> TFX = FlipX(T);
	std::array<int, 9> TFXY = FlipX(FlipY(T));
	//Loop through bounds to test for Duplications.
	for (unsigned short int b2 = begin; b2 <= end; b2++) {
		if (b2 < Boards.size()) {
			std::array<int, 9> temp = Boards[b2].state;
			//Return the Duplicate board number.
			if (n == temp || FY == temp || FX == temp || FXY == temp || T == temp || TFY == temp || TFX == temp || TFXY == temp) { return b2; }
		}
	}
	//No Duplications
	return -1;
}
//Function to test if there is a duplicate board in a boards children.
inline int TicTacToe::TestChildren(std::array<int, 9> newb, int parent) {
	//Generate all permutations a board could be. 
	//Done by flipping it, rotating it, and transposing it.
	std::array<int, 9> n = newb;
	std::array<int, 9> FY = FlipY(n);
	std::array<int, 9> FX = FlipX(n);
	std::array<int, 9> FXY = FlipX(FY);
	std::array<int, 9> T = Transpose(n);
	std::array<int, 9> TFY = FlipY(T);
	std::array<int, 9> TFX = FlipX(T);
	std::array<int, 9> TFXY = FlipX(FlipY(T));
	//Loop through bounds to test for Duplications.
	for each (int child in Boards[parent].childs) {
		std::array<int, 9> temp = Boards[child].state;
		//Return the Duplicate board number.
		if (n == temp || FY == temp || FX == temp || FXY == temp || T == temp || TFY == temp || TFX == temp || TFXY == temp) { return child; }
	}
	//No Duplications
	return -1;
}

//Make all valid Boards
inline void TicTacToe::MakeBoard(int move, int player, int begin, int end) {
	for (unsigned short int b = begin; b <= end; b++) {
		//Make sure it's;
		//NOT a already won board
		//NOT an invalid board
		if (Boards[b].Won == 0 && ((Boards[b].Xs == Boards[b].Os && player == 1) || ((Boards[b].Xs - 1) == Boards[b].Os && player == 2))) {
			for (unsigned short int p = 0; p < 9; p++) {
				if (Boards[b].state[p] == 0) {
					Board temp = Boards[b];
					//Set move on board
					temp.state[p] = player;
					//Check for Duplications
					if (TestBoard(temp.state, end, Boards.size()-1) == -1) {
						//Clear all children
						temp.childs.clear();
						//Set depth
						temp.Depth = move;
						// Where the player moved that turn
						temp.Placement = p; 
						//Check if player Won
						if (CheckWin(temp, player)) {
							temp.Won = player;
							if (player == 1) { temp.Worth = (10-move); } // X
							else { temp.Worth = -(10-move); } // O
						}
						else { temp.Won = 0; }
						//Add 1 to the counter
						if (player == 1) { temp.Xs += 1; }
						else { temp.Os += 1; }
						//Push onto stack
						Boards.push_back(temp);
						//Add as a child
						Boards[b].childs.push_back((Boards.size() - 1));
					}
				}
			}
		}
	}
}

//Link all the Boards to their correct parents.
inline void TicTacToe::LinkBoard(int move, int player) {
	for (unsigned short int b = Bounds[move-2]+1; b <= Bounds[move-1]; b++) {
		//std::cout << "[" << b << ", " << move << "][";
		for (unsigned short int p = 0; p < 9; p++) {
			if (Boards[b].state[p] == 0) {
				std::array<int, 9> temp = Boards[b].state;
				temp[p] = player;
				//Check for Duplications
				int ts = TestBoard(temp, Bounds[move-1], Bounds[move]);
				if (ts != -1) { 
					if (!(std::find(Boards[b].childs.begin(), Boards[b].childs.end(), ts) != Boards[b].childs.end())) { 
						Boards[b].childs.push_back(ts);
						//std::cout << " n" << ts;
					}
				}
			}
		}
		//std::cout << "]" << std::endl;
	}
}

//Tic-Tac-Toe is has 19683 states. (3^9)
//Tic-Tac-Toe is has  5477 valid states excluding the empty position.
//Tic-Tac-Toe is has   765 valid states excluding duplicates under rotation and mirroring.
inline void TicTacToe::GenerateAllBoards(){
	Boards.clear();
	int begin = 0, end = 0;

	//X = 1
	//First move
	{
		//Clear board
		Board temp = Board({ 0,0,0, 0,0,0, 0,0,0 });
		temp.Depth = -1; temp.Placement = -1;
		temp.Won = 0; temp.Worth = 0;
		temp.Xs = 0; temp.Os = 0;
		temp.childs.push_back(1);
		temp.childs.push_back(2);
		temp.childs.push_back(3);
		Boards.push_back(temp);
		//Reset for the first 3 boards
		temp.childs.clear();
		temp.Depth = 0;
		temp.Xs = 1;
		//Middle
		temp.state[4] = 1;
		temp.Placement = 4;
		Boards.push_back(temp);
		temp.state[4] = 0;
		//Corner
		temp.state[6] = 1;
		temp.Placement = 6;
		Boards.push_back(temp);
		temp.state[6] = 0;
		//Edge
		temp.state[7] = 1;
		temp.Placement = 7;
		Boards.push_back(temp);

		begin = 1;
		end = Boards.size()-1;
		Bounds[0] = end;
		std::cout << 0 << "[" << begin << ", " << end << ", " << (end - begin)+1 << "]" << std::endl;
	}

	//Make Boards
	for (unsigned short int move = 1; move < 9; move++) {
		//O = 2
		if ((move % 2) == 1) { MakeBoard(move, 2, begin, end); }
		//X = 1
		else if ((move % 2) == 0) { MakeBoard(move, 1, begin, end); }

		begin = end+1;
		end = Boards.size() - 1;
		Bounds[move] = end;
		std::cout << move << "[" << begin << ", " << end << ", " << (end - begin)+1 << "]" << std::endl;
	}
	//Link Boards
	for (unsigned short int move = 2; move < 9; move++) {
		//O = 2
		if ((move % 2) == 1) { LinkBoard(move, 2); }
		//X = 1
		else if ((move % 2) == 0) { LinkBoard(move, 1); }
	}
	//Set all ties to #
	for (unsigned short int b = Bounds[7]; b < Bounds[8]; b++) {
		if (Boards[b].Won == 0) { Boards[b].Worth = 0; }
	}
	
	int pos = 0; int XWon = 0; int OWon = 0;
	for each (Board b in Boards) {
		////Display boards IF something
		//if (b.Depth == 0) {
		//	std::cout << std::endl << "[" << pos << "]";
		//	for (unsigned short int i = 0; i < 9; i++) {
		//		if (i % 3 == 0) { std::cout << std::endl; }
		//		std::cout << b.state[i] << " ";
		//	}
		//	std::cout << std::endl << "---";
		//}

		//Count all the wins
		if (b.Won > 0) {
			if (b.Won == 1) { XWon++; }
			if (b.Won == 2) { OWon++; }
		}
		pos++;
	}
	//Display info about the boards.
	std::cout << "Total Matches-" << Boards.size() << ", Total Ties- " << Boards.size()-(XWon + OWon) << std::endl;
	std::cout << "Wins: X-" << XWon << ", O-" << OWon << ", Total-" << (XWon+OWon) << std::endl;
}

