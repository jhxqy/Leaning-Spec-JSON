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
using namespace std;
int main(int argc, const char * argv[]) {
    using namespace JSON::Imple;
    Tokenizer tokens(s);
    Token *t;
    while ((t=tokens.scan())!=nullptr) {
        cout<<ws2s(t->lexeme_)<<endl;
    }
    return 0;
}
