/*
  AWS Iot Core

  DHT Sensor Library - https://github.com/adafruit/DHT-sensor-library
  Hornbill-Examples - https://github.com/ExploreEmbedded/Hornbill-Examples
  AWS - https://aws.amazon.com/
  
  
  In this script will help you to Read temperature and humidity from DHT11 and publishes that data to AWS using MQTT Protocol.

  Note: 1)Connect VCC to 3.3V, GND to GND and DATA pin to pin specified in #define DHT_PIN of DHT11
        2) If you wanted to work with DHT22 you can use the same script by changing #define DHT_TYPE DHT22
        2)In this sketch Hornbill-Examples library is used for the handling of publishing data via MQTT
          Hornbill-Examples uses aws-iot-device-sdk-embedded-C library provided by AWS
          to interact with AWS.
          Use aws-iot-device-sdk-embedded-C library for creating your own code for handling transfer.
          Link for aws-iot-device-sdk-embedded-C is given above.
        3)DHT-sensor-library is used for reading data from DHT11
          
         All the Links are given above.
 */

#include<WiFi.h>
#include<DHT.h>
#include<AWS_IOT.h>

#define DHT_PIN 12 // pin connected to data pin of DHT11
#define DHT_TYPE DHT11  // Type of the DHT Sensor, DHT11/DHT22

#define WIFI_SSID "[INSERT NETWORK NAME(Wi-Fi name)]" // SSID of your WIFI
#define WIFI_PASSWD "[INSERT WI-FI PASSWORD]" //your wifi password

#define CLIENT_ID "Temp_Humidity"// thing unique ID, this id should be unique among all things associated with your AWS account.
#define MQTT_TOPIC "[FOLLOW THE SITE FOR THIS SECTION]" //topic for the MQTT data
#define AWS_HOST "[FOLLOW THE SITE FOR THIS SECTION]" // your host for uploading data to AWS,

DHT dht(DHT_PIN, DHT_TYPE);
AWS_IOT aws;

void setup(){
  Serial.begin(9600);
  Serial.print("\nInitializing thing Temp_Humidity_DHT11_0 \n");

  Serial.print("\n  Initializing WIFI: Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  Serial.print("  ");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n  Connected.\n  Done");

  Serial.print("\n  Initializing DHT11...");
  dht.begin();
  Serial.println("  Done.");

  Serial.println("\n  Initializing connetction to AWS....");
  if(aws.connect(AWS_HOST, CLIENT_ID) == 0){ // connects to host and returns 0 upon success
    Serial.println("  Connected to AWS\n  Done.");
  }
  else {
    Serial.println("  Connection failed!\n make sure your subscription to MQTT in the test page");
  }
  Serial.println("  Done.\n\nDone.\n");
}

void loop(){
  // read temperature and humidity
  float temp = dht.readTemperature(); // return temperature in °C
  float humidity = dht.readHumidity();// return humidity in %

  // check whether reading was successful or not
  if(temp == NAN || humidity == NAN){ // NAN means no available data
    Serial.println("Reading failed.");
  }
  else{
    //create string payload for publishing
    String temp_humidity = "Temperature: ";
    temp_humidity += String(temp);
    temp_humidity += "°C Humidity: ";
    temp_humidity += String(humidity);
    temp_humidity += " %";

   
    char payload[40];
    temp_humidity.toCharArray(payload, 40);

    Serial.println("Publishing:- ");
    Serial.println(payload);
     if(aws.publish(MQTT_TOPIC, payload) == 0){// publishes payload and returns 0 upon success
      Serial.println("Success\n");
    }
    else{
      Serial.println("Failed!\n");
    }
  }

  delay(1000);
}
