# MiniSTL
  一个STL的子集，采用C++11实现STL的基本组件，各类容器和算法
* 开发语言 :  C++11
* 编译环境 :  Visual Studio Community 2013
* 参考资料1:  STL源码剖析 侯捷著
* 参考资料2:  gcc 4.9 源码 [点我](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)
* 参考资料3:  QTBase 源码 [点我](https://github.com/qtproject/qtbase)

## 项目计划

|基本组件|进度|链接|
|---|---|---|
|空间配置器|100%|[Allocator.h](MiniSTL/Allocator.h), [Alloc.h](MiniSTL/Alloc.h), [Alloc.cpp](MiniSTL/Alloc.cpp), [Construct.h](MiniSTL/Construct.h)|
|iterator|100%|[Iterator.h](MiniSTL/Iterator.h)|
|reverse_iterator|100%|[ReverseIterator.h](MiniSTL/ReverseIterator.h)|
|Algorithm|15%|[Algo.h](MiniSTL/Algo.h), [AlgoBase.h](MiniSTL/AlgoBase.h), [Algorithm.h](MiniSTL/Algorithm.h)|

|工具|进度|链接|
|---|---|---|
|uninitialized|100%|[Uninitialized.h](MiniSTL/Uninitialized.h)|
|type_traits|50%|[TypeTraits.h](MiniSTL/TypeTraits.h)|
|pair|30%|[Pair.h](MiniSTL/Pair.h)|
|heap|0%|[Heap.h](MiniSTL/Heap.h)|
|bitset|--|--|

|顺序容器|进度|链接|
|---|---|---|
|string|80%|[String.h](MiniSTL/String.h) 计划加入一些QT的string接口|
|vector|95%|[Vector.h](MiniSTL/Vector.h)|
|list|95%|[List.h](MiniSTL/List.h)|
|deque|90%|[Deque.h](MiniSTL/Deque.h)|

|配接器|进度|链接|
|---|---|---|
|stack|90%|[Stack.h](MiniSTL/Stack.h)|
|queue|90%|[Queue.h](MiniSTL/Queue.h)|
|priority_queue|--|--|

|关联容器|进度|链接|
|---|---|---|
|---|---|---|



## 单元测试

先开好坑，有空补
