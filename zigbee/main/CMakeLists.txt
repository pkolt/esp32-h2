idf_component_register(SRCS "led.c" "main.c"
                    PRIV_REQUIRES spi_flash nvs_flash
                    INCLUDE_DIRS "")
