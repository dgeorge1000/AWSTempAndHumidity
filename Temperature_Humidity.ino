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

// includes the necessary libraries(if you changed the name of the libraries during the tutorial you should change them here)
#include<WiFi.h>
#include<DHT.h>
#include<AWS_IOT.h>

#define DHT_PIN 32                    // pin connected to data pin of DHT11, change if the DHT pin is not in 32
#define DHT_TYPE DHT11                // Type of the DHT Sensor, DHT11/DHT22. If you are using a DHT22 then replace "DHT11" with "DHT12"

#define WIFI_SSID "[INSERT WI-FI NETWORK NAME HERE]"   // SSID of your Wi-Fi(name of the Wi-Fi)
#define WIFI_PASSWD "[INSERT WI-FI NETWORK PASSWORD HERE]"      // your Wi-Fi password

#define CLIENT_ID "Temp_Humidity_sensor"                                              // thing unique ID, this id should be unique among all things associated with your AWS account.
#define MQTT_TOPIC "[FOLLOW INSTRUCTIONS FROM TUTORIAL TO INSERT YOUR MQTT]"          // topic for the MQTT data found in you AWS(check in the tutorial for directions)
#define AWS_HOST "[FOLLOW INSTRUCTINOS FROM TUTORIAL TO INSERT YOUR AWS HOST]"        // your host for uploading data to AWS found in your AWS(check in the tutorial for directions)

DHT dht(DHT_PIN, DHT_TYPE);   // identifies a dht sensor
AWS_IOT aws;                  // "aws" object

void setup(){
  Serial.begin(9600);                                                       // start the Serial Monitor
  Serial.print("\nInitializing thing Temp_Humidity_DHT11_0 \n");

  Serial.print("\n  Initializing WIFI: Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);       
  Serial.print("  ");
  while(WiFi.status() != WL_CONNECTED){                                     // checks if the Wi-Fi connected properly
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n  Connected.\n  Done");

  Serial.print("\n  Initializing DHT11...");
  dht.begin();
  Serial.println("  Done.");

  Serial.println("\n  Initializing connetction to AWS....");
  if(aws.connect(AWS_HOST, CLIENT_ID) == 0){                              // connects to host(AWS server) and returns 0 upon success
    Serial.println("  Connected to AWS\n  Done.");
  }
  else {
    Serial.println("  Connection failed!\n make sure your subscription to MQTT in the test page");
  }
  Serial.println("  Done.\n\nDone.\n");
}

void loop(){
  // read temperature and humidity
  float temp_Celsius = dht.readTemperature();    
  float temp = (temp_Celsius * 9) / 5 + 32;             // return temperature in °F => °F = (°C * 9) / 5 + 32
  float humidity = dht.readHumidity();                  // return humidity in %

  // check whether reading was successful or not
  if(temp == NAN || humidity == NAN){ // NAN means no available data
    Serial.println("Reading failed.");
  }
  else{
    //create string payload for publishing
    String temp_humidity = "Temperature: ";
    temp_humidity += String(temp);
    temp_humidity += "°F Humidity: ";
    temp_humidity += String(humidity);
    temp_humidity += " %";

   
    char payload[40];
    temp_humidity.toCharArray(payload, 40);

    Serial.println("Publishing:- ");
    Serial.println(payload);
     if(aws.publish(MQTT_TOPIC, payload) == 0){       // publishes payload and returns 0 upon success, checks to see if it uploaded to the AWS server
      Serial.println("Success\n");
    }
    else{
      Serial.println("Failed!\n");
    }
  }
  delay(5000);                                       // wait 5000 ms till next read(5 seconds)
}
