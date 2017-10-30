#pragma once

#include <string>
#include <iostream>

namespace Logger {
	std::ostream& info();

	std::ostream& warn();

	std::ostream& error();

	std::ostream& fatal();

	void pushDomain(std::string next);

	void popDomain();
}

