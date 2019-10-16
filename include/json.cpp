//
//  json.cpp
//  Json
//
//  Created by 贾皓翔 on 2019/10/16.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include "json.hpp"
#include <locale>
#include <codecvt>
namespace JSON{
namespace Imple{

std::wstring s2ws(const std::string &s){
    using T=std::codecvt_utf8<wchar_t>;
    std::wstring_convert<T,wchar_t> conv;
    return conv.from_bytes(s);
}
std::string ws2s(const std::wstring &s){
    using T=std::codecvt_utf8<wchar_t>;
    std::wstring_convert<T,wchar_t> conv;
    return conv.to_bytes(s);
}

bool Character::isKey(const std::wstring &s
                      , size_t index, const std::wstring &key){
    std::wstring res;
    std::wstring k2=key;
    for(auto i=0;i<key.size();i++){
        res+=s[index+i];
        if(res[i]>=L'a'&&res[i]<='z'){
            res[i]=res[i]-L'a'+L'A';
        }
        if(k2[i]>=L'a'&&k2[i]<='z'){
            k2[i]=k2[i]-L'a'+L'A';
        }
        if(res[i]!=k2[i]){
            return false;
        }
    }
    return true;
    
    return true;
}

bool Tokenizer::EofTest(){
        if(eof_||index_>=data_.size()){
            eof_=true;
            return true;
        }else{
            return false;
        }
    
}
Token* Tokenizer::makeToken(TokenType type_, const std::wstring  &lex){
    Token *t=new Token(type_,lex);
    tokens_.push_back(t);
    return t;
}
Tokenizer::~Tokenizer(){
    for(auto i:tokens_){
        delete i;
    }
}

Token* Tokenizer::scan(){

    for(;;){
        if(!EofTest()&& (data_[index_]==' '||data_[index_]=='\n'||data_[index_]=='\t')){
            continue;
        }else{
            break;
        }
        
    }
    if(EofTest()){
        return nullptr;
    }
    if(Character::isDight(data_[index_])||data_[index_]==L'-'){
        std::wstring res;
        do{
            res+=data_[index_];
            index_++;
        }while(!EofTest()&&( Character::isDight(data_[index_])));
        if (data_[index_]==L'.') {
           res+=data_[index_];
           index_++;
           for(;!EofTest()&& Character::isDight(data_[index_]);index_++){
           res+=data_[index_];
           }
           return makeToken(TokenType::Number, res);
        }else{
            return makeToken(TokenType::Number, res);
        }
    }
    if(data_[index_]==L'"'){
        std::wstring res;
        do{
            res+=data_[index_];
            index_++;
            
        }while(!EofTest()&&data_[index_]!=L'"');
        if(data_[index_]!=L'"'){
            throw std::runtime_error("缺少必要的引号");
        }else{
            res+=data_[index_];
            index_++;
        }
        return makeToken(TokenType::String, res);
    }
    if(data_[index_]==L'T'||data_[index_]==L't'){
        if(data_.size()-index_>=5&&Character::isKey(data_, index_, L"true")){
            index_+=4;
            return makeToken(TokenType::Bool,L"true");
        }else{
            throw std::runtime_error("true error!");
        }
    }
    if(data_[index_]==L'F'||data_[index_]==L'f'){
        if(data_.size()-index_>=5&&Character::isKey(data_, index_, L"false")){
            index_+=4;
            return makeToken(TokenType::Bool,L"false");
        }else{
            throw std::runtime_error("false error!");
        }
    }
    
    if(data_[index_]==L'N'||data_[index_]==L'n'){
        if(data_.size()-index_>=4&&Character::isKey(data_, index_, L"null")){
            index_+=4;
            return makeToken(TokenType::Null,L"null");
        }else{
            throw std::runtime_error("null error!");
        }
    }
    
    Token *res=makeToken(TokenType::Symbol, std::wstring(1,data_[index_]));
    index_++;
    return res;
    
}

}
}
