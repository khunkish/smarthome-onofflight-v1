#include <ESP8266WiFi.h>  // สำหรับ ESP8266
// #include <WiFi.h>      // สำหรับ ESP32

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

WiFiServer server(80);

const int relayPin = 5;  // GPIO ที่เชื่อมต่อกับ Relay

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  // เชื่อมต่อ Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // เริ่มต้นเซิร์ฟเวอร์
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client");
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    if (request.indexOf("/ON") != -1) {
      digitalWrite(relayPin, HIGH);
    }
    if (request.indexOf("/OFF") != -1) {
      digitalWrite(relayPin, LOW);
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<h1>Smart Home Control</h1>");
    client.println("<p><a href=\"/ON\"><button>ON</button></a></p>");
    client.println("<p><a href=\"/OFF\"><button>OFF</button></a></p>");
    client.println("</html>");

    delay(1);
    Serial.println("Client disconnected");
    Serial.println("");
  }
}
