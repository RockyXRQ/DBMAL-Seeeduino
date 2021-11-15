# DBMAL - Seeeduino

 [![standard-readme compliant](https://img.shields.io/badge/readme%20style-standard-brightgreen.svg?style=flat)](https://github.com/RichardLitt/standard-readme) [![Lang](https://img.shields.io/badge/Lang-zh--CN-Green?style=flat)]()

DBMAL(Dialog Between Music & Light) 是一个2021年红点奖入围项目，它拥有多种照明模式，既可以作为普通的护眼台灯，以为您的桌面工作进行基本照明，也可以根据所播放的音乐或周围环境声音自动生成绚丽的氛围灯光。

DBMAL主张一切灯光的变化都应该舒适且自然，所以在DBMAL中您将不会看到颜色或者亮度的突变，在本项目中本人针对灯光变化的速度和趋势等控制进行了一些大胆的思考与尝试，才最终完成了本项目。

本篇文档将教您如何使用该仓库构建您自己的DBMAL。

## 目录

1. 硬件清单
2. 烧录程序
3. 调整参数

## 1. 硬件清单

DBMAL的硬件清单非常简单，您只需要：

1. seeeduino-xiao 开发板(使用SAMD21芯片)。
2. 灯条(使用WS2812B)。

您还需要购买一块锂电池以为灯条和主控板供电。

## 2. 烧录程序

