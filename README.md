# QtTestbed
A test framework for Qt

# 特性：
+ 用树组织程序内部所有QObject对象
+ 针对对象进行操作
   + 获取子对象列表
   + 获取属性列表
   + 获取属性
   + 设置属性
   + 获取方法列表
   + 调用方法
+ 提供完成特殊功能的对象
   + 虚拟鼠标
   + 虚拟键盘
   + 观察者

# 使用：
## 在项目代码中集成
```cpp
    QTestbed::init();
    QTestbed::addGlobalObject(this);
    QTestbed::start();
```
