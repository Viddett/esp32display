#ifndef WIFIMANAGER_HPP
#define WIFIMANAGER_HPP

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <string.h>
#include <LocalDefines.h>


class WiFiManager {
    public:
    void setup();
    void poll();
    String getCurrentTime();

    private:    

    String _ssid { WIFI_SSID };
    String _pw { WIFI_PW };
};


#endif