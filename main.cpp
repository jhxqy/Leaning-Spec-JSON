//
//  main.cpp
//  Json
//
//  Created by 贾皓翔 on 2019/10/16.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include <iostream>
#include <string>
#include "Json.hpp"
using namespace std;
using namespace JSON;

void printValue(Value &v){
    switch (v.valueType_) {
        case JSON::ValueType::Bool:
            cout<<boolalpha<<"Value"<<endl;
            break;
        case JSON::ValueType::Array:
            cout<<"[";
            for(size_t i=0;i<v.getArraySize();i++){
                printValue(v[i]);
                if(i!=v.getArraySize()-1){
                    cout<<",";
                }
            }
            cout<<"]";
            break;

        case JSON::ValueType::Null:
                   cout<<"null";
                   break;
        case JSON::ValueType::Int:
                   cout<<v.getInt();
                   break;
        case JSON::ValueType::Double:
                   cout<<v.getDouble();
                   break;
        case JSON::ValueType::String:
            cout<<"\""<<v.getString()<<"\"";
                   break;
        case JSON::ValueType::Object:
            cout<<"{";
            size_t count=0;
            auto keyList=v.getKeyLists();
            for(auto i:keyList){
                cout<<"\""<<i<<"\""<<":";
                printValue(v[i]);
                if(count!=keyList.size()-1){
                    cout<<",";
                }
                count++;
            }
            cout<<"}";
            break;
    }
}

int main(int argc, const char * argv[]) {
    string s=R"(
    {"name":"jhx","age":20,"school":"常熟理工学院","专业":"网络工程","绩点":3.2,"电子设备":["平板","手机","电脑"],"单身状况":true}
    )";

    JsonParser Parser;
    Value& value=Parser.Parse(s);
    for(auto i:value.getKeyLists()){
        cout<<i<<",";
    }
    cout<<endl;
    if(value["name"].isString()){
        cout<<value["name"].getString()<<endl;
    }
    Value& age=value["age"];
    cout<<boolalpha<<age.isNumber()<<" "<<age.isInt()<<" "<<age.isDouble()<<endl;
    cout<<age.getInt()<<endl;
    Value& array=value["电子设备"];
    for(auto i=0;i<array.getArraySize();i++){
        cout<<array[i].getString()<<endl;
    }
    Value& solo=value["单身状况"];
    cout<<boolalpha<<solo.getBool()<<endl;
    return 0;
}
