#ifndef CHANNEL_H
#define CHANNEL_H

#include "PinChangeInterrupt.h"


struct ChannelCharacteristics {
  int connection_pin;
  uint16_t pwm_cycle_duration_us; 
  float min_ratio;
  float max_ratio;
};


class Channel {
  private:
    uint16_t high_state_duration, time_rising, time_rising_prev;
    uint16_t pwm_cycle_duration_us; 
    float min_ratio;
    float max_ratio;
    
    void rx_rising();
    void rx_falling();
    
  public:
    int id;
    int connection_pin;
    
    Channel(int id, 
            ChannelCharacteristics characteristics);
    void rx_interrupt_routine();
    int compute_output();
};



#endif
