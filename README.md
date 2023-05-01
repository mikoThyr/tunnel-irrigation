# Automation for watering the garden plants (project in progress)

## Description

*Automated process for watering garden plants at a particular time or if the soil humidity is too low. The device can work in two modes either "normal" or "eco". The first one can be use to communicate by rf433 with external device to gather data and seccond if there is need to decrase power consumption.*
*The system requires an external tank where the water is filled automatically. The pump is controlled by an rf433 to decrase the amount of wires. Also the temperature of the water is checked before irrigation to prevent thermal shock for the plants in case it is to cold.*
*Users can configure the upper and lower levels of humidity at which watering will start and stop, as well as the temperature of the water in the tank. The pump start can be set to watering only during the day to eliminate loud.*

## Work progress

*Already done:*
- "Normal" mode
- Web server for user configuration
- Checking water temperature
- Checking air temperature
- Checking soil humidity
- Checking the time of day
- Control of the pump for watering

*Todo:*
- "Eco" mode
- Filling the tank
- Checking the water level in the tank
- RF433 gathering data
- RF433 pump control


