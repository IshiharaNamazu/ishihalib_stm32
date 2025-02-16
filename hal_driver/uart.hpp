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

namespace ishihalib::stm32 {

class Uart {
  UART_HandleTypeDef *handle_;
  std::string send_str_; // 送信用の一時的なバッファ
  std::string send_buffer_;
  size_t r_dma_buffer_size_ = 0;
  uint8_t *r_dma_buffer_ptr_ = 0;
  size_t r_dma_read_index_ = 0;
  bool use_dma_transmit_with_buf_ = false;

public:
  Uart(UART_HandleTypeDef *handle) : handle_(handle) {}

  void use_dma_transmit_with_buffer(bool use_dma = true) {
    // dma送信と通常の送信を混ぜると問題を起こす可能性がある
    // 割り込みを有効にしないと送信完了しないから気を付けよう!
    use_dma_transmit_with_buf_ = use_dma;
  }
  void write(std::string str, bool skip_send = false, int timeout_ms = 100) {
    if (use_dma_transmit_with_buf_) {
      send_buffer_ += str;

      auto test = handle_->gState;
      if ((!skip_send) && (handle_->gState == HAL_UART_STATE_READY)) {
        send_str_ = send_buffer_;
        send_buffer_ = "";
        HAL_UART_Transmit_DMA(handle_, (uint8_t *)send_str_.c_str(), send_str_.length());
      }
    } else {
      send_str_ = str;
      HAL_UART_Transmit(handle_, (uint8_t *)send_str_.c_str(), send_str_.length(), timeout_ms);
    }
  }

  void start_receive_dma(size_t buffer_size, uint8_t *buffer_ptr = NULL) {
    if (buffer_ptr == NULL) {
      buffer_ptr = (uint8_t *)malloc(buffer_size);
    }
    r_dma_buffer_size_ = buffer_size;
    r_dma_buffer_ptr_ = buffer_ptr;

    HAL_UART_Receive_DMA(handle_, buffer_ptr, buffer_size);
  }
  size_t receive_dma_avalilable() {
    int32_t readable = (r_dma_buffer_size_ - handle_->hdmarx->Instance->NDTR) - r_dma_read_index_;
    if (readable < 0)
      readable += r_dma_buffer_size_;
    return readable;
  }
  uint8_t receive_dma_pop_buffer() {
    if (receive_dma_avalilable() == 0)
      return 0;

    uint8_t ret = r_dma_buffer_ptr_[r_dma_read_index_];
    r_dma_read_index_ = (r_dma_read_index_ + 1) % r_dma_buffer_size_;
    return ret;
  }
};

} // namespace ishihalib::stm32

#endif /* INC_ISHIHALIB_STM32_HAL_DRIVER_UART_HPP_ */

#endif
