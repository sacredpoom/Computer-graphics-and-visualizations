#include <iostream>     //cout, cerr
#include <cstdlib>      // EXIT_FAILURE
#include <GL/glew.h>    // GLEW lib
#include <GLFW/glfw3.h> //GLFW lib

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> //handles jpg image file

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define BASIC_LIGHT_VS "../3D_Scene/2.2.basic_lighting.vs"
#define BASIC_LIGHT_FS "../3D_Scene/2.2.basic_lighting.fs"
#define LIGHT_CUBE_VS "../3D_Scene/2.2.light_cube.vs"
#define LIGHT_CUBE_FS "../3D_Scene/2.2.light_cube.fs"

#include <shader_s.h>
#include <Sphere.h>
#include <FullCylinder.h> // cylinder
#include <Cube.h>
#include <Torus.h>
#include <Plane.h>
#include <Pyramid.h>
#include <camera.h>

#define M_PI 3.141592

/*		JOSEPH VENESKI    FINAL PROJECT 3D Scene with Lighting and Textures
*
*       Implemented camera system using camera.h code from LearnOpenGL.com -
*		https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h
*		with edits to include Q, E, and mouse-scroll movement speed adjustment. Standard
*		movements with W, A, S, D
*		Perspective mode by default but Orthographic view is toggleable with P key
*		Mouse-Scroll to adjust camera movement speed.
*		stb_image.h file by Sean Barrett downloaded from LearnOpenGL.com
*
*		Using semi-sphere for main portion of baseball cap
*		using cylinder object that is stretched to represent brim of hat
*		uses table object to cut off bottom section of brim cylinder.
*		Tori included as vent holes on cap as well as compressed sphere for
*		the button on the top.
*
*		Added tablelegs (4) and "Floor" plane to scene
*		Paper object and screwdriver objects rendered using
*		Cylinders, Spheres, Planes, and Pyramids.
*		All shapes intentionally colored uniquely for easier differentiation.
*
*		Added textures to all objects in scene, three different textures used on
*		baseball cap object, two different textures applied to screwdriver object
*		Baseball cap has two textures layerd on top of one another to demonstrate an
*		advanced texturing technique.
*
*		Edits made to classes to implement texture coordinates and/or specify texture in
*		object instantiation.
*
*		Implemented light source (sun) above scene using Phong lighting model.
*		Maneuver the lightsource around the scene using I J K L U O keys 
*		Scene using single directional light source
*		All objects calculate normals to handle lighting correctly
* 
*		Added second blue-ish light source per project requirements (specified a colored light source)
*		Added skybox around scene with a sphere. 
*		Changed "sun" to a "moon" (turned to white color)
*		-this creates an "evening" asthetic with the blue light from the side of the scene accenting the white
* 
*		Replaced some textures with higher resolution versions (greater than 1056x1056)
*		Hat now has high-res texture and is made of wool instead of leather
* 
*		Some variable renaming for clarity 
*		Will upload to GitHub in next update
* 
*		October 15, 2023
*/

namespace {
	//main window
	GLFWwindow* gWindow = nullptr;

	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	const char* const WINDOW_TITLE = "Joseph Veneski";
	float angle = 0.3f; //for rotation

	// variable for each different texture
	GLuint baseballCapText;
	GLuint baseballNub;
	GLuint baseballBrim;
	GLuint hatTori;
	GLuint wickTable;
	GLuint grass;
	GLuint screwHandle;
	GLuint screwShaft;
	GLuint paper;
	GLuint polka;
	GLuint sky; // added sky around scene
	GLuint blue; // blue bugspray can (didn't want to use brand image)
	GLuint pineapple; //specular maps could increase the illusion further
	GLuint brass; 

	//camera
	Camera gCamera(glm::vec3(5.0f, 0.3f, 8.0f));
	float gLastX = SCR_WIDTH / 2.0f;
	float gLastY = SCR_HEIGHT / 2.0f;
	bool gFirstMouse = true;
	GLuint uniView;
	GLuint uniProj;
	GLuint uniModel;
	bool orthographicMode = false; // variable to flip between ortho and perspective (default)

	//timing
	float gDeltaTime = 0.0f;
	float gLastFrame = 0.0f;

	//lighting
	glm::vec3 lightPos(0.0f, 54.0f, 6.0f);    //light position
	glm::vec3 backupLightPos(20.0f, 0.0f, 10.0f); //second light position
	float lightMovementSpeed = 5.0f;          //adjust movement speed of lightsource
	glm::vec3 lightBoxPos(0.0f, 20.0f, 6.0f); //keep shape and lightsource together

	//file names all here for convenience
	const char* capImage1 = "../Assets/wool.jpg"; // replaced with higher resolution texture
	const char* capImage2 = "../Assets/blackTexture3.jpg";
	const char* capBrimImage = "../Assets/blackTexture6.jpg";
	const char* capToriImage = "../Assets/blackStitch.jpg";
	const char* wickImage = "../Assets/wicker2.jpg"; // replaced with higher resolution texture
	const char* grassImage = "../Assets/grass.jpg";
	const char* screwHandleImage = "../Assets/yellowHandle.jpg";
	const char* screwShaftImage = "../Assets/steel2.jpg"; // replaced with higher resolution texture
	const char* paperImage = "../Assets/paper2.jpg";
	const char* polkaImage = "../Assets/polkaDots.png";
	const char* skyImage = "../Assets/sky.jpg";
	const char* blueImage = "../Assets/blue.jpg";
	const char* pineappleImage = "../Assets/myPineapple.jpg";
	const char* brassImage = "../Assets/brass.jpg";
}

// function prototypes
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height); //window resizing
void processInput(GLFWwindow* window); //check for keyboard events
void UCreateTexture(GLuint& textureId, const char* imageName);
// complexTexture added to demonstrate advanced texturing with layers
void UCreateComplexTexture(GLuint& textureId, GLuint& textureId2, const char* imageName, const char* imageName2);
void UDestroyTexture(GLuint& textureId);
void URender();
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos); //mouse cursor
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset); //scrollwheel adjust
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods); //mouse button (not really used)

//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //enable wireframe

// Main method
int main(int argc, char* argv[]) {

	//set up window
	if (!UInitialize(argc, argv, &gWindow))
		return EXIT_FAILURE;

	// load our textures
	UCreateComplexTexture(baseballCapText, polka, capImage1, polkaImage);
	UCreateTexture(baseballNub, capImage2);
	UCreateTexture(baseballBrim, capBrimImage);
	UCreateTexture(hatTori, capToriImage);
	UCreateTexture(wickTable, wickImage);
	UCreateTexture(grass, grassImage);
	UCreateTexture(screwHandle, screwHandleImage);
	UCreateTexture(screwShaft, screwShaftImage);
	UCreateTexture(paper, paperImage);
	UCreateTexture(sky, skyImage);
	UCreateTexture(blue, blueImage);
	UCreateTexture(pineapple, pineappleImage);
	UCreateTexture(brass, brassImage);

	// render loop
	while (!glfwWindowShouldClose(gWindow)) {

		float currentFrame = glfwGetTime();
		gDeltaTime = currentFrame - gLastFrame;
		gLastFrame = currentFrame;

		processInput(gWindow);
		URender();
		glfwPollEvents();
	}

	// clean-up
	UDestroyTexture(baseballCapText);
	UDestroyTexture(polka);
	UDestroyTexture(baseballNub);
	UDestroyTexture(baseballBrim);
	UDestroyTexture(hatTori);
	UDestroyTexture(wickTable);
	UDestroyTexture(grass);
	UDestroyTexture(screwHandle);
	UDestroyTexture(screwShaft);
	UDestroyTexture(paper);
	UDestroyTexture(sky);
	UDestroyTexture(blue);
	UDestroyTexture(pineapple);
	UDestroyTexture(brass);

	glfwTerminate();
	return 0;
}

// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
	// GLFW: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// GLFW: window creation
	// ---------------------
	* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (*window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(*window);
	glfwSetFramebufferSizeCallback(*window, UResizeWindow);
	glfwSetCursorPosCallback(*window, UMousePositionCallback);
	glfwSetScrollCallback(*window, UMouseScrollCallback);
	glfwSetMouseButtonCallback(*window, UMouseButtonCallback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLEW: initialize
	// ----------------
	// Note: if using GLEW version 1.13 or earlier
	glewExperimental = GL_TRUE;
	GLenum GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult)
	{
		std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
		return false;
	}

	// Displays GPU OpenGL version
	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	return true;
}

// keyboard inputs
void processInput(GLFWwindow* window) {

	static bool pKeyPressed = false; //to determine if the key was recently pressed
	static const float cameraSpeed = 2.5f;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //escape to close window
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //move forward
		gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //move back
		gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //move left
		gCamera.ProcessKeyboard(LEFT, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //move right
		gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) //added upward movement listener
		gCamera.ProcessKeyboard(UP, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) //added downward movement listener
		gCamera.ProcessKeyboard(DOWN, gDeltaTime);

	//lightbox movement (testing light effects)
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		lightPos.z += lightMovementSpeed * gDeltaTime;
		lightBoxPos = lightPos;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		lightPos.z -= lightMovementSpeed * gDeltaTime;
		lightBoxPos = lightPos;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		lightPos.x += lightMovementSpeed * gDeltaTime;
		lightBoxPos = lightPos;
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		lightPos.x -= lightMovementSpeed * gDeltaTime;
		lightBoxPos = lightPos;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		lightPos.y += lightMovementSpeed * gDeltaTime;
		lightBoxPos = lightPos;
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		lightPos.y -= lightMovementSpeed * gDeltaTime;
		lightBoxPos = lightPos;
	}

	//set boolean to not trigger multiple P button presses by accident
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !pKeyPressed) {//switch between ortho/perspective
		orthographicMode = !orthographicMode;
		pKeyPressed = true; //flag recent press
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
		pKeyPressed = false; //reset when button release
}

// window resizing
void UResizeWindow(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (gFirstMouse)
	{
		gLastX = xpos;
		gLastY = ypos;
		gFirstMouse = false;
	}

	float xoffset = xpos - gLastX;
	float yoffset = gLastY - ypos; // reversed since y-coordinates go from bottom to top

	gLastX = xpos;
	gLastY = ypos;

	gCamera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	gCamera.ProcessMouseScroll(yoffset);
}

// glfw: handle mouse button events
// --------------------------------
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		if (action == GLFW_PRESS)
			std::cout << "Left mouse button pressed" << std::endl;
		else
			std::cout << "Left mouse button released" << std::endl;
	}
	break;

	case GLFW_MOUSE_BUTTON_MIDDLE:
	{
		if (action == GLFW_PRESS)
			std::cout << "Middle mouse button pressed" << std::endl;
		else
			std::cout << "Middle mouse button released" << std::endl;
	}
	break;

	case GLFW_MOUSE_BUTTON_RIGHT:
	{
		if (action == GLFW_PRESS)
			std::cout << "Right mouse button pressed" << std::endl;
		else
			std::cout << "Right mouse button released" << std::endl;
	}
	break;

	default:
		std::cout << "Unhandled mouse button event" << std::endl;
		break;
	}
}

void UCreateTexture(GLuint& textureId, const char* imageName) {
	// Load texture using STB library
	int texWidth, texHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true); // Flip texture vertically as OpenGL expects the top-left corner to be the origin
	unsigned char* data = stbi_load(imageName, &texWidth, &texHeight, &nrChannels, 0);
	if (data)
	{
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		// Set texture parameters (optional, adjust as needed)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data); // Free image data after it has been loaded into OpenGL
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
}

// this method handles both jpg and png files 
void UCreateComplexTexture(GLuint& textureId, GLuint& textureId2, const char* imageName, const char* imageName2) {
	// Load texture using STB library
	int texWidth, texHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true); // Flip texture vertically as OpenGL expects the top-left corner to be the origin
	unsigned char* data = stbi_load(imageName, &texWidth, &texHeight, &nrChannels, 0);
	if (data)
	{
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		// Set texture parameters (optional, adjust as needed)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data); // Free image data after it has been loaded into OpenGL
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// load second texture (png)
	unsigned char* data2 = stbi_load(imageName2, &texWidth, &texHeight, &nrChannels, 0);
	if (data2)
	{
		glGenTextures(1, &textureId2);
		glBindTexture(GL_TEXTURE_2D, textureId2);

		// Set texture parameters (optional, adjust as needed)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//								RGBA for PNGs                 And again here
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data2); // Free image data after it has been loaded into OpenGL
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
}

void URender() {
	//compile and link vertex/fragment shaders
	Shader basic_light(BASIC_LIGHT_VS, BASIC_LIGHT_FS);
	Shader light_cube(LIGHT_CUBE_VS, LIGHT_CUBE_FS);

	//camera parameters 
	glm::mat4 view = gCamera.GetViewMatrix();
	uniView = glGetUniformLocation(basic_light.ID, "view"); //get view from shader
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	//create projection matrix
	float fov = glm::radians(45.0f);
	float aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	float nearClip = 0.1f;
	float farClip = 100.0f;
	float orthScale = 50.0;
	glm::mat4 projection = glm::perspective(fov, aspectRatio, nearClip, farClip); //set frustum
	// if P key was pressed switch to ortho mode
	if (orthographicMode) {
		projection = glm::ortho(-(400.0f / orthScale), 400.0f / orthScale, -(300.0f / orthScale), 300.0f / orthScale, -4.5f, 6.5f);
	}

	uniProj = glGetUniformLocation(basic_light.ID, "projection"); //get projection from shader
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(projection));

	//set shader
	basic_light.use();
	basic_light.setVec3("lightPos", lightPos);
	basic_light.setVec4("lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	basic_light.setVec3("backupLightPos", backupLightPos);						//   adding second lightsource per requirements
	basic_light.setVec4("backupLightColor", glm::vec4(0.24f, 0.25f, 0.6f, 1.0f)); // blue-ish color accents white color of main light

	basic_light.setMat4("projection", projection);
	basic_light.setMat4("view", view);
	// only need to set these once
	basic_light.setInt("ourTexture", 0);
	basic_light.setInt("secondTexture", 1);

	glEnable(GL_DEPTH_TEST); // required to correctly display graphics

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	angle += 0.001f; //update rotation angle
	float radians = (270.0 * M_PI) / 180.0;

	//SKY SURROUNDING SCENE
	//********************************
	radians = (100 * M_PI) / 180.0;
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 scale = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
	glm::mat4 rotation = glm::rotate(model, radians, glm::vec3(1.0f, 0.0f, 0.0f)); //rotation
	glm::mat4 translation = glm::translate(model, glm::vec3(0.05f, 1.0f, 0.05f));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Sphere skySphere(1.0, 16, 16, &sky); 
	skySphere.Draw();
	
	//DRAW SEMI CIRCLE **BASEBALLCAP**
	//********************************
	radians = (90.0 * M_PI) / 180.0;
	model = glm::mat4(1.0f); //identity matrix
	translation = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); //translate
	scale = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); //scale
	rotation = glm::rotate(model, radians, glm::vec3(-2.0f, 0.6f, 0.6f)); //rotation
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Sphere bballCap(1.0f, 16, 16, &baseballCapText, &polka); // layering two textures
	bballCap.halfDraw();
	
	//DRAW CYLINDER **BRIM OF CAP**
	//*****************************
	radians = (100 * M_PI) / 180.0;
	model = glm::mat4(1.0f);
	scale = glm::scale(model, glm::vec3(0.5f, 1.7f, 0.5f));
	rotation = glm::rotate(model, radians, glm::vec3(1.1f, 1.0f, 2.3f)); //rotation
	translation = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.4f));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	FullCylinder brim(0.5f, 4.0f, 15, 20, &baseballBrim); // different texture
	brim.Draw();

	//DRAW TORI ON CAP
	//*****************************
	radians = (270.0 * M_PI) / 180.0;
	model = glm::mat4(1.0f);
	scale = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	rotation = glm::rotate(model, radians, glm::vec3(0.8f, 1.0f, 1.2f)); //rotation
	translation = glm::translate(model, glm::vec3(-0.2f, 0.8f, 0.55f));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Torus torus(0.5f, 0.3f, 36, 18);
	torus.setTexture(&hatTori); // all tori use the same texture
	torus.Draw();

	//***********SECOND TORUS***********
	//**********************************
	radians = (180.0 * M_PI) / 180.0;
	model = glm::mat4(1.0f);
	scale = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	rotation = glm::rotate(model, radians, glm::vec3(0.8f, 1.0f, 1.2f)); //rotation
	translation = glm::translate(model, glm::vec3(0.55f, 0.8f, -0.25f));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Torus torus2(0.5f, 0.3f, 36, 18);
	torus2.setTexture(&hatTori); // all tori use the same texture
	torus2.Draw();

	//***********THIRD TORUS***********
	//*********************************
	radians = (50.0 * M_PI) / 180.0;
	model = glm::mat4(1.0f);
	scale = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	rotation = glm::rotate(model, radians, glm::vec3(1.5f, 0.7f, 0.9f)); //rotation
	translation = glm::translate(model, glm::vec3(-0.58f, 0.7f, -0.44f));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Torus torus3(0.5f, 0.3f, 36, 18);
	torus3.setTexture(&hatTori);  // all tori use the same texture
	torus3.Draw();

	//********BUTTON ON TOP OF CAP**********
	//**************************************
	radians = (100.0 * M_PI) / 180.0;
	model = glm::mat4(1.0f);
	scale = glm::scale(model, glm::vec3(0.02f, 0.06f, 0.06f));
	rotation = glm::rotate(model, radians, glm::vec3(0.2f, 0.2f, 0.5f)); //rotation
	translation = glm::translate(model, glm::vec3(0.05f, 1.0f, 0.05f));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Sphere button(1.0, 16, 16, &baseballNub); // and another texture
	button.Draw();

	//DRAW PAPER ON THE TABLE (PLANE OBJECT)
	//**************************************
	radians = (0 * M_PI) / 180.0;
	model = glm::mat4(1.0f);
	translation = glm::translate(model, glm::vec3(5.5f, 0.01f, -1.0f));
	rotation = glm::rotate(model, radians, glm::vec3(1.0f, 0.0f, 0.0f));
	scale = glm::scale(model, glm::vec3(1.0, 1.0, 1.5));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Plane paperObj(&paper); // all work and no play makes Jack a dull boy
	paperObj.Draw();

	//DRAW SCREWDRIVER HANDLE (CYLINDER)
	//**********************************
	radians = (165 * M_PI) / 180.0;
	model = glm::mat4(1.0f);
	scale = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	rotation = glm::rotate(model, radians, glm::vec3(0.1f, -0.01f, -0.2f)); //rotation
	translation = glm::translate(model, glm::vec3(4.1f, 0.1f, -0.9f));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	FullCylinder screwDriveHandle(0.5f, 4.0f, 20, 40, &screwHandle); // full cylinder, got rid of extra spheres for ends
	screwDriveHandle.Draw();

	//DRAW STEEL SHAFT OF SCREWDRIVER (CYLINDER)
	//******************************************
	radians = (165 * M_PI) / 180.0;
	model = glm::mat4(1.0f);
	scale = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	rotation = glm::rotate(model, radians, glm::vec3(0.1f, -0.01f, -0.2f)); //rotation
	translation = glm::translate(model, glm::vec3(4.5f, 0.09f, -1.22f));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	FullCylinder screwMetal(0.5f, 6.0f, 10, 40, &screwShaft); //specify color 
	screwMetal.Draw();

	//DRAW SCREWDRIVER TIP (PYRAMID)
	//******************************
	radians = (255 * M_PI) / 180.0;
	model = glm::mat4(1.0f);
	scale = glm::scale(model, glm::vec3(0.05f, 0.1f, 0.05f));
	rotation = glm::rotate(model, radians, glm::vec3(1.1f, 0.2f, 1.1f)); //rotation
	translation = glm::translate(model, glm::vec3(4.78f, 0.08f, -1.43));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Pyramid screwTip(&screwShaft); // tip doesn't line up perfectly, could adjust
	screwTip.Draw();

	//DRAW CUBE **TABLE OBJECT**
	//**************************
	radians = (120 * M_PI) / 180.0;
	model = glm::mat4(1.0f);
	translation = glm::translate(model, glm::vec3(3.0f, -0.5f, -1.0f));
	rotation = glm::rotate(model, radians, glm::vec3(1.0f, 1.0f, 1.0f));
	scale = glm::scale(model, glm::vec3(1.0, 6.0, 8.0));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Cube table(&wickTable);
	table.Draw();

	//DRAW FOUR TABLE LEGS (USING CYLINDERS)
	//*********************************
	radians = (120 * M_PI) / 180.0;
	model = glm::mat4(1.0f);
	scale = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	rotation = glm::rotate(model, radians, glm::vec3(1.0f, -1.0f, 1.0f)); //rotation
	translation = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.9f));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	// Leg 1 ********************************************************
	FullCylinder tableLeg1(0.5f, 4.0f, 15, 20, &wickTable); // reuse wick texture
	tableLeg1.Draw();

	model = glm::mat4(1.0f);
	scale = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	rotation = glm::rotate(model, radians, glm::vec3(1.0f, -1.0f, 1.0f)); //rotation
	translation = glm::translate(model, glm::vec3(0.0f, -3.0f, -3.0));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	// Leg 2 *****************************************************
	FullCylinder tableLeg2(0.5f, 4.0f, 15, 20, &wickTable);
	tableLeg2.Draw();

	model = glm::mat4(1.0f);
	scale = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	rotation = glm::rotate(model, radians, glm::vec3(1.0f, -1.0f, 1.0f)); //rotation
	translation = glm::translate(model, glm::vec3(6.0f, -3.0f, -3.0));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	// Leg 3 ***************************************************
	FullCylinder tableLeg3(0.5f, 4.0f, 15, 20, &wickTable);
	tableLeg3.Draw();

	model = glm::mat4(1.0f);
	scale = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	scale = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	rotation = glm::rotate(model, radians, glm::vec3(1.0f, -1.0f, 1.0f)); //rotation
	translation = glm::translate(model, glm::vec3(6.0f, -3.0f, 0.9));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	// Leg 4 **************************************************
	FullCylinder tableLeg4(0.5f, 4.0f, 15, 20, &wickTable);
	tableLeg4.Draw();

	//DRAW GROUND UNDER TABLE (PLANE OBJECT)
	//**************************************
	radians = (0 * M_PI) / 180.0;
	model = glm::mat4(1.0f);
	translation = glm::translate(model, glm::vec3(2.5f, -5.0f, -1.0f));
	rotation = glm::rotate(model, radians, glm::vec3(1.0f, 0.0f, 0.0f));
	scale = glm::scale(model, glm::vec3(15.0, 5.0, 15.0));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Plane planeGround(&grass);
	planeGround.Draw();

	//DRAW BLUE BUG SPRAY CAN (CYLINDER OBJECT)
	//*****************************************
	radians = (270 * M_PI) / 180.0;
	model = glm::mat4(1.0f);
	scale = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	rotation = glm::rotate(model, radians, glm::vec3(1.0f, 0.0f, 0.0f)); 
	translation = glm::translate(model, glm::vec3(3.1f, 0.4f, -2.8f));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	FullCylinder bugSpray(1.0f, 10.0f, 10, 40, &blue); 
	bugSpray.Draw();

	//PINEAPPLE OBJECT
	//*****************************************
	radians = (270 * M_PI) / 180.0;
	model = glm::mat4(1.0f);
	scale = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	rotation = glm::rotate(model, radians, glm::vec3(1.0f, 0.0f, 0.0f)); 
	translation = glm::translate(model, glm::vec3(1.8f, 1.0f, -2.8f));
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	FullCylinder tiki(3.0f, 10.0f, 10, 40, &pineapple); // Using cylinder primitive and texture to simulate complexity
	tiki.Draw();

	// PINAPPLE SPHERE?
	//*****************************
	radians = (90.0 * M_PI) / 180.0;
	model = glm::mat4(1.0f); //identity matrix
	translation = glm::translate(model, glm::vec3(1.8f, 2.0f, -2.8f)); //translate
	scale = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f)); //scale
	rotation = glm::rotate(model, radians, glm::vec3(-1.0f, 0.0f, 0.0f)); //rotation
	model = translation * rotation * scale;
	uniModel = glGetUniformLocation(basic_light.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Sphere pineappleTop(1.0f, 16, 16, &brass); // layering two textures
	pineappleTop.halfDraw();

	//LightSphere (Sun object)
	//*************************************
	light_cube.use(); //switch shader
	light_cube.setMat4("projection", projection);
	light_cube.setMat4("view", view);

	model = glm::mat4(1.0f); //create an identity matrix
	scale = glm::scale(model, glm::vec3(5.1f, 5.1f, 5.1f));
	translation = glm::translate(model, lightPos); //using variable to control movement with keys 
	model = translation * scale;
	uniModel = glGetUniformLocation(light_cube.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Sphere lightSphere(1.0, 16, 16, &grass); // texture doesn't matter, will only show a white sphere
	lightSphere.Draw();

	//SECOND LIGHT SOURCE 
	//**************************************************
	model = glm::mat4(1.0f); //create an identity matrix
	scale = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	translation = glm::translate(model, backupLightPos);
	model = translation * scale;
	uniModel = glGetUniformLocation(light_cube.ID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Cube lightCube(&polka);
	lightCube.Draw();

	glfwSwapBuffers(gWindow);
}

// clean up everything when done
void UDestroyTexture(GLuint& texture) {
	glDeleteTextures(1, &baseballCapText);
	glDeleteTextures(1, &polka);
	glDeleteTextures(1, &baseballNub);
	glDeleteTextures(1, &baseballBrim);
	glDeleteTextures(1, &hatTori);
	glDeleteTextures(1, &grass);
	glDeleteTextures(1, &screwHandle);
	glDeleteTextures(1, &screwShaft);
	glDeleteTextures(1, &paper);
	glDeleteTextures(1, &sky);
	glDeleteTextures(1, &blue);
	glDeleteTextures(1, &pineapple);
	glDeleteTextures(1, &brass);
}