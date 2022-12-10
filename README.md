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

<img alt="Rectangle.png" src="image/Rectangle.png" style="zoom:20%;"/>

**v2.1**

+ 修复BUG（大概）

**v2.2**

+ Task完成后动画完成（加入原有model类）

+ 修改类名和文件名

+ 为Component类加入dirty机制

**v2.3**

+ 根据C++书籍提供的一些条款进行小改动

+ 将render_pass归类

**待实现（可能也不会实现）：**

+ 更好的异步执行（Job System）

+ 尝试更更好的分层

+ 任务执行完删除资源（验证层报错，不会改，接口已实现）

+ 继续模仿Piccolo小引擎优化主循环

+ 接入粒子系统，更新物体

+ 求出多面体的凸包，而不是严格限制顶点顺序（不想写）

  + 白框（即空矩形）需按边顺序放点

  + 满矩形需按右上、左上、右下、左下的顺序放点（主要为了task的删除动画）