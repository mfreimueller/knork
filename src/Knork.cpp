//============================================================================
// Name        : Knork.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>

#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

string getConfigPath();
json getConfigJson();
json getTemplateJson();

int executeTemplate(json config);
int executeSteps(json config);
string replacePlaceholdersWithVariables(string, json);

int main(int argc, char **argv)
{
    auto config = getConfigJson();

    if (config.contains("template")) {
    	return executeTemplate(config);
    } else if (config.contains("steps")) {
    	return executeSteps(config);
    } else {
    	cerr << "Malformed knork.json. template or steps missing!";
    	return 1;
    }
}

int executeTemplate(json config) {
    auto templateName = config["template"];
    auto templates = getTemplateJson();

    if (!templates.contains(templateName.template get<std::string>())) {
        cerr << "Template name not found. Check your config file!";
        return 3;
    }

    auto templateToExecute = templates[templateName];
    for (auto& el : templateToExecute.items()) {
    	string step = el.value().template get<std::string>();

    	auto stepToExecute = replacePlaceholdersWithVariables(step, config["variables"]);

    	int returnCode = system(stepToExecute.c_str());
		if (returnCode != 0) {
			cerr << "Command '" << stepToExecute << "' failed with error code: " << returnCode;
		}
    }

    return 0;
}

int executeSteps(json config) {
    auto steps = config["steps"];
    for (auto& el : steps.items()) {
    	string step = el.value().template get<std::string>();

    	int returnCode = system(step.c_str());
		if (returnCode != 0) {
			cerr << "Command '" << step << "' failed with error code: " << returnCode;
		}
    }

    return 0;
}

string getConfigPath() {
	string homeDir = getenv("HOME");
	return homeDir + "/.config/knork";
}

json getConfigJson() {
    filesystem::path currentPath = std::filesystem::current_path();

	ifstream f(currentPath.string() + "/knork.json");
	return json::parse(f);
}

json getTemplateJson() {
	ifstream f(getConfigPath() + "/templates.json");
	return json::parse(f);
}

string replacePlaceholdersWithVariables(string executionStep, json _template) {
	string modifiedString = executionStep;
	size_t startPos = 0;
	while ((startPos = modifiedString.find('{', startPos)) != std::string::npos) {
		size_t endPos = modifiedString.find('}', startPos);
		if (endPos == std::string::npos) {
			break;
		}

		string placeholder = modifiedString.substr(startPos + 1, endPos - startPos - 1);

		if (_template.contains(placeholder)) {
			auto replacement = _template[placeholder].template get<std::string>();
			modifiedString.replace(startPos, endPos - startPos + 1, replacement);
			startPos += replacement.length();
		}
	}

	return modifiedString;
}
