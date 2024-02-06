#include <Arduino.h>
#include <StateMachine.h>
#include <ESP8266WiFi.h> //Import ESP 8266 WiFi library

// #include <DNSServer.h>
// #include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 1
#define DATA_PIN D7

#define PIN D7
#define NUMPIXELS 1 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

bool connected = false;
StateMachine machine = StateMachine();

void initAction()
{

  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  wifiManager.autoConnect("Yogurt maker");
  // if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
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

State *initialState = machine.addState([]()
                                       {
  if(machine.executeOnce){
    Serial.println("Initial state");
    Serial.println("Execute Once");
    whiteLed();
    initAction();
    //digitalWrite(LED,!digitalRead(LED));
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
  if(machine.executeOnce){
    Serial.println("Waiting state");
    Serial.println("Execute Once the waiting state");
    greenLed();
    //digitalWrite(LED,!digitalRead(LED));
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

  delay(1000);
}

void loop()
{
  machine.run();
  delay(1000);
}
