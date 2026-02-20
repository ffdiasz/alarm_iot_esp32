## AlarmClock IOT - ESP32

---

#### Introduction
AlarmClock IOT consists of a non-blocking system that integrates ESP32 with Telegram API,so you only need WIFI to control your ESP32 from anywhere.

#### About Project

Unlike standard DIY projects, this system was architected with professional firmware principles:

- **Asynchronous Communication**: Efficient handling of the Telegram API using the [UniversalTelegramBot](LINK) library without blocking critical system tasks.

- **Modular Architecture**: Decoupled logic where a central Orchestrator manages specialized modules for Alarms, User Data, and Network Connectivity.

- **Network Resilience**: Integration of NTP (Network Time Protocol) and Wi-Fi management to ensure time-critical accuracy for alarm triggers.

#### Tools
- C++
- ESP32
- Telegram API
- [BOM List](Hardware/BOM-List.pdf)

#### Hardware Features
Here I explain my hardware decisions, you can take the Schematics [here](Hardware/Schematic.PDF).

- **Speaker Driver:** Used a **_BC337 NTP Transistor_** as a current buffer. A **_1N4007 Flyback Diode_** to protect transistor of reverse voltage when speaker deactivated.

- **Debounce Circuit:** I prioritized doing the debouncing circuit in Hardware because is most effective than on software. The circuit consist on an **_RC Filter_** with a 1kΩ resistor and 10nF capacitor in series with the button to suppress mechanical noise.

- **Power Management:** Used **_LM7805CV_** to regulate 12v to 5v, following the recomendations described in his [datasheet](LINK), decoupling capacitors placed in parallel with INPUT and OUTPUT to stabilize voltage.


#### Design Pattern
- **System Control:**
- **Alarm Manager:**
- **User Manager:**
- **Button Driver:**
- **Buzzer Driver:**
- **NTP(Network Time Protocol):**
- **WiFI Manager:**



#### Step-by-Step Introductions



STEP 1:

``` 
    STEP 1;
```

_STEP 2_:
```
    STEP 2;
```

_STEP 3_:
```
    STEP 3;
```

_STEP 4_:
```
    STEP 4;
```