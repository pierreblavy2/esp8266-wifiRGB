#include "D1mini.hpp"

#define LED_R D1mini_D1
#define LED_G D1mini_D7
#define LED_B D1mini_D2



//--- HELPERS : blink pin scanner --
/*
void setup(){
  Serial.begin(9600);
  scan_out_pins_setup();
}

void loop(){
  scan_out_pins_loop();
}
*/

#include <array>  
inline static constexpr std::array<int,9>         D1mini_out       = {D1mini_D0, D1mini_D1, D1mini_D2, D1mini_D3, D1mini_D4, D1mini_D5,D1mini_D6,D1mini_D7,D1mini_D8};
inline static constexpr std::array<const char*,9> D1mini_out_names = {"D1mini_D0", "D1mini_D1", "D1mini_D2", "D1mini_D3", "D1mini_D4", "D1mini_D5","D1mini_D6","D1mini_D7","D1mini_D8"};

//don't forget Serial.begin(9600);
inline void scan_out_pins_setup(){
    for(const int pin : D1mini_out){
        pinMode(pin,OUTPUT);
    }  
}

inline void scan_out_pins_loop(decltype(LOW) default_status){
  decltype(LOW) A;
  decltype(LOW) B;
  if(default_status==LOW){A = HIGH; B=LOW;}
  else{A=LOW; B=HIGH;}

  for(size_t i=0; i< D1mini_out.size(); ++i){
          digitalWrite(D1mini_out[i], B );
  }

  
  for(size_t i=0; i< D1mini_out.size(); ++i){
    Serial.print(D1mini_out[i]);
    Serial.print(" ");
    Serial.println(D1mini_out_names[i]);
    for(size_t k = 0; k < 10; ++k){
      digitalWrite(D1mini_out[i], A);
      delay(200);
      digitalWrite(D1mini_out[i], B);
      delay(200);
    }
  }
}
