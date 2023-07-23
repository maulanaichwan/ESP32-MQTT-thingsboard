// #include <Arduino.h>
// #include <WiFi.h>
// #include <ThingsBoard.h>

// #define TRIG_PIN 14
// #define ECHO_PIN 27

// //password wifi
// String wifiSSID = "V2030";
// String wifiPassword = "alanjanuari";

// //thingsboard
// String tbHost = "demo.thingsboard.io";
// String tbToken = "d0tcn3khlm4bb2dqa38n";

// WiFiClient client;
// ThingsBoard tb(client);
// PubSubClient mqtt(client);

// // put function declarations here:
// void connectWifi();
// void tbReconnect();
// int duration;
// int distance;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9600);
//   connectWifi();
//   Serial.println("tb connected");
//   Serial.println(tb.connected());
//   Serial.println("Wifi Status");
//   Serial.println(WiFi.status());
//   Serial.println("WL connected");
//   Serial.println(WL_CONNECTED);
//   pinMode(LED_BUILTIN, OUTPUT);
//   pinMode(TRIG_PIN, OUTPUT);
//   pinMode(ECHO_PIN, INPUT);
// }

// void loop() {
//   if(tb.connected() != WL_CONNECTED) {
//     tbReconnect();
//   }
//   delay(1000);
//   digitalWrite(TRIG_PIN, LOW);
//   delayMicroseconds(5);
//   digitalWrite(TRIG_PIN, HIGH);
//   delayMicroseconds(8);
//   digitalWrite(TRIG_PIN, LOW);
//   duration = pulseIn(ECHO_PIN, HIGH);
//   distance = duration*0.034/2;
//   delay(100);

//   Serial.println(distance);
//   Serial.println("sending thingsboard");
//   tb.sendTelemetryFloat("distance", distance);
//   tb.loop();
//   delay(1000);
//   // put your main code here, to run repeatedly:
// }

// void connectWifi() {
//   Serial.println("Connecting to wifi...");
//   WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.println(".");
//     delay(5000);
//   }
//   Serial.println("Wifi Connected");
//   Serial.println(WiFi.SSID());
//   Serial.println(WiFi.RSSI());
//   Serial.println(WiFi.localIP());
//   Serial.println(WiFi.gatewayIP());
// }

// void tbReconnect() {
//   while(tb.connected() != WL_CONNECTED) {
//     if(WiFi.status() != WL_CONNECTED) {
//       connectWifi();
//     }
//     Serial.println("connecting to thingsboard");
//     if(tb.connect(tbHost.c_str(), tbToken.c_str())) {
//       Serial.println("thingsboard connected");
//     } else {
//       Serial.println("thingsboard connection failed");
//       Serial.println("Retrying in 5 seconds");
//       delay(5000); 
//     }
//   }
// }

