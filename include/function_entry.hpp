#pragma once

#include "value.hpp"
#include <functional>
#include <vector>
#include <string>
#include <memory>

namespace pangea
{

    class Interpreter; // Forward declaration

    /**
     * @brief Function signature for native C++ functions (original API)
     */
    using NativeFunction = std::function<Value(const std::vector<int> &, Interpreter *)>;

    /**
     * @brief Function signature for built-in functions (simplified API)
     */
    using BuiltinFunction = std::function<Value(const std::vector<Value> &)>;

    /**
     * @brief Represents a function entry in the namespace registry
     *
     * Equivalent to the Java FunctionEntry class, stores function metadata
     * and implementation for both built-in and user-defined functions
     */
    class FunctionEntry
    {
    public:
        enum class FunctionType
        {
            Native,  // Built-in C++ function
            UserDef, // User-defined function (def)
            Lambda,  // Lambda function (lambda#n)
            Method   // Method function (method#n)
        };

        enum class OperatorType
        {
            Prefix,
            Infix,
            Postfix
        };

    private:
        int arity_;
        OperatorType operatorType_;
        NativeFunction function_;
        BuiltinFunction builtinFunction_; // For simplified builtin functions
        std::vector<std::string> aliases_;
        int wordIndex_;                       // For user-defined functions
        std::shared_ptr<Value> boundContext_; // For future object method binding
        bool isLambda_;
        int methodArity_; // For object methods: arity excluding 'this'
        bool isMethod_;
        FunctionType functionType_;
        bool isBuiltin_; // Whether this uses BuiltinFunction

    public:
        // Constructors
        FunctionEntry();
        FunctionEntry(int arity, OperatorType operatorType, NativeFunction function);
        FunctionEntry(const std::string &name, int arity, BuiltinFunction function); // For builtin functions

        // Copy and move constructors/operators
        FunctionEntry(const FunctionEntry &other) = default;
        FunctionEntry(FunctionEntry &&other) noexcept = default;
        FunctionEntry &operator=(const FunctionEntry &other) = default;
        FunctionEntry &operator=(FunctionEntry &&other) noexcept = default;

        // Getters and setters
        int getArity() const { return arity_; }
        void setArity(int arity) { arity_ = arity; }

        OperatorType getOperatorType() const { return operatorType_; }
        void setOperatorType(OperatorType operatorType) { operatorType_ = operatorType; }

        const NativeFunction &getFunction() const { return function_; }
        void setFunction(NativeFunction function) { function_ = std::move(function); }

        const std::vector<std::string> &getAliases() const { return aliases_; }
        void setAliases(std::vector<std::string> aliases) { aliases_ = std::move(aliases); }

        int getWordIndex() const { return wordIndex_; }
        void setWordIndex(int wordIndex) { wordIndex_ = wordIndex; }

        std::shared_ptr<Value> getBoundContext() const { return boundContext_; }
        void setBoundContext(std::shared_ptr<Value> boundContext) { boundContext_ = std::move(boundContext); }

        bool getIsLambda() const { return isLambda_; }
        void setIsLambda(bool isLambda) { isLambda_ = isLambda; }

        int getMethodArity() const { return methodArity_; }
        void setMethodArity(int methodArity) { methodArity_ = methodArity; }

        bool getIsMethod() const { return isMethod_; }
        void setIsMethod(bool isMethod) { isMethod_ = isMethod; }

        FunctionType getFunctionType() const { return functionType_; }
        void setFunctionType(FunctionType functionType) { functionType_ = functionType; }

        bool getIsBuiltin() const { return isBuiltin_; }

        // Utility methods
        int getEffectiveArity() const;
        int getInternalArity() const;
        std::string getOperatorTypeString() const;
        std::string getFunctionTypeString() const;

        // Function call interface
        Value call(const std::vector<int> &params, Interpreter *interpreter) const;
        Value invoke(const std::vector<Value> &args) const; // For builtin functions
    };

} // namespace pangea
