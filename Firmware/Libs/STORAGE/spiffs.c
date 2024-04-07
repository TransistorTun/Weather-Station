#include "spiffs.h"

static const char *TAG = "SPIFFS";

esp_err_t mountSPIFFS(char * path, char * label, int max_files)
{
	esp_vfs_spiffs_conf_t conf = {
		.base_path = path,
		.partition_label = label,
		.max_files = max_files,
		.format_if_mount_failed =true,
	};

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Mount SPIFF Failed");
    }
        
    //Get information spiffs partition
    // size_t total = 0, used = 0;
    // ret = esp_spiffs_info(NULL, &total, &used);
    // if (ret != ESP_OK) 
    // {
    //     ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    // } 
    // else 
    // {
    //     ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    // }
    return ret;
}