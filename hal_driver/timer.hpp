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
  int ms_ = 0;
  int count_per_sec_;              // 1カウントが何分の1秒か
  int it_per_count_;               // 何カウントで割り込みが入るか
  unsigned long long n_of_it_ = 0; // 今まで割り込みが入った回数

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
  Timer(TIM_HandleTypeDef *handle, int count_per_sec = 1e6, int it_per_count = 1000)
      : handle_(handle), count_per_sec_(1e6), it_per_count_(1000) {
    // 割り込み開始
    HAL_TIM_Base_Start_IT(handle_);

    // HALから自身のcallback()が呼び出されるように
    timer_handle_its_.insert(std::make_pair(handle_, [this] { it_callback(); }));
  }
  uint32_t get_counter(void) { return __HAL_TIM_GET_COUNTER(handle_); }
  double get_time() { return ((double)n_of_it_ * it_per_count_ + get_counter()) / count_per_sec_; }

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
