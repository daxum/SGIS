#pragma once

#include <unordered_map>
#include <string>

class Model;

class ModelManager {
public:
	~ModelManager();

	const Model* getModel(std::string name) const;
	void loadModel(std::string name, std::string path = "", std::string extension = ".obj");

private:
	std::unordered_map<std::string, Model*> models;
};
