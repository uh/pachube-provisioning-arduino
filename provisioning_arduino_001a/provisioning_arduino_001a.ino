// Provisioning for Arduino 0.01a
// Code for using Arduino with Pachube.com's provisioning server
// You will need to have your Pachube account enabled for provisioning server access
// You will then be able to create a new "device" category; retrieve the Device Key; and
// upload serial numbers. An arduino that 'knows' the device key and its own serial number
// will contact the provisioning server, which will automagically generate a feed, specified
// datastreams and an API key and respond to the arduino with this information.
// The arduino can then use these credentials to update Pachube - there is no need for \
// manually creating and hard-coding feeds and API keys.
//
//
// uses Peter Knight's Cryptosuite for Arduino
// found here: https://github.com/Cathedrow/Cryptosuite
// and zipped: https://github.com/Cathedrow/Cryptosuite/zipball/master

#include "sha1.h"
#include <SPI.h>
#include <Ethernet.h>


// variables to set

#define USERAGENT          "Usmanode Arduino 0.1"

uint8_t deviceKey[]={
  0x75,0x6c,0xe1,0xbc,0x9e,0x77,0x02,0xf5,0xc4,0xcb,0xc3,0x4e,0xbe,0xc6,0x51,0xda,0xcf,0x9e,0xbb,0xd2
};

String serialNumber = "234923409823409822111";

byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x00, 0x71, 0x81};

const int postingInterval = 5000;   


// fallback IP address if DHCP fails
IPAddress ip(10,0,0,20);

EthernetClient client;

unsigned long lastConnectionTime = 0;        
boolean lastConnected = false;      
String hashKey;

// split apiKey into two strings means fewer memory issues
String apiKey1 = "";
String apiKey2 = "";
String feedId = "";
String thisLine = "";  

boolean provisioningAttempted = false;
boolean provisioned = false;
boolean responseComplete = false;

int statusCode = 0;

void setup() {
  Serial.begin(57600);
  Serial.println("computing hmac sha: ");
  Sha1.initHmac(deviceKey,20);
  Sha1.print(serialNumber); 
  hashKey = convertHash(Sha1.resultHmac());
  Serial.println(hashKey);
  startEthernet();
}

void loop() {

  if (!provisioningAttempted){
    getProvisioned();
  }

  if (client.available()) {
    processIncoming();
  }

  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop(); 
    responseComplete = true;
  }

  if (responseComplete && !provisioned && (statusCode == 200)){
    extractProvisioningData(); 
  }

  if(provisioned && !client.connected() && timeToConnect()) {
    String dataString = "";
    dataString += "light,";
    dataString += analogRead(A0);
    dataString += "\ntemperature,";
    dataString += analogRead(A1);
    dataString += "\nhumidity,";
    dataString += analogRead(A2);
    sendData(dataString);
  }
  
  lastConnected = client.connected();

}


void processIncoming(){
  char c = client.read();
  Serial.print(c);
  if (c == '\n') {     
    if (thisLine.startsWith("HTTP/1.")){
      if (thisLine.startsWith("200 OK", 9)){
        statusCode = 200;      
      } 
      else if (thisLine.startsWith("40", 9)){
        statusCode = 401;  
        provisioningAttempted = false;
        provisioned = false;
        responseComplete = false;
      } 
      else
      {
        statusCode = 999; 
      }
      Serial.print("Status: ");
      Serial.println(statusCode);
    }   
    thisLine = "";
  } 
  else {
    thisLine += c;
  }
}

void startEthernet(){
  client.stop();
  Serial.println("Starting ethernet");
  delay(1000);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // DHCP failed, so use a fixed IP address:
    Ethernet.begin(mac, ip);
  } 
  else {
    Serial.println("Ethernet ready"); 
  }
}

boolean timeToConnect(){
  
  if ((millis() - lastConnectionTime) > postingInterval) {
    return true;
  } 
  else {   
    if (millis() < lastConnectionTime){
     lastConnectionTime = millis(); 
    }    
    return false;
  }
}







