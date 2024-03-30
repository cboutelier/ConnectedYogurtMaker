#include <Arduino.h>
#include <StateMachine.h>
#include <ESP8266WiFi.h> //Import ESP 8266 WiFi library
#include <WiFiManager.h>

#include <PubSubClient.h>

// #include <DNSServer.h>
// #include <ESP8266WebServer.h>

#include <Adafruit_NeoPixel.h>
#include "OneButton.h"
#include "mqtt_manager.h"

#define NUM_LEDS 1

#define PIN D7
#define NUMPIXELS 1

#define BUTTON_PIN D5

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long processStartTime = 0;

bool connected = false;
bool startPressed = false;
StateMachine machine = StateMachine();

OneButton button(BUTTON_PIN, true);

WiFiClient espClient;
PubSubClient client(espClient);
MqttManager *mqtt;

bool redState = false;

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void initAction()
{

  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  wifiManager.autoConnect("Yogurt maker");
  // if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  mqtt = new MqttManager(&client);
  connected = true;
}
void checkConnected();

void greenLed()
{
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));

  pixels.show();
  delay(1000);
}

void whiteLed()
{
  pixels.setPixelColor(0, pixels.Color(255, 255, 255));

  pixels.show();
}
void blueLed()
{
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  pixels.show();
}
void redLed()
{
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();
}

State *initialState = machine.addState([]()
                                       {
  if(machine.executeOnce){
    Serial.println("Initial state");
    Serial.println("Execute Once");
    whiteLed();
    initAction();
    
  } });

State *connectedState = machine.addState([]()
                                         {
  if(machine.executeOnce){
    Serial.println("Connected state");
    Serial.println("Execute Once the connected state");
    //digitalWrite(LED,!digitalRead(LED));
    blueLed();
  } });

State *waitingState = machine.addState([]()
                                       {
  if(machine.executeOnce)
  {
    Serial.println("Waiting state");
    Serial.println("Execute Once the waiting state");
    button.attachClick( [](){  
      Serial.println("Clicked");
      startPressed = true;
    });
    greenLed();
    //digitalWrite(LED,!digitalRead(LED));
  } 
   button.tick(); });

State *runningState = machine.addState([]()
                                       {
if( machine.executeOnce){
  redLed();
  redState = true;
  
  mqtt->SendStatus("launched!");
  
} });

void test()
{
}

void setup()
{

  Serial.begin(9600);
  pixels.begin();
  pixels.setBrightness(50);

  // Adding transitions
  initialState->addTransition([]()
                              { return connected; },
                              connectedState);

  connectedState->addTransition([]()
                                {
                                delay( 5000);
                                return true; },
                                waitingState);
  waitingState->addTransition([]()
                              {
    if( startPressed)
    {
      processStartTime = millis();
      startPressed = false;
      return true;
    }
    return false; },
                              runningState);

  runningState->addTransition([]()
                              {
    unsigned long currentTime = millis();
    unsigned long deltaTime = currentTime-processStartTime;
    if( deltaTime > 1000*60*2){
      return true;
    }
    return false; },
                              waitingState);

  pinMode(D5, INPUT_PULLUP);
  delay(1000);

}

void loop()
{
  machine.run();
  if (redState)
  {
    redState = false;
    Serial.println("end");
  }

  client.loop();
  /*
    unsigned long now = millis();
    if (now - lastMsg > 2000)
    {
      lastMsg = now;
      ++value;
      snprintf(msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("outTopic", msg);
    }
    */
}
