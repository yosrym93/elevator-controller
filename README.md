# Elevator-Controller
A generic Arduino controller that manages the operations of an elevator.

# Dependencies
- Proteus (for simulation)
- PlatformIO `5.1.1+` (for build/test)
- gcc/g++ `8.3.0+`  (for local test)
- gnu-make `4.2.1` (for build)

```bash
$ sudo apt update &&
  sudo apt install -y g++ gcc python3-pip make &&
  sudo python3 -m pip install platformio &&
  pio update
```

# VSCode Integration
You may install `PlatformIO IDE` extension for VSCode.
Make sure to disable/uninstall `Arduino` extension before it, if you have it enabled.

# Simulation
- Open proteus project file `.proteus\elevator-controller.pdsprj` using Proteus 8.6.
- Load the hex file `./.pio/build/default/firmware.hex` which is generated after building into the Arduino Uno component.
- Download the ultrasonic sensor library from the following link, follow the instructions to place its library files in Proteus's library directory and load its hex file to the ultrasonic sensor:
https://www.theengineeringprojects.com/2015/02/ultrasonic-sensor-library-proteus.html
- Run the simulation and have fun.

# Make Targets
## Build
`make` or `make build`
## Test
`make test`
## Clean
`make clean`
