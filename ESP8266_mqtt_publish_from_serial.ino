//The first character recived will be lost.

#include <ESP8266WiFi.h>
#include <MQTTClient.h>

const char *ssid = "ssid";
const char *pass = "password";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect(); // <- predefine connect() for setup()

void setup() {
  Serial.begin(4800);
  WiFi.begin(ssid, pass);
  client.begin("mqttbrokerip", net);

  connect();
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "try", "try")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/example");
  // client.unsubscribe("/example");
}

void loop() {
  client.loop();
  delay(10);
  String content = "";
  char character;
  String text = "";
  if (!client.connected()) {
    connect();
  }
  character = Serial.read();
  if (Serial.available() > 0) {
      content = Serial.readString();
    client.publish("hello", String(content));
    } 
}


void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
}
