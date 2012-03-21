void getProvisioned() { 
  Serial.println("connecting to provisioning...");
  if (client.connect("provisioning.connectedenvironments.com", 80)) {
    Serial.println("connected to provisioning...");
    client.print("GET /activate/");
    client.print(hashKey);
    client.println(".csv HTTP/1.1");
    client.print("Host: provisioning.connectedenvironments.com\n");
    client.print("User-Agent: ");
    client.println(USERAGENT);
    client.println("Connection: close\n");
    provisioningAttempted = true;
    Serial.println("\nProvisioning request sent");
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    lastConnected = client.connected();
    startEthernet();
  }
}

void extractProvisioningData(){

  int comma1 = thisLine.indexOf(",");
  int comma2 = thisLine.indexOf(",", comma1+1);

  if (comma1>32){

    apiKey1 = thisLine.substring(0,comma1/2);
    apiKey2 = thisLine.substring(comma1/2,comma1);
    //Serial.print("api key: ");
    //Serial.print(apiKey1);
    //Serial.println(apiKey2);

    feedId = thisLine.substring(comma1+1, comma2);

    Serial.print("feedId: ");
    Serial.println(feedId);

    thisLine = "";
    provisioned = true;
    client.stop();
  } 
  else {
    Serial.print("Feed or api key not appropriate: ");
    Serial.println(thisLine);    
    
    provisioningAttempted = false;
    provisioned = false;
    client.stop();
  }
}









