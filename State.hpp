#ifndef STATE_HPP_pierre_
#define STATE_HPP_pierre_

#include <Arduino.h>
#include <optional>
#include "LittleFS.h"

namespace fs{class File;}

struct Rgb{
  byte r;
  byte g;
  byte b;
  void make_default(){r=0;g=0;b=0;}
};
static_assert(std::is_trivial<Rgb>::value, "Rgb must be a trivial class");

std::optional<Rgb> to_rgb(const String &s);
String to_string(const Rgb &);


//--- persistant state ---
struct State {
    void make_default();
    void save(fs::File& f);
    void load(fs::File& f);


    void save();
    void load();
    

    void print()const;
    
    //--- wifi ssid and pass ---
    void set_wifi_ssid(const char*);
    void set_wifi_pass(const char*); 
    void set_wifi_ssid(const String&s){set_wifi_ssid(s.c_str());}
    void set_wifi_pass(const String&s){set_wifi_pass(s.c_str());}
    const char* get_wifi_ssid()const{return wifi_ssid;}
    const char* get_wifi_pass()const{return wifi_pass;}

    //--- DATA WIFI ---
    char wifi_ssid[33];
    char wifi_pass[128];
    
    //--- DATA COLORS ---
    byte         color_number;
    unsigned int color_delay; //ms
    std::array<Rgb,4> colors;     
};

#endif
