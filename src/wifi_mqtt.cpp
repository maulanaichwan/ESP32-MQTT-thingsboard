#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

#define wifi_ssid "aio-utility"
#define wifi_password "OTSuk@123" 

#define mqtt_server "demo.thingsboard.io" //set host
#define mqtt_user "sensor-ultrasonic-aio" //set token
#define mqtt_password "" //empty if demo

#define ANALOG_IN_PIN 34     // some analog input sensor ref: https://lastminuteengineers.com/voltage-sensor-arduino-tutorial/

float adc_voltage, in_voltage;
float R1 = 30000.0; //value resistor
float R2 = 7500.0;
float ref_voltage = 5.0; //voltage reference
int kirim = 0; // counter for msg send
int adc_value = 0; //read adc value through sensor


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup_wifi() {
  delay(10);
  Serial.println(); // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_readVoltage() {
  adc_value = analogRead(ANALOG_IN_PIN);
}

void readVoltage() {
  adc_voltage = (adc_value*ref_voltage) / 1024;
  in_voltage = adc_voltage*(R1+R2)/R2;//0.034 is speed of sound wave
  Serial.print("Input Voltage: ");
  Serial.println(in_voltage, 2);
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  setup_readVoltage();
  client.setServer(mqtt_server, 1883); // client.setServer(mqtt_server, port)
  client.setCallback(callback);
  delay(1500);
}

void loop() {
  while (!client.connected()) {
    // Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Client Connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  readVoltage();
  String data = "{\"voltage\":\"" + String (in_voltage) + "\"}";
  // String data = "{\"humidity\":\"" + String(data1) + "\",\"temperature\":\"" + String(data2) + "\",\"rainfall\":\"" + String(data3) + "\"}";
  client.publish("v1/devices/me/telemetry", data.c_str());
  kirim = kirim + 1;
  Serial.print("Pengiriman Berhasil ");
  Serial.println(kirim);

  client.loop();
  delay(2000);
}