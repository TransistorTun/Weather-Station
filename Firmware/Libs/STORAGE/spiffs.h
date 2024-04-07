#ifndef SPIFFS_H_
#define SPIFFS_H_

#include <stdio.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "esp_heap_caps.h"

esp_err_t mountSPIFFS(char * path, char * label, int max_files);

#endif /* SPIFFS_H_ */