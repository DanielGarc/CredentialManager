#ifndef CredentialManager_H
#define CredentialManager_H

#include "ESP8266WiFiMulti.h"

class CredentialManager
{

public:
    CredentialManager();

    ~CredentialManager();

    bool AddCredential(const char *ssid, const char *passphrase);

    bool AddCredential(void);

    bool ClearCredentialMemory(void);

    wl_status_t run(void);

private:
    bool FileRead = false;

    bool ReadCredentialsFromMemory(void);

    bool addCredentialtoMemory(const char *ssid, const char *passphrase);

    bool WriteCredentialToMemory(const char *ssid, const char *passphrase);
};

#endif
