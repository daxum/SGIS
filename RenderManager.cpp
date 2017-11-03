#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>
#include "RenderManager.hpp"
#include "Logger.hpp"
#include "callbackList.h"
#include "ExtraMath.hpp"
#include "Model.hpp"

using namespace std;

RenderManager::RenderManager(Game* game) :
	game(game),
	window(nullptr),
	shaderManager(),
	textureManager(),
	spriteRenderer(this),
	cameraPos(0.0, 0.0, 3.0),
	cameraLook(0.0, 0.0, 0.0),
	cameraUp(0.0, 0.0, 1.0),
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

	modelManager.loadModel("square");
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

glm::vec3 RenderManager::getCameraPos() {
	return cameraPos;
}

void RenderManager::setViewPort(int width, int height) {
	glViewport(0, 0, width, height);

	viewWidth = width;
	viewHeight = height;

	proj = glm::perspective(PI / 4.0f, (float)width / height, 0.1f, 400.0f);
}

bool RenderManager::windowClosed() const {
	return glfwWindowShouldClose(window);
}

const Shader* RenderManager::getShader(string name) const {
	return shaderManager.getShader(name);
}

const void RenderManager::renderModel(std::string model, std::string texture, const Shader* shader) const {
	modelManager.getModel(model)->draw(this, texture, shader);
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

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int createWidth = mode->width / 2.0f;
	int createHeight = mode->height / 2.0f;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Stop myself from going insane

	window = glfwCreateWindow(createWidth, createHeight, "Squares", NULL, NULL);

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
	glEnable(GL_DEPTH_TEST);

	Logger::popDomain();
}
