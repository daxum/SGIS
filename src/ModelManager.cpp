/******************************************************************************
 * SGIS - A simple game involving squares
 * Copyright (C) 2017
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

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
}

