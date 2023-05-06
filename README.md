# Automation for watering the garden plants (project in progress)

## Description
Automated process for watering garden plants at a particular time or if the soil humidity is too low. The device can work in two modes either "normal" or "eco". The first one can be use to communicate by rf433 with external device to gather data and seccond if there is need to decrase power consumption.
The system requires an external tank where the water is filled automatically. The pump is controlled by an rf433 to decrase the amount of wires. Also the temperature of the water is checked before irrigation to prevent thermal shock for the plants in case it is to cold.
Users can configure the upper and lower levels of humidity at which watering will start and stop, as well as the temperature of the water in the tank. The pump start can be set to watering only during the day to eliminate loud.


## Work progress
- [x] "Normal" mode
- [ ] "Eco" mode
- [x] Web server for user configuration
- [x] Checking water temperature
- [x] Checking air temperature
- [ ] Checking soil humidity
- [x] Checking the time of day
- [x] Control of the pump for watering
- [ ] Filling the tank
- [ ] Checking the water level in the tank
- [ ] RF433 gathering data
- [ ] RF433 pump control


## Hardware
The project is built on the **ESP32-WROOM-32D** [^1] microcontroller which can be powered by batteries source.
...


## External links
[^1]:[ESP32-WROOM-32D documentation](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf)
