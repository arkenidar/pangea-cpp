#include <catch2/catch_test_macros.hpp>
#include "interpreter.hpp"
#include "value.hpp"
#include "parser.hpp"

using namespace pangea;

TEST_CASE("Value construction and type checking", "[value]") {
    SECTION("Number values") {
        Value v(42.0);
        REQUIRE(v.isNumber());
        REQUIRE_FALSE(v.isString());
        REQUIRE_FALSE(v.isBool());
        REQUIRE(v.asNumber() == 42.0);
    }
    
    SECTION("String values") {
        Value v("hello");
        REQUIRE(v.isString());
        REQUIRE_FALSE(v.isNumber());
        REQUIRE_FALSE(v.isBool());
        REQUIRE(v.asString() == "hello");
    }
    
    SECTION("Boolean values") {
        Value v(true);
        REQUIRE(v.isBoolean());
        REQUIRE_FALSE(v.isNumber());
        REQUIRE_FALSE(v.isString());
        REQUIRE(v.asBoolean() == true);
    }
    
    SECTION("Array values") {
        std::vector<Value> arr = {Value(1.0), Value("test"), Value(true)};
        Value v(arr);
        REQUIRE(v.isArray());
        REQUIRE(v.asArray().size() == 3);
    }
    
    SECTION("Object values") {
        std::unordered_map<std::string, Value> obj;
        obj["key1"] = Value(42.0);
        obj["key2"] = Value("value");
        Value v(obj);
        REQUIRE(v.isObject());
        REQUIRE(v.asObject().size() == 2);
    }
}

TEST_CASE("Value equality", "[value]") {
    SECTION("Number equality") {
        Value a(42.0);
        Value b(42.0);
        Value c(43.0);
        REQUIRE(a == b);
        REQUIRE_FALSE(a == c);
    }
    
    SECTION("String equality") {
        Value a("hello");
        Value b("hello");
        Value c("world");
        REQUIRE(a == b);
        REQUIRE_FALSE(a == c);
    }
    
    SECTION("Boolean equality") {
        Value a(true);
        Value b(true);
        Value c(false);
        REQUIRE(a == b);
        REQUIRE_FALSE(a == c);
    }
}

TEST_CASE("Parser tokenization", "[parser]") {
    Parser parser;
    
    SECTION("Simple tokens") {
        auto tokens = parser.tokenize("hello world");
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0] == "hello");
        REQUIRE(tokens[1] == "world");
    }
    
    SECTION("Quoted strings") {
        auto tokens = parser.tokenize("print \"hello world\"");
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0] == "print");
        REQUIRE(tokens[1] == "\"hello world\"");
    }
    
    SECTION("Numbers and operators") {
        auto tokens = parser.tokenize("plus 2 3");
        REQUIRE(tokens.size() == 3);
        REQUIRE(tokens[0] == "plus");
        REQUIRE(tokens[1] == "2");
        REQUIRE(tokens[2] == "3");
    }
    
    SECTION("Comment removal") {
        auto tokens = parser.tokenize("hello # this is a comment\nworld");
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0] == "hello");
        REQUIRE(tokens[1] == "world");
    }
}

TEST_CASE("Basic interpreter execution", "[interpreter]") {
    Interpreter interpreter;
    
    SECTION("Arithmetic operations") {
        Value result = interpreter.execute("plus 2 3");
        REQUIRE(result.isNumber());
        REQUIRE(result.asNumber() == 5.0);
        
        result = interpreter.execute("minus 10 4");
        REQUIRE(result.isNumber());
        REQUIRE(result.asNumber() == 6.0);
        
        result = interpreter.execute("times 3 4");
        REQUIRE(result.isNumber());
        REQUIRE(result.asNumber() == 12.0);
        
        result = interpreter.execute("divide 15 3");
        REQUIRE(result.isNumber());
        REQUIRE(result.asNumber() == 5.0);
    }
    
    SECTION("String operations") {
        Value result = interpreter.execute("plus \"hello\" \" world\"");
        REQUIRE(result.isString());
        REQUIRE(result.asString() == "hello world");
    }
    
    SECTION("Comparison operations") {
        Value result = interpreter.execute("equal 5 5");
        REQUIRE(result.isBoolean());
        REQUIRE(result.asBoolean() == true);
        
        result = interpreter.execute("less 3 5");
        REQUIRE(result.isBoolean());
        REQUIRE(result.asBoolean() == true);
        
        result = interpreter.execute("greater 7 5");
        REQUIRE(result.isBoolean());
        REQUIRE(result.asBoolean() == true);
    }
    
    SECTION("Logical operations") {
        Value result = interpreter.execute("and true false");
        REQUIRE(result.isBoolean());
        REQUIRE(result.asBoolean() == false);
        
        result = interpreter.execute("or true false");
        REQUIRE(result.isBoolean());
        REQUIRE(result.asBoolean() == true);
        
        result = interpreter.execute("not false");
        REQUIRE(result.isBoolean());
        REQUIRE(result.asBoolean() == true);
    }
    
    SECTION("Nested expressions") {
        Value result = interpreter.execute("plus times 2 3 4");
        REQUIRE(result.isNumber());
        REQUIRE(result.asNumber() == 10.0); // (2 * 3) + 4
    }
}

TEST_CASE("Type conversion", "[interpreter]") {
    Interpreter interpreter;
    
    SECTION("String to number") {
        Value result = interpreter.execute("number \"42\"");
        REQUIRE(result.isNumber());
        REQUIRE(result.asNumber() == 42.0);
    }
    
    SECTION("Number to string") {
        Value result = interpreter.execute("string 42");
        REQUIRE(result.isString());
        REQUIRE(result.asString() == "42");
    }
    
    SECTION("Type detection") {
        Value result = interpreter.execute("type 42");
        REQUIRE(result.isString());
        REQUIRE(result.asString() == "number");
        
        result = interpreter.execute("type \"hello\"");
        REQUIRE(result.isString());
        REQUIRE(result.asString() == "string");
        
        result = interpreter.execute("type true");
        REQUIRE(result.isString());
        REQUIRE(result.asString() == "boolean");
    }
}

TEST_CASE("Error handling", "[interpreter]") {
    Interpreter interpreter;
    
    SECTION("Division by zero") {
        REQUIRE_THROWS_AS(interpreter.execute("divide 5 0"), std::runtime_error);
    }
    
    SECTION("Invalid number conversion") {
        // This should handle gracefully or throw a specific error
        Value result = interpreter.execute("number \"not_a_number\"");
        // The behavior here depends on implementation - could be 0 or throw
    }
}
