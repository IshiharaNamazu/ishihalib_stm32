/*
 * dac.hpp
 *
 *  Created on: Feb 14, 2025
 *      Author: ishik
 */
#pragma once

#include "main.h"

#ifdef HAL_DAC_MODULE_ENABLED

namespace ishihalib::stm32 {

class DigitalAnalogConverter { // DACは#defineされてる，あほ
private:
  DAC_HandleTypeDef *handle_;
  uint32_t ch_;
  const uint32_t resolution_bit_;

public:
  DigitalAnalogConverter(DAC_HandleTypeDef *handle, uint32_t ch, const uint32_t resolution_bit = 12)
      : handle_(handle), ch_(ch), resolution_bit_(resolution_bit) {
    HAL_DAC_Start(handle_, ch_);
  }

  void write(float value) {
    if (value < 0)
      value = 0;
    if (value > 1)
      value = 1;
    const uint32_t MAX_VALUE = (1 << resolution_bit_) - 1;
    switch (resolution_bit_) {
    case 8:
      HAL_DAC_SetValue(handle_, ch_, DAC_ALIGN_8B_R, static_cast<uint32_t>(0.5 + value * MAX_VALUE));
      break;

    case 12:
    default:
      HAL_DAC_SetValue(handle_, ch_, DAC_ALIGN_12B_R, static_cast<uint32_t>(0.5 + value * MAX_VALUE));
      break;
    }
  }
};

} // namespace ishihalib::stm32

#endif
