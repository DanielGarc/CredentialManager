#ifndef CredentialManager_H
#define CredentialManager_H

class CredentialManager
{

public:
    CredentialManager();

    ~CredentialManager();

    bool AddCredential(const char *ssid, const char *passphrase);

    bool ClearCredentialMemory();

private:
    bool FileRead = false;

    void ReadCredentialsFromMemory(void);

    bool addCredentialtoMemory(const char *ssid, const char *passphrase);

    bool WriteCredentialToMemory(const char *ssid, const char *passphrase);
};

#endif
