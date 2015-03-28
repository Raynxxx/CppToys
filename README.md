# MiniSTL
  一个STL的子集，采用C++11实现STL的基本组件，各类容器和算法
* 开发语言 :  `C++11`
* 编译环境 :  `Visual Studio Community 2013`
* 参考资料1:  `STL源码剖析 侯捷著`
* 参考资料2:  `gcc 4.9 源代码` [点我](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)

## 项目计划

|组件|进度|链接|
|---|---|---|
|空间配置器|100%|[Allocator.h](MiniSTL/Allocator.h), [Alloc.h](MiniSTL/Alloc.h), [Alloc.cpp](MiniSTL/Alloc.cpp), [Construct.h](MiniSTL/Construct.h)|
|uninitialized|100%|[Uninitialized.h](MiniSTL/Uninitialized.h)|
|type_traits|100%|[TypeTraits.h](MiniSTL/TypeTraits.h)|
|iterator|100%|[Iterator.h](MiniSTL/Iterator.h)|
|reverse_iterator|100%|[ReverseIterator.h](MiniSTL/ReverseIterator.h)|
|Algorithm|15%|[Algo.h](MiniSTL/Algo.h), [AlgoBase.h](MiniSTL/AlgoBase.h), [Algorithm.h](MiniSTL/Algorithm.h)|
|string|50%|[String.h](MiniSTL/String.h)|
|pair|--|--|

|顺序容器|进度|链接|
|---|---|---|
|vector|95%|[Vector.h](MiniSTL/Vector.h)|
|list|---|---|

|关联容器|进度|链接|
|---|---|---|
|---|---|---|
