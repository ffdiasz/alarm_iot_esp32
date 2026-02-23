/*-------------------------- O QUE FAZER ----------------------------------

=-=-=-= SOFTWARE =-=-=-
1- REFATORAR OS COMANDOS DO TELEGRAM PARTE DO UX -> TEM PARTES QUE PRECISA COMPLETAR TIPO O /HELP
2- OLHAR ALGUMAS PARTES DA HSM PARA TIRAR ALGUNS COMENTARIOS NO SERIAL QUE PODEM DEIXAR O SISTEMA MAIS LENTO
3- ADICIONAR UMA FORMA DE SALVAR AS INFORMAÇÕES NA MEMÓRIA, PARA NÃO SE PERDER EM CASOS DE DESLIGAMENTO

=-=-=-= HARDWARE =-=-=-
1- FAZER UMA FONTE EXTERNA PARA O ESP32 SE MANTER LIGADO
*/

// PINOUT: 
constexpr const uint8_t buzzerPin = 4;
constexpr const uint8_t buttonPin = 5;

// INCLUDES
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "user_manager.h"
#include "wifi_manager.h"
#include "secure.h"
#include "ntp.h"
#include "SystemControl.h"
#include "buzzer.hpp"
#include "button.h"

//Time to main tasks
constexpr const uint32_t maxWaitTimeWifi    = 1000 * 15;      // 15 secs
constexpr const uint32_t maxWaitTimeNtp     = 1000 * 15;      // 15 secs
constexpr const uint32_t wifiCheckTime      = 1000 * 60;      // 01 minutes
constexpr const uint32_t ntpCheckTime       = 1000 * 60 * 30; // 30 minutes
constexpr const uint32_t syncCheckTime      = 1000 * 60;      // 01 minutes
constexpr const uint32_t messageCheckTime   = 500;            // 0.5 secs
constexpr const uint32_t alarmCheckTime     = 1000 * 60;      // 01 minutes

//PreviousTime to main tasks
uint32_t previousWifiCheck;
uint32_t previousNtpCheck;
uint32_t previousSyncCheck;
uint32_t previousMessageCheck;
uint32_t previousAlarmCheck;

//NTP
constexpr const char* ntpServer = "pool.ntp.org";
constexpr const int32_t  gmtOffset_sec = -3 * 3600; //BRT - brazil
constexpr const int32_t   daylightOffset_sec = 0;   //daylight off

//StateFlags
bool wifiConnected = false;
bool NTPstatus = false;

//Init telegram bot
WiFiClientSecure secured_client;
UniversalTelegramBot AlarmClockBot(Secure::BOT_TOKEN, secured_client);

//Init Users
user_manager users;

//SystemControl
SystemControl SystemManager(AlarmClockBot, users);

//Buzzer
constexpr const uint16_t freq = 300; //300 Hz
bool hasAlarmTriggered;
buzzer BuzzerAlarm(buzzerPin,freq);

//button
constexpr const bool pullResistorType = true; //pull_up
button buttonBuzzer(buttonPin);

void setup() {
  delay(500); //Dude... if you love your life, DON'T TOUCH! OK?
  Serial.begin(115200);

  buttonBuzzer.begin(pullResistorType,FALLING);

  //Connect to wifi and do some configs
  wifiConnected = wifiConect(Secure::SSID, Secure::PASSWORD, maxWaitTimeWifi); // wait 15 secs max

  // Add root certificate for api.telegram.org
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  //Get Current time
  NTPstatus = ntpSync(gmtOffset_sec, daylightOffset_sec, ntpServer, maxWaitTimeNtp); // wait 15 secs max

  uint32_t timeNow = millis(); 
  previousWifiCheck = timeNow;
  previousMessageCheck = timeNow;
  previousNtpCheck = timeNow;
  previousSyncCheck = timeNow;
  previousAlarmCheck = timeNow;
}

void loop() {
  uint32_t now = millis();
  
  // Turn off buzzer when button pressed
  if(hasAlarmTriggered && buttonBuzzer.wasClicked()){
    hasAlarmTriggered = false;
    BuzzerAlarm.off();
  }

  // TASK 1: HandleMessages in telegram every second (1 sec)
  if ((wifiConnected && NTPstatus) && (now - previousMessageCheck > messageCheckTime)){
    SystemManager.TelegramManager();
  
    previousMessageCheck = millis();
  }

  // TASK 2: Check alarm state every minute (1 minute)
  if (now - previousAlarmCheck >= alarmCheckTime){
      struct tm timeNow = getTime();
      hasAlarmTriggered = users.CheckAlarms(timeNow);

      if (hasAlarmTriggered){
        BuzzerAlarm.on(128);
      }
      else{
        BuzzerAlarm.off();
      }

      previousAlarmCheck = millis();
  }
  
  // TASK 3: Check Wifi Connection (1 minute)
  if (now - previousWifiCheck > wifiCheckTime){
    wifiConnected = checkWifiStatus();

    previousWifiCheck = millis();
  }

  // TASK 4: Resync NTP (30 Minutes)
  if (wifiConnected && (now - previousNtpCheck > ntpCheckTime)){
    NTPstatus = ntpSync(gmtOffset_sec, daylightOffset_sec, ntpServer, 0);

    previousNtpCheck = millis();
  }
}

