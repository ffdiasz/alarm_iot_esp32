#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "alarm_manager.h"
#include "user_manager.h"
#include "wifi_manager.h"
#include "secure.h"
#include "ntp.h"
#include "SystemControl.h"
#include "array"

//Time to main tasks
constexpr const uint32_t maxWaitTimeWifi    = 1000 * 15;      // 15 secs
constexpr const uint32_t maxWaitTimeNtp     = 1000 * 15;      // 15 secs
constexpr const uint32_t wifiCheckTime      = 1000 * 60;      // 1 minute
constexpr const uint32_t ntpCheckTime       = 1000 * 60 * 30; // 30 minutes
constexpr const uint32_t syncCheckTime      = 1000 * 60;      // 1 minute
constexpr const uint32_t messageCheckTime   = 500;           // 0.5 sec

//PreviousTime to main tasks
uint32_t previousWifiCheck;
uint32_t previousNtpCheck;
uint32_t previousSyncCheck;
uint32_t previousMessageCheck;

//NTP
constexpr const char* ntpServer = "pool.ntp.org";
constexpr const int32_t  gmtOffset_sec = -3 * 3600; //BRT - brazil
constexpr const int32_t   daylightOffset_sec = 0; //daylight off

//StateFlags
bool wifiConnected = false;
bool NTPstatus = false;

//Init telegram bot
WiFiClientSecure secured_client;
UniversalTelegramBot AlarmClockBot(Secure::BOT_TOKEN, secured_client);

//Init Users
std::array <user, maxUsers> users;

//SystemControl
SystemControl SystemManager(AlarmClockBot, users);

void setup() {
  delay(500); //ESSA LINHA GARANTE A ESTABILIDADE DO SISTEMA NÃO MEXER!

  Serial.begin(115200);
  //Connect to wifi and do some configs
  wifiConnected = wifiConect(Secure::SSID, Secure::PASSWORD, maxWaitTimeWifi); // wait 15 secs max

  // Add root certificate for api.telegram.org
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  //Get Current time
  NTPstatus = ntpSync(gmtOffset_sec, daylightOffset_sec, ntpServer, maxWaitTimeNtp); // wait 15 secs max

  previousWifiCheck = millis();
  previousMessageCheck = millis();
  previousNtpCheck = millis();
  previousSyncCheck = millis();
}

void loop() {
  uint32_t now = millis();

  // TASK 1: Alarms and Telegram (1 sec)
  if ((wifiConnected && NTPstatus) && (now - previousMessageCheck > messageCheckTime)){
    SystemManager.TelegramManager();
  
    previousMessageCheck = millis();
  }

  //TASK 2: Check Wifi Connection (1 minute)
  if (now - previousWifiCheck > wifiCheckTime){
    wifiConnected = checkWifiStatus();

    previousWifiCheck = millis();
  }

  //TASK 3: Resync NTP (30 Minutes)
  if (wifiConnected && (now - previousNtpCheck > ntpCheckTime)){
    NTPstatus = ntpSync(gmtOffset_sec, daylightOffset_sec, ntpServer, 0);

    previousNtpCheck = millis();
  }
}

