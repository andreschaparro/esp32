#include <stdio.h>
#include "esp_random.h"
#include "api_dado.h"

uint32_t tirar_dado(void)
{
    return ((esp_random() % 6) + 1);
}