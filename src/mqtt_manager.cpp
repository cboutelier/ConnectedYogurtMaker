#include <mqtt_manager.h>
#include <ESP8266WiFi.h> 

MqttManager::MqttManager(PubSubClient *client)
{
    prefs = new Preferences();

    prefs->begin("mqtt", true);
    this->host = prefs->getString("host", "notset");
   // this->host = "notset";
    this->channel = prefs->getString("channel");
    this->username = prefs->getString("username");
    this->password = prefs->getString("password");
    prefs->end();
    delete (prefs);

    Serial.println("Mqttmanager constructor");
    Serial.println(this->host);
    Serial.println(client->state());
    Serial.println(WiFi.localIP());
    this->mqttClient = client;

    if (!this->host.equals("notset"))
    {
        isSet = true;
        this->mqttClient = client;
        this->mqttClient->setServer(this->host.c_str(), 1883);
    }
}

MqttManager::~MqttManager()
{
    if (prefs != nullptr)
    {
        delete (prefs);
    }
}

void MqttManager::SendStatus(String step)
{
    // IPAddress server(192, 168, 1, 28);
    /*  bool connected = this->mqttClient->connect("42", this->username.c_str(), this->password.c_str());
      Serial.print("Connected ");
      Serial.println(connected);
      Serial.print("Publish message: ");
      Serial.println("hello");
      this->mqttClient->publish(this->channel.c_str(), "hello");
      */
    if (!isSet)
    {
        Serial.println("Send status, not set");
        this->FirstInit();
    }

    if (!this->mqttClient->connected())
    {
        Serial.println("Not connected");
        Reconnect();
         //this->mqttClient->publish(this->channel.c_str(), step.c_str());
Serial.println("Sending a step after reconnect");
         this->mqttClient->publish("yogurt/step", step.c_str());
    }else{
        Serial.println("Sending a step");
        this->mqttClient->publish("yogurt/step", step.c_str());
    }
}

void MqttManager::Reconnect()
{
    // Loop until we're reconnected
    while (!this->mqttClient->connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);

        Serial.print("host ");
        Serial.println(this->host.c_str());
        // Attempt to connect
        if (this->mqttClient->connect(clientId.c_str(), this->username.c_str(), this->password.c_str()))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
           
            // ... and resubscribe
            //  client.subscribe("inTopic");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(this->mqttClient->state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void MqttManager::FirstInit()
{
    //jeedom:sAhR5RQN3eQmR90rP0EEgcJfMDgYJWRO465fEXub6NWjc9XmsAEyqpvp4ZvjpZd7
    this->host = "192.168.1.189";
    this->channel = "yogurt";
    this->username = "jeedom";
    this->password = "KvxDsutLk8siF0eUhrseLkBRYcChd6henAS4tlVQnu6Aj89EOmmdJsxLDBS4PWcX";
    prefs = new Preferences();
    prefs->begin("mqtt", false);
    prefs->putString("host", this->host);
    prefs->putString("channel", this->channel);
    prefs->putString("username", this->username);
    prefs->putString("password", this->password);

    prefs->end();
    isSet = true;

    this->mqttClient->setServer(this->host.c_str(), 1883);
}