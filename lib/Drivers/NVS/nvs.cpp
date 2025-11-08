#include "nvs.hpp"

static nvs_handle_t my_handle;

Nvs::Nvs(){}

void Nvs::init()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    is_initialized = true;
}

void Nvs::write(const char* key, void* buffer, uint32_t len)
{
    if(!is_initialized) {
        return;
    }

    if (nvs_open("DEFAULT", NVS_READWRITE, &my_handle) != ESP_OK) {
        return;
    }

    if (nvs_set_blob(my_handle, key, buffer, len) == ESP_OK) {
        nvs_commit(my_handle);
    }
    
    
    nvs_close(my_handle);
}

void Nvs::read(const char* key, void* buffer, uint32_t* len)
{
    if(!is_initialized) {
        return;
    }

    if (nvs_open("DEFAULT", NVS_READONLY, &my_handle) != ESP_OK) {
        return;
    }

    // Read
    nvs_get_blob(my_handle, key, buffer, (size_t *)len);
    nvs_close(my_handle);
}