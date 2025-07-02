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
     * equivalent to the parsing methods in the Java Interpreter class.
     *
     * The parser implements a phrase-building approach where code is first
     * tokenized into meaningful units while preserving string literals and
     * handling comments appropriately.
     */
    class Parser
    {
    public:
        /**
         * @brief Parse Pangea source code into tokens
         *
         * Complete parsing pipeline that:
         * 1. Removes comments (preserving # in function arity notation)
         * 2. Tokenizes while preserving quoted string literals
         * 3. Returns clean token vector for interpreter consumption
         *
         * @param code The source code to parse
         * @return Vector of tokens ready for interpretation
         *
         * @example
         * Parser::parseCode("print \"hello world\" # comment")
         * // Returns: ["print", "\"hello world\""]
         *
         * Parser::parseCode("add#2 5 3 # arithmetic")
         * // Returns: ["add#2", "5", "3"]
         */
        static std::vector<std::string> parseCode(const std::string &code);

        /**
         * @brief Parse a token into a Value (number, string, boolean, null)
         *
         * Converts string tokens into typed Value objects:
         * - Numbers: "123" -> Value(123.0), "3.14" -> Value(3.14)
         * - Strings: "\"hello\"" -> Value("hello") (quotes removed)
         * - Booleans: "true" -> Value(true), "false" -> Value(false)
         * - Null: "null" -> Value() (null value)
         * - Others: Returns null Value for non-literals
         *
         * @param text The token to parse
         * @return Parsed Value or null Value if not a literal
         *
         * @example
         * parseValue("42")       // -> Value(42.0)
         * parseValue("\"text\"") // -> Value("text")
         * parseValue("true")     // -> Value(true)
         * parseValue("variable") // -> Value() (null, not a literal)
         */
        static Value parseValue(const std::string &text);

        /**
         * @brief Check if a token is a string literal
         *
         * Validates string literal format by checking:
         * - Minimum length of 2 characters
         * - Starts with double quote (")
         * - Ends with double quote (")
         *
         * @param text The token to check
         * @return true if it's a string literal (starts and ends with quotes)
         *
         * @example
         * isString("\"hello\"")   // -> true
         * isString("\"\"")        // -> true (empty string)
         * isString("hello")       // -> false
         * isString("\"incomplete") // -> false
         */
        static bool isString(const std::string &text);

    private:
        /**
         * @brief Tokenize code while preserving quoted strings
         *
         * Core tokenization algorithm that handles:
         *
         * **String Preservation Logic:**
         * - Tracks quote state with `inQuotes` flag
         * - Preserves all characters (including spaces) within quotes
         * - Maintains quotes as part of the token for later processing
         *
         * **Whitespace Handling:**
         * - Uses std::isspace() to identify delimiters outside quotes
         * - Treats spaces, tabs, newlines as token separators
         * - Ignores whitespace inside quoted strings
         *
         * **Error Handling:**
         * - Detects unterminated string literals
         * - Outputs warning to std::cerr for missing closing quotes
         * - Gracefully continues parsing despite errors
         *
         * **Token Building:**
         * - Accumulates characters in currentToken buffer
         * - Flushes complete tokens to result vector
         * - Handles empty tokens and edge cases
         *
         * @param code The code to tokenize
         * @return Vector of tokens with preserved string literals
         *
         * @example
         * tokenizeWithStringPreservation("hello \"world of code\" test")
         * // Returns: ["hello", "\"world of code\"", "test"]
         *
         * tokenizeWithStringPreservation("func \"unterminated)
         * // Outputs: "Error: Unterminated string literal"
         * // Returns: ["func", "\"unterminated"]
         */
        static std::vector<std::string> tokenizeWithStringPreservation(const std::string &code);

        /**
         * @brief Remove comments from a line while preserving # in function names
         *
         * Sophisticated comment removal that distinguishes between:
         *
         * **Function Arity Notation (Preserved):**
         * - Pattern: `functionName#number` (e.g., "add#2", "map#3")
         * - Detection: # preceded by non-whitespace character
         * - Behavior: Preserves the # as part of function identifier
         *
         * **Line Comments (Removed):**
         * - Pattern: `# comment text` (# preceded by whitespace or start of line)
         * - Behavior: Truncates line at comment start
         *
         * **String Context Awareness:**
         * - Tracks string literal boundaries with quote state
         * - Ignores # characters inside quoted strings
         * - Handles escape sequences within strings
         *
         * **Escape Sequence Handling:**
         * - Recognizes backslash escapes (\")
         * - Prevents escaped quotes from terminating strings
         * - Preserves escape sequences in output
         *
         * @param line The line to process
         * @return Line with comments removed, function notation preserved
         *
         * @example
         * removeComments("add#2 5 3 # calculate sum")
         * // Returns: "add#2 5 3 "
         *
         * removeComments("print \"text with # symbol\" # comment")
         * // Returns: "print \"text with # symbol\" "
         *
         * removeComments("# full line comment")
         * // Returns: ""
         *
         * removeComments("escaped \" # inside string \"")
         * // Returns: "escaped \" # inside string \""
         */
        static std::string removeComments(const std::string &line);
    };

} // namespace pangea
