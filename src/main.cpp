#include <main.h>

byte mymac[6];
const byte myMac[] PROGMEM = {0x70, 0x69, 0x69, 0x2D, 0x30, 0x31};
byte Ethernet::buffer[3000];
byte res;

String redirectURL = "/dashboard.html";
const char *defaultPage = "/dashboard.html";
AsyncWebServer server(80);


const long gmtOffset_sec = 10800;
const int daylightOffset_sec = 0;
bool ntp_ok = false;

Ticker siren_tick;
Ticker plastik_buton_isik_tick;
Ticker plastik_parca_iptal_tick;
Ticker ethernet_tick;
Ticker sifirla_tick;



byte siren_pin = EXT_OUT1_pin;
byte plastik_isik_pin = EXT_OUT3_pin;
byte plastik_buton_pin = EXT_IN1_pin;
int tansiyon_ham = 0;
int tansiyon = 0;
int vargel_sayisi_pr = 0;
int hata_sayisi_pr = 0;
int vargel_sayisi_hr = 0;
int parca_sayisi_hr = 0;
int hata_sayisi_hr = 0;
long start = 0;
long temp = 0;
long diff = 0;
long id = 0;
long temp_id = 0;
int hiz = 0;
int adim = 0;
int duraklama_adim = 0;
int ilk_adim = 10;
int offset = 0;
int buff = 0;
int siren_adim = 0;
int log_index = 0;
int plastik_parca_sayisi_cr = 0;
int hatali_plastik_parca_sayisi = 0;
int tansiyon_buff = 0;
int pulse = 0;
int ornekleme_sayisi = 0;
int buton_basilma_sayisi = 0;
int plastik_parca_iptal_millis = 0;

bool ikaz_sireni = false;
bool alt_limit_alarm = true;
bool ust_limit_alarm = true;
bool ip_koptu_alarm = true;
bool ip_durumu = false;
bool FS_durumu = false;
bool wifi_durumu = false;
bool ethernet_durumu=false;
bool vargel_iptal = false;
bool makina_stop = false;
bool plastik_parca_iptal = false;
bool not_okey = false;
bool apmod_durumu=false;
bool reset_durumu=false;
bool sifirlama_sure_durumu=false;
bool sifirla=false;
String hour, day, year;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

String realm = "realtekno";
String hatalar[4] = {"Alt Limit", "Ust Limit", "Ip Koptu", "Adim Atlama"};

String hatalogCSV[100];
String cihazKodu;
JSONVar lastJSON;
JSONVar hatalogJSON[100];
String lastJSON_str;
String macCode;
String code;
struct tm timeinfo;
timeval time_val;
timezone time_zone;
short tansiyon_temp[2048];




void setup()
{

  //setlocale(LC_ALL, "Turkish");

  ///SERIAL///
    Serial.begin(921600);
    Serial.setDebugOutput(true);
    Serial.print("\n");
  ///////////

  ///EEPROM///
    Serial.println("Testing EEPROMClass\n");
    delay(1000);
    if (!EEPROM.begin(1024))
    {
      Serial.println("Failed to initialise EEPROMClass");
      delay(1000);
      ESP.restart();
    }
  ///////////

  ///SPIFFS///

    if (!SPIFFS.begin())
    {
      Serial.println("An Error has occurred while mounting SPIFFS");
    }

    code = SPIFFS.open("/key.txt", FILE_READ).readString().c_str();
    Serial.println(code);
  ////////////

  ///SDCARD///
    if (!SD.begin())
    {
      Serial.println("SD initiatization failed. Retrying.");
      while (!SD.begin())
      {
        delay(250);
      }
    }
    Serial.println("SD Initialized.");
    Serial.print("SD card size:");
    Serial.print(float(SD.cardSize() / 1000000000.0), 3);
    Serial.println(" GB");
    Serial.println(F("SD Webserver"));
  ////////////

 #pragma region  ///REGISTER UPDATE///
    if (EEPROM.readShort(DEVICE_CONFIG + (device_config_lenght * fabrika_ayari)) != 1)
    {
      wifi_begin[sta_ssid] = "PDA";
      wifi_begin[sta_password] = "Pda12345678";
      wifi_begin[ap_ssid] = "DIKIS";
      wifi_begin[ap_password] = "12345678";
      wifi_begin[log_username] = "admin";
      wifi_begin[log_password] = "realtekno";
      wifi_config[sta_ip][0] = 10;
      wifi_config[sta_ip][1] = 7;
      wifi_config[sta_ip][2] = 9;
      wifi_config[sta_ip][3] = 0;
      wifi_config[sta_gateway][0] = 10;
      wifi_config[sta_gateway][1] = 7;
      wifi_config[sta_gateway][2] = 9;
      wifi_config[sta_gateway][3] = 254;
      wifi_config[sta_subnet][0] = 255;
      wifi_config[sta_subnet][1] = 255;
      wifi_config[sta_subnet][2] = 255;
      wifi_config[sta_subnet][3] = 0;
      wifi_config[sta_dns][0] = 10;
      wifi_config[sta_dns][1] = 7;
      wifi_config[sta_dns][2] = 0;
      wifi_config[sta_dns][3] = 1;
      wifi_config[wifi_ntp][0] = 10;
      wifi_config[wifi_ntp][1] = 7;
      wifi_config[wifi_ntp][2] = 0;
      wifi_config[wifi_ntp][3] = 1;
      eth_config[eth_ip][0] = 10;
      eth_config[eth_ip][1] = 7;
      eth_config[eth_ip][2] = 11;
      eth_config[eth_ip][3] = 0;
      eth_config[eth_subnet][0] = 255;
      eth_config[eth_subnet][1] = 255;
      eth_config[eth_subnet][2] = 255;
      eth_config[eth_subnet][3] = 0;
      eth_config[eth_gateway][0] = 10;
      eth_config[eth_gateway][1] = 7;
      eth_config[eth_gateway][2] = 11;
      eth_config[eth_gateway][3] = 250;
      eth_config[eth_dns][0] = 10;
      eth_config[eth_dns][1] = 7;
      eth_config[eth_dns][2] = 0;
      eth_config[eth_dns][3] = 1;      
      device_config[offset_hizi] = 2500;
      device_config[offset_etkisi] = 0;
      device_config[hatali_adim_sayisi] = 5;
      device_config[makina_numarasi] = 0;
      device_config[baslangic_adim_sayisi] = 5;
      device_config[alt_limit] = 100;
      device_config[ust_limit] = 500;
      device_config[duraklama_adim_sayisi] = 2;
      device_config[parca_basina_vargel_sayisi] = 2;
      device_config[siren_tekrar_sayisi] = 3;
      device_config[siren_aralik_suresi] = 100;
      device_config[plastik_parca_adim_toleransi] = 100;
      device_config[plastik_parca_sayisi] = 1;
      device_config[plastik_parca_iptal_suresi] = 1000;
      device_config[utc]=180;
      device_config[dst]=0;
      device_config[ap_mode]=1;
      device_config[tansiyon_modu]=1;
      device_config[plastik_sayimi_mod]=1;
            
      for (int i = 0; i < wifi_begin_size; i++)
        EEPROM.writeString(WIFI_BEGIN + (wifi_begin_lenght * i), wifi_begin[i]);
      for (int i = 0; i < wifi_config_size; i++)
        EEPROM.writeBytes(WIFI_CONFIG + (wifi_config_lenght * i), wifi_config[i], wifi_config_lenght);
      for (int i = 0; i < eth_config_size; i++)
        EEPROM.writeBytes(ETH_CONFIG + (eth_config_lenght * i), eth_config[i], eth_config_lenght);
      for (int i = 0; i < device_config_size; i++)
        EEPROM.writeShort(DEVICE_CONFIG + (device_config_lenght * i), device_config[i]);
      for (int i = 0; i < plastik_size; i++)
        EEPROM.writeShort(PLASTIK + (plastik_set_lenght * i), 0);
      EEPROM.writeShort(DEVICE_CONFIG + (device_config_lenght * fabrika_ayari), 1);
      EEPROM.commit();
    // delay(1000);
    //   for (int i = 0; i < 8; i++)
    //   {
    //     lastJSON["VS"][i] = 0;
    //     lastJSON["PS"][i] = 0;
    //     lastJSON["HS"][i] = 0;
    //   }
    //   lastJSON["dt"] = 1571642902;
    //   lastJSON["id"] = 0;
    //   lastJSON_str = JSON.stringify(lastJSON);
    //   File fileLast = SD.open("/data/last.txt", FILE_WRITE);
    //   fileLast.println(lastJSON_str);
    //   fileLast.close();
    //   delay(1000);
    }
    registerUpdate();
  ////////////////////

  ///LAST///
    File dataFile = SD.open("/data/last.txt", FILE_READ);
    lastJSON_str = dataFile.readString();
    lastJSON = JSON.parse(lastJSON_str);
    dataFile.close();
    vargel_sayisi_hr = (int)lastJSON["VS"][7];
    parca_sayisi_hr = (int)lastJSON["PS"][7];
    hata_sayisi_hr = (int)lastJSON["HS"][7];
    id = (int)lastJSON["id"];

    temp_id = id;
 #pragma endregion //////////


 #pragma region  ///PINMODE///
    pinMode(APMOD_BUTON_pin, INPUT_PULLUP);
    pinMode(plastik_buton_pin, INPUT);
    pinMode(POS2_OUT_pin, INPUT);
    pinMode(G1_OUT_pin, INPUT);
    pinMode(COM_IN_pin, INPUT);
    pinMode(FS_pin, INPUT);
    pinMode(LSP_IN_pin, OUTPUT);
    digitalWrite(LSP_IN_pin, LOW);
    pinMode(siren_pin, OUTPUT);
    digitalWrite(siren_pin, LOW);
    pinMode(plastik_isik_pin, OUTPUT);
    digitalWrite(plastik_isik_pin, LOW);

 #pragma endregion ////////////


 #pragma region  ///WIFI SETUP///
    WiFi.setAutoConnect(false);
    WiFi.setAutoReconnect(false);
    WiFi.disconnect(false);       // Wifi OFF
    WiFi.softAPdisconnect(false); // Wifi already OFF
    delay(1000);
    if(!digitalRead(APMOD_BUTON_pin)||device_config[ap_mode])
    apmod_durumu=true;
    
    if (apmod_durumu)
    {
      WiFi.softAP(wifi_begin[ap_ssid].c_str(), wifi_begin[ap_password].c_str());
      WiFi.mode(WIFI_AP_STA);    
      delay(1000);
    }
    else
    WiFi.mode(WIFI_STA);
    WiFi.config(IPAddress(wifi_config[sta_ip]), IPAddress(wifi_config[sta_gateway]), IPAddress(wifi_config[sta_subnet]), IPAddress(wifi_config[sta_dns]));
    WiFi.begin(wifi_begin[sta_ssid].c_str(), wifi_begin[sta_password].c_str());
    delay(1000);

    Serial.print("Connecting to ");
    Serial.println(wifi_begin[sta_ssid]);

    // Wait for connection
    uint8_t i = 0;
    while (WiFi.status() != WL_CONNECTED && i++ < 6)
    { //wait 10 seconds
      delay(500);
    }
    if (i == 7)
    {
      wifi_durumu = false;
      Serial.print("Could not connect to");
      Serial.println(wifi_begin[sta_ssid]);
      WiFi.disconnect();
    }
    else
    {
      wifi_durumu = true;
      Serial.print("Connected! IP address: ");
      Serial.println(WiFi.localIP());
      WiFi.setAutoConnect(true);
      WiFi.setAutoReconnect(true);
    }

 #pragma endregion ///////////////

  #pragma region  ///ETHERNET///
    
      WiFi.macAddress(mymac);
      mymac[0] = 0X70;
      mymac[1] = 0X69;
      mymac[2] = 0X69;
      char macStr[18] = { 0 };
       sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mymac[0], mymac[1], mymac[2], mymac[3], mymac[4], mymac[5]);
      Serial.println(macStr);
      if (!ether.begin(sizeof Ethernet::buffer, mymac, ETH_CS_PIN))
      {
        Serial.println(F("Failed to initialize Ethernet controller"));
      }
      else
      Serial.println(F("Ethernet controller initialized"));
      
      
      delay(200);
      if(ether.isLinkUp()||!wifi_durumu)      
      {
        if(ether.dhcpSetup()){          
          Serial.println(F("DHCP OK"));         
        }
        ethernet_durumu=true;
      }
         
      
      if (!ether.staticSetup(eth_config[eth_ip], eth_config[eth_gateway], eth_config[eth_dns], eth_config[eth_subnet]))
      {
        Serial.println(F("Failed to set IP address"));
      }
      
      
      ether.printIp("IP:  ", ether.myip);
      ether.printIp("GW:  ", ether.gwip);
      ether.printIp("DNS: ", ether.dnsip);

      ether.printIp("Configured IP address:\t", ether.myip);
      Serial.println();
    
  #pragma endregion//////////////

  #pragma region ///WIFISERVER///
    server.addHandler(new SPIFFSEditor(SD, "admin", "admin"));
    server.on("/wifi_settings.html", handlePage);
    server.on("/device_settings.html", handlePage);
    server.on("/bakim.html", handlePage);
    server.on("/genel", handleGenel);
    server.on("/tansiyon", handleTansiyon);
    server.on("/plastik", handlePlastik);
    server.on("/last", handleLast);
    server.on("/state", handleState);
    server.on("/keySet", handleSetKey);
    server.onNotFound(handleNotFound);
    server.onFileUpload([](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {
      if (!index)
        Serial.printf("UploadStart: %s\n", filename.c_str());
      Serial.printf("%s", (const char *)data);
      if (final)
        Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index + len);
    });
    server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      if (!index)
        Serial.printf("BodyStart: %u\n", total);
      Serial.printf("%s", (const char *)data);
      if (index + len == total)
        Serial.printf("BodyEnd: %u\n", total);
    });
    server.begin();
    Serial.println("HTTP server started");

  #pragma endregion///////////////


  ///INT///
    macCode = hash(WiFi.macAddress(), MBEDTLS_MD_MD5, 16);

    if (macCode == code)
    {
      attachInterrupt(digitalPinToInterrupt(POS2_OUT_pin), POS2_INT, RISING);
      attachInterrupt(digitalPinToInterrupt(FS_pin), FS_INT, FALLING);
      if (device_config[tansiyon_modu] == 1)
        attachInterrupt(digitalPinToInterrupt(G1_OUT_pin), G1_INT, RISING);
      if (device_config[plastik_sayimi_mod] == 1)
      {
        attachInterrupt(digitalPinToInterrupt(plastik_buton_pin), PLASTIK_BUTON_INT, CHANGE);
        attachInterrupt(digitalPinToInterrupt(COM_IN_pin), RU_RA_INT, FALLING);
      }
      Serial.println("key ok");
      
    }
    attachInterrupt(digitalPinToInterrupt(APMOD_BUTON_pin), SIFIRLA_INT, CHANGE);
  ////////

  ///TIME///
  
   
    if (ethernet_durumu){      
      localTimeUpdate(getNtpTime(wifi_config[wifi_ntp]));
    }
    else if (wifi_durumu)
    { ///10.7.0.1
      if(Ping.ping(IPAddress(wifi_config[wifi_ntp]), 3)){
        char ntpServer_char[15];
       
        sprintf(ntpServer_char,"%d.%d.%d.%d",wifi_config[wifi_ntp][0],wifi_config[wifi_ntp][1],wifi_config[wifi_ntp][2],wifi_config[wifi_ntp][3]);
        configTime(device_config[utc]*60, device_config[dst]*60,ntpServer_char );

      }

        
    }
    else
      localTimeUpdate((long)lastJSON["dt"]);
    
      
    //localTimeUpdate(1571642902);
    Serial.println(printLocalTime("%A, %B %d %Y %H:%M:%S"));
    hour = printLocalTime("%H");
    day = printLocalTime("%d");
    year = printLocalTime("%Y");

    Serial.println(printLocalTime("%Y/%d.%m.%Y.csv"));

  //////////

  if (!SD.exists("/data"))
    SD.mkdir("/data");
  if (!SD.exists("/data/" + year))
    SD.mkdir("/data/" + year);

  cihazKodu = WiFi.macAddress();
  Serial.println(cihazKodu);

} ///end setup///

void loop()
{

  
  

  logHandle();
  lasWrite();  
  handleEthernet();
  if(sifirla){
    Serial.println("sifirlaniyor....");
    siren_tick.attach_ms(1000,SIREN_INT);
    wifi_begin[ap_ssid] = "DIKIS";
    wifi_begin[ap_password] = "12345678";
    wifi_begin[log_username] = "admin";
    wifi_begin[log_password] = "realtekno";    
    for (int i = 0; i < wifi_begin_size; i++)
    EEPROM.writeString(WIFI_BEGIN + (wifi_begin_lenght * i), wifi_begin[i]);
    delay(100);
    EEPROM.commit();
    delay(1000);
    ESP.restart();
  }
}


void IRAM_ATTR SIFIRLA_INT(){
Serial.println("sifirlama");
    if(!digitalRead(APMOD_BUTON_pin))
    sifirla_tick.once_ms(5000,[]{sifirlama_sure_durumu=true;});
    else
    {
      if(sifirlama_sure_durumu)
      {

        sifirla=true;
      
      }
      else
      sifirla_tick.detach();
    }
    
}


void IRAM_ATTR RU_RA_INT()
{

  if (digitalRead(COM_IN_pin) == LOW)
    vargel_iptal = true;

  Serial.println(digitalRead(COM_IN_pin));
}

void IRAM_ATTR PLASTIK_PARCA_IPTAL_INT()
{

  if (plastik_parca_sayisi_cr > 0 && adim == 0)
  {

    if (!plastik_parca_iptal)
    {
      Serial.println("iptal");
      plastik_buton_isik_tick.attach_ms(device_config[siren_aralik_suresi], PLASTIK_BUTON_ISIK_INT);
      plastik[plastik_parca_sayisi_cr].status = 0;
      plastik_parca_sayisi_cr--;
      plastik[plastik_parca_sayisi_cr].real = 0;
      plastik[plastik_parca_sayisi_cr].status = SELECT;
      buton_basilma_sayisi = 0;
      plastik_parca_iptal = true;
      if (makina_stop)
      {
        digitalWrite(plastik_isik_pin, LOW);
        digitalWrite(LSP_IN_pin, LOW);
        makina_stop = false;
      }
    }
  }
}

void IRAM_ATTR PLASTIK_BUTON_INT()
{

  // if((millis()-plastik_parca_iptal_millis)>500||plastik_parca_sayisi_cr==0||adim>5)
  //   buton_basilma_sayisi=0;
  Serial.println("butoon");
  if (digitalRead(plastik_buton_pin) == LOW)
  {
    Serial.println("LOW");
    plastik_parca_iptal_tick.once_ms(device_config[plastik_parca_iptal_suresi], PLASTIK_PARCA_IPTAL_INT);
  }
  else
  {
    plastik_parca_iptal_tick.detach();
    if (not_okey)
    {
      digitalWrite(LSP_IN_pin, LOW);
      plastik[plastik_parca_sayisi_cr].status = 0;
      plastik_parca_sayisi_cr--;
      plastik[plastik_parca_sayisi_cr].real = 0;
      plastik[plastik_parca_sayisi_cr].status = SELECT;
      buton_basilma_sayisi = 0;
      plastik_parca_iptal = true;
      not_okey = false;
      makina_stop = false;
    }

    else if (makina_stop)
    {
      digitalWrite(plastik_isik_pin, LOW);
      digitalWrite(LSP_IN_pin, LOW);
      for (int i = 0; i < device_config[plastik_parca_sayisi]; i++)
      {
        plastik[i].real = 0;
        plastik[i].status = 0;
      }
      plastik_parca_sayisi_cr = 0;
      makina_stop = false;
    }
  }
  // else if(!plastik_parca_iptal&&buton_basilma_sayisi>=2){
  //  plastik_buton_isik_tick.attach_ms(device_config[siren_aralik_suresi],PLASTIK_BUTON_ISIK_INT);
  //  plastik[plastik_parca_sayisi_cr].status=0;
  //  plastik_parca_sayisi_cr--;
  //  plastik[plastik_parca_sayisi_cr].real=0;
  //  plastik[plastik_parca_sayisi_cr].status=SELECT;
  //  buton_basilma_sayisi=0;
  //  plastik_parca_iptal=true;
  // }
  // else
  // {
  //   plastik_parca_iptal_millis=millis();
  //   buton_basilma_sayisi++;
  // }
}

int buton_isik_adim = 0;
void IRAM_ATTR PLASTIK_BUTON_ISIK_INT()
{
  if (not_okey)
    digitalWrite(plastik_isik_pin, !digitalRead(plastik_isik_pin));
  else if (buton_isik_adim < device_config[siren_tekrar_sayisi] * 2)
  {
    digitalWrite(plastik_isik_pin, !digitalRead(plastik_isik_pin));
    buton_isik_adim++;
  }
  else
  {
    plastik_buton_isik_tick.detach();
    digitalWrite(plastik_isik_pin, LOW);
    buton_isik_adim = 0;
  }
}

void IRAM_ATTR G1_INT()
{
  //portENTER_CRITICAL(&mux);
  if (digitalRead(G1_OUT_pin) == HIGH)
  {
    // if(pulse>500&&pulse<512){
    //   tansiyon_buff+=analogRead(TANS_ADC_pin)/4;
    //   ornekleme_sayisi++;
    // }

    tansiyon_buff = analogRead(TANS_ADC_pin) / 4;
    if (tansiyon_buff > tansiyon_ham)
      tansiyon_ham = tansiyon_buff;
    //pulse++;
  }

  //portEXIT_CRITICAL(&mux);
}

void IRAM_ATTR POS2_INT()
{
  //Serial.println(pulse);
  //if(ornekleme_sayisi>0)
  //tansiyon_ham=analogRead(TANS_ADC_pin)/4;
  // if(ornekleme_sayisi>0)
  // tansiyon_ham=tansiyon_buff/ornekleme_sayisi;
  // pulse=0;
  // tansiyon_buff=0;
  // ornekleme_sayisi=0;
  plastik[plastik_parca_sayisi_cr].status = SELECT;
  temp = millis();
  diff = temp - start;
  start = temp;
  adim++;
  if (diff > 19)
    hiz = 60000 / (diff);

  if (adim > device_config[baslangic_adim_sayisi])
  {
    if (FS_durumu)
    {
      vargel_sayisi_hr = vargel_sayisi_hr + device_config[parca_basina_vargel_sayisi];
      parca_sayisi_hr++;
      id++;
      FS_durumu = false;
    }
  }

  if (device_config[tansiyon_modu] == 1)
  {
    offset = ((device_config[offset_hizi] - hiz) * device_config[offset_etkisi]) / 100; //1600 5.5
    if (tansiyon_ham > 20)
    {
      ip_durumu = true;
      tansiyon = tansiyon_ham + offset;
    }
    else
      tansiyon = tansiyon_ham;
    tansiyon_ham = 0;

    if (hiz < 100)
      duraklama_adim = 0;
    else if (hiz >= 100)
    {
      duraklama_adim++;

      tansiyon_temp[adim] = tansiyon;
      if (duraklama_adim > duraklama_adim_sayisi)
        if (adim > device_config[baslangic_adim_sayisi])
        {
          alt_limit_alarm = true;
          ust_limit_alarm = true;
          ip_koptu_alarm = true;
          for (size_t i = adim; i > (adim - device_config[hatali_adim_sayisi]); i--)
          {

            ip_koptu_alarm = ip_koptu_alarm && (tansiyon_temp[i] <= 0);
            alt_limit_alarm = alt_limit_alarm && (tansiyon_temp[i] < device_config[alt_limit]);
            ust_limit_alarm = ust_limit_alarm && (tansiyon_temp[i] > device_config[ust_limit]);
          }

          if (alt_limit_alarm || ust_limit_alarm || ip_koptu_alarm)
          {

            if (ip_koptu_alarm)
            {
              if (ip_durumu)
              {
                logWrite(2);
                ip_durumu = false;
              }
            }
            else if (alt_limit_alarm)
              logWrite(0);
            // Serial.println("alt limit");
            if (ust_limit_alarm)
              logWrite(1);
            // Serial.println("üst limit");
            if (!ikaz_sireni)
            {
              siren_tick.attach_ms(device_config[siren_aralik_suresi], SIREN_INT);
              ikaz_sireni = true;
            }
            siren_adim = 0;
          }
        }
    }
  }
}

void IRAM_ATTR FS_INT()
{

  //Serial.println("FS");
  FS_durumu = true;

  if (device_config[plastik_sayimi_mod] == 1)
  {

    if (adim > 5)
    {

      if (!vargel_iptal)
      {

        if (abs(adim - plastik[plastik_parca_sayisi_cr].set) > device_config[plastik_parca_adim_toleransi])
        {
          plastik_buton_isik_tick.attach_ms(device_config[siren_aralik_suresi], PLASTIK_BUTON_ISIK_INT);
          plastik[plastik_parca_sayisi_cr].status = NOT_OKEY;
          hatali_plastik_parca_sayisi++;
          digitalWrite(LSP_IN_pin, HIGH);
          not_okey = true;
        }
        else
          plastik[plastik_parca_sayisi_cr].status = OKEY;

        plastik[plastik_parca_sayisi_cr].real = adim;

        plastik_parca_sayisi_cr++;
        if (plastik_parca_sayisi_cr >= device_config[plastik_parca_sayisi])
        {
          digitalWrite(LSP_IN_pin, HIGH);
          digitalWrite(plastik_isik_pin, HIGH);
          makina_stop = true;
        }

        plastik[plastik_parca_sayisi_cr].status = SELECT;
        plastik_parca_iptal = false;
      }
      else
      {
        vargel_iptal = false;
      }
    }
  }

  hata_sayisi_pr = 0;
  adim = 0;
}

void IRAM_ATTR SIREN_INT()
{

  if (siren_adim < device_config[siren_tekrar_sayisi] * 2)
  {
    digitalWrite(siren_pin, !digitalRead(siren_pin));
    siren_adim++;
  }
  else
  {
    siren_tick.detach();
    digitalWrite(siren_pin, LOW);
    ikaz_sireni = false;
  }
}

String dataTypeGet(String path)
{
  String dataType = "text/plain";

  if (path.endsWith(".src"))
    path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".htm"))
    dataType = "text/html";
  else if (path.endsWith(".html"))
    dataType = "text/html";
  else if (path.endsWith(".css"))
    dataType = "text/css";
  else if (path.endsWith(".js"))
    dataType = "application/javascript";
  else if (path.endsWith(".png"))
    dataType = "image/png";
  else if (path.endsWith(".gif"))
    dataType = "image/gif";
  else if (path.endsWith(".jpg"))
    dataType = "image/jpeg";
  else if (path.endsWith(".ico"))
    dataType = "image/x-icon";
  else if (path.endsWith(".xml"))
    dataType = "text/xml";
  else if (path.endsWith(".pdf"))
    dataType = "application/pdf";
  else if (path.endsWith(".zip"))
    dataType = "application/zip";
  else if (path.endsWith(".csv"))
    dataType = "text/csv";
  else if (path.endsWith(".gz"))
    dataType = "application/x-gzip";
  return dataType;
}
bool loadFromSdCard(AsyncWebServerRequest *request, String path)
{

  AsyncWebServerResponse *response;
  if (path.endsWith("/"))
    path += "dashboard.html";
  String dataType = dataTypeGet(path);

  String pathWithGz = path + ".gz";
  if ((SD.exists(pathWithGz) || SD.exists(path)))
  {
    if (SD.exists(pathWithGz))
    {
      path = pathWithGz;
      dataType = "application/x-gzip";
    }
  }

  if (!SD.exists(path.c_str()))
    return false;
   if (dataType == "text/html")
     response = request->beginResponse(SD, path, dataType, false, processorRead);
  else if(request->hasArg("download")){

      response = request->beginResponse(SD,path,"application/x-www-form-urlencoded;charset=UTF-8",true);
   
  }
  else{
   
    response = request->beginResponse(SD, path, dataType, false); 
     if(dataType=="application/x-gzip"){ 
       response->addHeader("Content-Encoding","gzip");   
      response->addHeader("Cache-Control","max-age=2592000"); 
      
    }
    else if(dataType=="image/png")
      response->addHeader("Cache-Control","max-age=2592000"); 
  } 
 
  request->send(response);

  return true;
}


String processorRead(const String &var)
{

  short address = var.toInt();
  if (address > 0)
  {
    if (address <= WIFI_BEGIN_END)
      return EEPROM.readString(address);
    else if (address <= WIFI_CONFIG_END)
      return String(EEPROM.readByte(address));
    else if (address <= ETH_CONFIG_END)
      return String(EEPROM.readByte(address));
    else if (address <= DEVICE_CONFIG_END)
      return String(EEPROM.readShort(address));
    else if (address <= PLASTIK_END)
      return String(EEPROM.readShort(address));
  }

  return String();
}

void processorWrite(String name, String value)
{
  detachInterrupt(digitalPinToInterrupt(G1_OUT_pin));
  detachInterrupt(digitalPinToInterrupt(POS2_OUT_pin));
  detachInterrupt(digitalPinToInterrupt(FS_pin));
  detachInterrupt(digitalPinToInterrupt(COM_IN_pin));
  detachInterrupt(digitalPinToInterrupt(plastik_buton_pin));
  short address = name.toInt();
  Serial.print(address);
  Serial.print("--");
  Serial.println((uint8_t)value.toInt());
  if (address > 0)
  {
    if (address <= WIFI_BEGIN_END)
      EEPROM.writeString(address, value);
    else if (address <= WIFI_CONFIG_END)
      EEPROM.writeByte(address, (uint8_t)value.toInt());
    else if (address <= ETH_CONFIG_END)
      EEPROM.writeByte(address, (uint8_t)value.toInt());
    else if (address <= DEVICE_CONFIG_END)
      EEPROM.writeShort(address, (int16_t)value.toInt());
    else if (address <= PLASTIK_END)
      EEPROM.writeShort(address, (int16_t)value.toInt());

    EEPROM.commit();
    registerUpdate();
  }

  if (macCode == code)
  {
    attachInterrupt(digitalPinToInterrupt(POS2_OUT_pin), POS2_INT, RISING);
    attachInterrupt(digitalPinToInterrupt(FS_pin), FS_INT, FALLING);
    if (device_config[tansiyon_modu] == 1)
      attachInterrupt(digitalPinToInterrupt(G1_OUT_pin), G1_INT, RISING);
    if (device_config[plastik_sayimi_mod] == 1)
    {
      attachInterrupt(digitalPinToInterrupt(plastik_buton_pin), PLASTIK_BUTON_INT, CHANGE);
      attachInterrupt(digitalPinToInterrupt(COM_IN_pin), RU_RA_INT, FALLING);
    }
  }
}

void handleNotFound(AsyncWebServerRequest *request)
{

  if (loadFromSdCard(request, request->url()))
  {
    return;
  }
  String message = "\nNo Handler\r\n";
  message += "URI: ";
  message += request->url();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nParameters: ";
  message += request->params();
  message += "\n";
  for (uint8_t i = 0; i < request->params(); i++)
  {
    AsyncWebParameter *p = request->getParam(i);
    message += String(p->name().c_str()) + " : " + String(p->value().c_str()) + "\r\n";
  }
  request->send(404, "text/plain", message);
  Serial.print(message);
}

void handlePage(AsyncWebServerRequest *request)
{

  Serial.println(wifi_begin[log_username]);
  Serial.println(wifi_begin[log_password]);
  if (request->authenticate(wifi_begin[log_username].c_str(), wifi_begin[log_password].c_str(), realm.c_str()))
  {
    if (request->method() == HTTP_POST)
    {
      for (uint8_t i = 0; i < request->params(); i++)
      {
        AsyncWebParameter *p = request->getParam(i);
        processorWrite(p->name(), p->value());
      }
    }

    loadFromSdCard(request, request->url());
    return;
  }
  else
  {
    request->requestAuthentication(realm.c_str(), true, redirectURL);
    ///AsyncWebServerResponse * r = beginResponse(401,"text/html","<script> window.location.href='"+redirectUrl+"'</script>");//  requestAuthentication overload
    return;
  }
}

void handleSetKey(AsyncWebServerRequest *request)
{
  if (request->authenticate("realtekno", "8684", realm.c_str()))
  {
    if (request->method() == HTTP_GET)
    {
      for (uint8_t i = 0; i < request->params(); i++)
      {
        AsyncWebParameter *p = request->getParam(i);
        if (p->name() == "key")
          SPIFFS.open("/key.txt", FILE_WRITE).print(p->value());
      }
    }

    request->send(200, "text/plain", "OK");
  }
  request->requestAuthentication(realm.c_str(), true);
}

String updateTansiyon(int tempAdim)
{

  JSONVar jvar;
  for (int i = 0; i <= (adim - tempAdim); i++)
    jvar["data"][i] = tansiyon_temp[i + tempAdim];

  jvar["adim"] = adim;
  jvar["id"] = id;
  jvar["alt"] = device_config[alt_limit];
  jvar["ust"] = device_config[ust_limit];

  return JSON.stringify(jvar);
}

String updateState()
{

  JSONVar jvar;
  jvar["ikaz"] = digitalRead(siren_pin);
  jvar["restart"] = reset_durumu;
  jvar["ap_mod"] = apmod_durumu;  
  
  return JSON.stringify(jvar);
}
String updateGenel()
{



  JSONVar jvar;

  jvar["hadim"] = hata_sayisi_pr;
  jvar["adim"] = adim;
  jvar["hiz"] = hiz;
  jvar["id"] = id;
  jvar["RSII"]=WiFi.RSSI();
  return JSON.stringify(jvar);
}

String updatePlastik()
{

  JSONVar jvar;
  plastik[plastik_parca_sayisi_cr].real = adim;
  for (int i = 0; i < device_config[plastik_parca_sayisi]; i++)
  {
    jvar["data"][i][0] = plastik[i].real;
    jvar["data"][i][1] = plastik[i].status;
  }

  return JSON.stringify(jvar);
}
String setPlastikSend()
{

  JSONVar jvar;
  for (int i = 0; i < device_config[plastik_parca_sayisi]; i++)
  {
    jvar["data"][i][0] = plastik[i].set;
  }
  return JSON.stringify(jvar);
}

void handleGenel(AsyncWebServerRequest *request)
{

  request->send(200, "text/plain", updateGenel());
}
void handleTansiyon(AsyncWebServerRequest *request)
{
  int tempAdim = 0;
  if (request->hasArg("adim"))
    tempAdim = request->arg("adim").toInt();

  request->send(200, "text/plain", updateTansiyon(tempAdim));
}

void handlePlastik(AsyncWebServerRequest *request)
{

  if (request->hasArg("setDegerleri"))
    request->send(200, "text/plain", setPlastikSend());
  else
    request->send(200, "text/plain", updatePlastik());
}

void handleLast(AsyncWebServerRequest *request)
{

  request->send(200, "text/plain", lastJSON_str.c_str());
}

void handleState(AsyncWebServerRequest *request)
{
  
  if (request->hasArg("ikaz")){
    if(request->arg("ikaz").toInt())
      digitalWrite(siren_pin,HIGH);
      else
      digitalWrite(siren_pin,LOW);
  }
  else if (request->hasArg("ap_mod")){   
    apmod_durumu=true;
    request->send(200, "text/plain", updateState());
    delay(1000);
    WiFi.disconnect(false);       // Wifi OFF
    WiFi.softAPdisconnect(false); // Wifi already OFF
    delay(200);
    WiFi.softAP(wifi_begin[ap_ssid].c_str(), wifi_begin[ap_password].c_str());
    WiFi.mode(WIFI_AP_STA);    
      
    WiFi.config(IPAddress(wifi_config[sta_ip]), IPAddress(wifi_config[sta_gateway]), IPAddress(wifi_config[sta_subnet]), IPAddress(wifi_config[sta_dns]));
    WiFi.begin(wifi_begin[sta_ssid].c_str(), wifi_begin[sta_password].c_str());
    return;
  } 
  else if (request->hasArg("restart")){
    reset_durumu=1;
    request->send(200, "text/plain", updateState());
    delay(1000);
    ESP.restart();
    return;
  } 

  request->send(200, "text/plain", updateState());
}


BufferFiller bfill;
String str;
//#define DEBUG_MODE
void handleEthernet()
{
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);

  if (pos)
  {

    char *request = (char *)Ethernet::buffer + pos;


  #ifdef DEBUG_MODE
      Serial.println(F("---------------------------------------- NEW PACKET ----------------------------------------"));
      Serial.println(request);
      Serial.println(F("--------------------------------------------------------------------------------------------"));
      Serial.println();
  #endif

    // if the request is for the "default" page
    if (strncmp("GET / ", request, 6) == 0)
    {

      Serial.print(F("Requested default page: "));
      Serial.print(defaultPage);

      sendFileHTML(SD, defaultPage);
    }
    else if (strncmp("GET /genel", request, 10) == 0)
    {
      parseRequest(request);

      bfill = ether.tcpOffset();
      str = "HTTP/0.9 200 OK\r\nContent-Type: application/json\r\n\r\n" + updateGenel();
      bfill.emit_p(str.c_str());
      ether.httpServerReply(bfill.position());
    }
    else if (strncmp("GET /state", request, 10) == 0)
    {
      parseRequest(request);
      
      
     if (hasArg("ikaz")){
        if(arg("ikaz").toInt())
          digitalWrite(siren_pin,HIGH);
          else
          digitalWrite(siren_pin,LOW);
      }
      else if (hasArg("ap_mod")){   
        apmod_durumu=true;        
        delay(1000);
        WiFi.disconnect(false);       // Wifi OFF
        WiFi.softAPdisconnect(false); // Wifi already OFF
        delay(200);
        WiFi.softAP(wifi_begin[ap_ssid].c_str(), wifi_begin[ap_password].c_str());
        WiFi.mode(WIFI_AP_STA);           
        WiFi.config(IPAddress(wifi_config[sta_ip]), IPAddress(wifi_config[sta_gateway]), IPAddress(wifi_config[sta_subnet]), IPAddress(wifi_config[sta_dns]));
        WiFi.begin(wifi_begin[sta_ssid].c_str(), wifi_begin[sta_password].c_str());
        
      } 
      else if (hasArg("restart")){
        reset_durumu=1;
        bfill = ether.tcpOffset();
        str = "HTTP/0.9 200 OK\r\nContent-Type: application/json\r\n\r\n" + updateState();
        bfill.emit_p(str.c_str());
        ether.httpServerReply(bfill.position());
        delay(1000);
        ESP.restart();
        return;
      } 

      bfill = ether.tcpOffset();
      str = "HTTP/0.9 200 OK\r\nContent-Type: application/json\r\n\r\n" + updateState();
      bfill.emit_p(str.c_str());
      ether.httpServerReply(bfill.position());
    }
    else if (strncmp("GET /tansiyon", request, 13) == 0)
    {
      parseRequest(request);
      int tempAdim = 0;
      if (hasArg("adim"))
        tempAdim = arg("adim").toInt();

      bfill = ether.tcpOffset();
      str = "HTTP/0.9 200 OK\r\nContent-Type: application/json\r\n\r\n" + updateTansiyon(tempAdim);
      bfill.emit_p(str.c_str());
      ether.httpServerReply(bfill.position());
    }
    else if (strncmp("GET /plastik", request, 12) == 0)
    {
      parseRequest(request);

      if (hasArg("setDegerleri"))
      {

        str = "HTTP/0.9 200 OK\r\nContent-Type: application/json\r\n\r\n" + setPlastikSend();
      }
      else
        str = "HTTP/0.9 200 OK\r\nContent-Type: application/json\r\n\r\n" + updatePlastik();

      bfill = ether.tcpOffset();
      bfill.emit_p(str.c_str());
      ether.httpServerReply(bfill.position());
    }
    else if (strncmp("GET /last", request, 9) == 0)
    {
      bfill = ether.tcpOffset();
      str = "HTTP/0.9 200 OK\r\nContent-Type: text/plain\r\n\r\n" + lastJSON_str;
      bfill.emit_p(str.c_str());
      ether.httpServerReply(bfill.position());
    }
    if (strncmp("GET /log.html", request, 13) == 0)
    {
      sendFile(SD, "/log.html");
    }
    else if (strncmp("GET /wifi_settings.html", request, 23) == 0)
    {
      
      if(authentication(request,wifi_begin[log_username].c_str(), wifi_begin[log_password].c_str()))      
      sendFileHTML(SD, "/wifi_settings.html");
    }
    else if (strncmp("POST /wifi_settings.html", request, 24) == 0)
    {
      if(authentication(request,wifi_begin[log_username].c_str(), wifi_begin[log_password].c_str())) {
        parseRequest(request);

        for (int i = 0; i < _currentArgCount; i++)
        {
          processorWrite(argName(i), arg(i));
        }

        sendFileHTML(SD, "/wifi_settings.html");
      }
    }
    else if (strncmp("GET /device_settings.html", request, 25) == 0)
    {
      if(authentication(request,wifi_begin[log_username].c_str(), wifi_begin[log_password].c_str()))
       sendFileHTML(SD, "/device_settings.html");
    }
    else if (strncmp("POST /device_settings.html", request, 26) == 0)
    {
      if(authentication(request,wifi_begin[log_username].c_str(), wifi_begin[log_password].c_str())){
        parseRequest(request);

        for (int i = 0; i < _currentArgCount; i++)
        {
          processorWrite(argName(i), arg(i));
        }

        sendFileHTML(SD, "/device_settings.html");
      }
    }
    else if (strncmp("GET /bakim.html", request, 15) == 0)
    {
      if(authentication(request,wifi_begin[log_username].c_str(), wifi_begin[log_password].c_str()))
       sendFileHTML(SD, "/bakim.html");
    }
    else if (strncmp("POST /bakim.html", request, 16) == 0)
    {
      if(authentication(request,wifi_begin[log_username].c_str(), wifi_begin[log_password].c_str())){
        parseRequest(request);

        for (int i = 0; i < _currentArgCount; i++)
        {
          processorWrite(argName(i), arg(i));
        }

        sendFileHTML(SD, "/bakim.html");
      }
    }
    else if (strncmp("GET /keySet", request, 10) == 0)
    {
      if(authentication(request,"realtekno", "8684")){
        parseRequest(request);      
        if (hasArg("key"))
          SPIFFS.open("/key.txt", FILE_WRITE).print(arg("key"));
        

        bfill = ether.tcpOffset();
        str = "HTTP/0.9 200 OK\r\nContent-Type: text/plain\r\n\r\n OK" ;
        bfill.emit_p(str.c_str());
        ether.httpServerReply(bfill.position());
      }
    }

    else if (strncmp("GET /", request, 5) == 0)
    {

      // DOS format only: 8 + 3 chars, add the "." and the terminator "\0"
      char filename[36];

      // search for the first space (0x32), that is the end of the name (filename starts at position 5)
      int i = 0;
      while ((request[4 + i] != 32) && (i < 35))
      {
        filename[i] = request[4 + i];
        i++;
      }
      filename[i] = '\0';

      Serial.print(F("Requested: "));
      Serial.print(filename);
      if (String("text/html") == dataTypeGet(filename))
        sendFileHTML(SD, filename);
      else
        sendFile(SD, filename);
    }

    else
    {
      
      //Serial.println(F("Unknown request"));
      #ifdef DEBUG_MODE
        Serial.println(F("---------------------------------------- NEW PACKET ----------------------------------------"));
        Serial.println(request);
        Serial.println(F("--------------------------------------------------------------------------------------------"));
        Serial.println();
      #endif
      bfill = ether.tcpOffset();
      bfill.emit_p(PSTR("HTTP/1.0 400 Bad Request\r\n\r\nBad Request"));
      ether.httpServerReply(bfill.position());
    }
  }
}


bool authentication(String req,const char* username,const char* password)
{

      int authorization_start = req.indexOf("Authorization:");
      int authorization_end = req.indexOf("\r\n", authorization_start);
      String authorization = req.substring(authorization_start, authorization_end);
      int hash_start=authorization.indexOf("Basic");
      authorization=authorization.substring(hash_start+6, authorization_end);      
      if(authorization.length()){

        if(checkBasicAuthentication(authorization.c_str(),username, password))
          return true;
        else
        {
          requestAuthentication(defaultPage);
          
        }
        
      }
      else
      {
        requestAuthentication(defaultPage);
        
      }

    return false;
}
void requestAuthentication(String redirectUrl){

  BufferFiller bfill = ether.tcpOffset();      
  String header=" HTTP/1.1 401 Unauthorized\r\nAccept-Ranges: none\r\nConnection: close\r\nContent-Type: text/html\r\nWWW-Authenticate: Basic realm=\"realtekno\"\r\n\r\n";
  header.concat("<script> window.location.href='"+redirectUrl+"'</script>");
  bfill.emit_p(PSTR(header.c_str()));
  ether.httpServerReply(bfill.position());

}
void sendFile(fs::FS &fs, String filename)
{


  String dataType = dataTypeGet(filename);

  String pathWithGz = filename + ".gz";
  if ((SD.exists(pathWithGz) || SD.exists(filename)))
  {
    if (SD.exists(pathWithGz))
    {
      filename = pathWithGz;
      dataType = "application/x-gzip";
      
    }
  }
  File file = SD.open(filename);
  static uint8_t buf[512];
  size_t len = 0;

  if (!file)
  {

    Serial.println(F(" ...not found :("));

    BufferFiller bfill = ether.tcpOffset();
    bfill.emit_p(PSTR("HTTP/1.0 404 Not Found\r\n\r\nNot Found"));
    ether.httpServerReply(bfill.position());
  }

  else
  {
    // send the ACK

    ether.httpServerReplyAck();
    len = file.size();
     if (dataType == "application/x-gzip")
    {    
    
    
      BufferFiller bfill = ether.tcpOffset();      
      String header=" HTTP/1.1 200 OK\r\nAccept-Ranges: none\r\nCache-Control:max-age=2592000\r\nContent-Disposition: inline; filename=\""+filename+"\"\r\nContent-Length: "+len+"\r\nContent-Encoding: gzip\r\nContent-Type: application/x-gzip\r\n\r\n";
      bfill.emit_p(PSTR(header.c_str()));
      ether.httpServerReply_with_flags(bfill.position(), TCP_FLAGS_ACK_V);
    
    }
    
    else if (dataType == "image/png")
    {    
    
    
      BufferFiller bfill = ether.tcpOffset();      
      String header=" HTTP/1.1 200 OK\r\nAccept-Ranges: none\r\nCache-Control:max-age=2592000\r\nContent-Disposition: inline; filename=\""+filename+"\"\r\nContent-Length: "+len+"\r\nContent-Type: image/png\r\n\r\n";
      bfill.emit_p(PSTR(header.c_str()));
      ether.httpServerReply_with_flags(bfill.position(), TCP_FLAGS_ACK_V);
    
    }
    
    while (len)
    {
      size_t toRead = len;
      if (toRead > 512)
      {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;

      uint8_t *payloadBuffer = ether.tcpOffset();
      for (int i = 0; i < toRead; i++)
        payloadBuffer[i] = buf[i];

      if (toRead == 512)
        ether.httpServerReply_with_flags(toRead, TCP_FLAGS_ACK_V);
      else
        ether.httpServerReply_with_flags(toRead, TCP_FLAGS_ACK_V + TCP_FLAGS_FIN_V);
    }
    file.close();
  }
}

void sendFileHTML(fs::FS &fs, String filename)
{

  File file = SD.open(filename);
  int tempRead = 0;

  if (!file)
  {

    Serial.println(F(" ...not found :("));

    BufferFiller bfill = ether.tcpOffset();
    bfill.emit_p(PSTR("HTTP/1.0 404 Not Found\r\n\r\nNot Found"));
    ether.httpServerReply(bfill.position());
  }

  else
  {
    // send the ACK

    ether.httpServerReplyAck();

    while (file.available())
    {
      uint8_t *payloadBuffer = ether.tcpOffset();

      bool pos1 = false;
      bool pos2 = false;
      char c;
      String var = "";

      int i = 0;
      while (file.available() && i <= 512)
      {
        tempRead = file.read();

        if (tempRead == 37)
        {
          if (!pos1)
            pos1 = true;
          else
            pos2 = true;
        }
        else if ((pos1) && (!pos2))
        {
          c = (int8_t)tempRead;
          var += c;
        }
        else if (pos1 && pos2)
        {
          var = processorRead(var);
          for (int j = 0; j < var.length(); j++)
          {
            payloadBuffer[i + j] = (uint8_t)var[j];
          }
          i = i + var.length();
          payloadBuffer[i] = (uint8_t)tempRead;
          i++;
          pos1 = false;
          pos2 = false;
          var = "";
        }
        else
        {
          payloadBuffer[i] = (uint8_t)tempRead;
          i++;
        }
      }

      ether.httpServerReply_with_flags(i, TCP_FLAGS_ACK_V);
    }
    BufferFiller bfill = ether.tcpOffset();
    bfill.emit_p("\n");
    ether.httpServerReply_with_flags(bfill.position(), TCP_FLAGS_ACK_V + TCP_FLAGS_FIN_V);

    file.close();
    // Serial.println(F(" ...sent :)"));
  }
}

String hash(String arg, mbedtls_md_type_t md_type, const byte size)
{
  String hash;
  const char *payload = arg.c_str();
  byte shaResult[size];

  mbedtls_md_context_t ctx;
  //mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

  const size_t payloadLength = strlen(payload);

  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char *)payload, payloadLength);
  mbedtls_md_finish(&ctx, shaResult);
  mbedtls_md_free(&ctx);

  for (int i = 0; i < sizeof(shaResult); i++)
  {
    char str[3];

    sprintf(str, "%02x", (int)shaResult[i]);
    hash += str;
  }
  return hash;
}

void registerUpdate()
{
  for (int i = 0; i < wifi_begin_size; i++)
    wifi_begin[i] = EEPROM.readString(WIFI_BEGIN + (wifi_begin_lenght * i));

  for (int i = 0; i < wifi_config_size; i++)
    EEPROM.readBytes(WIFI_CONFIG + (wifi_config_lenght * i), wifi_config[i], wifi_config_lenght);
  for (int i = 0; i < eth_config_size; i++)
    EEPROM.readBytes(ETH_CONFIG + (eth_config_lenght * i), eth_config[i], eth_config_lenght);
  for (int i = 0; i < device_config_size; i++)
    device_config[i] = EEPROM.readShort(DEVICE_CONFIG + (device_config_lenght * i));
  for (int i = 0; i < plastik_size; i++)
    plastik[i].set = EEPROM.readShort(PLASTIK + (plastik_set_lenght * i));
}

String printLocalTime(const char *format)
{

  if (!getLocalTime(&timeinfo))
  {
    localTimeUpdate((long)lastJSON["dt"]);
    //localTimeUpdate(1571642902);
  }
  char buf[64];
  strftime(buf, 64, format, &timeinfo);
  return String(buf);
}

void logWrite(int kod)
{

  hatalogCSV[log_index] = String(id) + ";" + hatalar[kod] + ";" + adim + ";" + tansiyon_temp[adim];
  hatalogJSON[log_index]["pid"] = id;
  hatalogJSON[log_index]["kod"] = hatalar[kod];
  hatalogJSON[log_index]["adm"] = adim;
  hatalogJSON[log_index]["tns"] = tansiyon_temp[adim];
  log_index++;
  hata_sayisi_hr++;
  hata_sayisi_pr++;
}
short temp_index;
void logHandle()
{
  temp_index = log_index;
  if (temp_index > 0)
  {
    gettimeofday(&time_val, &time_zone);
    String str = printLocalTime("%A, %B %d %Y %H:%M:%S");
    File dataFile = SD.open("/data/" + printLocalTime("%Y/%d.%m.%Y.csv"), FILE_APPEND);
    if (dataFile)
    {
      for (int i = 0; i < temp_index; i++)
        dataFile.println(hatalogCSV[i] + ";" + str + ";" + time_val.tv_sec + ";" + String(device_config[makina_numarasi]) + ";" + cihazKodu);
      dataFile.close();
      for (int i = 0; i < 8; i++)
      {
        if ((temp_index + i) < 8)
        {

          lastJSON["HL"][i]["hdt"] = (int)lastJSON["HL"][i + temp_index]["hdt"];
          lastJSON["HL"][i]["pid"] = (int)lastJSON["HL"][i + temp_index]["pid"];
          lastJSON["HL"][i]["kod"] = (const char *)lastJSON["HL"][i + temp_index]["kod"];
          lastJSON["HL"][i]["adm"] = (int)lastJSON["HL"][i + temp_index]["adm"];
          lastJSON["HL"][i]["tns"] = (int)lastJSON["HL"][i + temp_index]["tns"];
        }
        else
        {
          lastJSON["HL"][i]["hdt"] = time_val.tv_sec;
          lastJSON["HL"][i]["pid"] = (int)hatalogJSON[temp_index + i - 8]["pid"];
          lastJSON["HL"][i]["kod"] = (const char *)hatalogJSON[temp_index + i - 8]["kod"];
          lastJSON["HL"][i]["adm"] = (int)hatalogJSON[temp_index + i - 8]["adm"];
          lastJSON["HL"][i]["tns"] = (int)hatalogJSON[temp_index + i - 8]["tns"];
        }
      }
      temp_index = 0;
      log_index = 0;
      // Serial.println("OK");
    }
    else
      Serial.println("Faield");
  }
}

void lasWrite()
{

  if (id != temp_id)
  {
    temp_id = id;
    //Serial.println("lastWrite");
    gettimeofday(&time_val, &time_zone);
    int timeDiff = (time_val.tv_sec / 3600) - ((int)lastJSON["dt"] / 3600);
    if (timeDiff > 0)
    {
      int j = timeDiff;
      for (int i = 0; i < 8; i++)
      {
        if ((i + j) < 8)
        {
          lastJSON["VS"][i] = (int)lastJSON["VS"][i + j];
          lastJSON["PS"][i] = (int)lastJSON["PS"][i + j];
          lastJSON["HS"][i] = (int)lastJSON["HS"][i + j];
        }
        else
        {
          lastJSON["VS"][i] = 0;
          lastJSON["PS"][i] = 0;
          lastJSON["HS"][i] = 0;
        }
      }
      vargel_sayisi_hr = 0;
      parca_sayisi_hr = 0;
      hata_sayisi_hr = 0;
    }
    lastJSON["dt"] = time_val.tv_sec;
    lastJSON["id"] = id;
    lastJSON["VS"][7] = vargel_sayisi_hr;
    lastJSON["PS"][7] = parca_sayisi_hr;
    lastJSON["HS"][7] = hata_sayisi_hr;

    lastJSON_str = JSON.stringify(lastJSON);
    File fileLast = SD.open("/data/last.txt", FILE_WRITE);
    fileLast.println(lastJSON_str);
    fileLast.close();
  }
}

void localTimeUpdate(long epoch_time)
{
  configTime(device_config[utc]*60, device_config[dst]*60,(const char*)nullptr );
  timeval epoch = {epoch_time, 0};
  const timeval *tv = &epoch;
  timezone utc = {0, 0};
  const timezone *tz = &utc;
  settimeofday(tv, tz);
  getLocalTime(&timeinfo);
  
  
}

uint32_t getNtpTime(uint8_t *ntpServer)
{
  uint32_t timeFromNTP;
  const unsigned long seventy_years = 2208988800UL;
  int i = 30; //Number of attempts
  Serial.println("NTP request sent");
  while (i > 0)
  {
    ether.ntpRequest(ntpServer, 123);
    Serial.print("."); //Each dot is a NTP request
    word length = ether.packetReceive();
    ether.packetLoop(length);
    if (length > 0 && ether.ntpProcessAnswer(&timeFromNTP, 123))
    {
      Serial.println();
      Serial.println("NTP reply received");
      return timeFromNTP - seventy_years + 0;
    }
    delay(500);
    i--;
  }
  Serial.println();
  Serial.println("NTP reply failed");
  return 0;
}

bool parseRequest(String req)
{

  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);

  if (addr_start == -1 || addr_end == -1)
  {
    return false;
  }

  String methodStr = req.substring(0, addr_start);
  String url = req.substring(addr_start + 1, addr_end);
  String versionEnd = req.substring(addr_end + 8);

  String searchStr = "";

  int hasSearch = url.indexOf('?');
  if (hasSearch != -1)
  {
    searchStr = url.substring(hasSearch + 1);
    url = url.substring(0, hasSearch);
  }

  WebRequestMethod method = HTTP_GET;
  if (methodStr == F("POST"))
  {
    method = HTTP_POST;
  }
  else if (methodStr == F("DELETE"))
  {
    method = HTTP_DELETE;
  }
  else if (methodStr == F("OPTIONS"))
  {
    method = HTTP_OPTIONS;
  }
  else if (methodStr == F("PUT"))
  {
    method = HTTP_PUT;
  }
  else if (methodStr == F("PATCH"))
  {
    method = HTTP_PATCH;
  }

  if (method == HTTP_POST)
  {

    int content_start = req.indexOf("Content-Type: ");
    int content_end = req.indexOf('\n', content_start);
    String content_type = req.substring(content_start, content_end);
    if (content_type.startsWith("Content-Type: application/x-www-form-urlencoded"))
    {
      int pos_start = req.indexOf('&');
      int pos_end = req.indexOf('\n', pos_start + 1);
      searchStr = req.substring(pos_start + 1, pos_end);
    }
  }
  parseArguments(searchStr);
  return true;
}

String arg(String name)
{
  for (int j = 0; j < _postArgsLen; ++j)
  {
    if (_postArgs[j].key == name)
      return _postArgs[j].value;
  }
  for (int i = 0; i < _currentArgCount; ++i)
  {
    if (_currentArgs[i].key == name)
      return _currentArgs[i].value;
  }
  return "";
}

String arg(int i)
{
  if (i < _currentArgCount)
    return _currentArgs[i].value;
  return "";
}

String argName(int i)
{
  if (i < _currentArgCount)
    return _currentArgs[i].key;
  return "";
}
bool hasArg(String name)
{
  for (int j = 0; j < _postArgsLen; ++j)
  {
    if (_postArgs[j].key == name)
      return true;
  }
  for (int i = 0; i < _currentArgCount; ++i)
  {
    if (_currentArgs[i].key == name)
      return true;
  }
  return false;
}

void parseArguments(String data)
{

  if (_currentArgs)
    delete[] _currentArgs;
  _currentArgs = 0;
  if (data.length() == 0)
  {
    _currentArgCount = 0;
    _currentArgs = new RequestArgument[1];
    return;
  }
  _currentArgCount = 1;

  for (int i = 0; i < (int)data.length();)
  {
    i = data.indexOf('&', i);
    if (i == -1)
      break;
    ++i;
    ++_currentArgCount;
  }

  _currentArgs = new RequestArgument[_currentArgCount + 1];
  int pos = 0;
  int iarg;
  for (iarg = 0; iarg < _currentArgCount;)
  {
    int equal_sign_index = data.indexOf('=', pos);
    int next_arg_index = data.indexOf('&', pos);

    if ((equal_sign_index == -1) || ((equal_sign_index > next_arg_index) && (next_arg_index != -1)))
    {

      if (next_arg_index == -1)
        break;
      pos = next_arg_index + 1;
      continue;
    }
    RequestArgument &arg = _currentArgs[iarg];
    arg.key = urlDecode(data.substring(pos, equal_sign_index));
    arg.value = urlDecode(data.substring(equal_sign_index + 1, next_arg_index));

    ++iarg;
    if (next_arg_index == -1)
      break;
    pos = next_arg_index + 1;
  }
  _currentArgCount = iarg;
}

String urlDecode(const String &text)
{
  String decoded = "";
  char temp[] = "0x00";
  unsigned int len = text.length();
  unsigned int i = 0;
  while (i < len)
  {
    char decodedChar;
    char encodedChar = text.charAt(i++);
    if ((encodedChar == '%') && (i + 1 < len))
    {
      temp[2] = text.charAt(i++);
      temp[3] = text.charAt(i++);

      decodedChar = strtol(temp, NULL, 16);
    }
    else
    {
      if (encodedChar == '+')
      {
        decodedChar = ' ';
      }
      else
      {
        decodedChar = encodedChar; // normal ascii char
      }
    }
    decoded += decodedChar;
  }
  return decoded;
}