#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SHT31.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Settings.h>

void initializationWiFi();
void initializationSHT31();
void initializationSerial();
String createJSON(float temperature, float humidity, String deviceId);
int sendRequest(float temperature, float humidity);
void printTemperatureAndHumidity(float temperature, float humidity);

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup()
{
  initializationSerial();
  initializationSHT31();
  initializationWiFi();
}

void loop()
{
  float temperature = sht31.readTemperature();
  float humidity = sht31.readHumidity();

  printTemperatureAndHumidity(temperature, humidity);

  int httpCode = sendRequest(temperature, humidity);
  Serial.print("is sent data? ");
  Serial.println(httpCode);

  delay(DELAY);
}

String createJSON(float temperature, float humidity, String deviceId)
{
  DynamicJsonDocument doc(140);

  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["deviceId"] = deviceId;

  String json;
  serializeJson(doc, json);

  return json;
}

int sendRequest(float temperature, float humidity)
{
  HTTPClient http;
  http.begin(URL_ADDRESS, URL_HTTPS_FINGERPRINT);
  http.addHeader("Content-type", "application/json");
  http.addHeader("Accept", "text/plain");

  int httpCode = http.POST(createJSON(temperature, humidity, DEVICE_ID));
  http.end();

  return httpCode;
}

void printTemperatureAndHumidity(float temperature, float humidity)
{
  if (!isnan(temperature))
  {
    Serial.print("Temp *C = ");
    Serial.print(temperature);
    Serial.print("\t\t");
  }
  else
    Serial.println("Failed to read temperature");

  if (!isnan(humidity))
  {
    Serial.print("Hum. % = ");
    Serial.println(humidity);
  }
  else
    Serial.println("Failed to read humidity");
}

void initializationSHT31()
{
  Serial.println("Initialize connection to SHT31");

  if (!sht31.begin())
  {
    Serial.println("Couldn't find SHT31");
    while (1)
      delay(1);
  }

  Serial.println("Heater Enabled State: DISABLED");
  sht31.heater(false);
}

void initializationWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void initializationSerial()
{
  Serial.begin(9600);

  while (!Serial)
    delay(10);
}