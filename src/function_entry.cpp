#include "function_entry.hpp"
#include "interpreter.hpp"
#include <stdexcept>

namespace pangea {

// Constructors
FunctionEntry::FunctionEntry() 
    : arity_(0)
    , operatorType_(OperatorType::Prefix)
    , function_(nullptr)
    , wordIndex_(-1)
    , boundContext_(nullptr)
    , isLambda_(false)
    , methodArity_(-1)
    , isMethod_(false)
    , functionType_(FunctionType::Native)
    , isBuiltin_(false)
{
}

FunctionEntry::FunctionEntry(int arity, OperatorType operatorType, NativeFunction function)
    : arity_(arity)
    , operatorType_(operatorType)
    , function_(std::move(function))
    , wordIndex_(-1)
    , boundContext_(nullptr)
    , isLambda_(false)
    , methodArity_(-1)
    , isMethod_(false)
    , functionType_(FunctionType::Native)
    , isBuiltin_(false)
{
}

FunctionEntry::FunctionEntry(const std::string& name, int arity, BuiltinFunction function)
    : arity_(arity)
    , operatorType_(OperatorType::Prefix)
    , builtinFunction_(std::move(function))
    , wordIndex_(-1)
    , boundContext_(nullptr)
    , isLambda_(false)
    , methodArity_(-1)
    , isMethod_(false)
    , functionType_(FunctionType::Native)
    , isBuiltin_(true)
{
}

// Utility methods
int FunctionEntry::getEffectiveArity() const {
    switch (functionType_) {
        case FunctionType::Lambda:
            // For lambda#n in object: user arity includes 'this', method arity excludes it
            return isMethod_ && methodArity_ >= 0 ? methodArity_ : arity_;
        case FunctionType::Method:
            // For method#n: declared arity excludes 'this' (cleaner)
            return arity_;
        case FunctionType::UserDef:
        case FunctionType::Native:
        default:
            return arity_;
    }
}

int FunctionEntry::getInternalArity() const {
    switch (functionType_) {
        case FunctionType::Lambda:
            return arity_; // Includes 'this'
        case FunctionType::Method:
            return arity_ + 1; // Add 'this'
        case FunctionType::UserDef:
        case FunctionType::Native:
        default:
            return arity_;
    }
}

std::string FunctionEntry::getOperatorTypeString() const {
    switch (operatorType_) {
        case OperatorType::Prefix:
            return "prefix";
        case OperatorType::Infix:
            return "infix";
        case OperatorType::Postfix:
            return "postfix";
        default:
            return "unknown";
    }
}

std::string FunctionEntry::getFunctionTypeString() const {
    switch (functionType_) {
        case FunctionType::Native:
            return "built-in function";
        case FunctionType::UserDef:
            return "user-defined function";
        case FunctionType::Lambda:
            return isMethod_ ? "lambda method" : "lambda function";
        case FunctionType::Method:
            return "method";
        default:
            return "function";
    }
}

// Function call interface
Value FunctionEntry::call(const std::vector<int>& params, Interpreter* interpreter) const {
    if (!function_) {
        throw std::runtime_error("Function implementation is null");
    }
    
    if (interpreter == nullptr) {
        throw std::runtime_error("Interpreter reference is null");
    }

    return function_(params, interpreter);
}

Value FunctionEntry::invoke(const std::vector<Value>& args) const {
    if (!isBuiltin_ || !builtinFunction_) {
        throw std::runtime_error("Function is not a builtin function or implementation is null");
    }
    
    return builtinFunction_(args);
}

} // namespace pangea
