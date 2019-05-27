#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"

#include "CredentialManager.h"
#include "FS.h"

CredentialManager wifiMultiSPIFFS;

String s_SerialMessage;
uint32_t i_index = 0;

const char *l_ssid;
const char *l_passphrase;

void setup(void)
{
    Serial.begin(115200);

    Serial.println("loaded\n");

    Serial.println(millis());
}

void loop()
{

    if (Serial.available())
    {
        s_SerialMessage = Serial.readString();
        if (s_SerialMessage == "delete" && wifiMultiSPIFFS.ClearCredentialMemory())
        {
            Serial.println("File Removed");
        }
        else if (s_SerialMessage == "read")
        {
            if (SPIFFS.begin())
                Serial.print("SPIFFS Started");

            File f = SPIFFS.open("/credentials", "r");

            if (!f)
                Serial.println("file open failed");

            int i = 0;
            while (f.available())
            { /* code */

                String s = f.readStringUntil('\n');
                s.trim();

                Serial.print(i);
                Serial.print(":");
                Serial.println(s);
                i++;
            }

            SPIFFS.end();
        }
        else if (s_SerialMessage)
        {
            Serial.println(i_index);
            if (i_index % 2 == 0)
            {
                l_ssid = s_SerialMessage.c_str();
                Serial.println(l_ssid);
            }
            else
            {
                l_passphrase = s_SerialMessage.c_str();
                Serial.print("Serial message: ");
                Serial.print(l_ssid);
                Serial.print(", ");
                Serial.println(l_passphrase);
                Serial.println();
                wifiMultiSPIFFS.AddCredential(l_ssid, l_passphrase);
            }
            i_index++;
        }
    }
    delay(1 * 1000);
}
