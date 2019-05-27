#include "CredentialManager.h"
#include "ESP8266WiFiMulti.h"

#include "FS.h"
#include "string.h"

#ifndef FILE_APPEND
#define FILE_APPEND "a"
#endif

#ifndef FILE_READ
#define FILE_READ "r"
#endif

#define CREDENTIAL_PATH "/credentials"

ESP8266WiFiMulti wifiMulti;

CredentialManager::CredentialManager()
{
}

CredentialManager::~CredentialManager()
{
}

bool CredentialManager::ClearCredentialMemory()
{
    Serial.println("[ClearCredentialMemory]\n");
    SPIFFS.begin();
    return SPIFFS.remove(CREDENTIAL_PATH);
}

bool CredentialManager::AddCredential(const char *ssid, const char *passphrase)
{

    Serial.print("[AddCredential] SSID: ");
    Serial.print(ssid);
    Serial.print(",Passphrase: ");
    Serial.println(passphrase);

    if (!FileRead)
    {
        ReadCredentialsFromMemory();
        FileRead = true;
    }

    if (!wifiMulti.existsAP(ssid, passphrase) && wifiMulti.addAP(ssid, passphrase))
    {
        Serial.print("Added to wifiMulti, writing to memory...\n");
        WriteCredentialToMemory(ssid, passphrase);
        return true;
    }
    else
    {
        Serial.print("Already exists, dont add to memory");
        return false;
    }
}

///Reads Credentials from memory, adds them to the wifiMulti object
void CredentialManager::ReadCredentialsFromMemory(void)
{
    Serial.print("[ReadCredentialsFromMemory]\n");

    SPIFFS.begin();

    File f = SPIFFS.open(CREDENTIAL_PATH, FILE_READ);

    if (!f)
        Serial.print("File doesn't exist, SPIFFS Not enabled or no SPIFFS.\n");
    else
    {
        Serial.print("File exists, start reading...\n");
        uint32_t index = 0;

        const char *_ssid;
        const char *_passphrase;
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
                _passphrase = s.c_str();
                Serial.println(s);

                if (!wifiMulti.existsAP(_ssid, _passphrase) && wifiMulti.addAP(_ssid, _passphrase))
                {
                    Serial.print("[Success]: Credential SSID: ");
                    Serial.print(_ssid);
                    Serial.print(", Password: ");
                    Serial.print(_passphrase);
                    Serial.print(", Added to WifiMulti\n");
                }
                else
                {
                    Serial.print("[FAILED]: Credential SSID: ");
                    Serial.print(_ssid);
                    Serial.print(", Password: ");
                    Serial.print(_passphrase);
                    Serial.print(", NOT Added to WifiMulti\n");
                }
            }

            index++;
        }
    }

    SPIFFS.end();
}

bool CredentialManager::WriteCredentialToMemory(const char *ssid, const char *passphrase)
{
    Serial.print("[WriteCredentialToMemory] @ssid: ");
    Serial.print(ssid);
    Serial.print(",@passphrase: ");
    Serial.println(passphrase);
    SPIFFS.begin();

    File f = SPIFFS.open(CREDENTIAL_PATH, FILE_APPEND);
    Serial.println(ssid);
    f.println(ssid);
    Serial.println(passphrase);
    f.println(passphrase);

    SPIFFS.end();

    return true;
}
