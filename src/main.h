#include <Arduino.h>
#include <Ticker.h>
#include <EtherCard.h>
#include <FS.h>
#include <ESPAsyncWebServer.h>
#include <WebAuthentication.h>
#include <AsyncTCP.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <SD.h>
#include <SPIFFS.h>
#include <EEPROM.h>
#include <Arduino_JSON.h>
#include <time.h>
#include <ESP32Ping.h>
#include "mbedtls/md.h"
#include <SPIFFSEditor.h>


#define FS_NO_GLOBALS
//#define DEBUG_MODE
#define ETH_CS_PIN 15


///////Register Start Address///////
#define WIFI_BEGIN 1
#define WIFI_CONFIG 100
#define ETH_CONFIG 200
#define DEVICE_CONFIG 300
#define PLASTIK 400
/////////////////////////////////////

///////Register END Address///////
#define WIFI_BEGIN_END 99
#define WIFI_CONFIG_END 199
#define ETH_CONFIG_END 299
#define DEVICE_CONFIG_END 399
#define PLASTIK_END 599
////////////////////////////////////

////PLASTIK_STATUS//////
#define SELECT 1
#define OKEY 2
#define NOT_OKEY 3
////////////////////////


#define wifi_begin_lenght 16
#define wifi_config_lenght 4
#define eth_config_lenght 4
#define device_config_lenght 2
#define plastik_set_lenght 2
#define device_register_lenght 2

 
  enum{
  sta_ssid,
  sta_password,
  ap_ssid,
  ap_password,
  log_username,
  log_password,  
  wifi_begin_size 
  };///WIFI_BEGIN
  
  enum{
  sta_ip,
  sta_subnet,
  sta_gateway,
  sta_dns,
  wifi_ntp,    
  wifi_config_size    
  };///WIFI_CONFIG
   
  enum{
  eth_ip,
  eth_subnet,  
  eth_gateway,
  eth_dns,
  eth_ntp,
  eth_config_size    
  };///ETHERNET_CONFIG
  
  enum {   
  offset_hizi,
  offset_etkisi,
  hatali_adim_sayisi,
  baslangic_adim_sayisi,
  makina_numarasi,
  alt_limit,
  ust_limit,
  duraklama_adim_sayisi,
  parca_basina_vargel_sayisi,
  siren_tekrar_sayisi,
  siren_aralik_suresi,
  plastik_sayimi_mod,
  plastik_parca_sayisi,
  tansiyon_modu,
  plastik_parca_iptal_suresi,
  plastik_parca_adim_toleransi,  
  fabrika_ayari,
  utc,
  dst,
  ap_mode,  
  device_config_size
  };///DEVICE_CONFIG
  


  enum{  
  plastik_size=50
  };///PLASTIK

  struct Plastik
  {
    short set;
    short real;
    short status;
  };
  


  // enum{ 
  // tansiyon_degeri,
  // offset_degeri,
  // ham_tansiyon_degeri,
  // hiz,
  // adim,
  // limit_asim_sayisi,
  // vargel_adet,
  // parca_adet,
  // siren_durumu,
  // device_register_size
  // };///DEVICE_REGISTER


static String wifi_begin[wifi_begin_size];
byte wifi_config[wifi_config_size][wifi_config_lenght];
byte eth_config[eth_config_size][eth_config_lenght];
short device_config[device_config_size];
Plastik plastik[plastik_size];
//short device_register[device_register_size];



const byte TANS_ADC_pin=36;
const byte IN1=39;
const byte IN2=34;
const byte IN3=35;
const byte IN4=32;
const byte IN5=33;
const byte OUT1=25;
const byte ENC_A=26;
const byte ENC_Z=27;

const byte APMOD_BUTON_pin=22;

const byte EXT_IN1_pin=17;
const byte EXT_IN2_pin=21;
const byte EXT_OUT1_pin=2;
const byte EXT_OUT2_pin=4;
const byte EXT_OUT3_pin=16;



struct RequestArgument {
    String key;
    String value;
  };
  int              _currentArgCount;
  RequestArgument* _currentArgs;
  int              _postArgsLen;
  RequestArgument* _postArgs;


void IRAM_ATTR SIFIRLA_INT();
void IRAM_ATTR G1_INT();
void IRAM_ATTR POS2_INT();
void IRAM_ATTR FS_INT();
void IRAM_ATTR SIREN_INT();
void IRAM_ATTR RU_RA_INT();
void IRAM_ATTR PLASTIK_BUTON_ISIK_INT();
void IRAM_ATTR PLASTIK_BUTON_INT();
void IRAM_ATTR PLASTIK_PARCA_IPTAL_INT();
String arg(String name);
String arg(int i);
String argName(int i);
String dataTypeGet(String path);
bool hasArg(String  name) ;
String urlDecode(const String& text);
String processorRead(const String& var);
String hash(String arg,mbedtls_md_type_t md_type,const byte size);
String printLocalTime(const char* format);
uint32_t getNtpTime(uint8_t* ntpServer);
String updateGenel();
String updateTansiyon(int tempAdim);
String updatePlastik();
String setPlastikSend();
String updateState();
void processorWrite(String name,String value);
void registerUpdate();
void sendFile(fs::FS &fs,String filename);
void sendFileHTML(fs::FS &fs,String filename);
void handleNotFound(AsyncWebServerRequest *request);
void handlePage(AsyncWebServerRequest *request);
void handleTansiyon(AsyncWebServerRequest * request);
void handlePlastik(AsyncWebServerRequest * request);
void handleGenel(AsyncWebServerRequest * request);
void handleLast(AsyncWebServerRequest * request);
void handleSetKey(AsyncWebServerRequest * request);
void handleState(AsyncWebServerRequest *request);
void handleEthernet();
void logWrite(int kod);
void logHandle();
void lasWrite();
bool isAuthenticated(AsyncWebServerRequest *request);
bool loadFromSdCard(AsyncWebServerRequest *request,String path);
bool authentication(String req,const char* username,const char* password);
void requestAuthentication(String redirectUrl);
void localTimeUpdate(long epoch_time);
void parseArguments(String data);
bool parseRequest(String req);