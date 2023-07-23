#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

#define wifi_ssid "KOSAN"
#define wifi_password "yanpri2023"

// #define mqtt_server "103.157.27.219"
// #define mqtt_user "agpot123"
// #define mqtt_password ""

#define mqtt_server "demo.thingsboard.io"
#define mqtt_user "sensor-ultrasonic-aio"
#define mqtt_password ""

float h, t, f;

int data1 = 24;
int data2 = 55;
int data3 = 0;
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
  // We start by connecting to a WiFi network
  Serial.println();
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


void setup() {
  Serial.begin(9600);
  setup_wifi();
  // client.setServer(mqtt_server, 1112);
  client.setServer(mqtt_server, 1883);

  client.setCallback(callback);

  delay(1500);
}

void loop() {


  // if (!client.connected()) {
  //   reconnect();
  // }


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

  String data = "{\"humidity\":\"" + String(data1) + "\",\"temperature\":\"" + String(data2) + "\",\"rainfall\":\"" + String(data3) + "\"}";
  client.publish("v1/devices/me/telemetry", data.c_str());
  kirim = kirim + 1;
  Serial.print("Pengiriman Berhasil ");
  Serial.println(kirim);

  client.loop();
  delay(2000);
}