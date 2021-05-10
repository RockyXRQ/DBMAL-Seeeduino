#include "Stater.h"

#include <Arduino.h>

/**
 * @brief 触摸开关状态器构造函数
 * @param pin 触摸开关输入引脚
 * @param long_holoding_ms 长按识别时间(毫秒)
 */
Stater::Stater(int pin, int long_holoding_ms) {
    m_switch_state_counter = 0;
    m_counter_limit = long_holoding_ms / SYSTEM_INTERVAL_MS;
    m_switch_state = RELEASE;
    m_system_state = CLOSE;

    m_pin = pin;
    pinMode(m_pin, INPUT);
}

/**
 * @brief 触摸开关状态器更新函数
 * @note 检测按键状态并更新台灯状态
 */
void Stater::StateUpdate() {
    if (digitalRead(m_pin)) {
        m_switch_state = TOUCH;
        m_switch_state_counter += 1;
    } else {
        m_switch_state = RELEASE;

        if (m_switch_state_counter > 0 &&
            m_switch_state_counter < m_counter_limit) {
            // 此时视为开关短按
            if (m_system_state == CLOSE) {
                m_system_state = SOLID;
            } else if (m_system_state == SOLID) {
                m_system_state = MIRAGE;
            } else if (m_system_state == MIRAGE) {
                m_system_state = MUSIC;
            } else if (m_system_state == MUSIC) {
                m_system_state = SOLID;
            }
        }

        if (m_switch_state_counter >= m_counter_limit) {
            m_system_state = CLOSE;
        }

        m_switch_state_counter = 0;
    }
}

/**
 * @brief 获取触摸开关状态器当前系统状态
 * @note 按键状态无法被获取, 只能获取系统状态
 */
int Stater::GetSysState() { return m_system_state; }