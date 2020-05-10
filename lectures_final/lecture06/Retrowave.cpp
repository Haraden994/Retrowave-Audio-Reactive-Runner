/*
OpenGL coordinate system (right-handed)
positive X axis points right
positive Y axis points up
positive Z axis points "outside" the screen


                              Y
                              |
                              |
                              |________X
                             /
                            /
                           /
                          Z
*/

// Std. Includes
#include <string>
#include <random>

// Loader for OpenGL extensions
// http://glad.dav1d.de/
// THIS IS OPTIONAL AND NOT REQUIRED, ONLY USE THIS IF YOU DON'T WANT GLAD TO INCLUDE windows.h
// GLAD will include windows.h for APIENTRY if it was not previously defined.
// Make sure you have the correct definition for APIENTRY for platforms which define _WIN32 but don't use __stdcall
#ifdef _WIN32
    #define APIENTRY __stdcall
#endif

#include <glad/glad.h>

// GLFW library to create window and to manage I/O
#include <glfw/glfw3.h>

// Audio libraries
// irrKlang for music reproduction
#include <irrKlang/irrKlang.h>
// Aubio for music processing
#include <aubio.h>

#include <imgui/imgui.h>
#include "imgui/examples/imgui_impl_glfw.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include "ImGuiFileDialog.h"

// another check related to OpenGL loader
// confirm that GLAD didn't include windows.h
#ifdef _WINDOWS_
    #error windows.h was included!
#endif

// classes developed during lab lectures to manage shaders and to load models
#include <utils/shader_v1.h>
#include <utils/model_v2.h>
#include <utils/camera.h>

// we load the GLM classes used in the application
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

// we include the library for images loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

struct PowerUp{
	glm::vec3 position;
	GLfloat radius;
	bool speedUp;
	bool hit;
	GLint explodeValue;
	GLfloat explosionStartTime;
};

struct Car{
	glm::vec3 position;
	glm::vec3 size;
};

// dimensions of application's window
GLuint screenWidth = 1920, screenHeight = 1080;

// callback functions for keyboard and mouse events
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// if one of the WASD keys is pressed, we call the corresponding method of the Camera class
void apply_camera_movements();

// setup of Shader Programs for the 5 shaders used in the application
void SetupShaders();
// delete Shader Programs whan application ends
void DeleteShaders();
// print on console the name of current shader
void PrintCurrentShader(int shader);
// load the 6 images from disk and create an OpenGL cubemap
GLint LoadTextureCube(string path);

// draw the GUI through ImGui
void DrawGUI();

// Aubio reset for errors or music change
void AubioReset(bool fftcheck);
// Setup aubio for spectrum analysis using FFT
void FFTInitialize(string musicPath);
// Compute and extract Fast Fourier Transform
void FFTCompute(GLfloat deltaTime);
// Merge the win_s/2 frequency bands into 8 frequency bands
void MergeFrequencyBands();
// Frequency bands normalization leading to a better manipulation inside the vertex shader
void FrequencyBandsNormalize();
// Through the use of a buffer we can avoid the grid's flickering
void CreateBandsBuffer();
// Stop all the current audio reproductions and start a new one
void PlayMusic(string musicPath);
// Collision check AABB - Sphere
bool CheckCollision(PowerUp pwUp, Car car);

// we initialize an array of booleans for each keybord key
bool keys[1024];
// we set the initial position of mouse cursor in the application window
GLfloat lastX = 400, lastY = 300;
// when rendering the first frame, we do not have a "previous state" for the mouse, so we need to manage this situation
bool firstMouse = true;

// we create a camera. We pass the initial position as a paramenter to the constructor. The last boolean tells that we want a camera "anchored" to the ground
Camera camera(glm::vec3(0.0f, 0.0f, 7.0f), GL_FALSE);

// parameters for time calculation
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat musicStartTime = 0.0f;
int remainingFrames = 0;

// boolean to activate/deactivate wireframe rendering
GLboolean wireframe = GL_FALSE;
GLboolean mouseEnabled = GL_FALSE;

// a vector for all the Shader Programs used in the application
vector<Shader> shaders;

// Uniforms to be passed to shaders
GLfloat sunAnimationSpeed = 2.0f;
GLfloat sunSize = 7.0f;
GLfloat gridScrollSpeed = 40.0f;
GLfloat gridSize = 0.1f;
GLfloat gridNoiseZoom = 10.0f;
GLfloat gridDisplacementPower = 40.0f;
GLfloat streetSize = 0.1f;
GLfloat fadeAfterStreet = 0.1f;
GLfloat palmOutline[] = {0.0f, 1.0, 1.0f};
// uniforms for light calculations
GLfloat diffuseColor[] = {1.0f, 0.17, 0.6};
GLfloat specularColor[] = {0.0f, 1.0f, 1.0f};
GLfloat ambientColor[] = {1.0f, 0.0f, 1.0f};
// diffuse, specular and ambient weights for Blinn-Phong lighting shader
GLfloat diffuse = 1.0f;
GLfloat specular = 0.6f;
GLfloat ambient = 0.3f;
// attenuation parameters
GLfloat constant = 1.0f;
GLfloat linear = 0.04f;
GLfloat quadratic = 0.00f;
// shininess coefficient
GLfloat shininess = 25.0f;

// Variables
bool once = true;
string musicPath = "../../../Music/SneakyDriver_KatanaZeroOST.wav";
GLfloat bufferDecreaseAmount = 0.00005f;
GLfloat sunPosition[] = {0.0f, 5.0f, -25.0f};
glm::vec3 lightPosition = glm::vec3(sunPosition[0], sunPosition[1], sunPosition[2]);
GLfloat carXPos = 0.0f;
GLfloat carTurnAngle = 0.0f;
GLfloat streetBorder = (streetSize*100.0f) / 2.0f;

// texture unit for the cube map
GLuint textureCube;

// vector to store the frequency bands extracted by FFTCompute
vector<float> frequencyBands;
// vector used as Buffer in order to smooth the descending vertex displacement. We avoid the unnecessary flicker of the audio reactive grid.
vector<float> bandsBuffer;
vector<float> bufferDecrease;
// Initialization of irrKlang for audio reproduction
irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();

/////////////////// MAIN function ///////////////////////
int main()
{
	srand(glfwGetTime());
	
	if (!soundEngine)
	{
		std::cout << "Could not startup the audio engine." << std::endl;
		return 0; // error starting up the engine
	}
		
    // Initialization of OpenGL context using GLFW
    glfwInit();
    // We set OpenGL specifications required for this application
    // In this case: 3.3 Core
    // It is possible to raise the values, in order to use functionalities of OpenGL 4.x
    // If not supported by your graphics HW, the context will not be created and the application will close
    // N.B.) creating GLAD code to load extensions, try to take into account the specifications and any extensions you want to use,
    // in relation also to the values indicated in these GLFW commands
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // we set if the window is resizable
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// Antialiasing samples
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	 
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	// we create the application's window
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Retrowave", monitor, NULL);
    //GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Retrowave", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // we put in relation the window and the callbacks
    glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

    // we disable the mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLAD tries to load the context set by GLFW
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // we define the viewport dimensions
    GLint width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // we enable Z test
    glEnable(GL_DEPTH_TEST);
	// MSAA Enable
	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //the "clear" color for the frame buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	ImGui::StyleColorsDark();
	
	ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

	Shader grid_shader("FFTDisplacement.vert", "neonGrid.frag");
	shaders.push_back(grid_shader);
	Shader sun_shader("retrosun.vert", "retrosunSphere.frag");
	shaders.push_back(sun_shader);
	Shader skybox_shader("20_skybox.vert", "20_skybox.frag");
	shaders.push_back(skybox_shader);
	Shader qSun_shader("retrosun.vert", "retrosunQuad.frag");
	shaders.push_back(qSun_shader);
	Shader palm_shader("palm.vert", "palm.frag");
	shaders.push_back(palm_shader);
	Shader full_color("palmOutline.vert", "palmOutline.frag");
	shaders.push_back(full_color);
	Shader car_shader("13_phong.vert", "14_ggx.frag");
	shaders.push_back(car_shader);
	Shader pwUp_shader("powerUp.vert", "../powerUp.geom", "palmOutline.frag");
	shaders.push_back(pwUp_shader);
	
	// we load the cube map (we pass the path to the folder containing the 6 views)
    textureCube = LoadTextureCube("../../../textures/cube/Purple/");
	
    // we load the model(s) (code of Model class is in include/utils/model_v1.h)
    Model sphereModel("../../../models/sphere.obj");
	Model skyboxModel("../../../models/flippedCube.obj");
	Model gridModel("../../../models/grid500m100x100.obj");
	Model quadModel("../../../models/myPlane.obj");
	Model palmModel("../../../models/palm.obj");
	Model carModel("../../../models/Countach.obj");

    // we set projection and view matrices
    // N.B.) in this case, the camera is fixed -> we set it up outside the rendering loop
    // Projection matrix: FOV angle, aspect ratio, near and far planes
    glm::mat4 projection = glm::perspective(45.0f, (float)screenWidth/(float)screenHeight, 0.1f, 10000.0f);
    // View matrix (=camera): position, view direction, camera "up" vector
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 7.0f), glm::vec3(0.0f, 0.0f, -7.0f), glm::vec3(0.0f, 1.0f, 7.0f));
	
	// start music reproduction and processing
	musicStartTime = glfwGetTime();
	FFTInitialize(musicPath);
	soundEngine->play2D(musicPath.c_str(), true);
	
	// Palms parameters initialization
	GLfloat palmStartingZ = -75.0f;
	GLuint palmAmount = 40;
	GLfloat zOffset = 100.0f / (float)palmAmount;
	GLfloat palmZPositions[palmAmount];
	for(int i = 0; i < palmAmount; i++){
		palmZPositions[i] = palmStartingZ + zOffset * i;
		palmZPositions[i+1] = palmStartingZ + zOffset * i;
		i++;
	}
	
	// Powerup parameters
	GLuint pwAmount = 10;
	GLint maxZ = 200;
	GLint minZ = 75;
	PowerUp powerUps[pwAmount];
	GLint respawnThreshold;
	GLfloat sphereScale = 0.3f;
	for(int i = 0; i < pwAmount; i++){
		powerUps[i].position = glm::vec3(0.0f, 0.0f, 29.0f);
		powerUps[i].radius = sphereScale;
		if(i % 2 == 0)
			powerUps[i].speedUp = true;
		else
			powerUps[i].speedUp = false;
		powerUps[i].explodeValue = 0;
		powerUps[i].hit = false;
		powerUps[i].explosionStartTime = 0.0f;
	}
	
	Car countach;
	GLfloat carScale = 0.6f;
	countach.position = glm::vec3(0.0f, 0.0f, 24.0f);
	countach.size = glm::vec3(1.4f * carScale, 1.2f * carScale, 4.3f * carScale);
	
	// Rendering loop: this code is executed at each frame
    while(!glfwWindowShouldClose(window))
    {
        // we determine the time passed from the beginning
        // and we calculate time difference between current frame rendering and the previous one
        GLfloat currentFrame = glfwGetTime() - musicStartTime;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		FFTCompute(deltaTime);
		
		// Draw the GUI through ImGui
		DrawGUI();

        // Check is an I/O event is happening
        glfwPollEvents();
		// we apply FPS camera movements
        apply_camera_movements();
		// View matrix (=camera): position, view direction, camera "up" vector
        glm::mat4 view = camera.GetViewMatrix();
		
		// we "clear" frame, z and stencil buffers
		glStencilMask(~0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // we set the rendering mode
        if (wireframe)
            // Draw in wireframe
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		// we activate the cube map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureCube);
		
		// We disable the stencil writing, we just need it for the palms' neon effect
		glStencilMask(0x00);
		
		///////////////////// NEONGRID /////////////////////
		grid_shader.Use();
		
		glUniformMatrix4fv(glGetUniformLocation(grid_shader.Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(grid_shader.Program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
		
		// animation and music uniforms
		glUniform1fv(glGetUniformLocation(grid_shader.Program, "frequencyBands"), bandsBuffer.size(), &bandsBuffer[0]);
		glUniform1f(glGetUniformLocation(grid_shader.Program, "time"), glfwGetTime());
		glUniform1f(glGetUniformLocation(grid_shader.Program, "scrollSpeed"), gridScrollSpeed);
		glUniform1f(glGetUniformLocation(grid_shader.Program, "zoom"), gridNoiseZoom);
		glUniform1f(glGetUniformLocation(grid_shader.Program, "dPower"), gridDisplacementPower);
		glUniform1f(glGetUniformLocation(grid_shader.Program, "streetSize"), streetSize);
		glUniform1f(glGetUniformLocation(grid_shader.Program, "fade"), fadeAfterStreet);
		// lighting uniforms
		glUniform3fv(glGetUniformLocation(grid_shader.Program, "pointLightPosition"), 1, glm::value_ptr(lightPosition));
		glUniform3fv(glGetUniformLocation(grid_shader.Program, "diffuseColor"), 1, diffuseColor);
		glUniform3fv(glGetUniformLocation(grid_shader.Program, "specularColor"), 1, specularColor);
		glUniform3fv(glGetUniformLocation(grid_shader.Program, "ambientColor"), 1, ambientColor);
		glUniform1f(glGetUniformLocation(grid_shader.Program, "Kd"), diffuse);
		glUniform1f(glGetUniformLocation(grid_shader.Program, "Ks"), specular);
		glUniform1f(glGetUniformLocation(grid_shader.Program, "Ka"), ambient);
		glUniform1f(glGetUniformLocation(grid_shader.Program, "constant"), constant);
		glUniform1f(glGetUniformLocation(grid_shader.Program, "linear"), linear);
		glUniform1f(glGetUniformLocation(grid_shader.Program, "quadratic"), quadratic);
		glUniform1f(glGetUniformLocation(grid_shader.Program, "shininess"), shininess);
		
		glm::mat4 gridModelMatrix;
		glm::mat3 gridNormalMatrix;
		gridModelMatrix = glm::translate(gridModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
		gridModelMatrix = glm::scale(gridModelMatrix, glm::vec3(gridSize, 1.0f, gridSize));
		// not considering translations on normal matrix, useful for lighting calculations
		gridNormalMatrix = glm::inverseTranspose(glm::mat3(view * gridModelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(grid_shader.Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(gridModelMatrix));
		glUniformMatrix3fv(glGetUniformLocation(grid_shader.Program, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(gridNormalMatrix));
		
		gridModel.Draw(grid_shader);
		
		gridModelMatrix = glm::translate(gridModelMatrix, glm::vec3(0.0f, 0.0f, -490.0f));
		glUniformMatrix4fv(glGetUniformLocation(grid_shader.Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(gridModelMatrix));
		
		gridModel.Draw(grid_shader);
		
		/////////////////// PALM ///////////////////////////////////
		glm::mat4* modelMatrices;
		glm::mat3* normalMatrices;
		modelMatrices = new glm::mat4[palmAmount];
		normalMatrices = new glm::mat3[palmAmount];
		streetBorder = (streetSize*100.0f) / 2.0f; // x position is streetSize depending
		GLfloat palmTranslationSpeed = gridScrollSpeed * 0.505f;
		for(int i = 0; i < palmAmount; i++){
			glm::mat4 rightModelMatrix = glm::mat4(1.0f);
			glm::mat4 leftModelMatrix = glm::mat4(1.0f);
			palmZPositions[i] += palmTranslationSpeed * deltaTime;
			if(palmZPositions[i] > 25.0f)
				palmZPositions[i] = palmStartingZ;
			rightModelMatrix = glm::translate(rightModelMatrix, glm::vec3(-streetBorder, -0.5f, palmZPositions[i]));
			leftModelMatrix = glm::translate(leftModelMatrix, glm::vec3(streetBorder, -0.5f, palmZPositions[i]));
			leftModelMatrix = glm::rotate(leftModelMatrix, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			rightModelMatrix = glm::scale(rightModelMatrix, glm::vec3(0.15f));
			leftModelMatrix = glm::scale(leftModelMatrix, glm::vec3(0.15f));
			modelMatrices[i] = rightModelMatrix;
			normalMatrices[i] = glm::inverseTranspose(glm::mat3(view * modelMatrices[i]));
			modelMatrices[i+1] = leftModelMatrix;
			normalMatrices[i+1] = glm::inverseTranspose(glm::mat3(view * modelMatrices[i+1]));
			i++;
		}
		/*
		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, palmAmount * sizeof(glm::mat4), &modelMatrices[0], GL_DYNAMIC_DRAW); // No difference in terms of FPS.....
		
		for(unsigned int i = 0; i < palmModel.meshes.size(); i++){
			unsigned int VAO = palmModel.meshes[i].VAO;
			glBindVertexArray(VAO);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
			
			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}
		*/
		for(int i = 0; i < palmAmount; i++){
			
			palm_shader.Use();
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			
			glUniformMatrix4fv(glGetUniformLocation(palm_shader.Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(glGetUniformLocation(palm_shader.Program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
			
			// lighting uniforms
			glUniform3fv(glGetUniformLocation(palm_shader.Program, "pointLightPosition"), 1, glm::value_ptr(lightPosition));
			glUniform3fv(glGetUniformLocation(palm_shader.Program, "diffuseColor"), 1, diffuseColor);
			glUniform3fv(glGetUniformLocation(palm_shader.Program, "specularColor"), 1, specularColor);
			glUniform3fv(glGetUniformLocation(palm_shader.Program, "ambientColor"), 1, ambientColor);
			glUniform1f(glGetUniformLocation(palm_shader.Program, "Kd"), 0.0f);
			glUniform1f(glGetUniformLocation(palm_shader.Program, "Ks"), 1.0f);
			glUniform1f(glGetUniformLocation(palm_shader.Program, "Ka"), 0.0f);
			glUniform1f(glGetUniformLocation(palm_shader.Program, "constant"), constant);
			glUniform1f(glGetUniformLocation(palm_shader.Program, "linear"), linear);
			glUniform1f(glGetUniformLocation(palm_shader.Program, "quadratic"), quadratic);
			glUniform1f(glGetUniformLocation(palm_shader.Program, "shininess"), shininess);
			
			glUniformMatrix4fv(glGetUniformLocation(palm_shader.Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
			glUniformMatrix3fv(glGetUniformLocation(palm_shader.Program, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrices[i]));
			palmModel.Draw(palm_shader);
			
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			
			full_color.Use();
			
			glUniformMatrix4fv(glGetUniformLocation(full_color.Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(glGetUniformLocation(full_color.Program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
			glUniform3fv(glGetUniformLocation(full_color.Program, "color"), 1, palmOutline);
			
			glm::mat4 palmModelMatrix = modelMatrices[i];
			palmModelMatrix = glm::translate(palmModelMatrix, glm::vec3(0.0f, -2.0f, 0.0f));
			palmModelMatrix = glm::scale(palmModelMatrix, glm::vec3(1.1f));
			glUniformMatrix4fv(glGetUniformLocation(full_color.Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(palmModelMatrix));
			
			palmModel.Draw(full_color);
			
		}
		
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		/*for(unsigned int i = 0; i < palmModel.meshes.size(); i++){
			glBindVertexArray(palmModel.meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, palmModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, palmAmount);
			glBindVertexArray(0);
		}*/
		
		/////////////////// CAR /////////////////////////////////
		
		car_shader.Use();
		
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		
		glUniformMatrix4fv(glGetUniformLocation(car_shader.Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(car_shader.Program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
		
		glUniform3fv(glGetUniformLocation(car_shader.Program, "pointLightPosition"), 1, glm::value_ptr(lightPosition));
		glUniform3fv(glGetUniformLocation(car_shader.Program, "diffuseColor"), 1, diffuseColor);
		glUniform1f(glGetUniformLocation(car_shader.Program, "Kd"), diffuse);
		glUniform1f(glGetUniformLocation(car_shader.Program, "alpha"), 0.2f);
		glUniform1f(glGetUniformLocation(car_shader.Program, "F0"), 0.9f);
		
		glm::mat4 carModelMatrix;
		glm::mat3 carNormalMatrix;
		// car engine tremble
		GLfloat trembleSpeed = 100.0f;
		GLfloat trembleTranslation = 0.002f;
		GLfloat maxTurnAngle = 2.0f;
		GLfloat turnSpeed = 100.0f + gridScrollSpeed * 0.1f;
		carModelMatrix = glm::translate(carModelMatrix, glm::vec3(std::cos(glfwGetTime() * trembleSpeed) * trembleTranslation, std::sin(glfwGetTime() * trembleSpeed) * trembleTranslation, countach.position.z));
		if(keys[GLFW_KEY_A] && countach.position.x >= (-streetBorder + 1.0f)){
			countach.position.x -= deltaTime * 2.0f;
			if(carTurnAngle <= maxTurnAngle)
				carTurnAngle += deltaTime * turnSpeed;
		}
		if(keys[GLFW_KEY_D] && countach.position.x <= (streetBorder - 1.0f)){
			countach.position.x += deltaTime * 2.0f;
			if(carTurnAngle >= -maxTurnAngle)
				carTurnAngle -= deltaTime * turnSpeed;
		}
		if(!keys[GLFW_KEY_A] && !keys[GLFW_KEY_D]){
			if(carTurnAngle != 0.0f){
				if(carTurnAngle >= 0.0f)
					carTurnAngle -= deltaTime * turnSpeed;
				else if(carTurnAngle <= 0.0f)
					carTurnAngle += deltaTime * turnSpeed;
			}
		}
		carModelMatrix = glm::translate(carModelMatrix, glm::vec3(countach.position.x, 0.0f, 0.0f));
		carModelMatrix = glm::rotate(carModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		carModelMatrix = glm::rotate(carModelMatrix, glm::radians(carTurnAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        carModelMatrix = glm::scale(carModelMatrix, glm::vec3(carScale));
		carNormalMatrix = glm::inverseTranspose(glm::mat3(view * carModelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(car_shader.Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(carModelMatrix));
        glUniformMatrix3fv(glGetUniformLocation(car_shader.Program, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(carNormalMatrix));
		
		carModel.Draw(car_shader);
		
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		
		full_color.Use();
		
		glUniformMatrix4fv(glGetUniformLocation(full_color.Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(full_color.Program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
		
		glUniform3fv(glGetUniformLocation(full_color.Program, "color"), 1, palmOutline);
		
		glm::mat4 carOutlineModelMatrix = carModelMatrix;
		carOutlineModelMatrix = glm::translate(carOutlineModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		carOutlineModelMatrix = glm::scale(carOutlineModelMatrix, glm::vec3(1.1f));
		glUniformMatrix4fv(glGetUniformLocation(full_color.Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(carOutlineModelMatrix));
		
		carModel.Draw(full_color);
		
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
	
		/////////////////// POWERUPS ///////////////////////////////
		pwUp_shader.Use();
		
		glUniformMatrix4fv(glGetUniformLocation(pwUp_shader.Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(pwUp_shader.Program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
		
		modelMatrices = new glm::mat4[pwAmount];
		
		for(int i = 0; i < pwAmount; i++){
			if(powerUps[i].speedUp)
				glUniform3f(glGetUniformLocation(pwUp_shader.Program, "color"), 0.0f, 1.0f, 1.0f);
			else
				glUniform3f(glGetUniformLocation(pwUp_shader.Program, "color"), 1.0f, 0.0f, 0.0f);
				
			glUniform1f(glGetUniformLocation(pwUp_shader.Program, "time"), glfwGetTime() - powerUps[i].explosionStartTime);
			glUniform1i(glGetUniformLocation(pwUp_shader.Program, "explodeValue"), powerUps[i].explodeValue);
			
			if(once){
				powerUps[i].position.x = (GLfloat)(rand()%((GLint)streetBorder+(GLint)streetBorder + 1) - (GLint)streetBorder);
				respawnThreshold = rand() % (maxZ - minZ + 1) + minZ;
			}
			powerUps[i].position.z += palmTranslationSpeed * deltaTime;
			if(powerUps[i].position.z > (GLfloat)respawnThreshold){
				powerUps[i].hit = false;
				powerUps[i].explodeValue = 0;
				powerUps[i].position.z = (GLfloat)(-(rand()%(maxZ-minZ + 1) + minZ));
				powerUps[i].position.x = (GLfloat)(rand()%(((GLint)streetBorder - 1) + ((GLint)streetBorder - 1) + 1) - ((GLint)streetBorder - 1));
				respawnThreshold = rand() % (maxZ - (minZ - 45) + 1) + (minZ - 45);
			}
			modelMatrices[i] = glm::translate(modelMatrices[i], powerUps[i].position);
			modelMatrices[i] = glm::scale(modelMatrices[i], glm::vec3(sphereScale));
			glUniformMatrix4fv(glGetUniformLocation(pwUp_shader.Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
			sphereModel.Draw(pwUp_shader);
			// Collision check for each powerup
			if(!powerUps[i].hit && CheckCollision(powerUps[i], countach)){
				powerUps[i].hit = true;
				if(powerUps[i].speedUp)
					gridScrollSpeed += gridScrollSpeed * 0.05f;
				else
					gridScrollSpeed -= gridScrollSpeed * 0.1f;
				powerUps[i].explodeValue = 1;
				powerUps[i].explosionStartTime = glfwGetTime();
			}

		}
		once = false;
		
        /////////////////// SKYBOX ////////////////////////////////////////////////
        glDepthFunc(GL_LEQUAL);
        skybox_shader.Use();
        // we activate the cube map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureCube);
         // we pass projection and view matrices to the Shader Program of the skybox
        glUniformMatrix4fv(glGetUniformLocation(skybox_shader.Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
        // to have the background fixed during camera movements, we have to remove the translations from the view matrix
        // thus, we consider only the top-left submatrix, and we create a new 4x4 matrix
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));    // Remove any translation component of the view matrix
        glUniformMatrix4fv(glGetUniformLocation(skybox_shader.Program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));

        // we determine the position in the Shader Program of the uniform variables
        GLint textureLocation = glGetUniformLocation(skybox_shader.Program, "tCube");
        // we assign the value to the uniform variable
        glUniform1i(textureLocation, 0);

        // we render the cube with the environment map
        skyboxModel.Draw(skybox_shader);
        // we set again the depth test to the default operation for the next frame
        glDepthFunc(GL_LESS);
		
		// Transparent objects are rendered after all opaque ones
		
		/////////// QUAD SUN ///////////////
		view = camera.GetViewMatrix();
		qSun_shader.Use();
		
		// uniforms are passed to the corresponding shader
		glUniform1f(glGetUniformLocation(qSun_shader.Program, "u_time"), glfwGetTime() * sunAnimationSpeed);
		
		// we pass projection and view matrices to the Shader Program
        glUniformMatrix4fv(glGetUniformLocation(qSun_shader.Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(qSun_shader.Program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
		
		glm::mat4 quadModelMatrix;
		
		quadModelMatrix = glm::translate(quadModelMatrix, glm::vec3(sunPosition[0], sunPosition[1], sunPosition[2]));
		quadModelMatrix = glm::rotate(quadModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        quadModelMatrix = glm::scale(quadModelMatrix, glm::vec3(sunSize, 1.0f, sunSize));
        glUniformMatrix4fv(glGetUniformLocation(qSun_shader.Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(quadModelMatrix));
		
		quadModel.Draw(qSun_shader);
		
		ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // Swapping back and front buffers
        glfwSwapBuffers(window);
    }

    // when I exit from the graphics loop, it is because the application is closing
    // we delete the Shader Programs
    DeleteShaders();
	
	AubioReset(true);
	// Delete irrKlang sound engine
	soundEngine->drop();
	
	// ImGui Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	
    // we close and delete the created context
    glfwTerminate();
    return 0;
}

//////////////////////////////////////////
// we delete all the Shaders Programs
void DeleteShaders()
{
    for(GLuint i = 0; i < shaders.size(); i++)
        shaders[i].Delete();
}

//////////////////////////////////////////
// we load the 6 images from disk and we create an OpenGL cube map
GLint LoadTextureCube(string path)
{
    GLuint textureImage;
    int w, h;
    unsigned char* image;
    string fullname;

    glGenTextures(1, &textureImage);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureImage);

    // we use as convention that the names of the 6 images are "posx, negx, posy, negy, posz, negz", placed at the path passed as parameter
    //POSX
    fullname = path + std::string("posx.jpg");
    image = stbi_load(fullname.c_str(), &w, &h, 0, STBI_rgb);
    if (image == nullptr)
        std::cout << "Failed to load texture!" << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // we free the memory once we have created an OpenGL texture
    stbi_image_free(image);
    //NEGX
    fullname = path + std::string("negx.jpg");
    image = stbi_load(fullname.c_str(), &w, &h, 0, STBI_rgb);
    if (image == nullptr)
        std::cout << "Failed to load texture!" << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // we free the memory once we have created an OpenGL texture
    stbi_image_free(image);
    //POSY
    fullname = path + std::string("posy.jpg");
    image = stbi_load(fullname.c_str(), &w, &h, 0, STBI_rgb);
    if (image == nullptr)
        std::cout << "Failed to load texture!" << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // we free the memory once we have created an OpenGL texture
    stbi_image_free(image);
    //NEGY
    fullname = path + std::string("negy.jpg");
    image = stbi_load(fullname.c_str(), &w, &h, 0, STBI_rgb);
    if (image == nullptr)
        std::cout << "Failed to load texture!" << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // we free the memory once we have created an OpenGL texture
    stbi_image_free(image);
    //POSZ
    fullname = path + std::string("posz.jpg");
    image = stbi_load(fullname.c_str(), &w, &h, 0, STBI_rgb);
    if (image == nullptr)
        std::cout << "Failed to load texture!" << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // we free the memory once we have created an OpenGL texture
    stbi_image_free(image);
    //NEGZ
    fullname = path + std::string("negz.jpg");
    image = stbi_load(fullname.c_str(), &w, &h, 0, STBI_rgb);
    if (image == nullptr)
        std::cout << "Failed to load texture!" << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // we free the memory once we have created an OpenGL texture
    stbi_image_free(image);

    // we set the filtering for minification and magnification
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // we set how to consider the texture coordinates outside [0,1] range
    // in this case we have a cube map, so
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureImage;

}

//////////////////////////////////////////
// callback for keyboard events
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // if ESC is pressed, we close the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // if L is pressed, we activate/deactivate wireframe rendering of models
    if(key == GLFW_KEY_L && action == GLFW_PRESS)
        wireframe=!wireframe;
		
	if(key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS){
		mouseEnabled=!mouseEnabled;
		if(mouseEnabled)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	// we keep trace of the pressed keys
    // with this method, we can manage 2 keys pressed at the same time:
    // many I/O managers often consider only 1 key pressed at the time (the first pressed, until it is released)
    // using a boolean array, we can then check and manage all the keys pressed at the same time
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

//////////////////////////////////////////
// If one of the WASD keys is pressed, the camera is moved accordingly (the code is in utils/camera.h)
void apply_camera_movements()
{
	float speed = 5.0;
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime * speed);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime * speed);
    /*if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime * speed);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime * speed);*/
}

//////////////////////////////////////////
// callback for mouse events
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
      // we move the camera view following the mouse cursor
      // we calculate the offset of the mouse cursor from the position in the last frame
      // when rendering the first frame, we do not have a "previous state" for the mouse, so we set the previous state equal to the initial values (thus, the offset will be = 0)
      if(firstMouse)
      {
          lastX = xpos;
          lastY = ypos;
          firstMouse = false;
      }

      // offset of mouse cursor position
      GLfloat xoffset = xpos - lastX;
      GLfloat yoffset = lastY - ypos;

      // the new position will be the previous one for the next frame
      lastX = xpos;
      lastY = ypos;

      // we pass the offset to the Camera class instance in order to update the rendering
	  if(!mouseEnabled)
		camera.ProcessMouseMovement(xoffset, yoffset);

}

// Aubio Parameters
aubio_source_t* aubioSource; // Aubio source, used to read a media file
uint_t win_s = 1024; // window size
uint_t hop_s = win_s / 4; // hop size
aubio_fft_t* fft; // Fast Fourier Transform struct
uint_t samplerate = 0;
fvec_t* fftin; // Input signal for FFT
cvec_t* fftout; // FFT's spectrum output
float samplesPerWindow;

void DrawGUI()
{
	static bool fileDialog = false;
	static bool showAubioUI = false;
	static string fileName = "";
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	ImGui::Begin("GeneralUI");
	ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "Press ALT to enable/disable cursor.");
	ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "WASD for movement.");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	if(fileName.size() > 0)
		ImGui::Text("Current Music: %s", fileName.c_str());
	else
		ImGui::Text("Current Music: SneakyDriver_KatanaZeroOST.wav");

	if(ImGui::Button("Restart Music")){
		AubioReset(true);
		lastFrame = 0;
		remainingFrames = 0;
		musicStartTime = glfwGetTime();
		FFTInitialize(musicPath);
		PlayMusic(musicPath);
	}
	ImGui::SameLine();
	
	if(ImGui::Button("Change Music"))
		fileDialog = true;
		
	if(fileDialog){
		if(ImGuiFileDialog::Instance()->FileDialog("Choose Music File", "", "../../../Music", "")){
			if(ImGuiFileDialog::Instance()->IsOk == true){
				string tempFileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
				if(tempFileName != ""){
					fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
					musicPath = ImGuiFileDialog::Instance()->GetFilepathName();
					AubioReset(true);
					lastFrame = 0;
					remainingFrames = 0;
					musicStartTime = glfwGetTime();
					FFTInitialize(musicPath);
					PlayMusic(musicPath);
				}
			}
			else{
			}
			fileDialog = false;
		}
	}
	ImGui::SameLine();
	
	if(ImGui::Button("Open/Close Aubio UI")){
		if(showAubioUI)
			showAubioUI = false;
		else
			showAubioUI = true;
	}
	ImGui::SameLine();
	ImGui::End();
	
	if(showAubioUI){
		ImGui::Begin("AubioUI");
		ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), "Frequency Bands (8x)");
		for(int i = 0; i < frequencyBands.size(); i++){
			ImGui::Text("FBand %02d: %f", i, bandsBuffer[i]);
		}
		ImGui::BeginChild("Scrolling");
		ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "FFT Frequency Samples");
		for(int j = 0; j < fftout->length; j++){
			ImGui::Text("Sample %03d: %f", j, (float)fftout->norm[j]);
		}
		ImGui::EndChild();
		ImGui::End();
	}
	
	ImGui::Begin("Environment Parameters");
	ImGui::TextColored(ImVec4(1.0, 0.0, 1.0, 1.0), "Neon Grid Parameters");
	ImGui::SliderFloat("Grid Size", &gridSize, 0.1f, 1.0f);
	ImGui::InputFloat("Scroll Speed", &gridScrollSpeed, 0.5f, 1.0f);
	ImGui::InputFloat("Noise Zoom", &gridNoiseZoom, 1.0f, 5.0f);
	ImGui::SliderFloat("Displacement Power", &gridDisplacementPower, 5.0f, 60.0f);
	ImGui::InputFloat("Street Size", &streetSize, 0.01f, 0.1f, "%.3f");
	ImGui::InputFloat("Fade After Street", &fadeAfterStreet, 0.01f, 0.1f, "%.3f");
	ImGui::InputFloat("Buffer Decrease Amount", &bufferDecreaseAmount, 0.000001f, 0.0001f, "%.6f");
	ImGui::TextColored(ImVec4(1.0, 0.8, 0.0, 1.0), "Retro Sun Parameters");
	ImGui::SliderFloat("Shader Animation Speed", &sunAnimationSpeed, 0.0f, 10.0f);
	ImGui::SliderFloat3("Sun Position", sunPosition, -50.0f, 50.0f);
	lightPosition = glm::vec3(sunPosition[0], sunPosition[1], sunPosition[2]);
	ImGui::SliderFloat("Size", &sunSize, 0.5f, 10.0f);
	ImGui::Text("Sun Light Parameters");
	ImGui::ColorEdit3("Diffuse Color", (float*)diffuseColor);
	ImGui::InputFloat("Diffuse Weight", &diffuse, 0.01f, 0.1f);
	ImGui::ColorEdit3("Specular Color", (float*)specularColor);
	ImGui::InputFloat("Specular Weight", &specular, 0.01f, 0.1f);
	ImGui::ColorEdit3("Ambient Color", (float*)ambientColor);
	ImGui::InputFloat("Ambient Weight", &ambient, 0.01f, 0.1f);
	ImGui::Text("Attenuation and Shininess parameters");
	ImGui::InputFloat("Linear", &linear, 0.01f, 0.1f);
	ImGui::InputFloat("Quadratic", &quadratic, 0.01f, 0.1f);
	ImGui::SliderFloat("Shininess", &shininess, 0.0f, 50.0f);
	ImGui::End();
}

void FFTInitialize(string musicPath)
{
	aubioSource = new_aubio_source(musicPath.c_str(), 0, hop_s);
	if(!aubioSource){
		aubio_cleanup();
		std::cout << "Something bad happened to Aubio Source." << std::endl;
	}
	
	samplerate = aubio_source_get_samplerate(aubioSource);
	samplesPerWindow = (float)samplerate / (float)win_s;
	
	fftin = new_fvec(win_s);
	fftout = new_cvec(win_s);
	fft = new_aubio_fft(win_s);
	
	bandsBuffer.assign(8, 0.0);
	bufferDecrease.assign(8, 0.0);
	
	if(!fft)
		AubioReset(false);
}

void FFTCompute(GLfloat deltaTime)
{
	// Taking time and frame relationship into account in order to compute FFT in real time.
	uint_t framesRead = 0;
	int n_frames = 0;
	int passedFrames = deltaTime * (int)samplerate - remainingFrames;
	
	frequencyBands.assign(8, 0.0);
	
	while(n_frames < passedFrames){
		aubio_source_do(aubioSource, fftin, &framesRead);
		n_frames += framesRead;
		aubio_fft_do(fft, fftin, fftout);
		
		MergeFrequencyBands();
		FrequencyBandsNormalize();
		CreateBandsBuffer();
		
		if(framesRead != hop_s)
			break;
	}
	remainingFrames = n_frames - passedFrames;
	//printf("Read %d frames at %dHz (%d blocks).\n", n_frames, samplerate, n_frames / hop_s);

}

void MergeFrequencyBands()
{
	int count = 0;
	float frequency;
	
	for(int i = 0; i < 8; i++){
		float average = 0;
		int sampleCount = (int)pow(2, i) * 2;
		if(i == 7)
			sampleCount += 2;
		for(int j = 0; j < sampleCount; j++){
			average += fftout->norm[count] * (count + 1);
			count++;
		}
		
		average /= count;
		frequencyBands[i] = average;
	}
}

// Sum up all the frequency bands and divide each one by the sum in order to get the normalized frequency bands.
void FrequencyBandsNormalize()
{
	float sum = 0.0f;
	for(int i = 0; i < frequencyBands.size(); i++)
		sum += frequencyBands[i];
	for(int i = 0; i < frequencyBands.size(); i++)
		frequencyBands[i] /= sum;
}

// Smoothly descend if the next frequency band value in the buffer is lower than the actual one.
// In case the next frequency is higher, the buffer will just spike up.
void CreateBandsBuffer()
{
	for(int i = 0; i < 8; i++){
		if(frequencyBands[i] > bandsBuffer[i]){
			bandsBuffer[i] = frequencyBands[i];
			bufferDecrease[i] = abs(bufferDecreaseAmount);
		}
		if(frequencyBands[i] < bandsBuffer[i]){
			bandsBuffer[i] -= bufferDecrease[i];
			bufferDecrease[i] *= 1.2f; // Buffer decrease is used in order to rapidly descend over time if frequencies in the buffer are always lower than the current
		}
	}
}

void AubioReset(bool fftcheck)
{
	if(fftcheck)
		del_aubio_fft(fft);
	del_fvec(fftin);
	del_cvec(fftout);
	aubio_cleanup();
}

void PlayMusic(string musicPath)
{
	soundEngine->stopAllSounds();
	soundEngine->play2D(musicPath.c_str(), true);
}

bool CheckCollision(PowerUp pwUp, Car car){
	glm::vec3 aabb_half_extents(car.size.x / 2.0f, car.size.y / 2.0f, car.size.z / 2.0f);
	// Distance vector between AABB and Sphere centers
	glm::vec3 difference = pwUp.position - car.position;
	// clamped vector used to get the AABB's closest point to the sphere
	glm::vec3 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// Position of the AABB's closest point to the sphere
	glm::vec3 closest = car.position + clamped;
	// the new distance vector between AABB's closest point and the Sphere's center
	difference = closest - pwUp.position;
	// Collision check
	return glm::length(difference) < pwUp.radius;
}
