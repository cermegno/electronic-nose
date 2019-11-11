#include <ESP8266WiFi.h>
#include <InfluxDb.h>

const char* ssid = "mySSID";
const char* password = "mypassword";
IPAddress staticIP(192,168,10,10);
IPAddress gateway(192,168,10,1);
IPAddress subnet(255,255,255,0);


#define MUX_A 15
#define MUX_B 13
#define MUX_C 12
#define ANALOG_INPUT A0

#define INFLUXDB_HOST "192.168.10.100"
Influxdb influx(INFLUXDB_HOST);

void setup() {
  //Define output pins for Mux
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);     
  pinMode(MUX_C, OUTPUT);

  Serial.begin(9600);
  Serial.println();
  Serial.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, password);
  WiFi.config(staticIP, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Setup done");
  influx.setDb("test");
}

void changeMux(int c, int b, int a) {
  digitalWrite(MUX_A, a);
  digitalWrite(MUX_B, b);
  digitalWrite(MUX_C, c);
}

void loop() {
  float value1;
  float value2;
  float value3;
  float value4;
  
  changeMux(LOW, LOW, LOW);
  value1 = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 0 pin of Mux
  
  changeMux(LOW, LOW, HIGH);
  value2 = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 1 pin of Mux
  
  changeMux(LOW, HIGH, LOW);
  value3 = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 2 pin of Mux

  changeMux(LOW, HIGH, HIGH);
  value4 = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 3 pin of Mux

//  changeMux(HIGH, LOW, LOW);
//  value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 4 pin of Mux
//
//  changeMux(HIGH, LOW, HIGH);
//  value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 5 pin of Mux
//
//  changeMux(HIGH, HIGH, LOW);
//  value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 6 pin of Mux
//
//  changeMux(HIGH, HIGH, HIGH);
//  value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 7 pin of Mux
  Serial.print(value1);
  Serial.print("  ");
  Serial.print(value2);
  Serial.print("  ");
  Serial.print(value3);
  Serial.print("  ");
  Serial.println(value4);
  delay(1000);

  InfluxData row("enose");
  //row.addTag("device", "enose4");
  row.addValue("MQ3", value1);
  row.addValue("MQ4", value2);
  row.addValue("MQ6", value3);
  row.addValue("MQ135", value4);

  influx.write(row);
  delay(5000);
}

