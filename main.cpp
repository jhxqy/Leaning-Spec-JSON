//
//  main.cpp
//  Json
//
//  Created by 贾皓翔 on 2019/10/16.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include <iostream>
#include "json.hpp"
#include <string>
#include "Document.hpp"
using namespace std;
int main(int argc, const char * argv[]) {
    using namespace JSON::Imple;
    string s=R"({
      "name": "RapidJSON",
      "version": "1.1.0",
      "keywords": "json, sax, dom, parser, generator",
      "description": "A fast JSON parser/generator for C++ with both SAX/DOM style API",
      "export": {
        "include": "include"
      },
      "examples": "example/*/*.cpp",
      "repository":
      {
        "type": "git",
        "url": "https://github.com/Tencent/rapidjson"
      }
    })";
    Tokenizer tokens(s);
    Token *t;
    while ((t=tokens.scan())!=nullptr) {
        cout<<ws2s(t->lexeme_)<<endl;
    }
    using namespace JSON;
    Document document;
    document.Parse(s);
    
    return 0;
}
