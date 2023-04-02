idf_component_register(SRCS "main.c"
                            "soil_humidity.c"
                            "water_level.c"
                            "temperature.c"
                            "control.c"
                            "idle.c"
                    INCLUDE_DIRS "../include"
                    REQUIRES esp32-owb esp32-ds18b20 esp_adc)
