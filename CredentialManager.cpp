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
#include "WString.h"

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

int CredentialManager::begin()
{
    return ReadCredentialsFromMemory();
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

/**
 * @brief
 * @param const char *ssid  Provide an SSID
 * @param const char *&password Updates the password if there is a match
 * @return       True If there is a saved credential for the given ssid
 */
bool CredentialManager::GetPassword(const char *ssid, const char *&password)
{
    if (ssid && *ssid != 0x00)
    {
        for (auto entry : wifiCredentials)
        {
            if (!strcmp(entry.ssid, ssid))
            {
                password = strdup(entry.password);
                return true;
            }
        }
    }
    password = strdup("");
    return false;
}

int CredentialManager::ReadCredentialsFromMemory(void)
{
    int credentialsAdded = 0;
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

        char *_ssid;
        char *_password;
        String s;

        while (f.available())
        {

            if (index % 2 == 0)
            {
                s = f.readStringUntil('\n');
                s.trim();
                _ssid = strdup(s.c_str());
            }
            else
            {
                s = f.readStringUntil('\n');
                s.trim();
                _password = strdup(s.c_str());

                int rslt = AddCredentialToList(_ssid, _password);

                if (rslt == 0 || rslt == 1)
                {
                    Serial.print("[Success]: Credential SSID: ");
                    Serial.print(_ssid);
                    Serial.print(", Password: ");
                    Serial.println(_password);

                    credentialsAdded += 1;
                }
                else
                {
                    Serial.print("[FAILED]: Credential SSID: ");
                    Serial.print(_ssid);
                    Serial.print(", Password: ");
                    Serial.println(_password);
                }
            }

            index++;
        }
    }
    SPIFFS.end();

    return credentialsAdded;
}

bool CredentialManager::ExistWifiCredential(const char *ssid, const char *password)
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
bool CredentialManager::AddCredential(const char *ssid, const char *password)
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

int CredentialManager::AddCredentialToList(const char *ssid, const char *password)
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