#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

// #define wifi_ssid "aio-utility" //for production
// #define wifi_password "OTSuk@123" 

#define wifi_ssid "V2030" //for testing
#define wifi_password "alanjanuari"

#define mqtt_server "demo.thingsboard.io" //set host
#define mqtt_user "ZGuQ0mfS3LxwMAi0UPht" //set token
#define mqtt_password "" //empty if demo

#define ANALOG_IN1_PIN 34
#define ANALOG_IN2_PIN 35     // some analog input sensor ref: https://lastminuteengineers.com/voltage-sensor-arduino-tutorial/

float adc_voltage1, in_voltage1;
float adc_voltage2, in_voltage2;
float R1 = 30000.0; //value resistor
float R2 = 7500.0;
float ref_voltage = 5.0; //voltage reference
int kirim = 0; // counter for msg send
int adc_value1 = 0,adc_value2 = 0; //read adc value through sensor


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

void readVoltage() {
  adc_value1 = analogRead(ANALOG_IN1_PIN);
  adc_value2 = analogRead(ANALOG_IN2_PIN);
  adc_voltage1 = (adc_value1*ref_voltage) / 4096;
  adc_voltage2 = (adc_value2*ref_voltage) / 4096;
  in_voltage1 = adc_voltage1*(R1+R2)/R2;
  in_voltage2 = adc_voltage2*(R1+R2)/R2; //0.034 is speed of sound wave
  Serial.print("Input Voltage1: ");
  Serial.println(in_voltage1, 2);
  Serial.print("Input Voltage2: ");
  Serial.println(in_voltage2, 2);
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
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
  String data = "{\"voltage1\":\"" + String (in_voltage1) + "\",\"voltage2\":\"" + String (in_voltage2) + "\"}";
  // String data = "{\"humidity\":\"" + String(data1) + "\",\"temperature\":\"" + String(data2) + "\",\"rainfall\":\"" + String(data3) + "\"}";
  client.publish("v1/devices/me/telemetry", data.c_str());
  kirim = kirim + 1;
  Serial.print("Pengiriman Berhasil ");
  Serial.println(kirim);
  Serial.print("ADC value1: ");
  Serial.println(adc_value1);
  Serial.print("ADC value2: ");
  Serial.println(adc_value2);

  client.loop();
  delay(2000);
}