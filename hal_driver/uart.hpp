/*
 * uart.hpp
 *
 *  Created on: 2025/02/11
 *      Author: ishik
 */
#ifndef INC_ISHIHALIB_STM32_HAL_DRIVER_UART_HPP_
#define INC_ISHIHALIB_STM32_HAL_DRIVER_UART_HPP_

#include "main.h"
#ifdef HAL_UART_MODULE_ENABLED

#include <string>

namespace ishihalib {

class Uart {
  UART_HandleTypeDef *handle_;
  std::string send_str_; // 送信用の一時的なバッファ
public:
  Uart(UART_HandleTypeDef *handle) : handle_(handle) {}

  void write(std::string str, int timeout_ms = 100) {
    send_str_ = str;
    HAL_UART_Transmit(handle_, (uint8_t *)send_str_.c_str(), send_str_.length(), timeout_ms);
  }
};

} // namespace ishihalib

#endif /* INC_ISHIHALIB_STM32_HAL_DRIVER_UART_HPP_ */

#endif
