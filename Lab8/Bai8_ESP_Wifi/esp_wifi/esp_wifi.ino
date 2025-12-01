#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "config.h"
//Wifi name
#define WLAN_SSID       "P425"
//Wifi password
#define WLAN_PASS       "87654321"

//setup Adafruit
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
//fill your username       
String uart_buffer = "";            
//setup MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

//setup publish
Adafruit_MQTT_Publish light_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/led");
Adafruit_MQTT_Publish temp_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp");

//setup subcribe
Adafruit_MQTT_Subscribe light_sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led", MQTT_QOS_1);
Adafruit_MQTT_Subscribe temp_sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/temp", MQTT_QOS_1);

int led_counter = 0;
int led_status = HIGH;
bool stm32_connected = false;

void lightcallback(char* value, uint16_t len){
  if(value[0] == '0') Serial.print('a');
  if(value[0] == '1') Serial.print('A');
}

void tempcallback(char* value, uint16_t len){
  Serial.print("Temperature callback: ");
  Serial.print(value);
  Serial.println("°C");
}

void setup() {
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);

  Serial.begin(115200);
  
  // In thông tin khi khởi động
  Serial.println("\n=== ESP8266 Starting ===");

  // Kết nối Wifi
  Serial.print("Connecting to WiFi");
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");  // In dấu chấm mỗi 500ms
  }
  Serial.println(" Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Subscribe
  light_sub.setCallback(lightcallback);
  mqtt.subscribe(&light_sub);
  mqtt.subscribe(&temp_sub);

  // Kết nối MQTT
  Serial.print("Connecting to MQTT");
  while (mqtt.connect() != 0) { 
    mqtt.disconnect();
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  digitalWrite(5, HIGH);
  Serial.println("=== Setup Complete ===\n");
}

void loop() {
    // Process incoming MQTT packets
    mqtt.processPackets(1);

    // Handle UART data from STM32
    while (Serial.available()) {
        char c = Serial.read();

        // Start of data packet
        if (c == '!') uart_buffer = "";

        uart_buffer += c;

        // End of data packet
        if (c == '#') {
            // Serial.print("WTF");
            if (uart_buffer.startsWith("!TEMP:")) {
                String tempStr = uart_buffer.substring(6, uart_buffer.length() - 1);
                float tempValue = tempStr.toFloat();
                temp_pub.publish(tempValue);
            }

            uart_buffer = "";
        }
    }

    // LED control based on UART commands
    if (Serial.available()) {
        int msg = Serial.read();
        if (msg == 'o') Serial.print('O');
        else if (msg == 'a') light_pub.publish(0);
        else if (msg == 'A') light_pub.publish(1);
    }

    // LED blinking logic
    led_counter++;
    if (led_counter == 100) {
        led_counter = 0;
        led_status = !led_status;
        digitalWrite(2, led_status);
    }
}