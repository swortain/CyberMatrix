
# Background

CyberMatrix was designed when JLC was making colored PCBs. At that time, I had 5 square dot matrices and a TMP1075 temperature sensor that  wanted to use but had not chance to. Since colored PCBs can have a layer of color images on top of regular PCBs, I decided to design a ornament.

# Feature List

1. ESP32 as the controller, 5 TM1640s driving 5 7x11 square dot matrices, with 5 WS2812s on the back as background light
2. Automatically connect to NTP for time synchronization after automatic networking
3. Backlight source transition light effect
4. Dot matrix switching animation
5. Onboard temperature sensor reading
6. Accelerometer recognizes desktop tapping action

# Usage

Open this project in VSCode. If you haven't installed PlatformIO, you need to install the plugin. Then, use PlatformIO to compile and upload. PIO will automatically download the dependency libraries and compilation environment. The project is based on the Arduino framework and theoretically can be used directly with ESP32S3, ESP32S2, ESP8266, and other ESP32 series, but it has not been tested.

# Font Editing

You can write the contents of font.h into [this address](https://tchapi.github.io/Adafruit-GFX-Font-Customiser/) for editing, and then overwrite the contents of font.h with the edited content (do not add the include statement).

# TODO

* [X] Complete transition animation
* [ ] Complete code for different row and column priorities
* [ ] Enclosure

## License

[BSD](https://chat.openai.com/LICENSE)
