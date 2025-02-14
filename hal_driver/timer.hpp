/*
 * timer.hpp
 *
 *  Created on: Feb 13, 2025
 *      Author: ishik
 */

#ifndef INC_ISHIHALIB_STM32_HAL_DRIVER_TIMER_HPP_
#define INC_ISHIHALIB_STM32_HAL_DRIVER_TIMER_HPP_

#include "main.h"
#ifdef HAL_TIM_MODULE_ENABLED

#include <functional>
#include <map>

/*
 * 現状初期化処理
 * 1カウント1us，Timerが1ms周期で割り込まれるように設定
 * */
namespace ishihalib::stm32 {

inline std::map<TIM_HandleTypeDef *, std::function<void(void)>> timer_handle_its_; // HALから呼ばれるhandle毎の割り込み

class Timer {
public:
  struct cb_item_t {
    std::function<void(void)> func;
    size_t div;
    size_t cnt;
  };

private:
  TIM_HandleTypeDef *handle_;
  unsigned long long input_clock_;
  int prescaler_;              // 1カウントが何分の1秒か
  int period_;                 // 何カウントで割り込みが入るか
  unsigned long long n_of_it_; // 今まで割り込みが入った回数

  std::multimap<uint8_t, cb_item_t> callback_fns_;

  void it_callback() {
    n_of_it_++;

    for (auto &cb_item_pair : callback_fns_) { // priorityが小さい順に呼び出し
      cb_item_t &cb_item = cb_item_pair.second;
      cb_item.cnt++;
      if (cb_item.cnt == cb_item.div) {
        (cb_item_pair.second).func();
        cb_item.cnt = 0;
      }
    }
  }

  // friend void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *);

public:
  Timer(TIM_HandleTypeDef *handle, unsigned long long input_clock = 0) : handle_(handle), n_of_it_(0) {
    prescaler_ = handle->Init.Prescaler;
    period_ = handle->Init.Period;

    input_clock_ = (input_clock == 0) ? (0.5 + ((prescaler_ + 1LL) * 1e6))
                                      : input_clock; // 指定されなければ1us周期でカウントするという仮定

    // 割り込み開始
    HAL_TIM_Base_Start_IT(handle_);

    // HALから自身のcallback()が呼び出されるように
    timer_handle_its_.insert(std::make_pair(handle_, [this] { it_callback(); }));
  }

  void set_clock_configration(int new_prescaler, int new_period) {
    // Auto Reload PreloadをEnableにすることを推奨

    // 時間の整合性(未検証)
    n_of_it_ =
        ((n_of_it_ + 1ULL) * (period_ + 1ULL) * (prescaler_ + 1ULL)) / ((new_prescaler + 1ULL) * (new_period + 1ULL));

    prescaler_ = new_prescaler;
    period_ = new_period;

    handle_->Init.Prescaler = new_prescaler;
    handle_->Init.Period = new_period;

    if (handle_->Init.AutoReloadPreload == TIM_AUTORELOAD_PRELOAD_ENABLE) {
      handle_->Instance->PSC = new_prescaler;
      handle_->Instance->ARR = new_period;
    } else {
      __HAL_TIM_DISABLE(handle_); // タイマー停止
      HAL_TIM_Base_Init(handle_);
      __HAL_TIM_ENABLE(handle_); // 再開
    }
  }

  uint32_t get_counter(void) { return __HAL_TIM_GET_COUNTER(handle_); }
  double get_time() { return (((double)n_of_it_ * (period_ + 1.) + get_counter()) * (prescaler_ + 1.)) / input_clock_; }

  double get_param() { return input_clock_; }

  void attach(std::function<void(void)> func, size_t division = 1, uint8_t priority = 100) {
    // 割り込みdivision回に一回attachで登録した関数が呼ばれる
    // 同時に割り込みが入った際はpriorityが小さい順に実行される(同じpriorityのものに関して呼び出される順番は不明)
    // ↑priorityほぼ意味ないね．
    if (division <= 0)
      division = 1;
    callback_fns_.insert({priority, {func, division, 0}});
  }
};

} // namespace ishihalib::stm32

#endif /* INC_ISHIHALIB_STM32_HAL_DRIVER_TIMER_HPP_ */
#endif
