#include "Channel.h"
#include "UnoJoy.h"


#define RC_CHANS 2
enum {
  STEERING,
  THROTTLE
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////  TO BE COMPLETED FOR EACH CHANNEL  /////////////////////////////////////////////////////////

// int connection_pin, uint16_t pwm_cycle_duration_us, float min_ratio, float max_ratio;
ChannelCharacteristics steering_channel = {6, 18500, 0.059, 0.101};
ChannelCharacteristics throttle_channel = {7, 18500, 0.049, 0.111};

// Put all characteristics arrays above in the array below
ChannelCharacteristics channels_data[RC_CHANS] = {steering_channel, throttle_channel};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

Channel* rc_channels[RC_CHANS];
volatile uint16_t rc_value[RC_CHANS] = {8, 8};




void setup() {
//  Serial.begin(115200); 
  setupUnoJoy();

  for (int i=0; i<RC_CHANS; i++){
    pinMode(channels_data[i].connection_pin, INPUT);
    rc_channels[i] = new Channel(i,channels_data[i]); 
  }
  attach_all_interruptions<RC_CHANS>();
  
}

void loop() {  
  for (int i=0; i<RC_CHANS; i++){
    int out = rc_channels[i]->compute_output();
//    Serial.print("Channel ");
//    Serial.print(i);
//    Serial.print(": ");
//    Serial.print(out);
//    Serial.print('\t');
    rc_value[i] = out;
  }
//  Serial.println("");
//  delay(5);

  // Always be getting fresh data
  dataForController_t controllerData = getControllerData();
  setControllerData(controllerData);
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



dataForController_t getControllerData(void){
  // Set up a place for our controller data
  //  Use the getBlankDataForController() function, since
  //  just declaring a fresh dataForController_t tends
  //  to get you one filled with junk from other, random
  //  values that were in those memory locations before
  dataForController_t controllerData = getBlankDataForController();
  // Since our buttons are all held high and
  //  pulled low when pressed, we use the "!"
  //  operator to invert the readings from the pins
  
//  controllerData.triangleOn = !digitalRead(2);
//  controllerData.circleOn = !digitalRead(3);
//  controllerData.squareOn = !digitalRead(4);
//  controllerData.crossOn = !digitalRead(5);
//  controllerData.dpadUpOn = !digitalRead(6);
//  controllerData.dpadDownOn = !digitalRead(7);
//  controllerData.dpadLeftOn = !digitalRead(8);
//  controllerData.dpadRightOn = !digitalRead(9);
//  controllerData.l1On = !digitalRead(10);
//  controllerData.r1On = !digitalRead(11);
//  controllerData.selectOn = !digitalRead(12);
//  controllerData.startOn = !digitalRead(A4);
//  controllerData.homeOn = !digitalRead(A5);
  
  // Set the analog sticks
  //  Since analogRead(pin) returns a 10 bit value,
  //  we need to perform a bit shift operation to
  //  lose the 2 least significant bits and get an
  //  8 bit number that we can use  
//  controllerData.leftStickX = analogRead(A0) >> 2;
//  controllerData.leftStickY = analogRead(A1) >> 2;
//  controllerData.rightStickX = analogRead(A2) >> 2;
//  controllerData.rightStickY = analogRead(A3) >> 2;
//  controllerData.leftStickX = analogRead(A0) >> 2;
  controllerData.leftStickY = rc_value[THROTTLE];
  controllerData.rightStickX = 255 - rc_value[STEERING];
//  controllerData.rightStickY = analogRead(A3) >> 2;
  // And return the data!
  return controllerData;
}
