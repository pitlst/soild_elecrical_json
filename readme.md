

# soild_elecrical_json

一个私人定制的json解析库
 
## 目录

- [上手指南](#上手指南)
  - [开发前的配置要求](#开发前的配置要求)
  - [安装步骤](#安装步骤)
- [文件目录说明](#文件目录说明)
- [部署](#部署)
- [版本控制](#版本控制)
- [作者](#作者)
- [版权说明](#版权说明)

### 上手指南

本项目使用xmake开发，以动态库形式生成，可以使用xmake引入项目，也可复制头文件和动态库到对应项目中使用

由于是给个人使用的，暂时没有考虑分发方式的需求，后续实现，后续实现（画饼）


#### 开发前的配置要求

1. xmake 要求 2.6.9 及以上
2. 支持 c++ 17 及以上的编译器版本
3. (可选) google test 测试框架 

#### **安装步骤**

1. 克隆该仓库
2. 进入仓库路径

```sh
cd path/to/soild_elecrical_json
```

3. 使用xmake编译

```sh
xmake 
# 或者多线程编译,数字换成你的cpu线程数
xmake --build -j4
```

### 文件目录说明
eg:

```
soild_elecrical_json
├── LICENSE
├── README.md
├── xmake.lua
├── /include/
│  ├── json.hpp
│  ├── parse.hpp
├── /src/
│  ├── json.cpp
│  ├── parse.cpp
├── /test/
│  ├── main.cpp
└── .gitignore
```

### 部署

暂无

### 版本控制

该项目使用Git进行版本管理。您可以在repository参看当前可用版本。

### 作者

sunmoming@gmail.com

 *您也可以在贡献者名单中参看所有参与该项目的开发者。*

### 版权说明

该项目签署了MIT 授权许可，详情请参阅 [LICENSE.txt](https://github.com/pitlst/soild_elecrical_json/blob/master/LICENSE)