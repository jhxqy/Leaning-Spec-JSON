//
//  Document.hpp
//  Json
//
//  Created by 贾皓翔 on 2019/10/23.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#ifndef Document_hpp
#define Document_hpp

#include <stdio.h>
#include <string>
#include <memory>
#include "json.hpp"
namespace JSON{
enum class ValueType{
    Null,Bool,Object,Array,String,Int,Double
};
struct Value;
struct Member{
    std::string key;
    Value *value;
    ~Member();
};
struct Value{
private:
    
    union{
        std::string *str;
        Member* members;
        Value* values;
        int i;
        double d;
        bool boolValue;
    };
    union{
        size_t size;
    };
    union{
        size_t capacity;
    };
    void destroy();
public:
    ~Value();
    
    
    
    bool isString();
    bool isNumber();
    bool isBool();
    bool isArray();
    bool isObject();
    bool isNull();
    bool isInt();
    bool isDouble;
    ValueType valueType_;
    Value& operator[](size_t index);
    Member& operator[](const std::string &key);
    int getInt();
    double getDouble();
    std::string getString();
    bool getBool();
    
    void setNumber(int i);
    void setNumber(double d);
    void setInt(int i);
    void setDouble(double d);
    void setNull();
    void setBool(bool b);
    void setArray(Value *values,size_t size);
    void setObject(Member *members,size_t size);
    void setString(const std::string &s);
    
};

/*
 S -> array | object
 array -> [ values ]
 object -> { members }
 values -> non-empty-values | ε
 non-empty-values -> value addition-values
 addition-values -> ε | , non-empty-values
 members -> non-empty-members | ε
 non-empty-members -> member addition-members
 addition-members -> ε | , non-empty-members
 member -> STRING : value
 value -> STRING | NUMBER | NULL | BOOLEAN | object | array
 */

class Document{
    std::shared_ptr<Imple::Tokenizer> tokens;
    Imple::Token *nowToken;
    Imple::Token* match(Imple::Token *t);
    Imple::Token* match(Imple::TokenType t);
    
    void S();
    void array();
    void object();
    void Values();
    void nonEmptyValues();
    void additionValues();
    void Members();
    void nonEmptyMembers();
    void additionMembers();
    void Member();
    void Value();
    
public:
    Document();
    void Parse(const std::string &s);
    
    
};

}
#endif /* Document_hpp */
