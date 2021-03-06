#include "Channel.h"



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////  TO BE COMPLETED FOR EACH CHANNEL  /////////////////////////////////////////////////////////

#define RC_CHANS 4
enum {
  ROLL,
  PITCH,
  THROTTLE,
  YAW
};

// int connection_pin, uint16_t pwm_cycle_duration_us, float min_ratio, float max_ratio;
// For this program, only connection pins matters. Leave the rest unchanged.
ChannelCharacteristics roll_channel = {5, 18500, 0.059, 0.101};
ChannelCharacteristics pitch_channel = {6, 18500, 0.059, 0.101};
ChannelCharacteristics throttle_channel = {7, 18500, 0.049, 0.111};
ChannelCharacteristics yaw_channel = {8, 18500, 0.049, 0.111};

// Put all characteristics arrays above in the array below
ChannelCharacteristics channels_data[RC_CHANS] = {roll_channel, pitch_channel, throttle_channel, yaw_channel};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

Channel* rc_channels[RC_CHANS];
volatile uint16_t rc_value[RC_CHANS] = {8, 8, 8, 8};




void setup() {
  Serial.begin(115200); 

  for (int i=0; i<RC_CHANS; i++){
    pinMode(channels_data[i].connection_pin, INPUT);
    rc_channels[i] = new Channel(i,channels_data[i]); 
  }
  attach_all_interruptions<RC_CHANS>();
  
}

void loop() {
  for (int i=0; i<RC_CHANS; i++){
    int out = rc_channels[i]->compute_output();
  }
  delay(5);
}



template<int channel_index>
void interrupt_routine(){
  Channel* channel = rc_channels[channel_index];
  channel->rx_interrupt_routine();
}


template<int total_indices>
void attach_all_interruptions(){
  // Not a native interrupt, allows any pin to act as interrupt. Arduino Uno/Nano/Mini: All pins are usable
  attachPCINT(digitalPinToPCINT(rc_channels[total_indices-1]->connection_pin), interrupt_routine<total_indices-1>, CHANGE);
  attach_all_interruptions<total_indices-1>();  
}
template<>
void attach_all_interruptions<1>(){
  // Not a native interrupt, allows any pin to act as interrupt. Arduino Uno/Nano/Mini: All pins are usable
  attachPCINT(digitalPinToPCINT(rc_channels[0]->connection_pin), interrupt_routine<0>, CHANGE);
}
