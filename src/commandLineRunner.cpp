#include "runner.hpp"

#include <vector>

using svector = std::vector<string>;

ErrorCode CommandLineRunner::execute() {
    string templateName = this->argumentParser->get<string>("--run");
    smap variables;

    try {
        auto inputVariables = this->argumentParser->get<svector>("variables");
        for (auto& variable : inputVariables) {
            // we require parameters to be of the form key=value
            size_t pos = variable.find('=');

            if (pos != string::npos) {
                // Extract substrings before and after '=' character
                string key = variable.substr(0, pos);
                string value = variable.substr(pos + 1);

                variables[key] = value;
            }
        }
    } catch (std::logic_error& e) {
        // we have no additional params
    }

    return this->executeTemplate(templateName, variables);
}