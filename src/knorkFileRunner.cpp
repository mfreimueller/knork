#include "runner.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>

using std::cerr;

ErrorCode KnorkFileRunner::execute() {
    json config = this->getLocalConfigJson();

    if (config.contains("template")) {
        string templateName = config["template"].template get<string>();
        auto variables = this->jsonToStringMap(config["variables"]);

        return this->executeTemplate(templateName, variables);
    } else if (config.contains("steps")) {
        auto steps = config["steps"];
        return this->executeSteps(steps);
    } else {
        return ErrorCode::MalformedKnorkJson;
    }
}

json KnorkFileRunner::getLocalConfigJson() {
    std::filesystem::path currentPath = std::filesystem::current_path();

	std::ifstream f(currentPath.string() + "/knork.json");
	return json::parse(f);
}

ErrorCode KnorkFileRunner::executeSteps(json steps) {
    for (auto& el : steps.items()) {
    	string step = el.value().template get<string>();

    	int returnCode = system(step.c_str());
		if (returnCode != 0) {
            cerr << "Command '" << step << "' failed with error code: " << returnCode;
            return ErrorCode::ExecutionError;
		}
    }

    return ErrorCode::None;
}
