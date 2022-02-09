#pragma once
#ifdef HAL_TIM_MODULE_ENABLED
#include <functional>
#include <map>
#include <utility>

#include "tim.h"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
namespace ishihalib {
class Ticker {
  private:
	TIM_HandleTypeDef *handle_;
	const long call_freq_;																			//割り込みが呼ばれるインターバル
	const long mx_counter_period_;																	//cubeideで設定したカウンター
	long long callback_counter_;																	//経過時間
	std::array<std::vector<std::pair<std::function<void(void)>, long>>, 4> callback_func_;			//callback_func_[priority][index]
	inline static std::map<TIM_HandleTypeDef *, std::function<void(void)>> timer_callbackcallback;	//コールバックのコールバック

  public:
	Ticker(TIM_HandleTypeDef *handle, const long call_freq = 1000, const long mx_counter_period = 1000)
		: handle_(handle), call_freq_(call_freq), mx_counter_period_(mx_counter_period), callback_counter_(0) {
		HAL_TIM_Base_Stop_IT(handle_);
		timer_callbackcallback.insert(std::make_pair(handle, [&]() {  //これ全体のコールバック
			callback_counter_++;
			for (int priority = callback_func_.size() - 1; priority >= 0; priority--)  //優先度大きい順に関数実行
				for (auto &f : callback_func_[priority])
					if (!(callback_counter_ % f.second)) (f.first)();
		}));
		HAL_TIM_Base_Start_IT(handle);
	}

	void add_callback(std::function<void(void)> fnc, double interval = 1, uint8_t priority = 0) {  //priority 0:low 3:high 4:extreme
		callback_func_[priority].push_back({fnc, static_cast<int>(interval * call_freq_ + 0.5)});  //最も近い呼び出し周期(回数)で呼び出す
	}

	static void timers_callback(TIM_HandleTypeDef *htim) {	//HAL_TIM_PeriodElapsedCallbackから呼ばれるための関数
		(timer_callbackcallback[htim])();
	}

	double get_time() {
		return (double)callback_counter_ / call_freq_;
	}
};
}  // namespace ishihalib

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	ishihalib::Ticker::timers_callback(htim);
}

#endif
