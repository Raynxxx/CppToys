# MiniSTL

C++ STL的自我实现，后期会加入一些额外的组件功能。
* 开发语言 :  C++11
* 编译环境 :  Visual Studio Community 2013
* 参考资料1:  STL源码剖析 侯捷著
* 参考资料2:  gcc 4.9 源码 [点我](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)
* 参考资料3:  QTBase 源码 [点我](https://github.com/qtproject/qtbase)

## 项目计划

|基本组件|进度|链接|单元测试|
|---|---|---|---|
|空间配置器|100%|[Allocator.h](Src/Allocator.h), [Alloc.h](Src/Alloc.h), [Alloc.cpp](Src/Alloc.cpp), [Construct.h](Src/Construct.h)|--|
|iterator|100%|[Iterator.h](Src/Iterator.h)|--|
|reverse_iterator|100%|[ReverseIterator.h](Src/ReverseIterator.h)|--|
|Algorithm|20%|[Algo.h](Src/Algo.h), [AlgoBase.h](Src/AlgoBase.h), [Algorithm.h](Src/Algorithm.h)|--|

|工具|进度|链接|单元测试|
|---|---|---|---|
|uninitialized|100%|[Uninitialized.h](Src/Uninitialized.h)|--|
|type_traits|80%|[TypeTraits.h](Src/TypeTraits.h)|[TestTypeTraits](UnitTest/TestTypeTraits.cpp)|
|utility|80%|[Utility.h](Src/Utility.h)|[TestUtility](UnitTest/TestUtility.cpp)|
|functional|40%|[Functional.h](Src/Functional.h)|--|
|heap|100%|[Heap.h](Src/Heap.h)|--|
|rb_tree|50%|[Tree.h](Src/Tree.h)[Tree.cpp](Src/Tree.cpp)|[TestTree](UnitTest/TestTree.cpp)|
|hashtable|--|--|--|
|pair|100%|[Pair.h](Src/Pair.h)|[TestUtility](UnitTest/TestUtility.cpp)|
|tuple|--|--|--|

|顺序容器|进度|链接|单元测试|
|---|---|---|---|
|string|80%|[String.h](Src/String.h)|[TestString](UnitTest/TestString.cpp)|
|vector|100%|[Vector.h](Src/Vector.h)|[TestVector](UnitTest/TestVector.cpp)|
|list|100%|[List.h](Src/List.h)|[TestList](UnitTest/TestList.cpp)|
|deque|100%|[Deque.h](Src/Deque.h)|--|
|array|100%|[Array.h](Src/Array.h)|[TestArray](UnitTest/TestArray.cpp)|
|bitset|--|--|--|

> 计划加入一些QT的string接口

|配接器|进度|链接|单元测试|
|---|---|---|---|
|stack|100%|[Stack.h](Src/Stack.h)|--|
|queue|100%|[Queue.h](Src/Queue.h)|--|
|priority_queue|100%|[Queue.h](Src/Queue.h)|--|

|关联容器|进度|链接|单元测试|
|---|---|---|---|
|set|20%|[Set.h](Src/Set.h)|[TestSet](UnitTest/TestSet.cpp)|
|map|---|---|---|
|multiset|---|---|---|
|multimap|---|---|---|
|unordered_set|---|---|---|
|unordered_multiset|---|---|---|
|unordered_map|---|---|---|
|unordered_multimap|---|---|---|



## 单元测试

最近get了一个超轻量级的单元测试框架 `Catch`， [链接点击这里](https://github.com/philsquared/Catch)。整个依赖就一个单独的头文件 `Catch.hpp`。

看一下作者的 Tutorial，是不是超简洁！

```
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}
```
