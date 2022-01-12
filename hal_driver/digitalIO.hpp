#pragma once

#ifdef HAL_GPIO_MODULE_ENABLED
#include "main.h"
namespace ishihalib {

class DigitalOut {
	GPIO_TypeDef* port_;
	uint16_t pin_;

  public:
	DigitalOut(GPIO_TypeDef* port, uint16_t pin) : port_(port), pin_(pin) {}

	void write(GPIO_PinState state) {
		HAL_GPIO_WritePin(port_, pin_, state);
	}

	GPIO_PinState read() {
		return HAL_GPIO_ReadPin(port_, pin_);
	}

	void toggle() {
		HAL_GPIO_TogglePin(port_, pin_);
	}

	DigitalOut& operator=(GPIO_PinState state) {
		write(state);
		return *this;
	}
	DigitalOut& operator=(DigitalOut& dio) {
		write(dio.read());
		return *this;
	}

	DigitalOut& operator++() {
		toggle();
		return *this;
	}

	operator int() {
		return read();
	}
};

class DigitalIn {
	GPIO_TypeDef* port_;
	uint16_t pin_;

  public:
	DigitalIn(GPIO_TypeDef* port, uint16_t pin) : port_(port), pin_(pin) {}

	GPIO_PinState read() {
		return HAL_GPIO_ReadPin(port_, pin_);
	}

	operator int() {
		return read();
	}
};

}  // namespace ishihalib
#endif