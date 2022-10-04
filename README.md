# DBMAL - Seeeduino

 [![standard-readme compliant](https://img.shields.io/badge/readme%20style-standard-brightgreen.svg?style=flat)](https://github.com/RichardLitt/standard-readme) [![Lang](https://img.shields.io/badge/Lang-zh--CN-Green?style=flat)]()

DBMAL(Dialog Between Music & Light) 是一个2021年红点奖入围项目，它拥有多种照明模式，既可以作为普通的护眼台灯，以为您的桌面工作进行基本照明，也可以根据所播放的音乐或周围环境声音自动生成绚丽的氛围灯光。

DBMAL主张一切灯光的变化都应该舒适且自然，所以在DBMAL中您将不会看到颜色或者亮度的突变，在本项目中本人针对灯光变化的速度和趋势等控制进行了一些大胆的思考与尝试，才最终完成了本项目。

本篇文档将教您如何使用该仓库构建您自己的DBMAL。

<img src=".\assets\DBMAL_2.jpg" alt="dbmal" style="zoom:50%;" />

## 目录

1. 硬件清单
2. 烧录程序
3. 调整参数
4. 视频展示

## 1. 硬件清单

DBMAL的硬件清单非常简单，您只需要：

1. Seeeduino-XIAO 开发板(使用SAMD21芯片)。
2. 灯条(使用WS2812B)。
3. 一块声强传感器。
4. 一块感应开关。
5. 一块锂电池。

## 2. 烧录程序

一般的，DBMAL的代码仓库提供了两种代码版本。其中位于根目录下的为多文件版本，而位于/singlefile路径下的为单文件版本，这两种版本代码的工作效果一致，可以任意选用。

在烧录之前请注意自己已经安装了以下库程序：

- SAMDTimerInterrupt
- FastLED

## 3. 调整参数

对于多文件版本的代码，其参数定义位于config.h文件中；对于单文件版本代码，则位于文件开头处的宏定义。可根据注释进行修改。

```c++
#define NUM_STRIPS 2           // 灯条数量
#define NUM_LEDS_PER_STRIP 62  // 每排灯条所含灯珠数量

#define LED_PIN_LEFT 1   // 左排灯条对应数字端
#define LED_PIN_MID 2    // 中排灯条对应数字端
#define LED_PIN_RIGHT 8  // 右排灯条对应数字端
#define VOLUME_PIN 9     // 声强传感器对应虚拟端
#define SWITCH_PIN 10    // 感应开关对应数字端

#define TOP_COLOR_GROUP_NUM 8  // 颜色集群最大数量
#define VOICE_UP_DB 315        // 环境声强上限
#define VOICE_DOWN_DB 250      // 环境声强下限
#define VOICE_WIDTH_DB ((VOICE_UP_DB) - (VOICE_DOWN_DB))  // 环境声强变化宽度

#define SYSTEM_INTERVAL_MS 8            // 系统频率在125hz左右
#define LONG_HOLDING_COUNTER_LIMIT 250  // 按住超过2秒视为长按

#define WARM_WHITE (CRGB(255, 248, 220))  // 纯色照明模式下灯珠颜色
```

## 4. 视频展示

[视频链接](https://www.youtube.com/watch?v=rtI6ewEUtK8&ab_channel=%E5%BC%A0%E4%B9%8B%E7%9D%BF)
