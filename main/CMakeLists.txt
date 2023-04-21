idf_component_register(SRCS "main.c"
                            "soil_humidity.c"
                            "water_level.c"
                            "temperature.c"
                            "control.c"
                            "idle.c"
                            "wifi.c"
                            "http.c"
                            "settings.c"
                            "adc_config.c"
                            "day_time.c"
                            "ap_mode.c"
                            "irrigation.c"
                    SRC_DIRS "ulp"
                    INCLUDE_DIRS "../include"
                    REQUIRES esp32-owb
                            esp32-ds18b20
                            esp_adc
                            esp_wifi
                            nvs_flash
                            esp_http_client
                            esp_http_server
                            esp-tls
                            ulp
                            freertos)

set(ulp_app_name ulp_${COMPONENT_NAME})
set(ulp_s_sources   "ulp/ulp_test.S"
                    "ulp/macro.S"
                    "ulp/wakeup.S")

set(ulp_exp_dep_srcs "main.c")

ulp_embed_binary(${ulp_app_name} "${ulp_s_sources}" "${ulp_exp_dep_srcs}")

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")

