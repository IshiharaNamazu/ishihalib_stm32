/*
 * digitalOut.hpp
 *
 *  Created on: 2025/02/13
 *      Author: ishik
 */

#ifndef INC_ISHIHALIB_STM32_HAL_DRIVER_DIGITALIO_HPP_
#define INC_ISHIHALIB_STM32_HAL_DRIVER_DIGITALIO_HPP_

#include "main.h"
#ifdef HAL_GPIO_MODULE_ENABLED

namespace ishihalib::stm32 {

class DigitalOut {
  GPIO_TypeDef *port_;
  uint16_t pin_;
  GPIO_PinState pre_state_ = GPIO_PIN_RESET;

public:
  DigitalOut(GPIO_TypeDef *port, uint16_t pin) : port_(port), pin_(pin) {}
  void write(GPIO_PinState state) {
    pre_state_ = state;
    HAL_GPIO_WritePin(port_, pin_, state);
  }
  void write(bool state) { write(static_cast<GPIO_PinState>(state)); }
  operator bool() { return pre_state_; }
  operator int() { return static_cast<int>(pre_state_); }

  void toggle() { write(pre_state_ == false); }

  DigitalOut &operator=(GPIO_PinState state) {
    write(state);
    return *this;
  }
  DigitalOut &operator=(bool state) {
    write(static_cast<GPIO_PinState>(state));
    return *this;
  }
};

class DigitalIn {
  GPIO_TypeDef *port_;
  uint16_t pin_;
  GPIO_PinState pre_state_ = GPIO_PIN_RESET;

public:
  DigitalIn(GPIO_TypeDef *port, uint16_t pin) : port_(port), pin_(pin) {}

  bool read() { return HAL_GPIO_ReadPin(port_, pin_); }
  operator bool() { return read(); }
  operator int() { return static_cast<int>(read()); }
};

} // namespace ishihalib::stm32

#endif
#endif /* INC_ISHIHALIB_STM32_HAL_DRIVER_DIGITALIO_HPP_ */
