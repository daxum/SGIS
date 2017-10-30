#include <vector>
#include "Logger.hpp"

using namespace std;

namespace {
	vector<string> domains = {"MAIN"};
	string domainString = "MAIN::";
	ostream* output = &cout;

	ostream& print(string level) {
		*(output) << level << "::" << domainString << " ";

		return *(output);
	}

	void buildDomain() {
		domainString = "";

		for (const string& s : domains) {
			domainString = domainString + s + "::";
		}
	}
}

ostream& Logger::info() {
	return print("INFO");
}

ostream& Logger::warn() {
	return print("WARN");
}

ostream& Logger::error() {
	return print("ERROR");
}

ostream& Logger::fatal() {
	return print("FATAL");
}

void Logger::pushDomain(string next) {
	domains.push_back(next);
	domainString = domainString + next + "::";
}

void Logger::popDomain() {
	if (domains.size() > 0) {
		domains.pop_back();
		buildDomain();
	}
}

