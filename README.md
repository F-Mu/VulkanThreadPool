基于VulkanLearning视频的简单引擎实现的线程池可视化

https://github.com/blurrypiano/littleVulkanEngine/tree/master

![ThreadPool.gif](image/ThreadPool.gif)

左边是线程池，中间是任务执行区域，右边是任务等待区域

按1～5可以生成执行时间为1～5秒的睡眠任务

**v2.0**

+ 大重构（感谢学长给的基本架构）

+ 去除3D特性（以后继续尝试添加）

+ 更好的分层以及优化主循环（依旧模仿Piccolo小引擎)，https://github.com/BoomingTech/Piccolo

+ v1.2版本在另一个分支中

+ 存在莫名其妙的bug导致界面不初始化或出现段错误

+ Rectangle的相关类图

<img alt="Rectangle.png" src="image/Rectangle.png" style="zoom:25%;"/>

**v2.1**

+ 修复BUG（大概）

**待实现（可能也不会实现）：**

+ 更好的异步执行（Job System）

+ 尝试更更好的分层

+ 任务执行完删除资源（原来的接口被删了）

+ 继续模仿Piccolo小引擎优化主循环

+ 接入粒子系统，更新物体
