/*
 * hal_callback.cpp
 *
 *  Created on: Feb 13, 2025
 *      Author: ishik
 */

#ifndef INC_ISHIHALIB_STM32_HAL_DRIVER_HAL_CALLBACK_CPP_
#define INC_ISHIHALIB_STM32_HAL_DRIVER_HAL_CALLBACK_CPP_

#include "main.h"

#include "./timer.hpp"

#ifdef HAL_TIM_MODULE_ENABLED
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) { ishihalib::Timer::hal_it_callback(htim); }
#endif // HAL_TIM_MODULE_ENABLED

#endif /* INC_ISHIHALIB_STM32_HAL_DRIVER_HAL_CALLBACK_CPP_ */
