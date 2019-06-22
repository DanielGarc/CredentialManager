/**
 * @file CredentialManager.h
 * @author Daniel Garcia (danielgarc22@gmail.con)
 * @brief
 * @version 0.1
 * @date 2019-06-21
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef CredentialManager_H
#define CredentialManager_H

#include "ESP8266WiFiMulti.h"

#include <vector>

struct WifiCredential
{
    char *ssid;
    char *password;
};

struct NetworkSSID
{
    char *ssid;
};

typedef std::vector<WifiCredential> WifiCredentials;

typedef std::vector<NetworkSSID> Networks;

class CredentialManager
{

public:
    /**
     * @brief Constructor
     *
     */
    CredentialManager();

    ~CredentialManager();

    /**
     * @brief Some brief explanation
     *
     * @return bool True if credential added
     *
     */
    bool AddCredential(const char *ssid, const char *passphrase);

    bool ExistWifiCredential(const char *ssid, const char *passphrase);

    bool ClearCredentialMemory(void);

    wl_status_t run(void);

private:
    bool fileRead = false;

    WifiCredentials wifiCredentials;

    Networks prevUsedNetworks;

    bool ReadCredentialsFromMemory(void);

    int AddCredentialToList(const char *ssid, const char *passphrase);

    bool WriteCredentialToMemory(const char *ssid, const char *passphrase);
};

#endif
