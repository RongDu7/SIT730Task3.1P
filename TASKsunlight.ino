#include <WiFiNINA.h>
#include <BH1750.h>
#include <Wire.h>

// Replace with your network credentials
const char* ssid     = "Optus_B8E72A";
const char* password = "corns39526ys";

// Replace with your IFTTT information
const String IFTTT_API_KEY = "b3RoZlEur7BVMMGubVBWSP";
const String IFTTT_EVENT_SUNLIGHT_RECEIVED = "sunlight_on";
const String IFTTT_EVENT_SUNLIGHT_STOPPED = "sunlight_stopped";

// Initialize the BH1750 sensor
BH1750 lightMeter;

WiFiClient client;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Initialize the light sensor
  lightMeter.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");
}

void loop() {
  static bool sunlightState = false; // false means sunlight stopped, true means received
  
  float lux = lightMeter.readLightLevel(); // Read light level in lux
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lux");

  // Check if sunlight is received
  if (lux > 1500 && !sunlightState) { 
    Serial.println("Sunlight Received");
    sendIFTTT(IFTTT_EVENT_SUNLIGHT_RECEIVED); 
    sunlightState = true; 
  }
  // Check if sunlight has stopped
  else if (lux <= 100 && sunlightState) {
    Serial.println("Sunlight Stopped");
    sendIFTTT(IFTTT_EVENT_SUNLIGHT_STOPPED); 
    sunlightState = false; 
  }
  
  delay(10000); // Check every 10 seconds
}

void sendIFTTT(String event) {
  if (client.connect("maker.ifttt.com", 80)) {
    String postData = "value1=Sunlight Event Triggered"; // You can send additional data
    client.print(String("POST /trigger/") + event + "/with/key/" + IFTTT_API_KEY + " HTTP/1.1\r\n" +
                 "Host: maker.ifttt.com\r\n" +
                 "Content-Type: application/x-www-form-urlencoded\r\n" +
                 "Content-Length: " + postData.length() + "\r\n\r\n" +
                 postData + "\r\n");
  }
}
