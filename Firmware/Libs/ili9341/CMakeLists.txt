set(component_srcs "spi_tft.c" "ili9341.c" "font.c" "icon.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES "freertos" "driver"
                       )