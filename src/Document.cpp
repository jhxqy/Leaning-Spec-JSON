//
//  Document.cpp
//  Json
//
//  Created by 贾皓翔 on 2019/10/23.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include "JSON.hpp"
#include <iostream>
#include <cassert>
namespace JSON {
Member::Member(const Member&m){
    this->key=m.key;
    //value=new Value(m.value);
}


Member::~Member(){
    if(value!=nullptr){
        delete value;
        value=nullptr;
    }
    
}
void Value::destroy(){
    
    switch (valueType_) {
        case ValueType::String:
            if(str!=nullptr){
                delete str;
                str=nullptr;
            }
            break;
        case ValueType::Object:
            if(members->size()!=0){
                for(auto i:*members){
                    delete i;
                }
            }
            members->clear();
            break;
        case ValueType::Array:
            if (values->size()!=0) {
                for(auto i:*values){
                    delete i;
                }
            }
            values->clear();
            break;
        default:
            break;
    }
}


Value::~Value(){
    destroy();
    switch (valueType_) {
        case ValueType::Object:
            delete members;
            break;
        case ValueType::Array:
            delete values;
        default:
            break;
    }
    
}

Value::Value(const Value&v){
    
    valueType_=v.valueType_;
    switch (valueType_) {
        case ValueType::Int:
            this->i=v.i;
            break;
        case ValueType::Double:
            this->d=v.d;
            break;
        case ValueType::String:
            this->str=new std::string(*v.str);
            break;
        case ValueType::Object:
            this->members=new std::vector<Member*>();
            for(auto i:*(v.members)){
                this->members->push_back(new Member(*i));
            }
            break;
        case ValueType::Array:
            this->values=new std::vector<Value*>;
            for(auto i:*(v.values)){
                this->values->push_back(new Value(*i));
            }
            break;
        case ValueType::Null:
            break;
        case ValueType::Bool:
            this->boolValue=v.boolValue;
            break;
        
    }
}

#define JudgeElem(name,type)  \
bool Value::is##name(){\
    return valueType_== type;\
}

JudgeElem(Int, ValueType::Int)
JudgeElem(Object, ValueType::Object)
JudgeElem(Array, ValueType::Array)
JudgeElem(Null, ValueType::Null)
JudgeElem(String, ValueType::String)
JudgeElem(Double, ValueType::Double)

bool Value::isNumber(){
    return valueType_==ValueType::Int||valueType_==ValueType::Double;
}
Value& Value::operator[](size_t index){
    assert(valueType_==ValueType::Array);
    return *((*values)[index]);
}
Value& Value::operator[](const std::string &key){
    assert(valueType_==ValueType::Object);
    for(auto i:*members){
        if(i->key.compare(key)==0){
            return *(i->value);
        }
    }
    throw std::runtime_error("don't have the member with this key");
}

int Value::getInt(){
    assert(valueType_==ValueType::Int);
    return i;
}
double Value::getDouble(){
    assert(valueType_==ValueType::Double);
       return d;
}
std::string Value::getString(){
    assert(valueType_==ValueType::String);
       return *str;
}
bool Value::getBool(){
    assert(valueType_==ValueType::Bool);
       return boolValue;
}

void Value::setNull(){
    destroy();
    valueType_=ValueType::Null;
    
}
void Value::setInt(int i){
    destroy();
    valueType_=ValueType::Int;
    this->i=i;
}

void Value::setDouble(double d){
    destroy();
    valueType_=ValueType::Double;
    this->d=d;
}

void Value::setNumber(int i){
    setInt(i);
}
void Value::setNumber(double d){
    setDouble(d);
}
void Value::setBool(bool b){
    destroy();
    valueType_=ValueType::Bool;
    this->boolValue=b;
}
void Value::setArray(std::vector<Value*> values){
    destroy();
    valueType_=ValueType::Array;
    this->values=new std::vector<Value*>();
    for(auto i:values){
        this->values->push_back(new Value(*i));
    }
}

void Value::setObject(std::vector<Member*> members){
    destroy();
    valueType_=ValueType::Object;
    this->members=new std::vector<Member*>();
     for(auto i:members){
           this->members->push_back(new Member(*i));
       }
}
void Value::setString(const std::string &s){
    destroy();
    valueType_=ValueType::String;
    this->str=new std::string(s);
}

std::list<std::string> Value::getKeyLists(){
    assert(valueType_==ValueType::Object);
    
    std::list<std::string> res;
    for(auto i:*members){
        res.push_back(i->key);
    }
    return res;
    
}
size_t Value::getArraySize(){
    assert(valueType_==ValueType::Array);
    return values->size();

}


Document::Document(){
    
}
Document::Document(const Document &d){
    for(auto i:d.roots){
        roots.push_back(new Value(*i));
    }
}
Document::~Document(){
    for(auto i:roots){
        delete i;
    }
}

Value& Document::Parse(const std::string &s){
    tokens=std::make_shared<Imple::Tokenizer>(s);
    nowToken=tokens->scan();
    Value*v=S();
    roots.push_back(v);
    return *v;
}
Imple::Token* Document::match(Imple::Token *t){
    if(nowToken->equal(t)){
        Imple::Token *temp=nowToken;
        nowToken=tokens->scan();
        return temp;
    }else{
        throw std::runtime_error(Imple::ws2s(t->lexeme_)+" match error!");
    }
}
Imple::Token* Document::match(Imple::TokenType t){
    if(nowToken->equal(t)){
        Imple::Token *temp=nowToken;
        nowToken=tokens->scan();
        return temp;
    }else{
        throw std::runtime_error("Type match error!");
    }
}

Value* Document::S(){
    static Imple::Token* leftBracket=new Imple::Token(Imple::TokenType::Symbol, L"[");
    if(nowToken->equal(leftBracket)){
        return array();
    }else{
        return object();
    }
}
Value* Document::array(){
    static Imple::Token* leftBracket=new Imple::Token(Imple::TokenType::Symbol, L"[");
    static Imple::Token* rightBracket=new Imple::Token(Imple::TokenType::Symbol, L"]");
    Value *val;
    match(leftBracket);
    val=Values();
    match(rightBracket);
    return val;
}

Value* Document::object(){
    static Imple::Token* leftBraces=new Imple::Token(Imple::TokenType::Symbol, L"{");
    static Imple::Token* rightBraces=new Imple::Token(Imple::TokenType::Symbol, L"}");
    Value *val;
    match(leftBraces);
    val= Members();
    match(rightBraces);
    return val;
}
Value* Document:: Values(){
    static Imple::Token* leftBraces=new Imple::Token(Imple::TokenType::Symbol, L"{");
    static Imple::Token* leftBracket=new Imple::Token(Imple::TokenType::Symbol, L"[");
    Value *val=new Value(ValueType::Array,std::vector<Value*>());
    
    switch (nowToken->type_) {
        case Imple::TokenType::String:
        case Imple::TokenType::Number:
        case Imple::TokenType::Null:
        case Imple::TokenType::Bool:
            nonEmptyValues(val);
            return val;
        default:
            break;
    }
    if(nowToken->equal(leftBracket)||nowToken->equal(leftBraces)){
        nonEmptyValues(val);
        
    }
    return val;
}

void Document::nonEmptyValues(Value *val){
    Value*v= _Value();
    val->values->push_back(v);
    additionValues(val);
}
void Document::additionValues(Value *val){
    static Imple::Token *comma=new Imple::Token(Imple::TokenType::Symbol,L",");
    if (nowToken->equal(comma)) {
        match(comma);
        nonEmptyValues(val);
    }
}

 Value* Document::Members(){
    Value* object=new Value(ValueType::Object,std::vector<Member*>());
    if(nowToken->equal(Imple::TokenType::String)){
        nonEmptyMembers(object);
    }
     return object;
}
void Document::nonEmptyMembers(Value* value){
    Member* v=_Member();
    value->members->push_back(v);
    additionMembers(value);
}
void Document::additionMembers(Value* value){
    static Imple::Token *comma=new Imple::Token(Imple::TokenType::Symbol,L",");
    if(nowToken->equal(comma)){
        match(comma);
        nonEmptyMembers(value);
    }
}
Member* Document::_Member(){
    static Imple::Token *colon=new Imple::Token(Imple::TokenType::Symbol, L":");
    Imple::Token *key= match(Imple::TokenType::String);
    match(colon);
    Value*v=_Value();
    Member *m=new Member;
    m->key=Imple::ws2s(key->lexeme_);
    m->value=v;
    return m;
    
    
}
Value* Document::_Value(){
    static Imple::Token* leftBraces=new Imple::Token(Imple::TokenType::Symbol, L"{");
    static Imple::Token* leftBracket=new Imple::Token(Imple::TokenType::Symbol, L"[");
    if(nowToken->equal(Imple::TokenType::String)){
        Imple::Token *t= match(Imple::TokenType::String);
        Value* v=new Value(ValueType::String,Imple::ws2s(t->lexeme_));
        return v;
    }else if(nowToken->equal(Imple::TokenType::Int)){
        Imple::Token *t=match(Imple::TokenType::Int);
        Value* v=new Value(ValueType::Int,std::stoi(Imple::ws2s(t->lexeme_)));
        return v;
    }else if(nowToken->equal(Imple::TokenType::Double)){
        Imple::Token *t=match(Imple::TokenType::Double);
        Value* v=new Value(ValueType::Double,std::stod(Imple::ws2s(t->lexeme_)));
        return v;
    }else if(nowToken->equal(Imple::TokenType::Null)){
        match(Imple::TokenType::Null);
        Value* v=new Value(ValueType::Null);
        return v;
    }else if(nowToken->equal(Imple::TokenType::Bool)){
        Imple::Token *t=match(Imple::TokenType::Bool);
        bool f=t->lexeme_.compare(L"true")==0?true:false;
        Value* v=new Value(ValueType::Bool,f);
        return v;
    }else if(nowToken->equal(leftBraces)){
        return object();
    }else if(nowToken->equal(leftBracket)){
        return array();
    }else{
        throw std::runtime_error("Value Type error!");
    }
}



}
