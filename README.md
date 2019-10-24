#  Learning-specific JSON
>在学习编译原理途中使用递归下降解析法实现的JSON解析器。

## 项目简介
    可将json字符串转化为一颗DOM树
## 演示
```cpp
using namespace JSON;
    string s=R"(
    {"name":"jhx","age":20,"school":"常熟理工学院","专业":"网络工程","绩点":3.2,"电子设备":["平板","手机","电脑"],"单身状况":true,"女友":null,"笔记本电脑":{"品牌":"apple","尺寸":"13"}}
    )";

    JsonParser Parser;
    Value& dom=Parser.Parse(s);
```

使用JsonParser对象将json串转化为以*Value*对象为节点的DOM树，使用Value&引用，获取Value对象；

#### String
```cpp
    Value& name=dom["name"];
    if(name.isString()){
        cout<<name.getString()<<endl;
    }

    /*output:
    jhx
    */
```
使用isString()方法判断Value是否是一个字符串，然后使用getString()获取字符串
#### Number
```cpp
    Value& age=dom["age"];
    cout<<boolalpha<<age.isNumber()<<" "<<age.isInt()<<" "<<age.isDouble()<<endl;
    cout<<age.getInt()<<endl;

    /*
    output:
    20
    true true false
    */
```

使用isNumber()判断是否为一个数字，使用isInt()判断是否为一个整数，isDouble()判断是否为一个浮点数，并使用getInt()和getDouble()来获取值;
#### Boolean
```cpp
    Value& solo=dom["单身状况"];
    cout<<boolalpha<<solo.getBool()<<endl;  

    /*
    output:
    true
    */
```
使用isBool()判断是否为bool型，并使用getBool()获取值

#### Null
```cpp
    Value& girlfriend=dom["女友"];
    cout<<boolalpha<<girlfriend.isNull()<<endl;

    /*
    output:
    true
    */
```
使用isNull()判断值是否为null

#### Array
```cpp
    Value& array=dom["电子设备"];
    for(auto i=0;i<array.getArraySize();i++){
        cout<<array[i].getString()<<endl;
    }  

    /*
    output:
    平板
    手机
    电脑
    */
```
数组可使用getArraySize()方法获取数组长度，并使用[index]来获取数字值

#### Object
```cpp
    cout<<dom.isObject()<<endl;
    for(auto i:dom.getKeyLists()){
        cout<<i<<",";
    }
    cout<<endl;
    
    cout<<dom["笔记本电脑"]["品牌"].getString()<<endl;
    /*
    output:
    true
    name,age,school,专业,绩点,电子设备,单身状况,女友,笔记本电脑,
    apple
    */
```
Object可以使用isObject()判断，使用getKeyLists()获取键名列表,使用字符串下标索引值。

