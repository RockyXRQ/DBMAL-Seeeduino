#include <Arduino.h>
#line 1 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBMAL\\DBMAL.ino"
#pragma message "DBMAL(Arduino-Demo) version 2.000.000"

#define DEBUG_MODE 0

#include <FastLED.h>
#include <SAMDTimerInterrupt.h>

#include "Stater.h"
#include "Strip.h"
#include "config.h"

SAMDTimer m_timer_tcc{TIMER_TCC};  // SAMD21定时器句柄
Stater m_stater{SWITCH_PIN, LONG_HOLDING_COUNTER_LIMIT};
Strip m_strip;

/**
 * @brief DBMAL定时器执行函数
 * @note 检查更新系统状态 -> 根据系统状态执行对应颜色方案 -> 更新灯珠颜色
 */
#line 20 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBMAL\\DBMAL.ino"
static void DBMAL_Event();
#line 46 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBMAL\\DBMAL.ino"
void setup();
#line 55 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBMAL\\DBMAL.ino"
void loop();
#line 20 "c:\\Personal\\University\\Code\\C_CPP\\Arduino_IDE\\DBMAL\\DBMAL.ino"
static void DBMAL_Event() {
    m_stater.StateUpdate();

    switch (m_stater.GetSysState()) {
        case CLOSE:
            m_strip.Close();
            break;

        case SOLID:
            m_strip.Solid();
            break;

        case MIRAGE:
            m_strip.Mirage();
            break;

        case MUSIC:
            m_strip.Music();
            break;
    }
    m_strip.UpdateFade();
    // m_strip.Update();

    FastLED.show();
}

void setup() {
    // 增加定时器中断执行函数, 每8ms执行一次
    m_timer_tcc.attachInterruptInterval(SYSTEM_INTERVAL_MS * 1000, DBMAL_Event);

#if DEBUG_MODE
    Serial.begin(115200);
#endif
}

void loop() {
#if DEBUG_MODE

#endif
}

