#include <DHT.h>
#include <DHT_U.h>

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//U8G2_SH1106_128X32_VISIONOX_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SH1106_128X32_VISIONOX_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED
//U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
//U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // pin remapping with ESP8266 HW I2C
//U8G2_FOR_ADAFRUIT_GFX u8g2;
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C dsp(U8G2_R0);


#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define TEMP  1
#define HUM   2
#define FUEL  3
#define TIME  4
#define POwER_STATE_MODE 5
#define SETUP 6

byte choise = TEMP;
bool flag = false;
bool power_state = false;

void setup() {
  // put your setup code here, to run once:
  dsp.begin();
  dht.begin();
  pinMode(3, INPUT_PULLUP);
}

void printHumidity() {
  dsp.setFont(u8g2_font_lubB18_tf);
  int h = dht.readHumidity();
  String hum = "Hum: ";
  hum.concat(h);
  hum.concat("%");
  dsp.setCursor(0,30);
  dsp.print(hum);
}

void printTemperature() {
  dsp.setFont(u8g2_font_inb16_mf);
  int t = dht.readTemperature();
  String temp = "Temp: ";
  temp.concat(t);
  temp.concat("C");
  dsp.setCursor(0,28);
  dsp.print(temp);
}

void printTime() {
  dsp.setFont(u8g2_font_amstrad_cpc_extended_8f);
  dsp.setCursor(0, 8);
  dsp.print("21/01/2021 Thu");
  dsp.setFont(u8g2_font_lubB18_tf);
  dsp.setCursor(8,34);
  dsp.print("22:22:22");
}

void printFuel() {
  /*dsp.setFont(u8g2_font_lubB18_tf);
  dsp.setCursor(0,26);
  dsp.print("empty");*/
  /*dsp.drawVLine(0,   16, 16);
  dsp.drawVLine(127, 16, 16);
  dsp.drawVLine(64,  16, 16);*/
  
  dsp.drawBox(2, 4, 20, 28);
  dsp.drawBox(23, 4, 20, 28);
  dsp.drawBox(44, 4, 20, 28);

  dsp.drawFrame(65, 4, 20, 28);
  dsp.drawFrame(86, 4, 20, 28);
  dsp.drawFrame(107, 4, 20, 28);
}

void setPowerSaveMode() {
  if(power_state) {
    dsp.setPowerSave(0);
    power_state = false;
  } else {
    dsp.setPowerSave(1);
    power_state = true;
  }
}

void loop() {
  bool btnState = !digitalRead(3);
  if(btnState && !flag) {
    flag = true;
  }
  if(!btnState && flag) {
    flag = false;
    choise++;
  }
  if(choise > 5) choise = 1;

  dsp.clearBuffer();                               // clear the graphcis buffer  
    // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  switch(choise) {
    case TEMP:
      printTemperature();
      break;
     case HUM:
      printHumidity();
      break;
     case FUEL:
      printFuel();
      break;
     case TIME:
      printTime();
      break;
     case POwER_STATE_MODE:
      setPowerSaveMode();
      break;
     default:
     break;
  }
  dsp.sendBuffer();
}
