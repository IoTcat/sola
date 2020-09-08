# Sola Proj
Yimian's Family IoT system in Taian.

[简体中文（推荐）](./zh.md)

## Objective
Combining the knowledge I have learned and using cutting-edge technology to benefit life has been my unremitting pursuit for a long time. In order to let my family experience a more stable, comfortable and intelligent lifestyle, and to consolidate the technical foundation and architecture experience accumulated in the school smartfarm project and my own ushio system, I decided to use 20 to 23 before going abroad. All day long, reconstruct the old IoT system at home. Adopt a layered architecture and interface-oriented, test-oriented, and **document-oriented** development principles, with **Available** and **Reliable** as the highest priority indicators to build an operational target 3. A highly modular, easy-to-remote control, open source home software and hardware intelligent sensor control system for more than years.

##  Hardware design

### Single Chip Microcomputer Material Selection
Use Arduino UNO as the calculation unit. On the one hand, Arduino Uno has high reliability. My previous smart curtain system used Arduino Uno as the main control board, and it ran smoothly for one and a half years without hardware problems. On the other hand, Arduino Uno has a larger RAM, so there is no need to bother to program the microcontroller in order to compete for RAM.

The main disadvantage of Arduino UNO is its high price, but this cost is necessary to ensure long-term stable operation.

### WiFi communication module selection

Use ESP-01 as the WiFi communication module. The main reason is that the ESP-01 has been used to build a remote control car in the previous school courses, and I have a better understanding of its working methods and performance to reduce development costs.

### LoRa communication module selection

Use **Anxinke LORA RA-02 LORA** as the communication module between LoRa node and LoRa gateway. The reason is that this module is used in the surf project, so I understand it better.

### Smart speaker selection

As the most important interface for human-computer interaction in this IoT system, smart speakers play a pivotal role. After research, I use Tmall Elf Sugar R as a home smart speaker. On the one hand, the price/performance ratio of Tmall Genie is very cheap. Second, unlike Xiaomi’s development of Xiao Ai, the purpose of Ali’s development of Tmall Genie is to provide a platform that allows developers to access its network in various forms, which is very important to me.

### Light toggle switch

Initial idea: one-way control + reset device.
Actual realization: The original single-opening is transformed into double-opening, one is connected in series with the relay, and the other is connected with two pin ports.


## Communication Design

![Network Topology Diagram](docs/img/img_43b85bd4_1096x660_8_null_normal.png)

Two communication modes of WiFi+LoRa are adopted. In scenarios that require high-speed and high-quality communication, such as Tmall Genie, mobile phones, and notebooks, use WiFi as a means of communication. In places where the WiFi signal is unstable, use LoRa for communication. Use MQTT as the application layer protocol.

## Main control system design

Use the python3 open source project Home Assistant as the basis to build the main control system of this project. The effect is as follows:

![Sola Website](docs/img/img_c1a1b81_1895x859_8_null_normal.jpeg)

## Multimedia system design

Multimedia is mainly provided by Tmall Genie. In addition, through the Xiaomi box, the video, pictures, and music in the home network disk can be played on the TV and stereo.

## Cloud support system design

dns.yimian.xyz provides dns resolution service.

## NAS storage system design

By mounting the idle 500G mechanical hard disk removed from the old computer to the old IBM server, the storage system is built.
In the local area network, since the home is a win system, smb is used as a sharing protocol to realize file transfer and supports mounting in the local area network. The speed is stable, and the whole network reads and writes about 10MB/s. Access to the external network is realized through http and frp reverse generation, but the speed is slow.

The following is a screenshot of the web interface:

![NAS Web Page](docs/img/img_352da66_1866x814_8_null_normal.jpeg)

