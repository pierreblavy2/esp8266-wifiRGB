#ifndef TIMER_PIERRE_HPP_
#define TIMER_PIERRE_HPP_

#include <functional>

struct Timer{
  unsigned int period = 0;
  unsigned int last_tick = 0;
  std::function<void()> callback=[](){};

  Timer(){}
  explicit Timer(const unsigned int period_):period(period_){}

  void update(unsigned int now){
    auto delta = now-last_tick;
    if(delta > period){
      last_tick+=period;
      callback();
    }    
  }

};




#endif
