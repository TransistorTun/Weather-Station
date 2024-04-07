set(component_srcs "spiffs.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES "spiffs" "log"
                       )