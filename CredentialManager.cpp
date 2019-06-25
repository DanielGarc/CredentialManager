/**
 * @file CredentialManager.cpp
 * @author Daniel Garcia (danielgarc22@gmail.con)
 * @brief
 * @version 0.1
 * @date 2019-06-21
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "CredentialManager.h"

#include "FS.h"
#include "string.h"

#ifndef FILE_APPEND
#define FILE_APPEND "a"
#endif

#ifndef FILE_READ
#define FILE_READ "r"
#endif

#define CREDENTIAL_PATH "/credentials"

CredentialManager::CredentialManager()
{
}

CredentialManager::~CredentialManager()
{
}

bool CredentialManager::ClearCredentialMemory()
{
    bool done = false;
    Serial.println("[ClearCredentialMemory]\n");
    SPIFFS.begin();
    done = SPIFFS.remove(CREDENTIAL_PATH);
    SPIFFS.end();
    return done;
}

bool CredentialManager::AddCredential(const char *ssid, const char *password)
{
    return _AddCredential(ssid, password);
}

//Reads Credentials from memory, adds them to the wifiMulti object
//returns true if at least one credential was added from memory
/*
    SSID and PASSWORD are saved in CREDENTIAL_PATH
    SSID is saved first then the password, each in a different line \n

 */
bool CredentialManager::ReadCredentialsFromMemory(void)
{
    return _ReadCredentialsFromMemory();
}

bool CredentialManager::WriteCredentialToMemory(const char *ssid, const char *password)
{
    Serial.print("[WriteCredentialToMemory] @ssid: ");
    Serial.print(ssid);
    Serial.print(",@password: ");
    Serial.println(password);

    SPIFFS.begin();
    File f = SPIFFS.open(CREDENTIAL_PATH, FILE_APPEND);
    Serial.println(ssid);
    f.println(ssid);
    Serial.println(password);
    f.println(password);
    SPIFFS.end();

    return true;
}

bool CredentialManager::ExistWifiCredential(const char *ssid, const char *password)
{
    return _ExistWifiCredential(ssid, password);
}

int CredentialManager::AddCredentialToList(const char *ssid, const char *password)
{
    return _AddCredentialToList(ssid, password);
}

const char *CredentialManager::GetPassword(const char *ssid)
{
    return _GetPassword(ssid);
}

bool CredentialManager::_ReadCredentialsFromMemory(void)
{
    bool credentialAdded = false;
    fileRead = true;
    Serial.print("[ReadCredentialsFromMemory]\n");

    SPIFFS.begin();

    File f = SPIFFS.open(CREDENTIAL_PATH, FILE_READ);

    if (!f)
    {
        Serial.print("File doesn't exist, SPIFFS Not enabled or no SPIFFS.\n");
    }
    else
    {
        Serial.print("File exists, start reading...\n");
        uint32_t index = 0;

        const char *_ssid;
        const char *_password;
        String s;

        while (f.available())
        {

            if (index % 2 == 0)
            {
                s = f.readStringUntil('\n');
                s.trim();
                _ssid = s.c_str();
                Serial.println(s);
            }
            else
            {
                s = f.readStringUntil('\n').c_str();
                s.trim();
                _password = s.c_str();
                Serial.println(s);

                int rslt = AddCredentialToList(_ssid, _password);

                if (rslt == 0 || rslt == 1)
                {
                    credentialAdded = true;
                    Serial.print("[Success]: Credential SSID: ");
                    Serial.print(_ssid);
                    Serial.print(", Password: ");
                    Serial.print(_password);
                }
                else
                {
                    Serial.print("[FAILED]: Credential SSID: ");
                    Serial.print(_ssid);
                    Serial.print(", Password: ");
                    Serial.print(_password);
                }
            }

            index++;
        }
    }
    SPIFFS.end();

    return credentialAdded;
}

bool CredentialManager::_ExistWifiCredential(const char *ssid, const char *password)
{
    for (auto entry : wifiCredentials)
    {
        if (!strcmp(entry.ssid, ssid))
        {
            if (!password)
            {
                if (!strcmp(entry.password, ""))
                {
                    return true;
                }
            }
            else
            {
                if (!strcmp(entry.password, password))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

/**
 * @brief Tries to add a new credential to the credential list
 *
 * @return int 0 = credential added, 1 = already exists, 2 = ssid too long or empty, 3 = password too long
 */

bool CredentialManager::_AddCredential(const char *ssid, const char *password)
{
    Serial.print("[AddCredential]: @ssid: ");
    Serial.print(ssid);
    Serial.print(",@password: ");
    Serial.println(password);

    if (!fileRead)
    {
        ReadCredentialsFromMemory();
    }

    if (AddCredentialToList(ssid, password) == 0)
    {
        WriteCredentialToMemory(ssid, password);
        return true;
    }
    else
    {
        return false;
    }
}

int CredentialManager::_AddCredentialToList(const char *ssid, const char *password)
{
    WifiCredential newCredential;

    //No ssid or longer than 31 characters
    if (!ssid || *ssid == 0x00 || strlen(ssid) > 32)
    {
        return 2;
    }

    //Password longer than 63 characters
    if (password && strlen(password) > 64)
    {
        return 3;
    }

    if (ExistWifiCredential(ssid, password))
    {
        return 1;
    }

    newCredential.ssid = strdup(ssid);

    if (password)
    {
        newCredential.password = strdup(password);
    }
    else
    {
        newCredential.password = strdup("");
    }

    wifiCredentials.push_back(newCredential);
    return 0;
}

const char *CredentialManager::_GetPassword(const char *ssid)
{
    for (auto entry : wifiCredentials)
    {
        if (!strcmp(entry.ssid, ssid))
        {
            return entry.password;
        }
    }

    return "";
}