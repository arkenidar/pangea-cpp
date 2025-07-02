#include "interpreter.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace pangea;

void printUsage(const std::string &programName)
{
    std::cout << "Usage: " << programName << " [options] [file]\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help    Show this help message\n";
    std::cout << "  -i, --interactive  Start interactive mode\n";
    std::cout << "  -e, --eval CODE    Evaluate CODE directly\n";
    std::cout << "\n";
    std::cout << "If no options are provided and a file is given, the file will be executed.\n";
    std::cout << "If no file is provided, interactive mode will be started.\n";
}

std::string readFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void interactiveMode()
{
    Interpreter interpreter;
    std::string line;

    std::cout << "Pangea C++ Interpreter\n";
    std::cout << "Type 'exit' to quit, 'help' for help.\n\n";

    while (true)
    {
        std::cout << "pangea> ";

        if (!std::getline(std::cin, line))
        {
            break; // EOF
        }

        if (line == "exit" || line == "quit")
        {
            break;
        }

        if (line == "help")
        {
            std::cout << "Available commands:\n";
            std::cout << "  exit, quit - Exit the interpreter\n";
            std::cout << "  help       - Show this help\n";
            std::cout << "\nExample Pangea code:\n";
            std::cout << "  println \"Hello, World!\"\n";
            std::cout << "  plus 2 3\n";
            std::cout << "  times 4 5\n";
            continue;
        }

        if (line.empty())
        {
            continue;
        }

        try
        {
            Value result = interpreter.execute(line);
            if (!result.isNull())
            {
                std::cout << "=> " << result.asString() << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    std::cout << "Goodbye!\n";
}

int main(int argc, char *argv[])
{
    try
    {
        if (argc == 1)
        {
            // No arguments - start interactive mode
            interactiveMode();
            return 0;
        }

        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];

            if (arg == "-h" || arg == "--help")
            {
                printUsage(argv[0]);
                return 0;
            }
            else if (arg == "-i" || arg == "--interactive")
            {
                interactiveMode();
                return 0;
            }
            else if (arg == "-e" || arg == "--eval")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "Error: --eval requires code argument\n";
                    return 1;
                }

                Interpreter interpreter;
                std::string code = argv[++i];
                Value result = interpreter.execute(code);

                if (!result.isNull())
                {
                    std::cout << result.asString() << std::endl;
                }
                return 0;
            }
            else if (arg[0] != '-')
            {
                // Assume it's a filename
                std::string code = readFile(arg);
                Interpreter interpreter;
                Value result = interpreter.execute(code);

                if (!result.isNull())
                {
                    std::cout << result.asString() << std::endl;
                }
                return 0;
            }
            else
            {
                std::cerr << "Unknown option: " << arg << std::endl;
                printUsage(argv[0]);
                return 1;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
