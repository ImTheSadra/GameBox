#pragma once

#include <string>
#include <variant>

using namespace std;

struct LuaValue
{
    enum class Type
    {
        NIL,
        NUMBER,
        STRING,
        BOOLEAN,
        INTEGER
    };

    Type type;

    variant<
        monostate,
        double,
        string,
        bool,
        int>
        value;

    LuaValue() : type(Type::NIL), value(monostate{}) {}

    LuaValue(double num) : type(Type::NUMBER), value(num) {}

    LuaValue(const string &str) : type(Type::STRING), value(str) {}

    LuaValue(bool b) : type(Type::BOOLEAN), value(b) {}

    LuaValue(int Integer) : type(Type::INTEGER), value(Integer) {}

    bool isNil() const { return type == Type::NIL; }
    double getNumber() const { return get<double>(value); }
    string getString() const { return get<string>(value); }
    bool getBoolean() const { return get<bool>(value); }
    int getInt() const { return get<int>(value); }
};