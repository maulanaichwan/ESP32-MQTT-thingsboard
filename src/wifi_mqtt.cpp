#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

#define wifi_ssid "aiocyber"
// #define wifi_password "" 

#define mqtt_server "demo.thingsboard.io" //set host
#define mqtt_user "sensor-ultrasonic-aio" //set token
#define mqtt_password "" //empty if demo

#define TRIG_PIN 14
#define ECHO_PIN 27

float duration, distance;
int kirim = 0;

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
  WiFi.begin(wifi_ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_readVOltage() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void readVoltage() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2); //clear trig
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW); //pulse set to 10ms
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration*0.034 / 2; //0.034 is speed of sound wave
  Serial.print("distance: ");
  Serial.println(distance);
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  setup_readVOltage();
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
  String data = "{\"distance\":\"" + String (distance) + "\"}";
  // String data = "{\"humidity\":\"" + String(data1) + "\",\"temperature\":\"" + String(data2) + "\",\"rainfall\":\"" + String(data3) + "\"}";
  client.publish("v1/devices/me/telemetry", data.c_str());
  kirim = kirim + 1;
  Serial.print("Pengiriman Berhasil ");
  Serial.println(kirim);

  client.loop();
  delay(2000);
}