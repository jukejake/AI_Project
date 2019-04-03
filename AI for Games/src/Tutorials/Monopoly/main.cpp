#include <GL/gl3w.h>
#include <GLFW/glfw3.h> // GLFW helper library
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp> // for glm::ortho

#include <imgui.h> // IMGUI
#include <imgui_impl_glfw_gl3.h>
#include <vector>

#include <iostream> // Used for 'cout'
#include <stdio.h>  // Used for 'printf'
#include <SOIL.h>

#include <stdlib.h>		//srand, rand
#include <time.h>       //time
#include <windows.h>	//WinApi header

#include "Shaders.h"
#include "MFunctions.h"



/*---------------------------- Variables ----------------------------*/

// GLFW window
GLFWwindow* window;
int width = 900;
int height = 900;

// Uniform locations
GLuint mvp_loc, col_loc;

// OpenGl stuff
GLuint shader_program;
GLuint quad_vbo; // vertex buffer object
GLuint quad_vao; // vertex array object

// Matrices
glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;

// Textures
GLuint XTexture, OTexture, BTexture, RTexture, monopolyBoard;
GLuint playerXTurn[4];	//	textures for the turn prompts
GLuint pawnIcons[4], houseIcons[4], diceSides[6];

const float boardSize = 720.0f, pawnIconSize = 20.0f, shiftDistance = 58.0f;
const glm::vec2 turnPromptSize = glm::vec2(300, 150);
glm::vec2 spacePos[4][42];	//	coordinates of the center of each space 
glm::vec2 jailPos;
float deltaTime, timerVar = 0.0f;

#pragma region User
int games_Finished = 0;
int PlayerNumber = 0;
int UI_State = 1;
bool game_Inprogress = false;
bool ShowUIData = false;
int UserMoney = 0;
int UserPosition = 0;
int UserHasTownships = 0;
int UserHasMortgaged = 0;
int UserHasHouses = 0;
int TradeWith = 1;
int AI_Version = 0;
std::pair <int, int> AIValue = { 0,0 };
std::vector<int> Offer = {0};
std::vector<int> Request = {0};

MonopolyGame game;


void UserInitialize() {
	// initialize random seed:
	srand(time(NULL));
	//Net blank0(topology);
	//Net blank1(topology);
	//Net blank2(topology);
	//Net blank3(topology);
	//network[0] = blank0;
	//network[1] = blank1;
	//network[2] = blank2;
	//network[3] = blank3;

	Net blank4(topology);
	SingleNetwork = blank4;

	//initialize Starting values
	for (int i = 0; i < PlayerNum; i++) {
		Data_Player Temp;
		Data_Info.Players.push_back(Temp);
	}
	for (int i = 0; i < 41; i++) {
		Data_Info.LandOwnerShip[i] = -1;
	}
	//Display at the end
	//std::cout << std::endl << std::endl;
	//if (!Display) { GetData(); }



}

void UserUpdate() {
	//Keep going until game games are done.
	if (game_Inprogress && UI_State == 0) {
		game.AIMove();
		if (ShowUIData) { game.MonopolyShowMe(game.CurrentPlayer); }

		if (2 > (PlayerNum - game.AmountDead)) {
			game.EndGame(AI_Version);
			games_Finished += 1;
			std::cout << games_Finished << std::endl;
			//There are more games to get through, so continue.
			if (games_Finished < Games) {
				game = MonopolyGame();
				game.StartGame();
			}
			//No games left, so stop.
			else {
				game_Inprogress = false;
				games_Finished = 0;
				UI_State = 1;
				if (Display) { DisplayStats(game.players, y); }
				CreateExcelFile("10000v4_AI");//Outfile
			}
		}
		else if (game.rolls >= RollsPerGame) {
			CalculateAmountOfHouses(game.players);
			CalculateTotalAssetValue(game.players);
			int highest = 0;
			int second = 0;
			int third = 0;
			int forth = 0;
			for (int i = 0; i < PlayerNum; i++) {
				if (game.players[i].TotalAssetValue > highest) { 
					forth = third;
					third = second;
					second = highest;
					highest = game.players[i].TotalAssetValue;
				}
			}
			for (int i = 0; i < PlayerNum; i++) {
				if (game.players[i].TotalAssetValue < highest && game.players[i].TotalAssetValue > second) {
					forth = third;
					third = second;
					second = game.players[i].TotalAssetValue;
				}
			}
			for (int i = 0; i < PlayerNum; i++) {
				if (game.players[i].TotalAssetValue < second && game.players[i].TotalAssetValue > third) {
					forth = third;
					third = game.players[i].TotalAssetValue;
				}
			}
			for (int i = 0; i < PlayerNum; i++) {
				if (game.players[i].TotalAssetValue < third && game.players[i].TotalAssetValue > forth) {
					forth  = game.players[i].TotalAssetValue;
				}
			}
			for (int i = 0; i < PlayerNum; i++) {
				if (game.players[i].Place == 0) {
						 if (game.players[i].TotalAssetValue == highest) { game.players[i].Place = 1; }
					else if (game.players[i].TotalAssetValue == second)	 { game.players[i].Place = 2; }
					else if (game.players[i].TotalAssetValue == third)	 { game.players[i].Place = 3; }
					else if (game.players[i].TotalAssetValue == forth)	 { game.players[i].Place = 4; }
					else { game.players[i].Place = 13; }
					game.players[i].DiedAt = (int)(game.rolls / PlayerNum);
					game.players[i].isDead = true;
				}
			}
			//for (unsigned short int i = 0; i < PlayerNum; i++) { game.players[i].isDead = true; }
			game.EndGame(AI_Version);
			games_Finished += 1;
			std::cout << games_Finished << std::endl;
			//There are more games to get through, so continue.
			if (games_Finished < Games) {
				game = MonopolyGame();
				game.StartGame();
			}
			//No games left, so stop.
			else {
				game_Inprogress = false;
				games_Finished = 0;
				UI_State = 1;
				if (Display) { DisplayStats(game.players, y); }
				CreateExcelFile("10000v4_AI");//Outfile
			}
		}
	} 
	else if (game_Inprogress && UI_State == 2) {
		//A single players move
		if (game.players[game.CurrentPlayer].AI) { game.AIMove(); }
		else { game.PlayerMove(); }
		if (2 > (PlayerNum - game.AmountDead)) {
			game.EndGame();
			game_Inprogress = false;
		}
	}
	
}

#pragma endregion

#pragma region other
// Functions
void DrawQuad(glm::vec2, glm::vec2, float = 0, glm::vec3 = glm::vec3(1.0f));

bool timer(float timeToWait) {
	timerVar += deltaTime;

	if (timerVar < timeToWait) { return false; }	//	return false to denote that the timer hasn't reached its goal yet

	timerVar = 0.0f;	//	reset
	return true;
}

void loadTextures() {
	monopolyBoard = SOIL_load_OGL_texture(
		ASSETS"Images/board.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	pawnIcons[0] = SOIL_load_OGL_texture(
		ASSETS"Images/pawn_red.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	pawnIcons[1] = SOIL_load_OGL_texture(
		ASSETS"Images/pawn_blue.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	pawnIcons[2] = SOIL_load_OGL_texture(
		ASSETS"Images/pawn_green.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	pawnIcons[3] = SOIL_load_OGL_texture(
		ASSETS"Images/pawn_yellow.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	playerXTurn[0] = SOIL_load_OGL_texture(
		ASSETS"Images/p1_Turn.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	playerXTurn[1] = SOIL_load_OGL_texture(
		ASSETS"Images/p2_Turn.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	playerXTurn[2] = SOIL_load_OGL_texture(
		ASSETS"Images/p3_Turn.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	playerXTurn[3] = SOIL_load_OGL_texture(
		ASSETS"Images/p4_Turn.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	houseIcons[0] = SOIL_load_OGL_texture(
		ASSETS"Images/house_red.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	houseIcons[1] = SOIL_load_OGL_texture(
		ASSETS"Images/house_blue.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	houseIcons[2] = SOIL_load_OGL_texture(
		ASSETS"Images/house_green.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	houseIcons[3] = SOIL_load_OGL_texture(
		ASSETS"Images/house_yellow.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	diceSides[0] = SOIL_load_OGL_texture(
		ASSETS"Images/1.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	diceSides[1] = SOIL_load_OGL_texture(
		ASSETS"Images/2.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	diceSides[2] = SOIL_load_OGL_texture(
		ASSETS"Images/3.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	diceSides[3] = SOIL_load_OGL_texture(
		ASSETS"Images/4.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	diceSides[4] = SOIL_load_OGL_texture(
		ASSETS"Images/5.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	diceSides[5] = SOIL_load_OGL_texture(
		ASSETS"Images/6.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
}

void Initialize()
{
	// Create a shader for the lab
	GLuint vs = buildShader(GL_VERTEX_SHADER, ASSETS"primitive.vs");
	GLuint fs = buildShader(GL_FRAGMENT_SHADER, ASSETS"primitive.fs");
	shader_program = buildProgram(vs, fs, 0);
	dumpProgram(shader_program, "Pong shader program");

	loadTextures();

	// Create all 4 vertices of the quad
	glm::vec3 p0 = glm::vec3(-1.0f, -1.0f, 0.0f);
	glm::vec3 p1 = glm::vec3(-1.0f, 1.0f, 0.0f);
	glm::vec3 p2 = glm::vec3(1.0f, -1.0f, 0.0f);
	glm::vec3 p3 = glm::vec3(1.0f, 1.0f, 0.0f);

	// Create a list of vertices
	glm::vec3 vertices[12] =
	{
		// Bottom face
		p0, p1, p2, p3,
	};

	glGenVertexArrays(1, &quad_vao);
	glBindVertexArray(quad_vao);

	glGenBuffers(1, &quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	glUseProgram(shader_program);
	GLuint vPosition = glGetAttribLocation(shader_program, "vPosition");
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	mvp_loc = glGetUniformLocation(shader_program, "modelViewProjMat");

	//Transparency
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//	first spaces
	spacePos[0][0] = glm::vec2(286, -305);
	spacePos[1][0] = glm::vec2(306, -305);
	spacePos[2][0] = glm::vec2(286, -325);
	spacePos[3][0] = glm::vec2(306, -325);

	for (int i = 1;i < 11;i++) {	//	set values for bottom spaces
		spacePos[0][i] = glm::vec2(spacePos[0][i - 1].x - (shiftDistance + 1), spacePos[0][i - 1].y);
		spacePos[1][i] = glm::vec2(spacePos[1][i - 1].x - (shiftDistance + 1), spacePos[1][i - 1].y);
		spacePos[2][i] = glm::vec2(spacePos[2][i - 1].x - (shiftDistance + 1), spacePos[2][i - 1].y);
		spacePos[3][i] = glm::vec2(spacePos[3][i - 1].x - (shiftDistance + 1), spacePos[3][i - 1].y);
	}
	spacePos[0][10].x -= 20;
	spacePos[1][10].x -= 20;
	spacePos[2][10].x -= 20;
	spacePos[3][10].x -= 20;


	spacePos[0][11] = glm::vec2(spacePos[0][10].x, spacePos[0][10].y + shiftDistance+20);
	spacePos[1][11] = glm::vec2(spacePos[1][10].x, spacePos[1][10].y + shiftDistance+20);
	spacePos[2][11] = glm::vec2(spacePos[2][10].x, spacePos[2][10].y + shiftDistance+20);
	spacePos[3][11] = glm::vec2(spacePos[3][10].x, spacePos[3][10].y + shiftDistance+20);
	for (int i = 12;i < 21;i++) {	//	set values for left spaces
		spacePos[0][i] = glm::vec2(spacePos[0][i - 1].x, spacePos[0][i - 1].y + shiftDistance+1);
		spacePos[1][i] = glm::vec2(spacePos[1][i - 1].x, spacePos[1][i - 1].y + shiftDistance+1);
		spacePos[2][i] = glm::vec2(spacePos[2][i - 1].x, spacePos[2][i - 1].y + shiftDistance+1);
		spacePos[3][i] = glm::vec2(spacePos[3][i - 1].x, spacePos[3][i - 1].y + shiftDistance+1);
	}

	spacePos[0][21] = glm::vec2(spacePos[0][10].x + shiftDistance*1.3, spacePos[0][1].y + 630);
	spacePos[1][21] = glm::vec2(spacePos[1][10].x + shiftDistance*1.3, spacePos[1][1].y + 630);
	spacePos[2][21] = glm::vec2(spacePos[2][10].x + shiftDistance*1.3, spacePos[2][1].y + 630);
	spacePos[3][21] = glm::vec2(spacePos[3][10].x + shiftDistance*1.3, spacePos[3][1].y + 630);
	for (int i = 22;i < 31;i++) {	//	set values for top spaces
		int NewX = i-9-(2*(i-20));
		spacePos[0][i] = glm::vec2(spacePos[0][NewX].x + shiftDistance, spacePos[0][i - 20].y + 630);
		spacePos[1][i] = glm::vec2(spacePos[1][NewX].x + shiftDistance, spacePos[1][i - 20].y + 630);
		spacePos[2][i] = glm::vec2(spacePos[2][NewX].x + shiftDistance, spacePos[2][i - 20].y + 630);
		spacePos[3][i] = glm::vec2(spacePos[3][NewX].x + shiftDistance, spacePos[3][i - 20].y + 630);
	}
	for (int i = 31;i < 40;i++) {	//	set values for right spaces
		int NewY = i-10-(2*(i-30));
		spacePos[0][i] = glm::vec2(spacePos[0][i - 20].x + 630, spacePos[0][NewY].y);
		spacePos[1][i] = glm::vec2(spacePos[1][i - 20].x + 630, spacePos[1][NewY].y);
		spacePos[2][i] = glm::vec2(spacePos[2][i - 20].x + 630, spacePos[2][NewY].y);
		spacePos[3][i] = glm::vec2(spacePos[3][i - 20].x + 630, spacePos[3][NewY].y);
	}
	
	//	set jail positions
	jailPos = glm::vec2(-296, -302.5);
	spacePos[0][10] = glm::vec2(-288.5, -347.5);
	spacePos[1][10] = glm::vec2(-325, -347.5);
	spacePos[2][10] = glm::vec2(-346.5, -320);
	spacePos[3][10] = glm::vec2(-346.5, -285);

	spacePos[0][40] = glm::vec2(-280, -320);
	spacePos[1][40] = glm::vec2(-280, -285);
	spacePos[2][40] = glm::vec2(-320, -320);
	spacePos[3][40] = glm::vec2(-320, -285);
}

void Update(float a_deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) { glfwSetWindowShouldClose(window, true); }
	//if (glfwGetKey(window, GLFW_KEY_UP)) { spacePos[0][0].y += 0.5; }
	//if (glfwGetKey(window, GLFW_KEY_DOWN)) { spacePos[0][0].y -= 0.5; }
	//if (glfwGetKey(window, GLFW_KEY_RIGHT)) { spacePos[0][0].x += 0.5; }
	//if (glfwGetKey(window, GLFW_KEY_LEFT)) { spacePos[0][0].x -= 0.5; }
	UserUpdate();	
}

void Render() {
	glUseProgram(shader_program);
	
	viewMatrix = glm::mat4(1.0f);
	//projectionMatrix = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f, -1.0f, 1.0f);
	projectionMatrix = glm::ortho(-360.0f, 360.0f, -360.0f, 360.0f, -1.0f, 1.0f);

	//glActiveTexture(GL_TEXTURE0);
	//int Spacing = 200, Size = 200;

	//glBindTexture(GL_TEXTURE_2D, monopolyBoard);
	//DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(boardSize, boardSize));

	//for (int i = 0; i < 4;i++) {
	//	switch (game.players[i].isDead) {
	//		case false:
	//			glBindTexture(GL_TEXTURE_2D, pawnIcons[i]);
	//			DrawQuad(spacePos[i][game.players[i].position], glm::vec2(pawnIconSize, pawnIconSize));
	//			break;
	//	}
	//}

	//glBindTexture(GL_TEXTURE_2D, diceSides[game.players[game.CurrentPlayer].FirstDice-1]);
	//DrawQuad(glm::vec2(-100, 0), glm::vec2(60, 60));

	//glBindTexture(GL_TEXTURE_2D, diceSides[game.players[game.CurrentPlayer].SecondDice-1]);
	//DrawQuad(glm::vec2(100, 0), glm::vec2(60, 60));
	////std::cout << "Position " + std::to_string(i) + ": " + std::to_string(spacePos[i].x) + ", " + std::to_string(spacePos[i].y) << std::endl;

	glUseProgram(GL_NONE);
}

void GUI() {
	ImGui::Begin("Settings", 0, ImVec2(650, 650), 0.8f);
	{

		PlayerInfo &player = game.players[game.CurrentPlayer];
		switch (UI_State) {
			//AI vs AI
		case 0: {
			if (ImGui::Button("Stop running games")) {
				game.EndGame();
				game_Inprogress = false;
				UI_State = 1;
			}

			ImGui::Text(" ");
			if (ImGui::Button("Show UI choices")) { ShowUIData = true; }
			ImGui::Text("   I means input values");
			ImGui::Text("   R means result values");
			if (ImGui::Button("Hide UI choices")) { ShowUIData = false; }
			if (ImGui::Button("End current game and start a new one")) {
				game.EndGame();
				game = MonopolyGame();
				game.StartGame();
			}
		} break;
			//Starting UI
		case 1:{
			//Start the game where a AI faces against each other 
			if (ImGui::Button("Start Game: AI vs AI")) {
				game_Inprogress = true;
				//Do not display the game to the CMD
				Display = false;
				//Reset game
				game = MonopolyGame();
				game.StartGame();
				UI_State = 0;
				//Version the AI will use as Input
				AI_Version = 0;
			}
			//Start the game where a AI faces against each other and learn to not buy a lot of properties
			ImGui::Text(" ");
			if (ImGui::Button("Start Game: AI vs AI : where AI learns to not buy properties")) {
				game_Inprogress = true;
				//Do not display the game to the CMD
				Display = false;
				//Reset game
				game = MonopolyGame();
				game.StartGame();
				UI_State = 0;
				//Version the AI will use as Input
				AI_Version = 1;
			}
			//Start the game where a player faces against an AI
			ImGui::Text(" ");
			ImGui::SliderInt("Player Number", &PlayerNumber, 0, 3);
			if (ImGui::Button("Start Game: Player vs AI")) {
				game_Inprogress = true;
				//Display the game to the CMD
				Display = true;
				//Reset game
				game = MonopolyGame();
				game.StartGame();
				game.players[PlayerNumber].AI = false;
				UI_State = 2;
			}
		}break;
			//Player vs AI
		case 2: {

			if (game_Inprogress) {
				if (ImGui::Button("Roll then end turn")) { game.PlayerMove(Action::RollDie); game.PlayerMove(Action::EndTurn); }
				if (ImGui::Button("Roll")) { game.PlayerMove(Action::RollDie); } ImGui::SameLine();
				if (ImGui::Button("End Turn")) { game.PlayerMove(Action::EndTurn); }

				UserMoney = player.Money;
				UserPosition = player.position;

				ImGui::Text("============================");
				ImGui::Text("Current Position: %s : %i : %s", StreetNames[UserPosition].c_str(), UserPosition, StreetColour[GetTownshipFromProperty(UserPosition)].c_str());
				ImGui::Text("Current Funds: $ %i", UserMoney);

				if (CheckLandPrice(Data_Info, UserPosition) != 0 && Data_Info.LandOwnerShip[UserPosition] == -1) {
					if (UserMoney < PropertyPrice[UserPosition]) { ImGui::Text("Not Enough Money To Purchase Property : $%i", PropertyPrice[UserPosition]); }
					else {
						ImGui::Text("Property Cost: $%i", PropertyPrice[UserPosition]); ImGui::SameLine();
						if (ImGui::Button("Buy Property")) { game.PlayerMove(Action::Buying); }
					}
				}
				else { ImGui::Text("Can't buy current Property"); }


				if (UserMoney < 0) {
					ImGui::Text("/// User doesn't have enough money to continue ///");
					ImGui::Text("/// Sell, Mortgage, or Trade to stay alive ///");
					if (ImGui::Button("Die")) { player.AI = true; player.isDead = true; } ImGui::SameLine();
					ImGui::Text(" or let the AI try "); ImGui::SameLine();
					if (ImGui::Button("AI")) { player.AI = true; }
					if (ImGui::Button("Die and quit")) { player.AI = true; game.EndGame(); }
					ImGui::Text(" ");
				}

				if (ImGui::Button("Buy houses")) {
					UI_State = 3;
					for (int i = 0; i < 8; i++) {
						//check to see if the player has that township.
						if (player.Townships[i] == 1) { UserHasTownships += 1; }
					}
				}
				if (UserHasHouses > 0 && ImGui::Button("Sell Houses")) { UI_State = 4; }

				if (ImGui::Button("Mortgage")) { UI_State = 5; }
				if (UserHasMortgaged > 0 && ImGui::Button("UnMortgaging")) { UI_State = 6; }

				if (ImGui::Button("Trade")) { UI_State = 7; }

				if (ImGui::Button("Who has what Properties")) { UI_State = 8; }

				if (ImGui::Button("Exit")) {
					game.EndGame();
					game_Inprogress = false;
					player.AI = true;
					UI_State = 1;
				}

				ImGui::Text("===Rolls===");
				for (int i = 0; i < 4; i++) {
					ImGui::Text("Player %i rolled %i :[%i] [%i]", (i + 1), (game.players[i].FirstDice + game.players[i].SecondDice), game.players[i].FirstDice, game.players[i].SecondDice);
				}
			}
			else {
				if (!player.isDead) { ImGui::Text("YOU WON!!!"); }
				else { ImGui::Text("You lost."); }
				if (ImGui::Button("Exit")) {
					UI_State = 1;
					player.AI = true;
				}
			}
		} break;
			//Upgrading
		case 3: {
			if (ImGui::Button("Go back")) { UI_State = 2; }
			ImGui::Text("===Buying houses===");
			if (UserHasTownships > 0) {
				ImGui::Text("Current Funds: $ %i", player.Money);
				std::vector<int> PinT;
				//Only check to (8) as (9 & 10) are [Utilities] & [Railroads] and can't be upgraded
				for (int i = 0; i < 8; i++) {
					//check to see if the player has that township.
					if (player.Townships[i] == 1) {
						PinT = GetPropertiesInTownship(i);

						ImGui::Text(StreetColour[i].c_str());
						for each (int P in PinT) {
							if (player.LandMortgaged[P]) {
								if (ImGui::Button(StreetNames[P].c_str())) {} ImGui::SameLine();
								ImGui::Text("/// Mortgaged ///");
							}
							else {
								if (ImGui::Button(StreetNames[P].c_str())) { game.PlayerMove(Action::Upgrading, P); UserHasHouses++; } ImGui::SameLine();
								ImGui::Text("$%i", HousePrice[P]); ImGui::SameLine();
								ImGui::Text("| Amount of homes: %i", player.Land[P] - 1);
							}
						}
						ImGui::Text(" ");
					}
				}
			}
			else { ImGui::Text("You don't have enough Properties to makeup a township"); }

		} break;
			//Sell houses
		case 4: {
			if (ImGui::Button("Go back")) { UI_State = 2; }
			ImGui::Text("===Sell houses===");
			ImGui::Text("Current Funds: $ %i", player.Money);
			for (int i = 0; i < 40; i++) {
				if (player.Land[i] > 0) {
					if (player.LandMortgaged[i]) {
						if (ImGui::Button(StreetNames[i].c_str())) {} ImGui::SameLine();
						ImGui::Text("/// Mortgaged ///");
					}
					else if (player.Land[i] == 1) {
						if (ImGui::Button(StreetNames[i].c_str())) {} ImGui::SameLine();
						ImGui::Text("/// No Houses ///");
					}
					else {
						if (ImGui::Button(StreetNames[i].c_str())) { game.PlayerMove(Action::Mortgaging, i, 1); UserHasHouses--; } ImGui::SameLine();
						ImGui::Text("Sell house for $%i", (int)(HousePrice[i] * 0.5)); ImGui::SameLine();
						ImGui::Text("| Amount of homes: %i", (player.Land[i] - 1));
					}
				}
			}
		} break;
			//Mortgage Property
		case 5: {
			if (ImGui::Button("Go back")) { UI_State = 2; }
			ImGui::Text("===Mortgage===");
			ImGui::Text("Current Funds: $ %i", player.Money);
			for (int i = 0; i < 40; i++) {
				if (player.Land[i] == 1) {
					if (player.LandMortgaged[i]) {
						ImGui::Button(StreetNames[i].c_str()); ImGui::SameLine();
						ImGui::Text(StreetColour[GetTownshipFromProperty(i)].c_str()); ImGui::SameLine();
						ImGui::Text("/// Already Mortgaged ///");
					}
					else {
						if (ImGui::Button(StreetNames[i].c_str())) { game.PlayerMove(Action::Mortgaging, i, 2); UserHasMortgaged++; } ImGui::SameLine();
						ImGui::Text(StreetColour[GetTownshipFromProperty(i)].c_str()); ImGui::SameLine();
						ImGui::Text("| Mortgage for $%i", (int)(PropertyPrice[i] * 0.5));
					}
				}
				else if (player.Land[i] > 1) {
					ImGui::Button(StreetNames[i].c_str()); ImGui::SameLine();
					ImGui::Text(StreetColour[GetTownshipFromProperty(i)].c_str()); ImGui::SameLine();
					ImGui::Text("/// Need to sell houses first ///");
				}
			}
		} break;
			//UnMortgaging Properties
		case 6: {

			if (ImGui::Button("Go back")) { UI_State = 2; }
			ImGui::Text("===UnMortgaging===");
			ImGui::Text("Current Funds: $ %i", player.Money);
			for (int i = 0; i < 40; i++) {
				if (player.Land[i] > 0 && !player.LandMortgaged[i]) {
					ImGui::Button(StreetNames[i].c_str()); ImGui::SameLine();
					ImGui::Text(StreetColour[GetTownshipFromProperty(i)].c_str()); ImGui::SameLine();
					ImGui::Text("/// Not Mortgaged ///");
				}
				else if (player.Land[i] > 0) {
					if (ImGui::Button(StreetNames[i].c_str())) { game.PlayerMove(Action::UnMortgaging, i); UserHasMortgaged--; } ImGui::SameLine();
					ImGui::Text(StreetColour[GetTownshipFromProperty(i)].c_str()); ImGui::SameLine();
					ImGui::Text("| UnMortgage for $%i", (int)(PropertyPrice[i] * 0.5));
				}
			}
		} break;
			//Trade
		case 7: {
			if (ImGui::Button("Go back")) { UI_State = 2; }
			ImGui::Text("===Trade===");
			ImGui::SliderInt("Trade with", &TradeWith, 0, 3);
			if (TradeWith != PlayerNumber) {
				if (ImGui::Button("Clear selection")) {
					Offer = { 0 };
					Request = { 0 };
				}
				AIValue = HowMuchDoesTheAIWant(Data_Info, game.players, PlayerNumber, TradeWith, Offer, Request);
				ImGui::Text("AI values that at $%i, $%i", AIValue.first, AIValue.second);
				ImGui::Text("The AI will accept when one of the values is at or above 0.");

				//Offer
				ImGui::Text(" ");
				ImGui::Text("================"); 
				ImGui::Text("Current Funds: $ %i", game.players[PlayerNumber].Money);
				ImGui::Text("===Offering===");
				ImGui::SliderInt("Offering $", &Offer[0], 0, player.Money);
				for (int i = 0; i < 40; i++) {
					if (game.players[PlayerNumber].Land[i] > 0) {
						if (ImGui::Button(StreetNames[i].c_str())) {
							std::vector<int>::iterator it; 
							it = std::find(Offer.begin()+1, Offer.end(), i);
							//Remove from vector
							if (it != Offer.end()) { Offer.erase(it); }
							//Add to offer vector
							else { Offer.push_back(i); }
						} ImGui::SameLine();
						ImGui::Text(StreetColour[GetTownshipFromProperty(i)].c_str()); ImGui::SameLine();
						if (game.players[PlayerNumber].LandMortgaged[i]) { ImGui::Text("/// Mortgaged ///"); }
						else { ImGui::Text("| Houses: %i", game.players[PlayerNumber].Land[i] - 1); }
					}
				}
				ImGui::Text("=Properties=");
				for (unsigned short int j = 1; j < Offer.size(); j++) {
					ImGui::Text("- %s", StreetNames[Offer[j]].c_str());
				}

				ImGui::Text(" ");
				//Request
				ImGui::Text("================");
				ImGui::Text("AI Funds: $ %i", game.players[TradeWith].Money);
				ImGui::Text("===Requesting===");
				ImGui::SliderInt("Requesting $", &Request[0], 0, game.players[TradeWith].Money);
				for (int i = 0; i < 40; i++) {
					if (game.players[TradeWith].Land[i] > 0) {
						if (ImGui::Button(StreetNames[i].c_str())) {
							std::vector<int>::iterator it;
							it = std::find(Request.begin() + 1, Request.end(), i);
							//Remove from vector
							if (it != Request.end()) { Request.erase(it); }
							//Add to offer vector
							else { Request.push_back(i); }
						} ImGui::SameLine();
						ImGui::Text(StreetColour[GetTownshipFromProperty(i)].c_str()); ImGui::SameLine();
						if (game.players[TradeWith].LandMortgaged[i]) { ImGui::Text("/// Mortgaged ///"); }
						else { ImGui::Text("| Houses: %i", game.players[TradeWith].Land[i] - 1); }
					}
				}
				ImGui::Text("=Properties=");
				for (unsigned short int j = 1; j < Request.size(); j++) {
					ImGui::Text("+ %s", StreetNames[Request[j]].c_str());
				}

				if (ImGui::Button("Trade")) {
					//AI will accept the deal
					if ((AIValue.first >= 0 && AIValue.first >= AIValue.second) || (AIValue.second >= 0 && AIValue.second >= AIValue.first)) 
					{ game.PlayerMove(Action::Trading, PlayerNumber, TradeWith, Offer, Request); }
					//No Deal
					else {}
					//Reset the Offer and Request
					Offer = { 0 };
					Request = { 0 };
				}
			}
		} break;
			//Who Has What
		case 8: {
			if (ImGui::Button("Go back")) { UI_State = 2; }
			ImGui::Text("===Who Has What===");
			for (int p = 0; p < 4; p++) {
				ImGui::Text("Player :%i", (p + 1));
				if (game.players[p].Money > 0) {
					ImGui::Text("Current Funds: $ %i", game.players[p].Money);
					for (int i = 0; i < 40; i++) {
						if (game.players[p].Land[i] > 0) {
							if (game.players[p].LandMortgaged[i]) {
								ImGui::Button(StreetNames[i].c_str()); ImGui::SameLine();
								ImGui::Text(StreetColour[GetTownshipFromProperty(i)].c_str()); ImGui::SameLine();
								ImGui::Text("/// Mortgaged ///");
							}
							else {
								ImGui::Button(StreetNames[i].c_str()); ImGui::SameLine();
								ImGui::Text(StreetColour[GetTownshipFromProperty(i)].c_str()); ImGui::SameLine();
								ImGui::Text("| Houses: %i", game.players[p].Land[i] - 1);
							}
						}
					}
				}
				else { ImGui::Text("Dead"); }
				ImGui::Text(" ");
			}
		} break;

		}
	}
	ImGui::End();
}

void Cleanup()
{
	glDeleteBuffers(1, &quad_vbo);
	glDeleteVertexArrays(1, &quad_vao);
	glDeleteProgram(shader_program);
}

void DrawQuad(glm::vec2 a_position, glm::vec2 a_size, float rot, glm::vec3 a_color) {
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(a_position.x, a_position.y, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rot, glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(a_size.x * 0.5f, a_size.y * 0.5f, 1.0f));

	glm::mat4 modelViewProjMat = projectionMatrix * viewMatrix * modelMatrix;

	glUniformMatrix4fv(mvp_loc, 1, 0, &modelViewProjMat[0][0]);
	glUniform3fv(col_loc, 1, &a_color[0]);

	glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

static void ResizeEvent(GLFWwindow* a_window, int a_width, int a_height)
{
	// Set the viewport incase the window size changed
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

int main()
{
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	window = glfwCreateWindow(width, height, "Monopoly", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, ResizeEvent);
	glfwSwapInterval(0);

	// start GL3W
	gl3wInit();

	// Setup ImGui binding. This is for any parameters you want to control in runtime
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsLight();

	// Get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	//glEnable(GL_DEPTH_TEST); // enable depth-testing
	//glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	Initialize();
	UserInitialize();

	float oldTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - oldTime;
		oldTime = currentTime;

		// update other events like input handling 
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui_ImplGlfwGL3_NewFrame();

		// Call the helper functions
		Update(deltaTime);
		Render();
		GUI();

		// Finish by drawing the GUI on top of everything
		ImGui::Render();
		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	ImGui_ImplGlfwGL3_Shutdown();
	Cleanup();
	return 0;
}

#pragma endregion



void main11() {
	PlayGameInCMD();
	CreateExcelFile("10000_No_AI");
	system("pause");
}
