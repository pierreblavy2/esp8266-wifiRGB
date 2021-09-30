#include "State.hpp"
#include "LittleFS.h"    // https://projetsdiy.fr/esp8266-lire-ecrire-modifier-fichier-librairie-littlefs/   
#include<cctype> //isdigit


//=== RGB ===
namespace{
byte convertCharToHex(char x){
  switch(x){
    case '0':return 0;
    case '1':return 1;
    case '2':return 2;
    case '3':return 3;
    case '4':return 4;
    case '5':return 5;
    case '6':return 6;
    case '7':return 7;
    case '8':return 8;
    case '9':return 9;
    case 'a':return 10;
    case 'b':return 11;
    case 'c':return 12;
    case 'd':return 13;
    case 'e':return 14;
    case 'f':return 15;
    case 'A':return 10;
    case 'B':return 11;
    case 'C':return 12;
    case 'D':return 13;
    case 'E':return 14;
    case 'F':return 15;
  }
  return 0;
}


char convertToChar(byte x){
  switch(x){
    case 0 :return '0';
    case 1 :return '1';
    case 2 :return '2';
    case 3 :return '3';
    case 4 :return '4';
    case 5 :return '5';
    case 6 :return '6';
    case 7 :return '7';
    case 8 :return '8';
    case 9 :return '9';
    case 10:return 'a';
    case 11:return 'b';
    case 12:return 'c';
    case 13:return 'd';
    case 14:return 'e';
    case 15:return 'f';
  }
  return '0';
}




//s must be 1 or 2 digits
byte hexstr_to_byte(const String &s){
  if(s.length()==1){
    return convertCharToHex(s[0]);
  }else{
        return convertCharToHex(s[0])*16 + convertCharToHex(s[1]);
  }
}



String byte_to_hexstr(byte b){
 return String(convertToChar(b&0xF0>>4)) + convertToChar(b&0x0F);
}


}//end namespace{



std::optional<Rgb> to_rgb(const String &s){
  if(s.length() != 7){return std::optional<Rgb>();}
  if(s[0]!='#')    {return std::optional<Rgb>();}
  Rgb r;
  r.r=hexstr_to_byte(s.substring(1,3));
  r.g=hexstr_to_byte(s.substring(3,5));
  r.b=hexstr_to_byte(s.substring(5,7));  
  return r;
}



String to_string(const Rgb &x){
  return "#"+byte_to_hexstr(x.r)+byte_to_hexstr(x.g)+byte_to_hexstr(x.b);
}



//=== State ===
void State::save(){
      //File f = SPIFFS.open("config", "w");
      File f = LittleFS.open("config", "w");
      if(f){
        this->save(f);
        Serial.println("State saved");
        //this->print();
        f.close();        
      }else{
        Serial.println("ERROR cannot save state");
      }      
}


void State::load(){
    //File f = SPIFFS.open("config", "r");
    File f = LittleFS.open("config", "r");
    if(f){
      Serial.println("State loaded");
      this->load(f);
      //Serial.println("---loaded as---");
      //this->print();
    }else{
      Serial.println("No configuration file, create default state");
      this->make_default();
    }
    f.close();
}



void State::make_default(){
    //default initialisation
    memset(this, 0, sizeof(State) );
    set_wifi_ssid("MY_SSID");
    set_wifi_pass("MY_PASSWORD");

    for(Rgb &color : colors){
      color.make_default();
    }
    color_number=1;
    color_delay=1000;//ms
}


void State::set_wifi_ssid(const char*s){
  static constexpr size_t n = sizeof(wifi_ssid)/sizeof(char);
  strncpy(wifi_ssid,s, n);
  wifi_ssid[n-1]='\0';
}


void State::set_wifi_pass(const char*s){
  static constexpr size_t n = sizeof(wifi_pass)/sizeof(char);
  strncpy(wifi_pass,s, n);
  wifi_pass[n-1]='\0';
}


 

void State::print()const{
	Serial.print("SSID:"); Serial.println(wifi_ssid);
	//Serial.print("PASS:"); Serial.println(wifi_pass);
	Serial.print("color_number:");Serial.println(color_number);

  Serial.println("colors:");
  for(const Rgb &color : colors){
    Serial.print("  ");
    Serial.print(color.r);Serial.print(" ");
    Serial.print(color.g);Serial.print(" ");
    Serial.print(color.b);Serial.print(" ");
    
    Serial.println(to_string(color));
   }


  /*Serial.print("raw ");
  Serial.print( sizeof(State) );
  Serial.print("{");
  const char* x = reinterpret_cast<const char*>(this);
  String s;
  for(size_t i = 0; i < sizeof(State); ++i){
    s+=String(int(x[i])) + " ";
  }
  Serial.print(s);
  Serial.println("}");*/
 
	Serial.println();
}




static_assert(std::is_trivial<State>::value, "State::save and State::load implementation expect a trivial class");
void State::save(fs::File& f){ 
  f.write    (reinterpret_cast<char*>(this), sizeof(State) );

}

void State::load(fs::File& f){ f.readBytes(reinterpret_cast<char*>(this), sizeof(State) ); }
