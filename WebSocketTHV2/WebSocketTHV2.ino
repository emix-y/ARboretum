#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <DHT.h>

const char* ssid = "HUAWEI-E5330-9920";
const char* password = "14et72by";


using namespace websockets;
WebsocketsClient client;
WebsocketsServer server;

#define DHTPIN 14          // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11     // DHT sensor type
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

void onMessageCallback(WebsocketsMessage message) {
  Serial.println("Got Message: " + message.data());
}

void onEventCallback(WebsocketsEvent event, WebsocketsClient client) {
  if (event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("New client connected");
  }
  else if (event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Client disconnected");
  }
}

void setup() {

  client = server.accept();

  Serial.begin(9600);
  dht.begin();
  WiFi.begin(ssid, password);

  for(int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) {
      Serial.print(".");
      delay(1000);
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

server.listen(80);
  Serial.print("Is server live? ");
  Serial.println(server.available());
}

void loop() {
  

  if(client.available()) {
    WebsocketsMessage msg = client.readBlocking();

    float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);


    String data2Send = String(humidity) + "," + String(temperature);

    Serial.print("Sending data: ");
    Serial.println(data2Send);

    client.send(data2Send);
  }

  delay(1000);
}