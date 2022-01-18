#pragma once

#ifdef HAL_GPIO_MODULE_ENABLED
#include "main.h"
namespace ishihalib {

class DigitalOut {
	GPIO_TypeDef* port_;
	uint16_t pin_;

  public:
	DigitalOut(GPIO_TypeDef* port, uint16_t pin) : port_(port), pin_(pin) {}

	void write(int state) {
		HAL_GPIO_WritePin(port_, pin_, static_cast<GPIO_PinState>(state));
	}

	int read() {
		return static_cast<int>(HAL_GPIO_ReadPin(port_, pin_));
	}

	void toggle() {
		HAL_GPIO_TogglePin(port_, pin_);
	}

	DigitalOut& operator=(int state) {
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

	int read() {
		return static_cast<int>(HAL_GPIO_ReadPin(port_, pin_));
	}

	operator int() {
		return read();
	}
};

}  // namespace ishihalib
#endif