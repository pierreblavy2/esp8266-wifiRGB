#include "config-wires.hpp"
#include "State.hpp"
#include "Fallback_wifi.hpp"
#include "page.hpp"
#include "Timer.hpp"

#include <LittleFS.h>
#include <ESP8266WebServer.h>


#ifndef PWMRANGE
  #define PWMRANGE 1023
#endif

inline void set_color(const Rgb& rgb){
  static byte old_r = 0;
  static byte old_g = 0;
  static byte old_b = 0;
 
  if(rgb.r!=old_r){  analogWrite(LED_R, map(rgb.r,0,255, 0,PWMRANGE ) ); old_r=rgb.r;}
  if(rgb.g!=old_g){  analogWrite(LED_G, map(rgb.g,0,255, 0,PWMRANGE ) ); old_g=rgb.g;}
  if(rgb.b!=old_b){  analogWrite(LED_B, map(rgb.b,0,255, 0,PWMRANGE ) ); old_b=rgb.b;}
}

inline void set_color(byte r, byte g, byte b){
  Rgb x;
  x.r=r; x.g=g; x.b=b;
  set_color(x);
}

template<typename T>
inline T map_t(T x, T in_min, T in_max, T out_min, T out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



//--- static data ---
State state;

Fallback_wifi fallback_wifi;
void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();
ESP8266WebServer server(80);

Timer timer_wifi    (10000);
Timer timer_log_wifi(30000);


void setup(){
  Serial.begin(9600);
  analogWriteRange(PWMRANGE);

  //--- default color : black ---
  analogWrite(LED_R,0);
  analogWrite(LED_G,0);
  analogWrite(LED_B,0);

    //--- filesystem ---
  if(!LittleFS.begin()){ 
    Serial.println("LittleFS ERROR");  
  }else{
        Serial.println("LittleFS OK");  
  }
  
  state.load();


  //=== This code should be run once, to initialise the filesystem ===
  /*LittleFS.format();
    state.make_default();
    state.set_wifi_ssid("xxxx");
    state.set_wifi_pass("xxxx");
    state.save();*/
  
  //--- WIFI (depends on filesystem) ---
  fallback_wifi.st_addAP(state.get_wifi_ssid(), state.get_wifi_pass() );
  fallback_wifi.ap_setAP("Lampe","");
  fallback_wifi.begin();
    
  //--- server (depends on wifi) ---
  server.on        ("/", handleRoot);       // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.begin();
  Serial.println("HTTP server started");

  //--- Timer callbacks ---
  timer_wifi.callback=[](){
      fallback_wifi.update();
  };
  
  timer_log_wifi.callback=[](){
    Serial.println(fallback_wifi.str_mode());
    Serial.println(fallback_wifi.str_mac());
    Serial.println(fallback_wifi.str_ip());
    Serial.println(fallback_wifi.str_stationNum());
    Serial.println(fallback_wifi.str_timeout());
    Serial.println(fallback_wifi.str_ssid());
    Serial.println();
    state.print();
  };
  
}

Rgb color_mix(const Rgb&a, const Rgb &b, double p){
 if(p>1){p=1;}
 if(p<0){p=0;}

 Rgb r;
 r.r = static_cast<double>(a.r) + p*(static_cast<double>(b.r) - static_cast<double>(a.r) );
 r.g = static_cast<double>(a.g) + p*(static_cast<double>(b.g) - static_cast<double>(a.g) );
 r.b = static_cast<double>(a.b) + p*(static_cast<double>(b.b) - static_cast<double>(a.b) );
 return r;
}



void update_color(double p){
  //p is a value between 0(begin cycle) and 1 (end cycle)
  //note that when there are n colors, C0 ... Cn, the cycle is C0->C1->...Cn->C0

  switch(state.color_number){
    case 2 : {
      if(p<0.5){set_color(color_mix(state.colors[0],state.colors[1], 2.0*p));       return;}
      else     {set_color(color_mix(state.colors[1],state.colors[0], 2.0*(p-0.5))); return;}
    }
    case 3 : {      
      if     (p<1.0/3.0){ set_color(color_mix(state.colors[0],state.colors[1], 3.0*p          )); return;}
      else if(p<2.0/3.0){ set_color(color_mix(state.colors[1],state.colors[2], 3.0*(p-1.0/3.0))); return;}
      else              { set_color(color_mix(state.colors[2],state.colors[0], 3.0*(p-2.0/3.0))); return;}
    }
    case 4 : {
      if     (p<1.0/4.0){ set_color(color_mix(state.colors[0],state.colors[1], 4.0*p          )); return;}
      else if(p<2.0/4.0){ set_color(color_mix(state.colors[1],state.colors[2], 4.0*(p-1.0/4.0))); return;}
      else if(p<3.0/4.0){ set_color(color_mix(state.colors[2],state.colors[3], 4.0*(p-2.0/4.0))); return;}
      else              { set_color(color_mix(state.colors[3],state.colors[0], 4.0*(p-3.0/4.0))); return;}
    }
    default: {set_color(state.colors[0]); return;}
  }
}


unsigned long run_once=0;
unsigned char frame_id=0;
unsigned long last_color_period = 0;

void loop(){
  ++frame_id;
  unsigned long current_time = millis();

  //--- LEDS ---
  unsigned long color_delta = current_time-last_color_period;
  if(color_delta > state.color_delay){
    color_delta=0;
    last_color_period=current_time;
  }
  double color_advance = static_cast<double>(color_delta)/static_cast<double>(state.color_delay);
  if(color_advance<0){color_advance=0;}
  
  update_color(color_advance);
  

  //--- WEB server ---
  server.handleClient();

  //--- Timers ---
  timer_wifi.update(current_time);
  timer_log_wifi.update(current_time);
  
}


void debug_GET(){
  //doc: https://techtutorialsx.com/2016/10/22/esp8266-webserver-getting-query-parameters/
  Serial.print("number_of_args:");
  Serial.println(server.args());
  Serial.println("Args:");
  for (int i = 0; i < server.args(); i++) {
    Serial.print(i);Serial.print(":");
    Serial.print( server.argName(i));
    Serial.print("->");
    Serial.println(server.arg(i));
  } 
  Serial.println("end of Args");
}


void handleRoot() {
  Serial.println("serving/");
  debug_GET();

  //--- change state according to args ---
  debug_GET();

  bool color_change=false;
  bool wifi_change = false;
  byte nb_colors = 0;

  if(server.args()>1){    
    for (int i = 0; i < server.args(); i++) {
      const auto & arg_name   = server.argName(i);
      const auto & arg_value  = server.arg(i);

      //--- wifi ssid and pass (don't forget to save them latter) ---
      if( arg_name=="wifi_ssid")  {
        Serial.println("set wifi_ssid");
        if(arg_value==state.get_wifi_ssid()){continue;}//no change
        //change
        state.set_wifi_ssid( arg_value ); 
        wifi_change=true;
        continue;
      }
      
      if( arg_name=="wifi_pass" and arg_value!="" ){
        if(arg_value==state.get_wifi_pass()){continue;}//no change
        //change
        state.set_wifi_pass( arg_value ); 
        wifi_change=true;
        continue;
      }

      //--- colors ---
      if( arg_name=="c0" ){ auto v = to_rgb(arg_value);  if(v.has_value()){state.colors[0]=v.value(); color_change=true; nb_colors=max(nb_colors,byte(1)); } }
      if( arg_name=="c1" ){ auto v = to_rgb(arg_value);  if(v.has_value()){state.colors[1]=v.value(); color_change=true; nb_colors=max(nb_colors,byte(2)); } }
      if( arg_name=="c2" ){ auto v = to_rgb(arg_value);  if(v.has_value()){state.colors[2]=v.value(); color_change=true; nb_colors=max(nb_colors,byte(3)); } }
      if( arg_name=="c3" ){ auto v = to_rgb(arg_value);  if(v.has_value()){state.colors[3]=v.value(); color_change=true; nb_colors=max(nb_colors,byte(4)); } }

      if(nb_colors != state.color_number){
         color_change=true; 
         state.color_number=nb_colors;
         Serial.print("change color_number"); Serial.println(state.color_number);
      }

      if( arg_name=="duration" ){
        double duration_d = arg_value.toDouble(); //error returns 0
        if(duration_d <0){duration_d=0;}
        unsigned int duration = 1000*duration_d;//seconds to milliseconds
        
        if(state.color_delay != duration){
            color_change=true; 
            state.color_delay=duration;
          }
      }
    }//end for
  }//end if(server.args()>1)

  if(color_change or wifi_change){
    state.save();
    state.print();
  }

  if(wifi_change){
    //reconnect
    Serial.print("RESET WIFI to "); Serial.println(state.get_wifi_ssid());
    //WiFi.mode(WIFI_OFF);
    fallback_wifi.mode_disconnect();
    fallback_wifi.st_cleanAP();
    fallback_wifi.st_addAP(state.get_wifi_ssid(),state.get_wifi_pass());    
  }

  if(color_change){
    //Do nothing as main loop already calls color_update
  }

  
  //--- serve page ---
  Serial.println("serve_page");
  String p = page;

  //--- page wifi ---
  p.replace("$wifi_mode$",        fallback_wifi.str_mode());
  p.replace("$wifi_mac$" ,        fallback_wifi.str_mac());
  p.replace("$wifi_ip$"  ,        fallback_wifi.str_ip());
  p.replace("$wifi_ssid$",        fallback_wifi.str_ssid());
  p.replace("$wifi_stationNum$",  fallback_wifi.str_stationNum());
  p.replace("$wifi_timeout$",     fallback_wifi.str_timeout());
  p.replace("$wifi_pass$",""); //dont't show password in page
  p.replace("$wifi_st_ssid$",state.wifi_ssid);

  //--- page color ---
  p.replace("$c0$",to_string(state.colors[0]));
  p.replace("$c1$",to_string(state.colors[1]));
  p.replace("$c2$",to_string(state.colors[2]));
  p.replace("$c3$",to_string(state.colors[3]));

  p.replace("$duration$",String(state.color_delay/1000.0));
  p.replace("$show_me$",String(state.color_number));

  server.sendHeader("Cache-Control"," no-cache");
  server.send(200, "text/html", p);
}


void handleNotFound(){
  Serial.println("ERROR 404 => root");
  handleRoot();
  //server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
