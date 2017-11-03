#pragma once

#include <vector>
#include <assimp/scene.h>
#include "RenderManager.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

class Model {
	friend class ModelManager;
public:
	~Model();

	void draw(const RenderManager* renderer, std::string texture, const Shader* shader) const;

private:
	std::vector<Mesh*> meshes;

	Model(const aiScene* scene);

	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
};
