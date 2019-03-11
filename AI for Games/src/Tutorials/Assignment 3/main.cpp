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

#include "Shaders.h"
#include "TicTacToe.h"

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
int PlayerChoice, AIChoice;

TicTacToe Game;
//Current board state
std::array<int, 9> CB{ 0,0,0, 0,0,0, 0,0,0 };

//Just used here to display the Red line over the win state.
int winStates[8][3] = {{0, 1, 2},  // Check First row. 
				 {3, 4, 5},  // Check Second Row 
				 {6, 7, 8},  // Check Third Row 
				 {0, 3, 6},  // Check First column 
				 {1, 4, 7},  // Check Second Column 
				 {2, 5, 8},  // Check Third Column 
				 {0, 4, 8},  // Check First Diagonal 
				 {2, 4, 6}}; // Check Second Diagonal 
 //Move Counter
static int MC = 1;
static int intInput = 0;
//Are the boards generated.
static bool generated = false;
//Where is the win state.
static int Where = -1;
//Is the AI on.
static bool AI_ON = true;

int TimeMinimaxS(int i, int k, int l, bool t) {
	return Game.minimaxS(i, k, l, t);
}
std::pair<int, int> TimeMinimaxD(int j, int k, int l, bool t) {
	return Game.minimaxD(j, k, l, t);
}
void TimeGenerateBoards() {
	Game.GenerateAllBoards();
}


void PickMove(int move, int p) {
	//Assign current board to a new board.
	auto newBoard = CB;
	//As long as the move is valid and the board is not already won.
	if (newBoard[move] == 0 && Game.Boards[Game.CurrentBoard].Won == 0) {
		//Assign move to the new board.
		newBoard[move] = p;
		//Test to see if the new board is a child of the old board.
		int CBnumber = Game.TestChildren(newBoard, Game.CurrentBoard);
		//If not a child; exit.
		if (CBnumber == -1) {
			std::cout << "[Can't pick this move as there are no children]" << std::endl;
			return;
		}
		//It was a valid move so assign it to the current board.
		CB[move] = p;
		//Update the current board number to the new board number.
		Game.CurrentBoard = CBnumber;

		//If playing against an AI.
		if (AI_ON && Game.Boards[CBnumber].Won == 0) {
			int p2;
			//Switch from X to O or O to X.
			if (p == 1) { p2 = 2; }
			else { p2 = 1; }
			//Get the AI's move.
			CB = Game.ChooseMove(CB, CBnumber, p2);
		}
		//If playing against a different player.
		else if (!AI_ON) {
			//Auto switch player
			if (MC == 1) { MC = 2; }
			else { MC = 1; }
		}
	}
}

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
	
	//Win
	//Block
	//Fork
	//Blocking the opponent's fork
	//Center
	//Opposite corner
	//Empty corner
	//Empty side
}

void Render() {
	glUseProgram(shader_program);
	
	viewMatrix = glm::mat4(1.0f);
	//projectionMatrix = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f, -1.0f, 1.0f);
	projectionMatrix = glm::ortho(-360.0f, 360.0f, -360.0f, 360.0f, -1.0f, 1.0f);

	glActiveTexture(GL_TEXTURE0);
	int Spacing = 200, Size = 200;

	if (!Game.Boards.empty()) {
		//Board
		glBindTexture(GL_TEXTURE_2D, BTexture);
		DrawQuad(glm::vec2(-Spacing / 2, 0.0f), glm::vec2(20, 3 * Size));
		glBindTexture(GL_TEXTURE_2D, BTexture);
		DrawQuad(glm::vec2(Spacing / 2, 0.0f), glm::vec2(20, 3 * Size));
		glBindTexture(GL_TEXTURE_2D, BTexture);
		DrawQuad(glm::vec2(0.0f ,-Spacing / 2), glm::vec2(3 * Size, 20));
		glBindTexture(GL_TEXTURE_2D, BTexture);
		DrawQuad(glm::vec2(0.0f, Spacing / 2), glm::vec2(3 * Size, 20));
		//Moves
		for (int i = 0; i < 9; i++) {
			//O
			if (CB[i] == 2) {
				switch (i)
				{
				case 0:
					glBindTexture(GL_TEXTURE_2D, OTexture);
					DrawQuad(glm::vec2(-Spacing, Spacing), glm::vec2(Size));
					break;
				case 1:
					glBindTexture(GL_TEXTURE_2D, OTexture);
					DrawQuad(glm::vec2(0.0f, Spacing), glm::vec2(Size));
					break;
				case 2:
					glBindTexture(GL_TEXTURE_2D, OTexture);
					DrawQuad(glm::vec2(Spacing, Spacing), glm::vec2(Size));
					break;
				case 3:
					glBindTexture(GL_TEXTURE_2D, OTexture);
					DrawQuad(glm::vec2(-Spacing, 0.0f), glm::vec2(Size));
					break;
				case 4:
					glBindTexture(GL_TEXTURE_2D, OTexture);
					DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(Size));
					break;
				case 5:
					glBindTexture(GL_TEXTURE_2D, OTexture);
					DrawQuad(glm::vec2(Spacing, 0.0f), glm::vec2(Size));
					break;
				case 6:
					glBindTexture(GL_TEXTURE_2D, OTexture);
					DrawQuad(glm::vec2(-Spacing, -Spacing), glm::vec2(Size));
					break;
				case 7:
					glBindTexture(GL_TEXTURE_2D, OTexture);
					DrawQuad(glm::vec2(0.0f, -Spacing), glm::vec2(Size));
					break;
				case 8:
					glBindTexture(GL_TEXTURE_2D, OTexture);
					DrawQuad(glm::vec2(Spacing, -Spacing), glm::vec2(Size));
					break;
				default:
					break;
				}
			}
			//X
			if (CB[i] == 1) {
				switch (i)
				{
				case 0:
					glBindTexture(GL_TEXTURE_2D, XTexture);
					DrawQuad(glm::vec2(-Spacing, Spacing), glm::vec2(Size));
					break;
				case 1:
					glBindTexture(GL_TEXTURE_2D, XTexture);
					DrawQuad(glm::vec2(0.0f, Spacing), glm::vec2(Size));
					break;
				case 2:
					glBindTexture(GL_TEXTURE_2D, XTexture);
					DrawQuad(glm::vec2(Spacing, Spacing), glm::vec2(Size));
					break;
				case 3:
					glBindTexture(GL_TEXTURE_2D, XTexture);
					DrawQuad(glm::vec2(-Spacing, 0.0f), glm::vec2(Size));
					break;
				case 4:
					glBindTexture(GL_TEXTURE_2D, XTexture);
					DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(Size));
					break;
				case 5:
					glBindTexture(GL_TEXTURE_2D, XTexture);
					DrawQuad(glm::vec2(Spacing, 0.0f), glm::vec2(Size));
					break;
				case 6:
					glBindTexture(GL_TEXTURE_2D, XTexture);
					DrawQuad(glm::vec2(-Spacing, -Spacing), glm::vec2(Size));
					break;
				case 7:
					glBindTexture(GL_TEXTURE_2D, XTexture);
					DrawQuad(glm::vec2(0.0f, -Spacing), glm::vec2(Size));
					break;
				case 8:
					glBindTexture(GL_TEXTURE_2D, XTexture);
					DrawQuad(glm::vec2(Spacing, -Spacing), glm::vec2(Size));
					break;
				default:
					break;
				}
			}
		}

		int WhoWon = Game.Boards[Game.CurrentBoard].Won;
		if (WhoWon > 0) {
			//
			if (Where == -1) {
				for (int i = 0; i < 8; i++) {
					if (CB[winStates[i][0]] == WhoWon && CB[winStates[i][1]] == WhoWon && CB[winStates[i][2]] == WhoWon) { Where = i; break; }
				}
			}

			switch (Where) {
				//Row
			case 0:
				glBindTexture(GL_TEXTURE_2D, RTexture);
				DrawQuad(glm::vec2(0.0f, Spacing), glm::vec2(3 * Size, 40)); 
				break;
			case 1:
				glBindTexture(GL_TEXTURE_2D, RTexture);
				DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(3 * Size, 40));
				break;
			case 2:
				glBindTexture(GL_TEXTURE_2D, RTexture);
				DrawQuad(glm::vec2(0.0f, -Spacing), glm::vec2(3 * Size, 40));
				break;
				//Column
			case 3:
				glBindTexture(GL_TEXTURE_2D, RTexture);
				DrawQuad(glm::vec2(-Spacing, 0.0f), glm::vec2(40, 3 * Size));
				break;
			case 4:
				glBindTexture(GL_TEXTURE_2D, RTexture);
				DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(40, 3 * Size));
				break;
			case 5:
				glBindTexture(GL_TEXTURE_2D, RTexture);
				DrawQuad(glm::vec2(Spacing, 0.0f), glm::vec2(40, 3 * Size));
				break;
				//Diagonal
			case 6:
				glBindTexture(GL_TEXTURE_2D, RTexture);
				DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(50, 4 * Size), 0.785398f);
				break;
			case 7:
				glBindTexture(GL_TEXTURE_2D, RTexture);
				DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(50, 4 * Size), -0.785398f);
				break;
			default: break;
			}
		}

		
	}
	glUseProgram(GL_NONE);
}

void GUI(){
	ImGui::Begin("Settings", 0, ImVec2(300, 300), 0.4f);
	{
		//If the boards haven't been generated.
		if (!generated) {
			//Generate all boards.
			if (ImGui::Button("GenerateAllBoards")) {
				std::cout << "Time it took to generate[ " << funcTime(TimeGenerateBoards) << "]" << std::endl;
				generated = true;
			}
		}
		//If the boards have been generated.
		if (generated) {
			//Switch if playing against AI.
			ImGui::Checkbox("Playing AI", &AI_ON);
			//Clear board.
			if (ImGui::Button("Clear")) {
				CB = { 0,0,0, 0,0,0, 0,0,0 };
				Game.CurrentBoard = 0; MC = 1;
				Where = -1;
			}
			//Pick move.
			if (ImGui::Button("0")) { PickMove(0,MC); } ImGui::SameLine();
			if (ImGui::Button("1")) { PickMove(1,MC); } ImGui::SameLine();
			if (ImGui::Button("2")) { PickMove(2,MC); }
			if (ImGui::Button("3")) { PickMove(3,MC); } ImGui::SameLine();
			if (ImGui::Button("4")) { PickMove(4,MC); } ImGui::SameLine();
			if (ImGui::Button("5")) { PickMove(5,MC); }
			if (ImGui::Button("6")) { PickMove(6,MC); } ImGui::SameLine();
			if (ImGui::Button("7")) { PickMove(7,MC); } ImGui::SameLine();
			if (ImGui::Button("8")) { PickMove(8,MC); }

			//Debug stuff
			ImGui::Text("");
			ImGui::Text("===DEBUG===");
			ImGui::Text("Input a number at or between 0 and 764,");
			ImGui::Text(" to display the board and it's minimax in the Debug menu.");
			ImGui::InputInt("board number ", &intInput);
			if (ImGui::Button("Minimax Test")) {
				if (intInput >= 0 && intInput < 765) {
					std::cout << "===Minimax===" << std::endl;
					//Get minimax Single and the duration it took.
					std::cout << "Function minimaxS" << std::endl;
					int minimaxS = funcTimeS(TimeMinimaxS, intInput, -1000, 1000, true);
					std::cout << "Worth: [" << minimaxS << "]" << std::endl << std::endl;
					//Get minimax Double and the duration it took.
					std::cout << "Function minimaxD" << std::endl;
					std::pair<int, int> minimaxD = funcTimeD(TimeMinimaxD, intInput, -1000, 1000, true);
					std::cout << "Worth: [" << minimaxD.first << "], Board Position: [" << minimaxD.second << "]" << std::endl << std::endl;
					//Display the boards.
					for (unsigned short int i = 0; i < 3; i++) {
						for (unsigned short int j = 0; j < 3; j++) {
							std::cout << Game.Boards[intInput].state[(i * 3) + j] << " ";
						}
						std::cout << "|| ";
						for (unsigned short int j = 0; j < 3; j++) {
							std::cout << Game.Boards[minimaxD.second].state[(i * 3) + j] << " ";
						}
						std::cout << std::endl;
					}
					std::cout << "=============" << std::endl;
				}
			}
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
