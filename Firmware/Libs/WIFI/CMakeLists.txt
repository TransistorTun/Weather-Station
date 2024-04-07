set(component_srcs "connect.c" "http_request.c")

idf_component_register(SRCS "connect.c" "${component_srcs}"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES "freertos" "lwip" "esp_wifi" NVS 
                       )