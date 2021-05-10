#ifndef __CONFIG_H
#define __CONFIG_H

#include <FastLED.h>

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

#endif