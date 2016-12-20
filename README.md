# AmbiLight
AmbiLight is the cheapjack version of Philips TV lighting system. The software grabs the currently displayed screen (be it a video or something else), performs a _k_-Means color clustering and then sends the _dominant_ color to the serial port. A controller script `controller.ino` reads the dominant color and addresses a LED strip to light in that specific color.

## Hardware
Here is what you need:
- Arduino Uno [0]
- LED Light Strips [1]
- A few cables and FETs [2]

## Software
### Arduino
- Download the Arduino IDE
- PlugIn the Arduino Uno using USB
- Upload the `controller.ino` to the board

### Computer
- Install Qt > 5.5.0
- Install CMake
- Run CMake
- Open the project in your favorite IDE
- Compile & run it
NOTE: the COM Port can be set in the cmd e.g. `AmbiLight.exe -c COM3`

Have fun!

### Authors
Markus Diem
Florian Kleber

## Links
[0] https://www.arduino.cc/en/Main/ArduinoBoardUno
[1] https://www.amazon.de/TOPLUS-Beleuchtungsset-Lichterkette-Fernbedienung-Verbesserte/dp/B01MPWK5C0/ref=sr_1_5?s=lighting&ie=UTF8&qid=1482231343&sr=1-5&keywords=led+streifen
[2] http://www.jerome-bernard.com/blog/2013/01/12/rgb-led-strip-controlled-by-an-arduino/
