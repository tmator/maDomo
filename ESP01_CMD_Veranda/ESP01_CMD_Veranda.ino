#include <ESP8266WiFi.h>
//http://192.168.1.19/cm?cmnd=Power%20TOGGLE
const char* ssid     = "";
const char* password = "";
const char* host = "192.168.1.19";

int poussoir = 2;
int led = 1;

void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(poussoir, INPUT);
  pinMode(poussoir, OUTPUT);
  digitalWrite(led, HIGH);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());



}

void loop() {

  delay(1000);
  if (digitalRead(poussoir))
  {
    Serial.println("push");
    digitalWrite(led, HIGH);
  

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

        // We now create a URI for the request
        //TODO
        String url = "/cm?cmnd=Power%20TOGGLE";
        Serial.print("Requesting URL: ");
        Serial.println(url);

        // This will send the request to the server
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n\r\n");
        unsigned long timeout = millis();
        while (client.available() == 0) {
          if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
          }
        }

        // Read all the lines of the reply from server and print them to Serial
        while(client.available()){
          String line = client.readStringUntil('\n');
          Serial.println(line);
          if(line.substring(0,7) == "Success")
          {
            Serial.write("Y");
          }else if(line.substring(0,6) == "Failed")
          {
            Serial.write("N");
          }else if(line.substring(0,4)=="User")
          {
            Serial.write("O");
          }

        Serial.println();
        Serial.println("closing connection");

 }
  }
}
