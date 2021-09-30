#pragma once
#include <vector>

#include "main.h"
#include "stdio.h"

class ADC_DMA {
	ADC_HandleTypeDef* handle;
	std::vector<uint16_t> buf;
	//uint16_t buf[16] = {};
	const int NumOfChannel;

  public:
	ADC_DMA(ADC_HandleTypeDef* _handle, const int _NumOfChannel) : handle(_handle), NumOfChannel(_NumOfChannel) {
		buf.resize(NumOfChannel);
		if (HAL_ADCEx_Calibration_Start(handle, ADC_SINGLE_ENDED) != HAL_OK) Error_Handler();
		HAL_ADC_Start_DMA(handle, (uint32_t*)&buf[0], 1);
	}

	uint16_t read(int ch) {
		//if (ch < 0 || ch >= NumOfChannel) return 4097;
		//printf("%d\n", buf[ch]);
		uint16_t ret = buf[ch];
		HAL_ADC_Start_DMA(handle, (uint32_t*)&buf[0], 1);

		return ret;
	}
};