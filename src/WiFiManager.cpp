#include <WiFiManager.hpp>
#include <Log.hpp>

WiFiUDP _ntpUDP;
NTPClient _timeClient(_ntpUDP);

void WiFiManager::setup()
{
    // Connect to WiFi
    WiFi.begin(_ssid.c_str(), _pw.c_str());
    Log() << "WiFi: connecting to " << _ssid;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Log() << ".";
    }
    Log() << "Connection successful, IP:" << WiFi.localIP();

    // Initialize NTP client
    _timeClient.begin();
    _timeClient.setTimeOffset(2 * 3600); // GMT+2
}

void WiFiManager::poll()
{
    // Update time
    _timeClient.forceUpdate();
}

String WiFiManager::getCurrentTime()
{
   return _timeClient.getFormattedTime();
}