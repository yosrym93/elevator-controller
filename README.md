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

# Load in Proteus
Use hex file `./.pio/build/default/firmware.hex` which is generated after build.

# Make Targets
## Build
`make` or `make build`
## Test
`make test`
## Clean
`make clean`
