#include "parser.hpp"
#include <sstream>
#include <iostream>
#include <algorithm>

namespace pangea
{

    std::vector<std::string> Parser::parseCode(const std::string &code)
    {
        std::vector<std::string> result;

        // Remove comments line by line
        std::istringstream iss(code);
        std::string line;
        std::ostringstream cleanCode;

        while (std::getline(iss, line))
        {
            std::string processedLine = removeComments(line);
            // Trim whitespace
            processedLine.erase(0, processedLine.find_first_not_of(" \t\r\n"));
            processedLine.erase(processedLine.find_last_not_of(" \t\r\n") + 1);

            if (!processedLine.empty())
            {
                cleanCode << processedLine << " ";
            }
        }

        std::string cleanCodeStr = cleanCode.str();
        // Trim final result
        cleanCodeStr.erase(0, cleanCodeStr.find_first_not_of(" \t\r\n"));
        cleanCodeStr.erase(cleanCodeStr.find_last_not_of(" \t\r\n") + 1);

        if (cleanCodeStr.empty())
        {
            return result;
        }

        // Smart tokenization that preserves quoted strings
        auto tokens = tokenizeWithStringPreservation(cleanCodeStr);
        result.insert(result.end(), tokens.begin(), tokens.end());

        return result;
    }

    Value Parser::parseValue(const std::string &text)
    {
        if (text.empty())
        {
            return Value();
        }

        // Try to parse as number
        try
        {
            if (text.find('.') != std::string::npos)
            {
                return Value(std::stod(text));
            }
            else
            {
                return Value(static_cast<double>(std::stoll(text)));
            }
        }
        catch (const std::exception &)
        {
            // Not a number, continue
        }

        // Try to parse as string literal
        if (text.length() >= 2 && text.front() == '"' && text.back() == '"')
        {
            return Value(text.substr(1, text.length() - 2));
        }

        // Try to parse as boolean or null
        if (text == "true")
            return Value(true);
        if (text == "false")
            return Value(false);
        if (text == "null")
            return Value();

        return Value(); // Return null value if not a literal
    }

    bool Parser::isString(const std::string &text)
    {
        return text.length() >= 2 && text.front() == '"' && text.back() == '"';
    }

    std::vector<std::string> Parser::tokenizeWithStringPreservation(const std::string &code)
    {
        std::vector<std::string> tokens;
        std::string currentToken;
        bool inQuotes = false;

        for (size_t i = 0; i < code.length(); ++i)
        {
            char c = code[i];

            if (c == '"')
            {
                if (!inQuotes)
                {
                    // Starting a string
                    if (!currentToken.empty())
                    {
                        tokens.push_back(currentToken);
                        currentToken.clear();
                    }
                    inQuotes = true;
                    currentToken += c;
                }
                else
                {
                    // Ending a string
                    currentToken += c;
                    tokens.push_back(currentToken);
                    currentToken.clear();
                    inQuotes = false;
                }
            }
            else if (inQuotes)
            {
                // Inside a string literal - preserve all characters including spaces
                currentToken += c;
            }
            else if (std::isspace(c))
            {
                if (!currentToken.empty())
                {
                    tokens.push_back(currentToken);
                    currentToken.clear();
                }
            }
            else
            {
                currentToken += c;
            }
        }

        // Handle any remaining token
        if (!currentToken.empty())
        {
            if (inQuotes)
            {
                std::cerr << "ERROR: Unterminated string literal: " << currentToken << std::endl;
                std::cerr << "       Missing closing quote character." << std::endl;
            }
            tokens.push_back(currentToken);
        }

        return tokens;
    }

    std::string Parser::removeComments(const std::string &line)
    {
        std::string result;
        bool inString = false;
        bool escaped = false;

        for (size_t i = 0; i < line.length(); ++i)
        {
            char c = line[i];

            if (escaped)
            {
                result += c;
                escaped = false;
                continue;
            }

            if (c == '\\')
            {
                escaped = true;
                result += c;
                continue;
            }

            if (c == '"')
            {
                inString = !inString;
                result += c;
                continue;
            }

            if (!inString && c == '#')
            {
                // Check if # is preceded by non-whitespace (function arity)
                if (i > 0 && !std::isspace(line[i - 1]))
                {
                    result += c;
                    continue;
                }
                // This is a comment, stop processing
                break;
            }

            result += c;
        }

        return result;
    }

    std::string Parser::handlePlus(const std::string &word)
    {
        // In modern Pangea C++, string literals are used as-is
        // No special processing of + characters needed
        return word;
    }

} // namespace pangea
