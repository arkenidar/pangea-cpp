#include "value.hpp"
#include "function_entry.hpp"
#include <stdexcept>
#include <sstream>

namespace pangea {

// Constructors
Value::Value() : data_(std::monostate{}), type_(Type::Null) {}

Value::Value(double value) : data_(value), type_(Type::Number) {}

Value::Value(const std::string& value) : data_(value), type_(Type::String) {}

Value::Value(bool value) : data_(value), type_(Type::Boolean) {}

Value::Value(const std::vector<Value>& value) : data_(value), type_(Type::Array) {}

Value::Value(const std::unordered_map<std::string, Value>& value) : data_(value), type_(Type::Object) {}

Value::Value(std::shared_ptr<FunctionEntry> function) : data_(function), type_(Type::Function) {}

// Value getters with type checking
double Value::asNumber() const {
    if (type_ != Type::Number) {
        throw std::runtime_error("Value is not a number");
    }
    return std::get<double>(data_);
}

const std::string& Value::asString() const {
    if (type_ != Type::String) {
        throw std::runtime_error("Value is not a string");
    }
    return std::get<std::string>(data_);
}

bool Value::asBoolean() const {
    if (type_ != Type::Boolean) {
        throw std::runtime_error("Value is not a boolean");
    }
    return std::get<bool>(data_);
}

const std::vector<Value>& Value::asArray() const {
    if (type_ != Type::Array) {
        throw std::runtime_error("Value is not an array");
    }
    return std::get<std::vector<Value>>(data_);
}

const std::unordered_map<std::string, Value>& Value::asObject() const {
    if (type_ != Type::Object) {
        throw std::runtime_error("Value is not an object");
    }
    return std::get<std::unordered_map<std::string, Value>>(data_);
}

std::shared_ptr<FunctionEntry> Value::asFunction() const {
    if (type_ != Type::Function) {
        throw std::runtime_error("Value is not a function");
    }
    return std::get<std::shared_ptr<FunctionEntry>>(data_);
}

// Mutable getters
std::vector<Value>& Value::asArrayMutable() {
    if (type_ != Type::Array) {
        throw std::runtime_error("Value is not an array");
    }
    return std::get<std::vector<Value>>(data_);
}

std::unordered_map<std::string, Value>& Value::asObjectMutable() {
    if (type_ != Type::Object) {
        throw std::runtime_error("Value is not an object");
    }
    return std::get<std::unordered_map<std::string, Value>>(data_);
}

// Truthiness evaluation (JavaScript-like)
bool Value::isTruthy() const {
    switch (type_) {
        case Type::Null:
            return false;
        case Type::Number:
            return asNumber() != 0.0;
        case Type::String:
            return !asString().empty();
        case Type::Boolean:
            return asBoolean();
        case Type::Array:
            return !asArray().empty();
        case Type::Object:
            return !asObject().empty();
        case Type::Function:
            return asFunction() != nullptr;
        default:
            return false;
    }
}

// String conversion
std::string Value::toString() const {
    switch (type_) {
        case Type::Null:
            return "null";
        case Type::Number: {
            double num = asNumber();
            if (num == static_cast<long>(num)) {
                return std::to_string(static_cast<long>(num));
            }
            return std::to_string(num);
        }
        case Type::String:
            return asString();
        case Type::Boolean:
            return asBoolean() ? "true" : "false";
        case Type::Array: {
            std::ostringstream oss;
            oss << "[";
            const auto& arr = asArray();
            for (size_t i = 0; i < arr.size(); ++i) {
                if (i > 0) oss << ", ";
                oss << arr[i].toString();
            }
            oss << "]";
            return oss.str();
        }
        case Type::Object: {
            std::ostringstream oss;
            oss << "{";
            const auto& obj = asObject();
            bool first = true;
            for (const auto& [key, value] : obj) {
                if (!first) oss << ", ";
                oss << "\"" << key << "\": " << value.toString();
                first = false;
            }
            oss << "}";
            return oss.str();
        }
        case Type::Function: {
            auto func = asFunction();
            return "[Function:" + std::to_string(func ? func->getArity() : 0) + "]";
        }
        default:
            return "undefined";
    }
}

// Print utility
void Value::print(std::ostream& os) const {
    os << toString();
}

// Comparison operators
bool Value::operator==(const Value& other) const {
    if (type_ != other.type_) {
        return false;
    }

    switch (type_) {
        case Type::Null:
            return true;
        case Type::Number:
            return asNumber() == other.asNumber();
        case Type::String:
            return asString() == other.asString();
        case Type::Boolean:
            return asBoolean() == other.asBoolean();
        case Type::Array:
            return asArray() == other.asArray();
        case Type::Object:
            return asObject() == other.asObject();
        case Type::Function:
            return asFunction() == other.asFunction();
        default:
            return false;
    }
}

// Stream operator
std::ostream& operator<<(std::ostream& os, const Value& value) {
    value.print(os);
    return os;
}

} // namespace pangea
