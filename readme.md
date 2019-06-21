# CredentialManager for ESP8266

Stores **ssid,password** value pairs in flash memory.
Automatically connects to a network if credentials were previously stored.

# TODO

[ ] Remove ESP8266WifiMulti dependecy.
[x] Read networks from FLASH and store them in RAM.
[ ] Scan networks, try to connect to available networks, priority to > signal.
[ ] Store previously used networks, and try to match existing networks to that.
