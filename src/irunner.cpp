#include "runner.hpp"

#include <iostream>
#include <fstream>

using std::cerr;

IRunner::IRunner(ArgumentParser *argumentParser) {
	this->argumentParser = argumentParser;

	string homeDir = getenv("HOME");
	auto configPath = homeDir + "/.config/knork/templates.json";

	std::ifstream f(configPath);
	this->templateJson = json::parse(f);
}

ErrorCode IRunner::executeTemplate(string templateName, smap variables) {
    auto templates = this->getTemplateJson();

    if (!templates.contains(templateName)) {
        return ErrorCode::UnknownTemplate;
    }

	// we want to merge the globally defined variables with the variables
	// passed through the specific runners class
	variables.merge(this->jsonToStringMap(this->getGloballyDefinedVariables()));

    auto templateToExecute = templates[templateName];
    for (auto& el : templateToExecute.items()) {
    	string step = el.value().template get<string>();

    	auto stepToExecute = this->replacePlaceholdersWithVariables(step, variables);
		// stepToExecute is empty iff a placeholder could not be resolved.
		if (stepToExecute.empty()) {
			return ErrorCode::UnresolvedPlaceholder;
		}

    	int returnCode = system(stepToExecute.c_str());
		if (returnCode != 0) {
			cerr << "Command '" << stepToExecute << "' failed with error code: " << returnCode;
            return ErrorCode::ExecutionError;
		}
    }

    return ErrorCode::None;
}

string IRunner::replacePlaceholdersWithVariables(string executionStep, smap variables) {
	string modifiedString = executionStep;
	size_t startPos = 0;
	while ((startPos = modifiedString.find('{', startPos)) != string::npos) {
		size_t endPos = modifiedString.find('}', startPos);
		if (endPos == string::npos) {
			break;
		}

		string placeholder = modifiedString.substr(startPos + 1, endPos - startPos - 1);

		if (variables.find(placeholder) != variables.end()) {
			auto replacement = variables[placeholder];
			modifiedString.replace(startPos, endPos - startPos + 1, replacement);
			startPos += replacement.length();
		} else {
			// In this case, there is a placeholder defined that is NOT
			// given as a variable. Therefore we need to break execution here
			// to remind the user to supply ALL required variables, as there
			// are currently no optional placeholders.
			return "";
		}
	}

	return modifiedString;
}

const json IRunner::getGloballyDefinedVariables() {
	if (!this->templateJson.contains("_variables")) {
		return json();
	}

	return this->templateJson["_variables"];
}

smap IRunner::jsonToStringMap(json jsonArray) {
	smap variables;
	for (json::iterator it = jsonArray.begin(); it != jsonArray.end(); ++it) {
		variables[it.key()] = it.value();
	}

    return variables;
}
