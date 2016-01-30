# MiniSTL
  造轮子计划1，一个STL的子集，后期会加入一些有趣的组件功能。
* 开发语言 :  C++11
* 编译环境 :  Visual Studio Community 2013
* 参考资料1:  STL源码剖析 侯捷著
* 参考资料2:  gcc 4.9 源码 [点我](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)
* 参考资料3:  QTBase 源码 [点我](https://github.com/qtproject/qtbase)

## 项目计划

|基本组件|进度|链接|单元测试|
|---|---|---|---|
|空间配置器|100%|[Allocator.h](MiniSTL/Allocator.h), [Alloc.h](MiniSTL/Alloc.h), [Alloc.cpp](MiniSTL/Alloc.cpp), [Construct.h](MiniSTL/Construct.h)|[TestTypeTraits](MiniSTL/UnitTest/TestTypeTraits.cpp)|
|iterator|100%|[Iterator.h](MiniSTL/Iterator.h)|--|
|reverse_iterator|100%|[ReverseIterator.h](MiniSTL/ReverseIterator.h)|--|
|Algorithm|20%|[Algo.h](MiniSTL/Algo.h), [AlgoBase.h](MiniSTL/AlgoBase.h), [Algorithm.h](MiniSTL/Algorithm.h)|--|

|工具|进度|链接|单元测试|
|---|---|---|---|
|uninitialized|100%|[Uninitialized.h](MiniSTL/Uninitialized.h)|--|
|type_traits|20%|[TypeTraits.h](MiniSTL/TypeTraits.h)|--|
|pair|100%|[Pair.h](MiniSTL/Pair.h)|--|
|heap|100%|[Heap.h](MiniSTL/Heap.h)|--|
|rb_tree|20%|[Tree.h](MiniSTL/Tree.h)|--|
|bitset|--|--|--|

|顺序容器|进度|链接|单元测试|
|---|---|---|---|
|string|80%|[String.h](MiniSTL/String.h)|[TestString](MiniSTL/UnitTest/TestString.cpp)|
|vector|100%|[Vector.h](MiniSTL/Vector.h)|[TestVector](MiniSTL/UnitTest/TestVector.cpp)|
|list|100%|[List.h](MiniSTL/List.h)|--|
|deque|100%|[Deque.h](MiniSTL/Deque.h)|--|

> 计划加入一些QT的string接口

|配接器|进度|链接|单元测试|
|---|---|---|---|
|stack|100%|[Stack.h](MiniSTL/Stack.h)|--|
|queue|100%|[Queue.h](MiniSTL/Queue.h)|--|
|priority_queue|100%|[Queue.h](MiniSTL/Queue.h)|--|

|关联容器|进度|链接|单元测试|
|---|---|---|---|
|---|---|---|---|



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
