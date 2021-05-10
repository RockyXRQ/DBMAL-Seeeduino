#include "Stater.h"

#include <Arduino.h>

Stater::Stater(int pin, int counter_limit) {
    m_switch_state_counter = 0;
    m_counter_limit = counter_limit;
    m_switch_state = RELEASE;
    m_system_state = CLOSE;

    m_pin = pin;
    pinMode(pin, INPUT);
}

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

int Stater::GetSysState() { return m_system_state; }