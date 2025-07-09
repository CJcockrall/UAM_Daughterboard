# Underwater Acoustic Modem Daughterboard Software
This is the software developed for the daughterboard of the Underwater Acoustic Modem project developed by Group #5 of the Engineering Physics Capstone of the University of Alberta in 2025 and continued in the summer for presentation at the OCEANS 2025 Great Lakes conference. The daughter board is responsible for external communication with the modem through Wi-Fi and Ethernet and uses an ESP32-S3 to do this. 

## Requirements: 
+ VSCode with the ESP-IDF extension. [ESP-IDF Installation Instructions](https://docs.espressif.com/projects/vscode-esp-idf-extension/en/latest/installation.html)
+ Underwater Acoustic Modem hardware. [Repo with PCB files](https://github.com/ericvoi/UAM_PCB/tree/main)
  + Main board firmware flashed [Main board Firmware](https://github.com/ericvoi/UAM_Firmware)

## Setup: 
1. Download repo and open in VSCode
2. Plug in daughter board and choose the correct COM port on the bottom toolbar
3. Put the ESP32 into download boot mode by holding down the GPIO0 button while the reset button is released
4. Build the project
5. Flash it

## Use:
