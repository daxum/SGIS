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

#include "combinedGl.h"
#include "Logger.hpp"

inline void printGlErrors() {
	glFinish();

	GLenum error = 0;
	unsigned int numErrors = 0;

	while ((error = glGetError()) != GL_NO_ERROR) {
		numErrors++;

		switch(error) {
			case GL_INVALID_ENUM: Logger::error() << "Invalid enum\n"; break;
			case GL_INVALID_VALUE: Logger::error() << "Invalid value\n"; break;
			case GL_INVALID_OPERATION: Logger::error() << "Ivalid operation\n"; break;
			case GL_OUT_OF_MEMORY: Logger::error() << "Out of memory\n"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: Logger::error() << "Invalid framebuffer operation\n"; break;
			default: Logger::error() << "Unknown error " << error << "\n";
		}
	}

	Logger::info() << numErrors << " errors found\n";
}
