#include <PubSubClient.h>

#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 4
#define DHTTYPE DHT22
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
unsigned long lastMsg = 0;
int value = 0;

const char* ssid = "...";  ///////////////////////
const char* password = "12345678"; ///////////////////////
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);





void setup() {
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  Serial.begin(115200);
  dht.begin();
  Serial.println(F("Sensor DHT22"));
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("---------------------------------"));
  Serial.println(F("Temperatura"));
  Serial.print  (F("Sensor: "));
  Serial.println(sensor.name);
  Serial.print  (F("Valor máximo"));
  Serial.print  (sensor.max_value);
  Serial.println(F("Valor mínimo"));
  Serial.print  (sensor.min_value);
  Serial.println(F("C°"));
  Serial.print  (F("Resolução: "));
  Serial.print  (sensor.resolution);
  Serial.println(F("C°"));
  Serial.println(F("---------------------------------"));
  dht.humidity().getSensor(&sensor);
  Serial.println(F("---------------------------------"));
  Serial.println(F("Umidade"));
  Serial.print  (F("Sensor: "));
  Serial.println(sensor.name);
  Serial.print  (F("Valor máximo"));
  Serial.print  (sensor.max_value);
  Serial.println(F("%"));
  Serial.println(F("Valor mínimo: "));
  Serial.print  (sensor.min_value);
  Serial.println(F("%"));
  Serial.print  (F("Resolução: "));
  Serial.print  (sensor.resolution);
  Serial.println(F("%"));
  Serial.println(F("---------------------------------"));
  //delayMS = sensor.min_value / 100;

}


void setup_wifi()
{
  delay(10);
  Serial.println("");
  Serial.print("Conectando com");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wifi conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}



void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Aguardando conexão MQTT...");
    String clientId = "ESP32Client";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())){
      Serial.println("conectado");
      client.subscribe("casa/publisher");}
      else {
        Serial.print("falhou, rc=");
        Serial.print(client.state());
        Serial.println("tente novamente em 5s");
        delay(5000);}
  }
}




void loop() {
    delay(1000);
    sensors_event_t event;


    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
      Serial.println(F("Erro na leitura da temperatura!"));
    }
    else
    {
      Serial.print(F("Temperatura: "));
      Serial.print(event.temperature);
      Serial.println(F("C°"));
      sprintf(msg, "%f",event.temperature);
      client.publish("daniel/temperatura", msg); ///////////////////////////////////
    }

    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
      Serial.println(F("Erro na leitura da umidade!"));
    }
    else
    {
      Serial.print(F("Umidade: "));
      Serial.print(event.relative_humidity);
      Serial.println(F("%"));
      sprintf(msg,"%f",event.relative_humidity);
      client.publish("daniel/umidade", msg); /////////////////////////////////
    }

    if (!client.connected())
    {
      reconnect();
    }
    client.loop();
    }

