#pragma once

#include "value.hpp"
#include <string>
#include <vector>

namespace pangea
{

    /**
     * @brief Parser for Pangea source code
     *
     * Handles tokenization, comment removal, and string processing
     * equivalent to the parsing methods in the Java Interpreter class
     */
    class Parser
    {
    public:
        /**
         * @brief Parse Pangea source code into tokens
         * @param code The source code to parse
         * @return Vector of tokens
         */
        static std::vector<std::string> parseCode(const std::string &code);

        /**
         * @brief Parse a token into a Value (number, string, boolean, null)
         * @param text The token to parse
         * @return Parsed Value or null Value if not a literal
         */
        static Value parseValue(const std::string &text);

        /**
         * @brief Check if a token is a string literal
         * @param text The token to check
         * @return true if it's a string literal (starts and ends with quotes)
         */
        static bool isString(const std::string &text);

    private:
        /**
         * @brief Tokenize code while preserving quoted strings
         * @param code The code to tokenize
         * @return Vector of tokens
         */
        static std::vector<std::string> tokenizeWithStringPreservation(const std::string &code);

        /**
         * @brief Remove comments from a line while preserving # in function names
         * @param line The line to process
         * @return Line with comments removed
         */
        static std::string removeComments(const std::string &line);

        /**
         * @brief Handle plus symbol replacement in strings
         * @param word The string token to process
         * @return Processed string with + converted to spaces
         */
        static std::string handlePlus(const std::string &word);
    };

} // namespace pangea
