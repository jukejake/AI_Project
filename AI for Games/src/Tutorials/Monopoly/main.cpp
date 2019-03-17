#include <GL/gl3w.h>
#include <GLFW/glfw3.h> // GLFW helper library
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp> // for glm::ortho

// IMGUI
#include <imgui.h>
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
int width = 720; //1280
int height = 720; //720

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
GLuint XTexture, OTexture, BTexture, RTexture;


#pragma region User
int games_Finished = 0;
bool game_NotInprogress = true;

MonopolyGame game;

//This is where we will need to parse the strings to get some information. (Not all needs to be from here)
void ParseString(std::string& Output) {
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
				//Position
				if (SBuffer == "[P:") {
					char Buffer2[4];
					std::size_t length2 = Output.copy(Buffer2, 2, f0 + 1);
					Buffer2[length2] = '\0';
					if (length != std::string::npos) {
						int Number = std::stoi((std::string)Buffer2);
						if (Number < 99 && Number >= 0) { ChangeColour(Number); }
					}
				}
				else if (SBuffer == "[$")				 { SetConsoleTextAttribute(hConsole, 10); } //Gained Money
				else if (SBuffer == "[-$")				 { SetConsoleTextAttribute(hConsole, 12); } //Lost Money
				else if (SBuffer == "[Passed GO]")		 { SetConsoleTextAttribute(hConsole,  2); } //Passed Go prompt
				else if (SBuffer == "[Move to]")		 { SetConsoleTextAttribute(hConsole,  2); } //Move to location prompt
				else if (SBuffer == "[Free Pass]")		 { SetConsoleTextAttribute(hConsole,  2); } //Gained a Free Pass prompt
				else if (SBuffer == "[Free Jail Pass]")	 { SetConsoleTextAttribute(hConsole,  2); } //Gained a Free Jail Pass prompt
				else if (SBuffer == "[C:")				 { SetConsoleTextAttribute(hConsole, 10); } //
				else if (SBuffer == "[M:")				 { SetConsoleTextAttribute(hConsole,  4); } //Mortgaging Land 
				else if (SBuffer == "[UM:")				 { SetConsoleTextAttribute(hConsole,  2); } //Un-Mortgaging Land
				else if (SBuffer == "[SH:")				 { SetConsoleTextAttribute(hConsole,  4); } //Selling Land
				else if (SBuffer == "[Trade:")			 { SetConsoleTextAttribute(hConsole,112); } //Trading Land
				else if (SBuffer == "[ERROR:")			 { SetConsoleTextAttribute(hConsole,206); } //224
				else if (SBuffer == "[R:")				 { SetConsoleTextAttribute(hConsole,  7); } //Roll number

				//Doubles
				else if (SBuffer == "[1|1]")			 { SetConsoleTextAttribute(hConsole, 12); }
				else if (SBuffer == "[2|2]")			 { SetConsoleTextAttribute(hConsole, 12); }
				else if (SBuffer == "[3|3]")			 { SetConsoleTextAttribute(hConsole, 12); }
				else if (SBuffer == "[4|4]")			 { SetConsoleTextAttribute(hConsole, 12); }
				else if (SBuffer == "[5|5]")			 { SetConsoleTextAttribute(hConsole, 12); }
				else if (SBuffer == "[6|6]")			 { SetConsoleTextAttribute(hConsole, 12); }

				//Street Names
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

void UserInitialize() {
	/* initialize random seed: */
	srand(time(NULL));

	//initialize Starting values
	Net blank(topology);
	for (int i = 0; i < PlayerNum; i++) {
		Data_Player Temp;
		Data_Info.Players.push_back(Temp);
		network[i] = blank;
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

	if (!game_NotInprogress) {

		//A single players move
		if (!game.players[game.CurrentPlayer].AI && !game.players[game.CurrentPlayer].isDead) {
			game.PlayerMove();
		}
		else { game.AIMove(); }

		
		if (1 >= (PlayerNum - game.AmountDead)) {
			game.EndGame();
			games_Finished += 1;
			//There are more games to get through, so continue.
			if (games_Finished < Games) { 
				game = MonopolyGame();
				game.StartGame(); 
			}
			//No games left, so stop.
			else {
				game_NotInprogress = true;
				games_Finished = 0;
				if (Display) { DisplayStats(game.players, y); }
			}
		}
		//std::cout << game.CurrentPlayer; ParseString(game.Output); std::cout << std::endl;
	}
	
}

#pragma endregion

#pragma region other
// Functions
void DrawQuad(glm::vec2, glm::vec2, float = 0, glm::vec3 = glm::vec3(1.0f));

void Initialize()
{
	// Create a shader for the lab
	GLuint vs = buildShader(GL_VERTEX_SHADER, ASSETS"primitive.vs");
	GLuint fs = buildShader(GL_FRAGMENT_SHADER, ASSETS"primitive.fs");
	shader_program = buildProgram(vs, fs, 0);
	dumpProgram(shader_program, "Pong shader program");

	{
	XTexture = SOIL_load_OGL_texture(
		ASSETS"Images/X.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	OTexture = SOIL_load_OGL_texture(
		ASSETS"Images/O.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	BTexture = SOIL_load_OGL_texture(
		ASSETS"Images/Black.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	RTexture = SOIL_load_OGL_texture(
		ASSETS"Images/Red.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	}

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
}

void Update(float a_deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) { glfwSetWindowShouldClose(window, true); }
	UserUpdate();
	
}

void Render() {
	glUseProgram(shader_program);
	
	viewMatrix = glm::mat4(1.0f);
	//projectionMatrix = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f, -1.0f, 1.0f);
	projectionMatrix = glm::ortho(-360.0f, 360.0f, -360.0f, 360.0f, -1.0f, 1.0f);

	glActiveTexture(GL_TEXTURE0);
	int Spacing = 200, Size = 200;

	glBindTexture(GL_TEXTURE_2D, BTexture);
	DrawQuad(glm::vec2(-Spacing / 2, 0.0f), glm::vec2(20, 3 * Size));


	glUseProgram(GL_NONE);
}

void GUI(){
	ImGui::Begin("Settings", 0, ImVec2(300, 300), 0.4f); 
	{
		if (ImGui::Button("Start Game")) { game = MonopolyGame(); game.StartGame(); game.players[3].AI = false; game_NotInprogress = false; }
		if (!game_NotInprogress) {
			if (ImGui::Button("Roll")) { game.PlayerMove(Action::RollDie); }
			if (ImGui::Button("End Turn")) { game.PlayerMove(Action::EndTurn); }
			if (ImGui::Button("Roll than end turn")) { game.PlayerMove(Action::RollDie); game.PlayerMove(Action::EndTurn); }
			ImGui::Text("Text");
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

	window = glfwCreateWindow(width, height, "Assignment 3 - Tic Tac Toe", NULL, NULL);
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
