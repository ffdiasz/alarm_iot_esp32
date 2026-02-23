## AlarmClock IOT - ESP32

---

#### INDEX

-   [Introduction](#introduction)
-   [About Project](#about-project)
-   [Tools](#tools)
-   [Hardware Features](#hardware-features)
-   [Design Pattern](#design-pattern)
-   [Step-by-Step Introductions](#step-by-step-introductions)

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

- **Speaker Driver:** Used a **_BC337 NPN Transistor_** as a current buffer. A **_1N4007 Flyback Diode_** to protect transistor of reverse voltage when speaker deactivated.

- **Debounce Circuit:** I prioritized doing the debouncing circuit in Hardware because is most effective than on software. The circuit consist on an **_RC Filter_** with a 1kΩ resistor and 10nF capacitor in series with the button to suppress mechanical noise.

- **Power Management:** Used **_LM7805CV_** to regulate 12v to 5v, following the recomendations described in his [datasheet](LINK), decoupling capacitors placed in parallel with INPUT and OUTPUT to stabilize voltage.


#### Design Pattern

- **_Orchestrator Pattern(System Control):_** System Control acts as an mediator between telegram and user libraries, handling messages and executing commands.

- **_Manager Pattern:_** Following the Single Responsability Principle (**SRP**), alarm, user, NTP and WIFI are decoupled libraries that handle only their functions, facilitating to add features and maintain code.

- **Hierarchical State Machine(HSM):_** The Telegram comunnication task use an HSM design to ensure that its functions will execute without block other tasks.

#### Step-by-Step Introductions



**1. _Install PlatformIO_:**

    First one, you need to install PlatformIO extension in Vscode, because it ability to maintain the libraries in a specific version.


**2. _Create an Telegram BOT_:**

    1. Open Telegram;
    2. Search for @BotFather;
    3. Click Start;
    4. Type /newbot;
    5. Enter a name (anything you want);
    6. Enter a username (must end in bot. Example: myfirstassistantbot);
    7. BotFather will generate your API token (Save it, you'll need in the nexts steps).


**3. _Clone git repository_:**

In Vscode Terminal:
```
    git clone https://github.com/ffdiasz/alarm_iot_esp32.git
```

**4. _Make Settings_:**

    1. in Software, navigate to 'include' folder;
    2. create an file named 'secure.h';
    3. paste the following code and replace with your credencials:

```c++
#pragma once

namespace Secure {
    const char* SSID = "YOUR_WiFI_SSID";
    const char* PASSWORD = "YOUR_WiFI_PASSWORD";
    const char* BOT_TOKEN = "YOUR_BOT_TOKEN";
}
```

**5. _Hardware:_**

1. for hardware part you can follow schematics [here](Hardware/Schematic.PDF);

2. PINOUT was defined in top of 'main.cpp' file, to see it or change, navigate to 'Software/src/main.cpp';