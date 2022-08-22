# TPS Game

基于UE4.27的第三人称多人射击游戏，采用服务器D/S服务器。



## 作业情况

### 任务书

<img src="README.assets/mmexport1658396522950-16611683895014.jpg" alt="mmexport1658396522950" style="zoom:50%;" />

### 要点拆解

1. 打包
2. 能够打中靶子。靶子是能射线检或碰撞检测的。
   * 实现方式1：玩家的枪械的射线检测，靶子相应响应动画。
   * 实现方式2：子弹做成`mesh`，使用**物理模拟**。
3. 通过使用`HUD`，并向`HUD`中添加 `widget`，根据操作隐藏或显示`widget` （`CreateWidget`， `AddToViewPoint`，`RemoveFromParent`）。
4. 击中靶子有击飞效果。靶子都是启用**物理模拟**的`mesh`，同时，需要为靶子开启**碰撞检测**给玩家计分。
5. 手雷的投掷，爆炸的范围击飞。需要为手雷向UE**注册碰撞响应函数**。
   * 伤害系统方面：手雷爆炸伤害使用 **`UGameplayStatics::ApplyRadialDamage`**添加径向伤害。
   * 物理效果方面：给手雷设置一个实体，一个物理碰撞半径。在爆炸时，碰撞检测函数中， 使用 `AddRadicalForce`模拟爆炸击飞效果。
6. 动作方面，实现了蹲走，跑动，跳跃，**八向移动**，**瞄准偏移（动画缓存骨骼分层）**。拳头和持枪、站立和蹲走的**动画机**。以及**死亡状态的Montage**。
7. 实现了特殊材质，使用**`Time`改变UV图**实现流光溢彩。枪上装手电筒（`Spot Light Component`, 开启阴影）。加入景深效果。
8. 网络支持上，本机DS多玩家对战（局域网联机）
   * 需要使用源代码构建版本。
9. AI行为树。



### 实现介绍

#### **枪械系统**

设计一个基类 `BaseWeapon`

**射线检测**

<img src="README.assets/image-20220822201303705.png" alt="image-20220822201303705" style="zoom:67%;" />

**投掷物碰撞检测，添加径向伤害**

<img src="README.assets/image-20220822201444684.png" alt="image-20220822201444684" style="zoom: 67%;" />





#### 界面系统

**开始界面**

<img src="README.assets/image-20220822195543515.png" alt="image-20220822195543515" style="zoom:50%;" />

**暂停界面（暂停widget）**

<img src="README.assets/image-20220822195837004.png" alt="image-20220822195837004" style="zoom: 67%;" />

**用户游戏信息界面**：（根据持有的武器选取**武器对应**的**贴图和瞄准图标**）

<img src="README.assets/image-20220822200024665.png" alt="image-20220822200024665" style="zoom:50%;" />

**死亡widget**：死亡时可见。

![image-20220822200300468](README.assets/image-20220822200300468.png)





### 未实现部分

#### 景深

任务应该指的是材质添加的景深吧。

参考资料：

1. [UE4景深后处理效果学习笔记（一）基本原理 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/335669216)
2. [UE4景深后处理效果学习笔记（二）效果实现 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/336007155)



还是场景摄像机的景深？

参考资料

1. [景深 | 虚幻引擎文档 (unrealengine.com)](https://docs.unrealengine.com/4.26/zh-CN/RenderingAndGraphics/PostProcessEffects/DepthOfField/)
2. [移动平台景深方法 | 虚幻引擎文档 (unrealengine.com)](https://docs.unrealengine.com/4.26/zh-CN/RenderingAndGraphics/PostProcessEffects/DepthOfField/MobileDOFMethods/)





#### 网络同步

> 若项目可能需要多人游戏功能，则从项目开始阶段起，构建所有gameplay时都应将多人游戏功能考虑在内。若开发团队通常会在创建多人游戏时实施额外步骤，相较于单人游戏，构建gameplay的流程并不会耗时过久。长远来看，项目将便于整个团队进行调试和维护。同时，虚幻引擎中编写的多人游戏gameplay仍可在单人游戏中使用。
>
> 但是，重构无网络情况下编译的基本代码需要梳理整个项目，几乎所有gameplay都需要重新编写。届时，开发团队成员需重新学习可能早已熟悉的编程实操。同时，网速和稳定的相关技术瓶颈也会让你措手不及。
>
> 相较于初期规划，在项目后期引入网络功能会占用大量资源，且极为复杂。因此，除非确定项目无需多人游戏功能，否则应 *始终* 按多人游戏方向进行编程.

我的项目一开始没有考虑到网络方面，如官方文档所言，需要重构甚至推导重来。

时间限制，虽然来不及重构，我也学习了这方面的内容。以下是我的实现思路。



#### AI



### 扩展部分

#### 动画

实现了蹲走，跑动，跳跃，**八向移动**，**瞄准偏移（动画缓存骨骼分层）**。拳头和持枪、站立和蹲走的**动画机**。以及**死亡状态的Montage**

<img src="README.assets/image-20220822195409360.png" alt="image-20220822195409360" style="zoom:50%;" />



* 八向移动 

  <img src="README.assets/image-20220822194531855.png" alt="image-20220822194531855" style="zoom: 67%;" />

  <img src="README.assets/image-20220822194506019.png" alt="image-20220822194506019" style="zoom: 67%;" />

  

* 瞄准偏移（动画缓存骨骼分层）

  <img src="README.assets/image-20220822194952792.png" alt="image-20220822194952792" style="zoom:50%;" />

  

  <img src="README.assets/image-20220822194652120.png" alt="image-20220822194652120" style="zoom:50%;" />

* 蹲走和战力、持枪和持拳状态机（持枪都设置了瞄准偏移）

  <img src="README.assets/image-20220822194856629.png" alt="image-20220822194856629" style="zoom:50%;" />







# Waiting Extent

* Multi People Support
* Target Bind Set
* Grenade Effect
* AI 
* show Player Information.

