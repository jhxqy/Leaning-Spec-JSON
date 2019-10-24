//
//  Document.hpp
//  Json
//
//  Created by 贾皓翔 on 2019/10/23.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#ifndef Json_hpp
#define Json_hpp

#include <stdio.h>
#include <string>
#include <memory>
#include <vector>
#include "Tokenizer.hpp"
#include <vector>
namespace JSON{
enum class ValueType{
    Null,Bool,Object,Array,String,Int,Double
};
struct Value;
struct Member{
    std::string key;
    Member(const Member&m);
    Member(){}
    Value *value;
    ~Member();
};
class Document;
class Value{
private:
    union{
        std::string *str;
        std::vector<Member*> *members;
        std::vector<Value*> *values;
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
    friend class Document;
    Value(ValueType t,const std::string &s):valueType_(t){
        str=new std::string(s);
    }
    Value(ValueType t,std::vector<Member*> m):valueType_(t),members(new std::vector<Member*>()){
        for(auto i:m){
            members->push_back(new Member(*i));
        }

    }
    Value(ValueType t,std::vector<Value*> v):valueType_(t),values(new std::vector<Value*>(v)){
        for(auto i:v){
            values->push_back(new Value(*i));
        }
    }
    Value(ValueType t,int i):valueType_(t){
        this->i=i;
    }
    Value(ValueType t,double d):valueType_(t){
        this->d=d;
    }
    Value(ValueType t,bool b):valueType_(t){
        this->boolValue=b;
    }
    Value(ValueType t):valueType_(t){
        this->d=0;
    }
    Value(const Value& v);
        
public:
    
    ~Value();
    
    
    
    bool isString();
    bool isNumber();
    bool isBool();
    bool isArray();
    bool isObject();
    bool isNull();
    bool isInt();
    bool isDouble();
    
    ValueType valueType_;
    
    
    Value& operator[](size_t index);
    Value& operator[](const std::string &key);
    
    std::list<std::string> getKeyLists();
    
    int getInt();
    double getDouble();
    std::string getString();
    bool getBool();
    size_t getArraySize();
    
    void setNumber(int i);
    void setNumber(double d);
    void setInt(int i);
    void setDouble(double d);
    void setNull();
    void setBool(bool b);
    void setArray(std::vector<Value*> values);
    void setObject(std::vector<Member*> members);
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
    
    Value* S();
    Value* array();
    Value* object();
    Value* Values();
    void nonEmptyValues(Value*);
    void additionValues(Value *val);
    Value* Members();
    void nonEmptyMembers(Value*);
    void additionMembers(Value* value);
    Member* _Member();
    Value* _Value();
    
public:
    Document();
    Value& Parse(const std::string &s);
};
using JsonParser = Document;

}
#endif /* Document_hpp */
