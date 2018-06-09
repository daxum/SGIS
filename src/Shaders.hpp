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

#pragma once

#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

class BasicShader : public Shader {
public:
	void setGlobalUniforms(std::shared_ptr<Camera> camera, std::shared_ptr<ScreenState> state) {
		shaderInterface->setUniform(UniformType::MAT4x4, "projection", glm::value_ptr(camera->getProjection()));
	}

	void setPerObjectUniforms(std::shared_ptr<RenderComponent> object, MatrixStack& matStack, std::shared_ptr<ScreenState> state) {
		matStack.translate(object->getTranslation());
		matStack.rotate(object->getRotation());
		matStack.scale(object->getScale());

		shaderInterface->setUniform(UniformType::MAT4x4, "modelView", glm::value_ptr(matStack.top()));
		shaderInterface->setUniform(UniformType::VEC3, "color", glm::value_ptr(object->getColor()));
		shaderInterface->setTexture(object->getModel().texture, 0);
	}
};

class PhongShader : public Shader {
public:
	void setGlobalUniforms(std::shared_ptr<Camera> camera, std::shared_ptr<ScreenState> state) {
		shaderInterface->setUniform(UniformType::MAT4x4, "projection", glm::value_ptr(camera->getProjection()));
		shaderInterface->setUniform(UniformType::VEC3, "lightDir", glm::value_ptr(glm::normalize(glm::vec3(camera->getView() * glm::vec4(1.0, 1.0, 0.01, 0.0)))));
	}

	void setPerObjectUniforms(std::shared_ptr<RenderComponent> object, MatrixStack& matStack, std::shared_ptr<ScreenState> state) {
		matStack.translate(object->getTranslation());
		matStack.rotate(object->getRotation());
		matStack.scale(object->getScale());

		shaderInterface->setUniform(UniformType::MAT4x4, "modelView", glm::value_ptr(matStack.top()));
		shaderInterface->setUniform(UniformType::VEC3, "color", glm::value_ptr(object->getColor()));

		const Model& model = object->getModel();

		shaderInterface->setTexture(model.texture, 0);

		shaderInterface->setUniform(UniformType::VEC3, "ka", glm::value_ptr(model.lighting.ka));
		shaderInterface->setUniform(UniformType::VEC3, "ks", glm::value_ptr(model.lighting.ks));
		shaderInterface->setUniform(UniformType::FLOAT, "s", &model.lighting.s);
	}
};

class SkyShader : public Shader {
public:
	void setGlobalUniforms(std::shared_ptr<Camera> camera, std::shared_ptr<ScreenState> state) {
		shaderInterface->setUniform(UniformType::MAT4x4, "projection", glm::value_ptr(camera->getProjection()));
	}

	void setPerObjectUniforms(std::shared_ptr<RenderComponent> object, MatrixStack& matStack, std::shared_ptr<ScreenState> state) {
		matStack.translate(object->getTranslation());
		matStack.rotate(object->getRotation());
		matStack.scale(object->getScale());

		shaderInterface->setUniform(UniformType::MAT4x4, "modelView", glm::value_ptr(matStack.top()));
		shaderInterface->setTexture(object->getModel().texture, 0);
	}
};
