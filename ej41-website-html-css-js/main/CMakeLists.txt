idf_component_register(    
    SRCS 
        "main.c"
        "api_spiffs/api_spiffs.c"
        "api_wifi_station/api_wifi_station.c"
    INCLUDE_DIRS 
        "."
        "api_spiffs"
        "api_wifi_station"
)

spiffs_create_partition_image(storage ../site FLASH_IN_PROJECT)