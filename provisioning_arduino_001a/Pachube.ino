void sendData(String dataString) {

  Serial.println("connecting to pachube...");
  // if there's a successful connection:
  if (client.connect("api.pachube.com", 80)) {
//    Serial.println("connected to pachube...");
//    Serial.print(apiKey1);
//    Serial.print(apiKey2);
//    Serial.print("\nupdating ");
//    Serial.println(feedId);
//    Serial.println(dataString);
//    Serial.println("....");

    // send the HTTP PUT request:
    client.print("PUT /v2/feeds/");
    client.print(feedId);
    client.println(".csv HTTP/1.1");
    client.println("Host: api.pachube.com");
    client.print("X-PachubeApiKey: ");
    client.print(apiKey1);
    client.println(apiKey2);
    client.print("User-Agent: ");
    client.println(USERAGENT);
    client.print("Content-Length: ");
    client.println(dataString.length(), DEC);

    // last pieces of the HTTP PUT request:
    client.print("Content-Type: text/csv\n");
    client.println("Connection: close\n");

    // here's the actual content of the PUT request:
    client.println(dataString);

    // note the time that the connection was made:
    lastConnectionTime = millis();
    thisLine = "";
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    lastConnected = client.connected();
  }
}



