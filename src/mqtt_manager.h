#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include <PubSubClient.h>

class MqttManager
{
public:
    MqttManager(PubSubClient *client);
    ~MqttManager();
    void SendStatus(String step);

    bool IsSet() { return isSet; }
    void FirstInit();

private:
    String host;
    String channel;
    String username;
    String password;
    Preferences *prefs;

    PubSubClient *mqttClient;

    void Reconnect();

    bool isSet = false;
};