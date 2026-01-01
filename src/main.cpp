#include <Arduino.h>
#include "secure.h"
#include <bot.h>
#include <alarm_manager.h>
#include <user_manager.h>
#include <wifi_manager.h>
#include <ntp.h>

//Time to main tasks
constexpr const uint32_t maxWaitTimeWifi = 1000 * 15;  // 15 secs
constexpr const uint32_t maxWaitTimeNtp = 1000 * 15;  // 15 secs
constexpr const uint32_t wifiCheckTime = 1000 * 60; // 1 minute
constexpr const uint32_t ntpCheckTime = 1000 * 60 * 30; // 30 minutes
constexpr const uint32_t syncCheckTIme = 1000 * 60; // 1 minute

//PreviousTime to main tasks
uint32_t previousWifiCheck;
uint32_t previousBotCheck;
uint32_t previousNtpCheck;
uint32_t previousSyncCheck;

//NTP
constexpr const char* ntpServer = "pool.ntp.org";
constexpr const int32_t  gmtOffset_sec = -3 * 3600; //BRT - brazil
constexpr const int32_t   daylightOffset_sec = 0; //daylight off

//StateFlags
bool wifiConnected = false;
bool NTPstatus = false;

//Init telegram bot
bot mamba(Secure::BOT_TOKEN);

//bot times
constexpr const int8_t tentativas = 5;
constexpr const uint32_t Bot_Time = 1000;

void setup() {
  delay(500); //ESSA LINHA GARANTE A ESTABILIDADE DO SISTEMA NÃO MEXER!

  Serial.begin(115200);
  //Connect to wifi and do some configs
  wifiConnected = wifiConect(Secure::SSID, Secure::PASSWORD, maxWaitTimeWifi); // wait 15 secs max

  //Get Current time
  NTPstatus = ntpSync(gmtOffset_sec, daylightOffset_sec, ntpServer, maxWaitTimeNtp); // wait 15 secs max

  previousWifiCheck = millis();
  previousBotCheck = millis();
  previousNtpCheck = millis();
}

void loop() {
  uint32_t now = millis();

  //Check Wifi Connection every minute
  if (now - previousWifiCheck > wifiCheckTime){
    wifiConnected = checkWifiStatus();
    previousWifiCheck = now;
  }

  //Resync NTP every 30 minutes for get a precision time
  if (wifiConnected && (now - previousNtpCheck > ntpCheckTime)){
    NTPstatus = ntpSync(gmtOffset_sec, daylightOffset_sec, ntpServer, 0);
    previousNtpCheck = now;
  }

  // try resync NTP
  if ((wifiConnected && !NTPstatus) && (now - previousSyncCheck > syncCheckTIme)){
    NTPstatus = ntpSync(gmtOffset_sec, daylightOffset_sec, ntpServer, 0);
  }

  //Check telegram bot
  if (wifiConnected) {
    mamba.check_telegram(Bot_Time);
  }   
}

