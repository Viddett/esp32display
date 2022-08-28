#include <WiFiManager.hpp>

WiFiUDP _ntpUDP;
NTPClient _timeClient(_ntpUDP);

void WiFiManager::setup()
{
    // Connect to WiFi
    WiFi.begin(_ssid.c_str(), _pw.c_str());
    Serial.print("WiFi: connecting to ");
    Serial.print(_ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.print("Connection successful, IP: ");
    Serial.println(WiFi.localIP());

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