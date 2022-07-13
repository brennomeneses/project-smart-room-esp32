/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-temperature-humidity-sensor
 */


#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "mywifiSSID";
const char* password = "myWiFiPassword";

// API Server URL
String serverName = "https://yourApiHosted.com/";
String jwtToken = "here comes your token";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

//--------------------
#include <DHT.h>
#define DHT_SENSOR_PIN  33 // ESP32 pin GIOP21 connected to DHT11 sensor
#define DHT_SENSOR_TYPE DHT11

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

void setup() {
  Serial.begin(9600);
  dht_sensor.begin(); // initialize the DHT sensor

  WiFi.begin(ssid, password); // initialize the wifi sensor
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  
}

void loop() {
  // read humidity
  float humi  = dht_sensor.readHumidity();
  // read temperature in Celsius
  float tempC = dht_sensor.readTemperature();
  // read temperature in Fahrenheit
  float tempF = dht_sensor.readTemperature(true);

  // check whether the reading is successful or not
  if ( isnan(tempC) || isnan(tempF) || isnan(humi)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    WiFiClient client;
    HTTPClient http;
      
    http.begin(serverName);
    String stringHumi = String(humi, 2);
    String stringTempC = String(tempC, 2);
    http.addHeader("Authorization", "Bearer "+jwtToken);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST("{\"temp\": \""+stringTempC+"\", \"umi\": \""+stringHumi+"\"}");
  }

  // wait a 30 seconds between readings (CHANGE FOR 5MIN WHEN GO TO PRODUCTION)
  delay(30000);
}