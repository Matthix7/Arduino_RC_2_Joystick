#include "Channel.h"

int Channel::channels_number = 0;

Channel::Channel(int id, 
                 ChannelCharacteristics characteristics){
  ++channels_number;
  this->id = id;
  this->connection_pin = characteristics.connection_pin;
//  this->pwm_cycle_duration_us = characteristics.pwm_cycle_duration_us;
  this->min_ratio = characteristics.min_ratio;
  this->max_ratio = characteristics.max_ratio; 
}



int Channel::compute_output(void){
  int output, raw, restricted;
  raw = 1000 * (float)this->high_state_duration/this->pwm_cycle_duration_us;
  restricted = constrain(raw, 1000 * this->min_ratio, 1000 * this->max_ratio);
  output = map(restricted, 1000 * this->min_ratio, 1000 * this->max_ratio, 0, 255);

  Serial.print(this->id);
  Serial.print("\t");
  Serial.print(this->high_state_duration);
  Serial.print("\t");
  Serial.print(this->pwm_cycle_duration_us);
  Serial.print("\t");
  Serial.print((float)this->high_state_duration/this->pwm_cycle_duration_us);
  if (this->id==channels_number-1) {Serial.println("");} else {Serial.print("\t");}
  
  return output;
}



void Channel::rx_interrupt_routine(void){
  if (digitalRead(this->connection_pin) == HIGH){
    rx_rising();
  }  
  else{
    rx_falling();
  }
}


void Channel::rx_rising(void) {
  this->time_rising = micros();
  this->pwm_cycle_duration_us = this->time_rising - this->time_rising_prev;
  this->time_rising_prev = this->time_rising;
}

void Channel::rx_falling(void) {
  uint16_t time_falling = micros();

//  if (time_falling>this->time_rising && 
//      time_falling-this->time_rising < this->pwm_cycle_duration_us){
    this->high_state_duration = time_falling - this->time_rising;
//  }
}
