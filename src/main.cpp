#include <ESP8266WiFi.h>

const char* WIFI_SSID = "Yettel_EAC69C";
const char* WIFI_PASS = "4ThtCzfS";

WiFiServer server(23);
WiFiClient client;

static const uint32_t UART_BAUD = 9600;

// Static IP konfiguracija
IPAddress local_IP(192, 168, 1, 66);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);  // može i gateway umjesto ovoga

void setup() {
  Serial.begin(UART_BAUD);
  Serial.setTimeout(5);

  WiFi.mode(WIFI_STA);

  // Postavi statičku IP prije WiFi.begin()
  if (!WiFi.config(local_IP, gateway, subnet, dns)) {
    Serial.println("Static IP config failed");
  }

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    yield();
  }

  server.begin();
  server.setNoDelay(true);
}

void loop() {
  if (!client || !client.connected()) {
    WiFiClient newClient = server.accept();
    if (newClient) {
      if (client) client.stop();
      client = newClient;
      client.setNoDelay(true);
    }
  }

  if (client && client.connected()) {
    while (client.available()) {
      int c = client.read();
      if (c >= 0) Serial.write((uint8_t)c);
    }
    while (Serial.available()) {
      int c = Serial.read();
      if (c >= 0) client.write((uint8_t)c);
    }
  }

  yield();
}