
#include "ESP8266WiFi.h"
#include "CredentialManager.h"

#define SCAN_NETWORKS_RETRIES 2

const char *ssid = "YOUR SSID";
const char *password = "YOUR PASSWORD";

bool ConnectWithStoredCredentials();
bool WiFiConnect(const char *ssid, const char *passphrase);

CredentialManager cmanager;

void setup(void)
{
    Serial.begin(115200);
    delay(10);

    //Saves credential in memory,
    //this can be deleted after the first run
    cmanager.AddCredential(ssid, password);

    //no need for this
    delay(5 * 1000);

    //connect with stored credentials
    if (ConnectWithStoredCredentials())
    {
        Serial.print("Connected. Local IP:  ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println("Not Connected, check credentials");
    }

    Serial.print("...done...");
}

void loop()
{
}

/* *
 * Tries to connecto to a wifi network using wifi credentials stored in memory
 *
 * Loops through the discovered networks and tries to find a matching ssid
 * */
bool ConnectWithStoredCredentials()
{
    const char *ssid;
    const char *password;
    CredentialManager cmanager;

    if (cmanager.begin() > 0) //check if there is more than 1 stored network
    {
        int retry = 0;

        while (retry < SCAN_NETWORKS_RETRIES)
        {
            Serial.printf("Retry %d \n", retry);
            Serial.println("Looking for networks");
            int networksCount = WiFi.scanNetworks(false);
            Serial.printf("%d\n", networksCount);

            if (networksCount >= 0)
            {
                for (int i = 0; i < networksCount; i++)
                {
                    ssid = strdup(WiFi.SSID(i).c_str());
                    Serial.printf("ssid: %s \n", ssid);

                    if (*ssid != 0x00 && ssid && cmanager.GetPassword(ssid, password))
                    {
                        Serial.println("Trying to connect");

                        if (WiFiConnect(ssid, password))
                        {
                            Serial.print("Connected to WiFi network with ssid from memory");
                            WiFi.scanDelete();
                            return true;
                        }
                    }
                }
            }
            retry++;
            delay(250);
        }
    }
    return false;
}

/* *
 * tries to connect with a given ssid and password
 * */
bool WiFiConnect(const char *ssid, const char *passphrase)
{
    WiFi.begin(ssid, passphrase); //WiFi connection

    while (WiFi.status() != WL_CONNECTED && WiFi.status() != WL_CONNECT_FAILED)
    {
        Serial.print(".");
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED)
        return true;
    return false;
}
