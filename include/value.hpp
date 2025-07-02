#pragma once

#include <variant>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>

namespace pangea
{

    class FunctionEntry; // Forward declaration

    /**
     * @brief Represents all possible values in the Pangea language
     *
     * Uses std::variant for type-safe value storage, equivalent to the Java Value class
     */
    class Value
    {
    public:
        enum class Type
        {
            Null,
            Number,
            String,
            Boolean,
            Array,
            Object,
            Function
        };

    private:
        using ValueVariant = std::variant<
            std::monostate,                         // Null
            double,                                 // Number
            std::string,                            // String
            bool,                                   // Boolean
            std::vector<Value>,                     // Array
            std::unordered_map<std::string, Value>, // Object
            std::shared_ptr<FunctionEntry>          // Function
            >;

        ValueVariant data_;
        Type type_;

    public:
        // Constructors
        Value();
        explicit Value(double value);
        explicit Value(const std::string &value);
        explicit Value(bool value);
        explicit Value(const std::vector<Value> &value);
        explicit Value(const std::unordered_map<std::string, Value> &value);
        explicit Value(std::shared_ptr<FunctionEntry> function);

        // Copy and move constructors/operators
        Value(const Value &other) = default;
        Value(Value &&other) noexcept = default;
        Value &operator=(const Value &other) = default;
        Value &operator=(Value &&other) noexcept = default;

        // Type checkers
        bool isNull() const { return type_ == Type::Null; }
        bool isNumber() const { return type_ == Type::Number; }
        bool isString() const { return type_ == Type::String; }
        bool isBoolean() const { return type_ == Type::Boolean; }
        bool isArray() const { return type_ == Type::Array; }
        bool isObject() const { return type_ == Type::Object; }
        bool isFunction() const { return type_ == Type::Function; }

        Type getType() const { return type_; }

        // Value getters (with type checking)
        double asNumber() const;
        const std::string &asString() const;
        bool asBoolean() const;
        const std::vector<Value> &asArray() const;
        const std::unordered_map<std::string, Value> &asObject() const;
        std::shared_ptr<FunctionEntry> asFunction() const;

        // Mutable getters for modification
        std::vector<Value> &asArrayMutable();
        std::unordered_map<std::string, Value> &asObjectMutable();

        // Truthiness evaluation (like JavaScript)
        bool isTruthy() const;

        // String conversion
        std::string toString() const;

        // Utility methods
        void print(std::ostream &os = std::cout) const;

        // Comparison operators
        bool operator==(const Value &other) const;
        bool operator!=(const Value &other) const { return !(*this == other); }
    };

    // Stream operator for easy printing
    std::ostream &operator<<(std::ostream &os, const Value &value);

} // namespace pangea
