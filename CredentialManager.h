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

#include <vector>
#include "WString.h"

struct WifiCredential
{
    char *ssid;
    char *password;
};

typedef std::vector<WifiCredential> WifiCredentials;

class CredentialManager
{

public:
    CredentialManager();

    ~CredentialManager();

    int begin();

    bool AddCredential(const char *ssid, const char *password);

    bool ExistWifiCredential(const char *ssid, const char *password);

    bool GetPassword(const char *ssid, const char *&password);

    bool ClearCredentialMemory(void);

private:
    bool fileRead = false;

    WifiCredentials wifiCredentials;

    int ReadCredentialsFromMemory(void);

    int AddCredentialToList(const char *ssid, const char *password);

    bool WriteCredentialToMemory(const char *ssid, const char *password);
};

#endif
