#ifndef NVS_HPP
#define NVS_HPP

#include <stdint.h> 
#include "nvs_flash.h"
#include "nvs.h"

class Nvs
{
public:
    Nvs();

    void init();

    void write(const char* key, void* buffer, uint32_t len);
    void read(const char* key, void* buffer, uint32_t* len);

    void operator=(float buffer)
    {
        write(m_key, (void*)&buffer, sizeof(buffer));
    }

    operator float()
    {
        float value;
        uint32_t len;
        read(m_key, (void*)&value, &len);
        return value;
    }

    Nvs operator[](const char* key)
    {
        m_key = key;
        return *this;
    }

private:
    bool is_initialized = false;
    const char* m_key;
};



#endif