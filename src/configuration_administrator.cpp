#include "configuration_administrator.h"

ulong ConfigurationAdministrator::GetProcessTime()
{

    Preferences *prefs = new Preferences();
    prefs->begin("process", false);
    // default is 10h  60x60x10
    ulong processTime = prefs->getULong("PROCESS_TIME", 60 *  1);

    prefs->end();
    return processTime;
}

void ConfigurationAdministrator::SaveProcessTime(ulong processTime){
    Preferences* prefs = new Preferences();
    prefs->putULong("PROCESS_TIME", processTime);
}