#include "interpreter.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <cmath>

namespace pangea {

Interpreter::Interpreter() {
    initBuiltins();
}

void Interpreter::initBuiltins() {
    // Arithmetic operators
    registerBuiltin("plus", 2, [this](const std::vector<Value>& args) {
        return plus(args[0], args[1]);
    });
    
    registerBuiltin("minus", 2, [this](const std::vector<Value>& args) {
        return minus(args[0], args[1]);
    });
    
    registerBuiltin("times", 2, [this](const std::vector<Value>& args) {
        return times(args[0], args[1]);
    });
    
    registerBuiltin("divide", 2, [this](const std::vector<Value>& args) {
        return divide(args[0], args[1]);
    });
    
    registerBuiltin("power", 2, [this](const std::vector<Value>& args) {
        return power(args[0], args[1]);
    });
    
    // Comparison operators
    registerBuiltin("equal", 2, [this](const std::vector<Value>& args) {
        return equal(args[0], args[1]);
    });
    
    registerBuiltin("less", 2, [this](const std::vector<Value>& args) {
        return less(args[0], args[1]);
    });
    
    registerBuiltin("greater", 2, [this](const std::vector<Value>& args) {
        return greater(args[0], args[1]);
    });
    
    // Logical operators
    registerBuiltin("and", 2, [this](const std::vector<Value>& args) {
        return logicalAnd(args[0], args[1]);
    });
    
    registerBuiltin("or", 2, [this](const std::vector<Value>& args) {
        return logicalOr(args[0], args[1]);
    });
    
    registerBuiltin("not", 1, [this](const std::vector<Value>& args) {
        return logicalNot(args[0]);
    });
    
    // I/O operations
    registerBuiltin("print", 1, [this](const std::vector<Value>& args) {
        print(args[0]);
        return Value();
    });
    
    registerBuiltin("println", 1, [this](const std::vector<Value>& args) {
        println(args[0]);
        return Value();
    });
    
    registerBuiltin("input", 0, [this](const std::vector<Value>& args) {
        return input();
    });
    
    // Control flow
    registerBuiltin("if", 3, [this](const std::vector<Value>& args) {
        return ifCondition(args[0], args[1], args[2]);
    });
    
    registerBuiltin("times_loop", 2, [this](const std::vector<Value>& args) {
        return timesLoop(args[0], args[1]);
    });
    
    registerBuiltin("each", 2, [this](const std::vector<Value>& args) {
        return each(args[0], args[1]);
    });
    
    // Utility functions
    registerBuiltin("length", 1, [this](const std::vector<Value>& args) {
        return length(args[0]);
    });
    
    registerBuiltin("type", 1, [this](const std::vector<Value>& args) {
        return type(args[0]);
    });
    
    registerBuiltin("string", 1, [this](const std::vector<Value>& args) {
        return toString(args[0]);
    });
    
    registerBuiltin("number", 1, [this](const std::vector<Value>& args) {
        return toNumber(args[0]);
    });
    
    // Array/object operations
    registerBuiltin("get", 2, [this](const std::vector<Value>& args) {
        return get(args[0], args[1]);
    });
    
    registerBuiltin("set", 3, [this](const std::vector<Value>& args) {
        return set(args[0], args[1], args[2]);
    });
    
    registerBuiltin("array", 0, [this](const std::vector<Value>& args) {
        return Value(std::vector<Value>{});
    });
    
    registerBuiltin("object", 0, [this](const std::vector<Value>& args) {
        return Value(std::unordered_map<std::string, Value>{});
    });
}

void Interpreter::registerBuiltin(const std::string& name, int arity, BuiltinFunction func) {
    namespace_[name] = FunctionEntry(name, arity, std::move(func));
    arities_[name] = namespace_[name];
}

Value Interpreter::execute(const std::string& code) {
    // Parse the code
    Parser parser;
    words_ = Parser::parseCode(code);
    
    if (words_.empty()) {
        return Value();
    }
    
    // Calculate phrase lengths
    phraseLengths_.clear();
    phraseLengths_.resize(words_.size());
    calculatePhraseLengths();
    
    // Execute the main phrase
    return wordExec(0, static_cast<int>(words_.size()) - 1);
}

void Interpreter::calculatePhraseLengths() {
    for (int i = static_cast<int>(words_.size()) - 1; i >= 0; --i) {
        phraseLengths_[i] = phraseLength(i);
    }
}

int Interpreter::phraseLength(int start) {
    if (start >= static_cast<int>(words_.size())) {
        return 0;
    }
    
    const std::string& word = words_[start];
    
    // Check if it's a function call
    auto it = arities_.find(word);
    if (it != arities_.end()) {
        int arity = it->second.getArity();
        int totalLength = 1; // The function name itself
        
        // Add lengths of all parameters
        int paramStart = start + 1;
        for (int i = 0; i < arity && paramStart < static_cast<int>(words_.size()); ++i) {
            int paramLength = phraseLength(paramStart);
            totalLength += paramLength;
            paramStart += paramLength;
        }
        
        return totalLength;
    }
    
    // For literals and variables, length is 1
    return 1;
}

Value Interpreter::wordExec(int start, int end) {
    if (start > end || start >= static_cast<int>(words_.size())) {
        return Value();
    }
    
    const std::string& word = words_[start];
    
    // Check if it's a function call
    auto it = namespace_.find(word);
    if (it != namespace_.end()) {
        const FunctionEntry& entry = it->second;
        int arity = entry.getArity();
        
        // Collect arguments
        std::vector<Value> args;
        args.reserve(arity);
        
        int paramStart = start + 1;
        for (int i = 0; i < arity && paramStart <= end; ++i) {
            int paramLength = phraseLengths_[paramStart];
            int paramEnd = paramStart + paramLength - 1;
            
            if (paramEnd <= end) {
                args.push_back(wordExec(paramStart, paramEnd));
            }
            
            paramStart += paramLength;
        }
        
        // Execute the function
        return entry.invoke(args);
    }
    
    // Try to parse as a literal
    return parseLiteral(word);
}

Value Interpreter::parseLiteral(const std::string& word) {
    // Try to parse as number
    try {
        size_t pos;
        double value = std::stod(word, &pos);
        if (pos == word.length()) {
            return Value(value);
        }
    } catch (const std::exception&) {
        // Not a number, continue
    }
    
    // Try to parse as string (quoted)
    if (word.length() >= 2 && word.front() == '"' && word.back() == '"') {
        return Value(word.substr(1, word.length() - 2));
    }
    
    // Try to parse as boolean
    if (word == "true") {
        return Value(true);
    } else if (word == "false") {
        return Value(false);
    }
    
    // Default to string (unquoted identifier)
    return Value(word);
}

// Built-in function implementations
Value Interpreter::plus(const Value& a, const Value& b) {
    if (a.isNumber() && b.isNumber()) {
        return Value(a.asNumber() + b.asNumber());
    } else {
        return Value(a.asString() + b.asString());
    }
}

Value Interpreter::minus(const Value& a, const Value& b) {
    return Value(a.asNumber() - b.asNumber());
}

Value Interpreter::times(const Value& a, const Value& b) {
    return Value(a.asNumber() * b.asNumber());
}

Value Interpreter::divide(const Value& a, const Value& b) {
    double divisor = b.asNumber();
    if (divisor == 0.0) {
        throw std::runtime_error("Division by zero");
    }
    return Value(a.asNumber() / divisor);
}

Value Interpreter::power(const Value& a, const Value& b) {
    return Value(std::pow(a.asNumber(), b.asNumber()));
}

Value Interpreter::equal(const Value& a, const Value& b) {
    return Value(a == b);
}

Value Interpreter::less(const Value& a, const Value& b) {
    if (a.isNumber() && b.isNumber()) {
        return Value(a.asNumber() < b.asNumber());
    } else {
        return Value(a.asString() < b.asString());
    }
}

Value Interpreter::greater(const Value& a, const Value& b) {
    if (a.isNumber() && b.isNumber()) {
        return Value(a.asNumber() > b.asNumber());
    } else {
        return Value(a.asString() > b.asString());
    }
}

Value Interpreter::logicalAnd(const Value& a, const Value& b) {
    return Value(a.asBoolean() && b.asBoolean());
}

Value Interpreter::logicalOr(const Value& a, const Value& b) {
    return Value(a.asBoolean() || b.asBoolean());
}

Value Interpreter::logicalNot(const Value& a) {
    return Value(!a.asBoolean());
}

void Interpreter::print(const Value& value) {
    std::cout << value.asString();
}

void Interpreter::println(const Value& value) {
    std::cout << value.asString() << std::endl;
}

Value Interpreter::input() {
    std::string line;
    std::getline(std::cin, line);
    return Value(line);
}

Value Interpreter::ifCondition(const Value& condition, const Value& thenValue, const Value& elseValue) {
    return condition.asBoolean() ? thenValue : elseValue;
}

Value Interpreter::timesLoop(const Value& count, const Value& body) {
    int times = static_cast<int>(count.asNumber());
    Value result;
    
    for (int i = 0; i < times; ++i) {
        // In a real implementation, body would be executable code
        // For now, just return the body value
        result = body;
    }
    
    return result;
}

Value Interpreter::each(const Value& collection, const Value& body) {
    // Simplified implementation - in real version would iterate over collection
    // and execute body for each element
    return body;
}

Value Interpreter::length(const Value& value) {
    if (value.isString()) {
        return Value(static_cast<double>(value.asString().length()));
    } else if (value.isArray()) {
        return Value(static_cast<double>(value.asArray().size()));
    } else if (value.isObject()) {
        return Value(static_cast<double>(value.asObject().size()));
    }
    return Value(0.0);
}

Value Interpreter::type(const Value& value) {
    if (value.isNumber()) return Value("number");
    if (value.isString()) return Value("string");
    if (value.isBoolean()) return Value("boolean");
    if (value.isArray()) return Value("array");
    if (value.isObject()) return Value("object");
    return Value("unknown");
}

Value Interpreter::toString(const Value& value) {
    return Value(value.asString());
}

Value Interpreter::toNumber(const Value& value) {
    return Value(value.asNumber());
}

Value Interpreter::get(const Value& collection, const Value& key) {
    if (collection.isArray() && key.isNumber()) {
        const auto& arr = collection.asArray();
        int index = static_cast<int>(key.asNumber());
        if (index >= 0 && index < static_cast<int>(arr.size())) {
            return arr[index];
        }
    } else if (collection.isObject() && key.isString()) {
        const auto& obj = collection.asObject();
        auto it = obj.find(key.asString());
        if (it != obj.end()) {
            return it->second;
        }
    }
    return Value();
}

Value Interpreter::set(const Value& collection, const Value& key, const Value& value) {
    // This would modify the collection in place in a real implementation
    // For now, just return the value
    return value;
}

} // namespace pangea
