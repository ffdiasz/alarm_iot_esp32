#include <Arduino.h>
#include "WiFi.h"
#include "secure.h"
#include "bot.h"
#include "alarm_manager.h"

int8_t tentativas = 5;
uint32_t Bot_Time = 1000;
bool WifiStatus = false;
bool NTPstatus = false;

//Inicializa telegram bot
bot mamba(Secure::BOT_TOKEN);

//WiFi Credentials
//SSID and PASSWORD in secure.h archive

void VerificarNTP(){
  time_t now = time(nullptr);

  if (now > 1600000000){
    Serial.print("\nRetrieving time: ");
    Serial.println(now);
    NTPstatus = true;
    return;
  }
}

void VerificarWifi(){
  if (WiFi.status() == WL_CONNECTED){
    Serial.print("\nWifi Conected - IP adress: ");
    Serial.println(WiFi.localIP());
    WifiStatus = true;
    return;
  }
}

void setup() {
  Serial.begin(115200);
  delay(500); //ESSA LINHA GARANTE A ESTABILIDADE DO SISTEMA NÃO MEXER!

  Serial.print("Connecting to Wifi SSID ");
  Serial.print(Secure::SSID);
  WiFi.begin(Secure::SSID, Secure::PASSWORD);

  int8_t cont = 0;
  while (WiFi.status() != WL_CONNECTED && cont < tentativas){
    Serial.print(".");
    delay(200);
    
    cont++;
  }

  VerificarWifi();

  Serial.print("\nRetrieving time: ");
  configTime(-3 * 3600, 0, "pool.ntp.org"); // horario de brasilia via NTP
  time_t now = time(nullptr);
  
  //Compara o ano menor que 2020
  cont = 0;
  while(now < 1600000000 && cont < tentativas){
    Serial.print(".");
    delay(100);
    now = time(nullptr);

    cont++;
  }

  VerificarNTP();
}

void loop() {
  if (!WifiStatus){
    VerificarWifi();
  }

  if (WifiStatus && !NTPstatus){
    VerificarNTP();
  }

  mamba.check_telegram(Bot_Time);
}

