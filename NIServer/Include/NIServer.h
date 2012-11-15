#ifndef NISERVER_H
#define NISERVER_H

enum license_State
{
    LICENSE_INVALID = 0,    // Invalid license
    LICENSE_VALID,          // Valid license
    LICENSE_TIMELIMITED,    // Valid license but with time limitation
    LICENSE_UNKNOWN         // Some errors might occur during the license validation
};

license_State CheckLicense();
void StartNIService();
void StopNIService();
int GetLimitedTime();

#endif // NISERVER_H
