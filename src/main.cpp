#include <sstream>
#include <ostream>

#include <argparse/argparse.hpp>

#include <iostream>

#include "runner.hpp"

using std::cout;
using std::cerr;

int main(int argc, char **argv) {
    argparse::ArgumentParser program("knork");
	program.add_argument("-r", "--run")
		.help("Run a specific template found in the global template.json config file")
		.default_value("NONE");

	program.add_argument("variables")
		.remaining();

	try {
		program.parse_args(argc, argv);
	} catch (const std::exception& err) {
		std::cerr << err.what() << std::endl;
		std::cerr << program << std::endl;
		std::exit(1);
	}

    IRunner *runner = nullptr;
	if (program.is_used("--run") || program.is_used("-r")) {
        runner = new CommandLineRunner(&program);
	} else {
        runner = new KnorkFileRunner(&program);
	}

    ErrorCode errorCode = runner->execute();
    delete runner;

    switch (errorCode) {
        case None:
        break; // ignore
        case MalformedKnorkJson:
        cerr << "Malformed knork.json. Template or steps missing!" << std::endl;
        break;
        case UnknownTemplate:
        cerr << "Template name not found. Check your config file!" << std::endl;
        break;
        case ExecutionError:
        cerr << "Not all commands executed successfully!" << std::endl;
        break;
        case Unknown:
        default:
        cerr << "An unknown error occurred. Good luck!" << std::endl;
        break;
    }

    return errorCode;
}