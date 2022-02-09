#pragma once
#include "main.h"
#ifdef HAL_TIM_MODULE_ENABLED
#include <stdint.h>

#include "tim.h"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
namespace ishihalib {
class Encoder {
  private:
	TIM_HandleTypeDef *handle_;
	long long counter_;
	double call_interval_;

  public:
	Encoder(TIM_HandleTypeDef *handle, double call_interval = -1) : handle_(handle), counter_(0), call_interval_(call_interval) {
	}

	void update(double dt = -1) {
	}

  private:
	long long get_count() {
		static long long over_counter = 0;
		static int cnt_prev = 0;
		int cnt = handle_->Instance->CNT;
		if (cnt - cnt_prev > (1 << 15)) over_counter++;	  //カウントがオーバーフロー
		if (cnt - cnt_prev < -(1 << 15)) over_counter--;  //カウントがアンダーフロー
		return over_counter * (1LL << 16) + cnt;
	}
};
}  // namespace ishihalib

#endif
