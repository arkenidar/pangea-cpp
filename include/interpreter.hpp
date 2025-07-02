#pragma once

#include "value.hpp"
#include "function_entry.hpp"
#include "parser.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <stack>
#include <memory>
#include <functional>

namespace pangea {

// Forward declaration
class Interpreter;

// Type alias for built-in functions
using BuiltinFunction = std::function<Value(const std::vector<Value>&)>;

/**
 * @brief Stack frame for function calls
 */
struct StackFrame {
    std::vector<Value> args;

    StackFrame() = default;
    explicit StackFrame(std::vector<Value> args) : args(std::move(args)) {}
};

/**
 * @brief Stack frame for iteration contexts (each loops)
 */
struct IterationFrame {
    bool stop = false;
    std::string key;
    Value value;

    IterationFrame() = default;
};

/**
 * @brief Main Pangea interpreter class
 * 
 * Core interpreter implementing the phrase-building parsing mechanism
 * and execution engine. Ported from the Java Interpreter class.
 */
class Interpreter {
private:
    std::vector<std::string> words_;
    std::vector<int> phraseLengths_;
    std::unordered_map<std::string, FunctionEntry> namespace_;
    std::unordered_map<std::string, FunctionEntry> arities_;
    std::stack<StackFrame> callStack_;
    std::stack<int> timesStack_;
    std::stack<IterationFrame> eachStack_;

public:
    // Constructor
    Interpreter();

    // Destructor
    ~Interpreter() = default;

    // Copy and move constructors (deleted - interpreter should be unique)
    Interpreter(const Interpreter&) = delete;
    Interpreter& operator=(const Interpreter&) = delete;
    Interpreter(Interpreter&&) = default;
    Interpreter& operator=(Interpreter&&) = default;

    /**
     * @brief Execute Pangea source code
     * @param code The source code to execute
     * @return The result of execution
     */
    Value execute(const std::string& code);

    // Public accessors for testing
    const std::vector<std::string>& getWords() const { return words_; }
    const std::unordered_map<std::string, FunctionEntry>& getNamespace() const { return namespace_; }

private:
    /**
     * @brief Initialize built-in functions
     */
    void initBuiltins();

    /**
     * @brief Register a built-in function
     */
    void registerBuiltin(const std::string& name, int arity, BuiltinFunction func);

    /**
     * @brief Calculate phrase lengths for all words
     */
    void calculatePhraseLengths();

    /**
     * @brief Calculate phrase length for parsing
     * @param start Starting word index
     * @return Length of the phrase
     */
    int phraseLength(int start);

    /**
     * @brief Execute a word/phrase range
     * @param start Starting word index
     * @param end Ending word index
     * @return Result of execution
     */
    Value wordExec(int start, int end);

    /**
     * @brief Parse a literal value from a string
     * @param word The word to parse
     * @return Parsed value
     */
    Value parseLiteral(const std::string& word);

    // Built-in function implementations
    Value plus(const Value& a, const Value& b);
    Value minus(const Value& a, const Value& b);
    Value times(const Value& a, const Value& b);
    Value divide(const Value& a, const Value& b);
    Value power(const Value& a, const Value& b);

    Value equal(const Value& a, const Value& b);
    Value less(const Value& a, const Value& b);
    Value greater(const Value& a, const Value& b);

    Value logicalAnd(const Value& a, const Value& b);
    Value logicalOr(const Value& a, const Value& b);
    Value logicalNot(const Value& a);

    void print(const Value& value);
    void println(const Value& value);
    Value input();

    Value ifCondition(const Value& condition, const Value& thenValue, const Value& elseValue);
    Value timesLoop(const Value& count, const Value& body);
    Value each(const Value& collection, const Value& body);

    Value length(const Value& value);
    Value type(const Value& value);
    Value toString(const Value& value);
    Value toNumber(const Value& value);

    Value get(const Value& collection, const Value& key);
    Value set(const Value& collection, const Value& key, const Value& value);
};

} // namespace pangea
