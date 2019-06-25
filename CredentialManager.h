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

    const char *GetPassword(const char *ssid);

    bool ClearCredentialMemory(void);

private:
    bool fileRead = false;

    WifiCredentials wifiCredentials;

    bool _ExistWifiCredential(const char *ssid, const char *password);

    bool _AddCredential(const char *ssid, const char *password);

    bool ReadCredentialsFromMemory(void);

    bool _ReadCredentialsFromMemory(void);

    int AddCredentialToList(const char *ssid, const char *password);

    int _AddCredentialToList(const char *ssid, const char *password);

    bool WriteCredentialToMemory(const char *ssid, const char *password);

    const char *_GetPassword(const char *ssid);
};

#endif
