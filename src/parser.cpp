#include "parser.hpp"
#include <sstream>
#include <iostream>
#include <algorithm>

namespace pangea {

std::vector<std::string> Parser::parseCode(const std::string& code) {
    std::vector<std::string> result;

    // Remove comments line by line
    std::istringstream iss(code);
    std::string line;
    std::ostringstream cleanCode;

    while (std::getline(iss, line)) {
        std::string processedLine = removeComments(line);
        // Trim whitespace
        processedLine.erase(0, processedLine.find_first_not_of(" \t\r\n"));
        processedLine.erase(processedLine.find_last_not_of(" \t\r\n") + 1);
        
        if (!processedLine.empty()) {
            cleanCode << processedLine << " ";
        }
    }

    std::string cleanCodeStr = cleanCode.str();
    // Trim final result
    cleanCodeStr.erase(0, cleanCodeStr.find_first_not_of(" \t\r\n"));
    cleanCodeStr.erase(cleanCodeStr.find_last_not_of(" \t\r\n") + 1);

    if (cleanCodeStr.empty()) {
        return result;
    }

    // Smart tokenization that preserves quoted strings
    auto tokens = tokenizeWithStringPreservation(cleanCodeStr);
    result.insert(result.end(), tokens.begin(), tokens.end());

    return result;
}

Value Parser::parseValue(const std::string& text) {
    if (text.empty()) {
        return Value();
    }

    // Try to parse as number
    try {
        if (text.find('.') != std::string::npos) {
            return Value(std::stod(text));
        } else {
            return Value(static_cast<double>(std::stoll(text)));
        }
    } catch (const std::exception&) {
        // Not a number, continue
    }

    // Try to parse as string literal
    if (text.length() >= 2 && text.front() == '"' && text.back() == '"') {
        return Value(text.substr(1, text.length() - 2));
    }

    // Try to parse as boolean or null
    if (text == "true") return Value(true);
    if (text == "false") return Value(false);
    if (text == "null") return Value();

    return Value(); // Return null value if not a literal
}

bool Parser::isString(const std::string& text) {
    return text.length() >= 2 && text.front() == '"' && text.back() == '"';
}

std::vector<std::string> Parser::tokenizeWithStringPreservation(const std::string& code) {
    std::vector<std::string> tokens;
    std::string currentToken;
    bool inQuotes = false;
    bool hasSpacesInString = false;

    for (size_t i = 0; i < code.length(); ++i) {
        char c = code[i];

        if (c == '"') {
            if (!inQuotes) {
                // Starting a string
                if (!currentToken.empty()) {
                    tokens.push_back(currentToken);
                    currentToken.clear();
                }
                inQuotes = true;
                hasSpacesInString = false;
                currentToken += c;
            } else {
                // Ending a string
                currentToken += c;

                // Check if this string had spaces and provide helpful error message
                if (hasSpacesInString) {
                    std::cerr << "WARNING: String literal contains spaces: " << currentToken << std::endl;
                    std::cerr << "         In Pangea, use '+' instead of spaces in strings." << std::endl;
                    std::cerr << "         Example: \"Hello+World\" instead of \"Hello World\"" << std::endl;
                    std::cerr << "         This will result in a null value." << std::endl;
                }

                tokens.push_back(handlePlus(currentToken));
                currentToken.clear();
                inQuotes = false;
            }
        } else if (inQuotes) {
            currentToken += c;
            if (std::isspace(c)) {
                hasSpacesInString = true;
            }
        } else if (std::isspace(c)) {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
        } else {
            currentToken += c;
        }
    }

    // Handle any remaining token
    if (!currentToken.empty()) {
        if (inQuotes) {
            std::cerr << "ERROR: Unterminated string literal: " << currentToken << std::endl;
            std::cerr << "       Missing closing quote character." << std::endl;
        }
        tokens.push_back(currentToken);
    }

    return tokens;
}

std::string Parser::removeComments(const std::string& line) {
    std::string result;
    bool inString = false;
    bool escaped = false;

    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];

        if (escaped) {
            result += c;
            escaped = false;
            continue;
        }

        if (c == '\\') {
            escaped = true;
            result += c;
            continue;
        }

        if (c == '"') {
            inString = !inString;
            result += c;
            continue;
        }

        if (!inString && c == '#') {
            // Check if # is preceded by non-whitespace (function arity)
            if (i > 0 && !std::isspace(line[i - 1])) {
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

std::string Parser::handlePlus(const std::string& word) {
    if (!isString(word)) {
        return word;
    }

    Value parsedVal = parseValue(word);
    if (parsedVal.isNull()) {
        return word;
    }

    std::string parsed = parsedVal.asString();
    
    // Replace patterns: (+ ) -> ♦PLUS♦, + -> space, ♦PLUS♦ -> +
    std::string temp = parsed;
    
    // First, protect literal plus signs
    size_t pos = 0;
    while ((pos = temp.find("(+)", pos)) != std::string::npos) {
        temp.replace(pos, 3, "♦PLUS♦");
        pos += 6; // Length of "♦PLUS♦"
    }
    
    // Convert remaining + to spaces
    pos = 0;
    while ((pos = temp.find('+', pos)) != std::string::npos) {
        temp[pos] = ' ';
        ++pos;
    }
    
    // Restore literal plus signs
    pos = 0;
    while ((pos = temp.find("♦PLUS♦", pos)) != std::string::npos) {
        temp.replace(pos, 6, "+");
        pos += 1; // Length of "+"
    }

    return "\"" + temp + "\"";
}

} // namespace pangea
