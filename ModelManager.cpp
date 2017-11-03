#include <stdexcept>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ModelManager.hpp"
#include "Model.hpp"
#include "Logger.hpp"

using namespace std;

ModelManager::~ModelManager() {
	for (auto model : models) {
		delete model.second;
	}

	models.clear();
}

const Model* ModelManager::getModel(string name) const {
	return models.at(name);
}

void ModelManager::loadModel(string name, string path, string extension) {
	Logger::pushDomain("MODEL");

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("models/" + path + name + extension, aiProcess_Triangulate | aiProcess_GenNormals);

	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
		Logger::fatal() << "Failed to load model " << path << name << extension << "\n";
		Logger::fatal() << "Error string: " << importer.GetErrorString() << "\n";

		throw runtime_error("Model loading failed for \"" + name + "\"\n");
	}

	Model* model = new Model(scene);
	models.insert(make_pair(name, model));

	Logger::info() << "Loaded model \"" << path << name << extension << "\"\n";
	Logger::popDomain();
}

