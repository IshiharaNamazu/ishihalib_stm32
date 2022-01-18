#pragma once
#ifdef HAL_GPIO_MODULE_ENABLED
#include <array>
#include <cstdint>

#include "../hal_driver/digitalIO.hpp"
#include "main.h"
namespace ishihalib {

class SevenSegmentLED {
  public:
	SevenSegmentLED(std::array<DigitalOut, 7> leds) : leds_(leds),
													  number_images{
														  0b00111111,  //0
														  0b00000110,  //1
														  0b01011011,  //2
														  0b01001111,  //3
														  0b01100110,  //4
														  0b01101101,  //5
														  0b01111101,  //6
														  0b00000111,  //7
														  0b01111111,  //8
														  0b01101111,  //9
														  0b01000000   //-
													  } {}
	void write_raw(uint8_t data) {
		for (int i = 0; i < 7; i++)
			leds_[i] = (data >> i) & 1;
	}

	void write(int num) {
		if (num > 9 || num < 0)
			write_raw(number_images[10]);
		else
			write_raw(number_images[num]);
	}

	SevenSegmentLED& operator=(int num) {
		write(num);
		return *this;
	}

  private:
	std::array<DigitalOut, 7> leds_;
	/*
   0
  5 1
   6
  4 2
   3
  */

	std::array<uint8_t, 11> number_images;
};
}  // namespace ishihalib
#endif