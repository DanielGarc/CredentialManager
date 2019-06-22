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

#include "ESP8266WiFi.h"

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
    bool AddCredential(const char *ssid, const char *password);

    bool ExistWifiCredential(const char *ssid, const char *password);

    bool ClearCredentialMemory(void);

    wl_status_t Connect(void);

private:
    bool fileRead = false;

    WifiCredentials wifiCredentials;

    Networks prevUsedNetworks;

    bool _ExistWifiCredential(const char *ssid, const char *password);

    bool _AddCredential(const char *ssid, const char *password);

    bool ReadCredentialsFromMemory(void);

    bool _ReadCredentialsFromMemory(void);

    int AddCredentialToList(const char *ssid, const char *password);

    int _AddCredentialToList(const char *ssid, const char *password);

    bool WriteCredentialToMemory(const char *ssid, const char *password);

    bool AddNetworkToList(const char *ssid);

    bool _AddNetworToList(const char *ssid);

    bool ReadNetworksFromMemory(const char *ssid);

    bool _ReadNetworksFromMemory(const char *ssid);

    bool WriteNetworkToMemory(const char *ssid);

    bool _WriteNetworkToMemory(const char *ssid);

    wl_status_t _Connect(void);
};

#endif
