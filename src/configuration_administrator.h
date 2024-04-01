#pragma once

#include <Arduino.h>
#include <Preferences.h>

class ConfigurationAdministrator
{

public:
    ulong GetProcessTime();
    void SaveProcessTime(ulong processTime);
};