# Esp32-IronSpine
A Simple Firmware to ESP32

# Necessary
1x Esp32 (I Used a EspWroom32)

# How to Install
1. Download Arduino IDE
2. Install Board Manager JSON (File > Preferences > Board Manager URL > Insert Link [https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json])
3. Install Board Manager (Board Manager > esp32 by Espressif Systems)
4. Open Project (Download Esp32_Iron_Spine_V2 > Open Arduino IDE > File > Open > Select Esp32_Iron_Spine_V2.ino)
5. Connect Esp32 to USB Port
6. Select Esp32 COM Port
7. Select Board ESP32 Dev Module
8. Upload File (Sketch > Upload)
9. Done!

# How To Use
1. Open WiFi Connection on Your Phone or Computer
2. Open Browser and Serch http://192.168.4.1/

# Functions
- Root (Functions Home Page)
- AP Connection (Connect Esp32 to Selected AP)
- AP Scanner (Scan and Show Close APs, Clicking on AP Start a Deauth Attack)
- ARP List (Show IPv4 and Mac From Connected Devices by ARP)

# PT-BR
Some characters do not have the correct accent due to a compilation error, which causes some characters that have an accent to be displayed incorrectly.<br>

Alguns caracteres não tem o acento correto devido erro de compilação, que faz que alguns caracteres que tenha acento serem exibidos de forma incorreta.


# Attention
I Don't Find a Library to Send Deauth Packet in Esp32. A Library i used to it, is too heavy to EspWroom32.

Sorry );

But if you can do it without use a heavy library, contact me.


# Optional Things
- Battery Module
- LCD Screen
- Buttons (To Functions Selection)
- Better Antenas
