#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>
#include "RenderManager.hpp"
#include "Logger.hpp"
#include "callbackList.h"
#include "ExtraMath.hpp"

using namespace std;

RenderManager::RenderManager(Game* game) :
	game(game),
	window(nullptr),
	shaderManager(),
	textureManager(),
	spriteRenderer(this),
	cameraPos(0.0, 0.0, 3.0),
	cameraLook(0.0, 0.0, 0.0),
	cameraUp(0.0, 1.0, 0.0),
	fieldOfView(PI / 4.0f),
	viewWidth(960),
	viewHeight(540) {

	loadGl();
	spriteRenderer.initSpriteBuffers();

	shaderManager.loadShader("arena", "arenaShader", "arenaShader");
	shaderManager.loadShader("sprite", "spriteShader", "spriteShader");

	textureManager.loadTexture("square", GL_NEAREST, GL_NEAREST);
	textureManager.loadTexture("arena", GL_NEAREST, GL_NEAREST);
	textureManager.loadTexture("main_menu");
	textureManager.loadTexture("button", GL_NEAREST, GL_NEAREST);
	textureManager.loadTexture("button_hover", GL_NEAREST, GL_NEAREST);
	textureManager.loadTexture("button_press", GL_NEAREST, GL_NEAREST);
}

RenderManager::~RenderManager() {
	if (window != nullptr) {
		glfwDestroyWindow(window);
		window = nullptr;
	}

	glfwTerminate();
}

void RenderManager::beginDraw() {
	glClearColor(0.0, 0.2, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderManager::finishDraw() {
	glfwSwapBuffers(window);
}

glm::mat4 RenderManager::getView() {
	return glm::lookAt(cameraPos, cameraLook, cameraUp);
}

glm::mat4 RenderManager::getProj() {
	return proj;
}

float RenderManager::getFOV() {
	return fieldOfView;
}

int RenderManager::getViewWidth() {
	return viewWidth;
}

int RenderManager::getViewHeight() {
	return viewHeight;
}

SpriteRenderer* RenderManager::getSpriteRenderer() {
	return &spriteRenderer;
}

void RenderManager::setCameraLook(glm::vec3 look) {
	cameraLook = look;
}

void RenderManager::setCameraPos(glm::vec3 pos) {
	cameraPos = pos;
}

void RenderManager::setViewPort(int width, int height) {
	float bigger;
	float smaller;

	if (width > height) {
		bigger = width;
		smaller = height;
	}
	else {
		bigger = height;
		smaller = width;
	}

	//Adjust for wierd window ratios (otherwise sizes like 960x10 zoom out really far)
	fieldOfView = (DEFAULT_ASPECT_RATIO / (bigger / smaller)) * (PI / 4.0f);

	glViewport(0, 0, width, height);

	viewWidth = width;
	viewHeight = height;

	proj = glm::perspective(fieldOfView, (float)width / height, 0.1f, 400.0f);
}

bool RenderManager::windowClosed() const {
	return glfwWindowShouldClose(window);
}

const Shader* RenderManager::getShader(string name) const {
	return shaderManager.getShader(name);
}

void RenderManager::useTexture(string name, GLenum textureUnit) const {
	textureManager.getTexture(name).use(textureUnit);
}

void RenderManager::loadGl() {
	Logger::pushDomain("OPENGL");

	if (!glfwInit()) {
		Logger::fatal() << "Couldn't initialize glfw! This is very very bad.\n";
		throw std::runtime_error("Couldn't initialize glfw");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(960, 540, "Squares", NULL, NULL);

	if (window == nullptr) {
		Logger::fatal() << "Failed to create window and context!\n";
		throw std::runtime_error("Failed to create window and context");
	}

	glfwMakeContextCurrent(window);

	Logger::info() << "Successfully created window and context\n";

	int loadStatus = ogl_LoadFunctions();

	if (loadStatus == 0) {
		Logger::fatal() << "OpenGl function loading failed!\n";
		throw new std::runtime_error("OpenGl function loading failed");
	}
	else if (loadStatus > 1) {
		Logger::fatal() << "Failed to load " << loadStatus - 1 << " openGl " << (loadStatus == 2 ? "function" : "functions") << "!\n";
		throw std::runtime_error("OpenGl function loading failed");
	}

	Logger::info() << "Successfully loaded all OpenGl functions\n";

	glfwSetFramebufferSizeCallback(window, framebufferSizeChange);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, mouseScroll);
	glfwSetCursorPosCallback(window, cursorMove);
	glfwSetMouseButtonCallback(window, mouseClick);

	int width = 0;
	int height = 0;

	glfwGetFramebufferSize(window, &width, &height);

	setViewPort(width, height);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH);

	Logger::popDomain();
}
