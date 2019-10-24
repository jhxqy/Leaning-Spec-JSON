//
//  json.hpp
//  Json
//
//  Created by 贾皓翔 on 2019/10/16.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#ifndef Tokenizer_hpp
#define Tokenizer_hpp

#include <stdio.h>

#include <string>
#include <list>

namespace JSON{
namespace Imple{
enum class TokenType{
    Number,String,Symbol,Bool,Null,Int,Double
};
struct Token{
    TokenType type_;
    std::wstring lexeme_;
    Token(TokenType t,const std::wstring &lex):type_(t),lexeme_(lex){
        
    }
    bool equal(TokenType t)const{
        return type_==t;
    }
    bool equal(const Token *t)const{
        return type_==t->type_&&lexeme_.compare(t->lexeme_)==0;
    }
    

};

std::wstring s2ws(const std::string &s);
std::string ws2s(const std::wstring &s);

class Character{
public:
    static bool isDight(wchar_t wc){
        return wc>=L'0'&&wc<=L'9';
    }
    static bool isAlpha(wchar_t wc){
        return (wc>=L'a'&&wc<=L'z')||(wc>=L'A'&&wc<='Z');
    }
    static bool isDightOrAlpha(wchar_t wc){
        return isDight(wc)||isAlpha(wc);
    }
    static bool isKey(const std::wstring &s,size_t index,const std::wstring &key);
};

class Tokenizer{
    std::list<Token *> tokens_;
    std::wstring data_;
    size_t index_;
    bool eof_;
    bool EofTest();
    
    Token* makeToken(TokenType type_,const std::wstring &lex);
public:
    Tokenizer(const std::string &s):data_(s2ws(s)),index_(0),eof_(false){
        
    }
    Tokenizer(const Tokenizer&t);
    ~Tokenizer();
    Token* scan();
};



}

}
#endif /* json_hpp */
