#pragma once

#include <string>
#include <map>

#include <argparse/argparse.hpp>
#include "nlohmann/json.hpp"

#include "errorCodes.hpp"

using argparse::ArgumentParser;
using std::string;
using json = nlohmann::json;

using smap = std::map<string, string>;

/**
 * Interface for all runner implementations.
*/
class IRunner {
    private:
    json templateJson;

    protected:
    ArgumentParser *argumentParser;

    IRunner(ArgumentParser *argumentParser);

    ErrorCode executeTemplate(string templateName, smap variables);

    const json getTemplateJson() { return this->templateJson; }
    const json getGloballyDefinedVariables();

    smap jsonToStringMap(json);

    public:
    virtual ErrorCode execute() = 0;

    virtual ~IRunner() {}

    private:
    string replacePlaceholdersWithVariables(string executionStep, smap variables);
};

/**
 * An IRunner implementation that requires a knork.json file to be present in
 * the current directory and executes a given template.
*/
class KnorkFileRunner : public IRunner {
    public:
    KnorkFileRunner(ArgumentParser *argumentParser): IRunner(argumentParser) {
    }

    virtual ~KnorkFileRunner() {}

    ErrorCode execute() override;

    private:
    json getLocalConfigJson();

    std::map<string, string> convertVariablesToMap(json config);

    ErrorCode executeSteps(json steps);
};

/**
 * An IRunner implementation that is configured on the fly by the parameters provided
 * by the user.
*/
class CommandLineRunner : public IRunner {
    public:
    CommandLineRunner(ArgumentParser *argumentParser): IRunner(argumentParser) {
    }

    virtual ~CommandLineRunner() {}

    ErrorCode execute() override;

};