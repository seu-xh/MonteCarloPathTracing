# MonteCarloPathTracing

1.开发环境以及第三方库
本次开发是以VS2017为平台，借用了ASSIMP库来读取模型文件，利用erand48库来进行随机数的生成。

2.主要的算法流程
首先通过assimp库读取模型obj文件以及mtl材质文件，判断出相关物体的材质属性，然后将assimp读取后的固有格式的物体转成我们需要的物体格式Model，Mesh，Triangle，Vertex。
接着是构造AABB包围盒技术构造BVH加速结构，实现光线与物体的快速求交。
最后实现蒙特卡洛光线追踪的相关算法。

3.主要加速
BVH的构造实现快速求交以及OpenMp实现cpu的并行

<img src="https://github.com/seu-xh/MonteCarloPathTracing/1.png">
