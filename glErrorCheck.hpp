#pragma once

#include "combinedGl.h"
#include "Logger.hpp"

inline void printGlErrors() {
	Logger::pushDomain("OPENGL");

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

	Logger::popDomain();
}
