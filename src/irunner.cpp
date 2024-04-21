#include "runner.hpp"

#include <iostream>
#include <fstream>

using std::cerr;

json IRunner::getTemplateJson() {
	string homeDir = getenv("HOME");
	auto configPath = homeDir + "/.config/knork/templates.json";

	std::ifstream f(configPath);
	return json::parse(f);
}

ErrorCode IRunner::executeTemplate(string templateName, smap variables) {
    auto templates = this->getTemplateJson();

    if (!templates.contains(templateName)) {
        return ErrorCode::UnknownTemplate;
    }

    auto templateToExecute = templates[templateName];
    for (auto& el : templateToExecute.items()) {
    	string step = el.value().template get<string>();

    	auto stepToExecute = this->replacePlaceholdersWithVariables(step, variables);

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
		}
	}

	return modifiedString;
}
