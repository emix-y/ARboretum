
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <DHT.h>

const char* ssid = "HUAWEI-E5330-9920";
const char* password = "14et72by";

using namespace websockets;
WebsocketsServer server;

#define DHTPIN 14          // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11     // DHT sensor type
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

WebsocketsClient client; // <------------------------------------

void setup() {
 
  // make this a global (declared above) so it doesn't close and reopen every loop <--------------------------
  client = server.accept();

  Serial.begin(9600);
  dht.begin();
  // Connect to wifi
  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  for(int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) {
      Serial.print(".");
      delay(1000);
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP

  server.listen(80);
  Serial.print("Is server live? ");
  Serial.println(server.available());
}

void loop() {
 
  if(client.available()) {
    WebsocketsMessage msg = client.readBlocking();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
   // Print results to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\tHumidity: ");
  Serial.print(humidity);
 
    // make a string of the data seperated by commas   <------------------------------------------
    String data2Send = String(humidity) + "," + String(temperature);

    // log
    Serial.print("Got Message: ");
    Serial.println(data2Send);

    // send that data through <-------------------------------------------
    client.send(data2Send);
 
  }
 
  delay(1000);
}