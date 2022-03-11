/* based on https://github.com/jandrassy/ArduinoOTA/blob/master/examples/Advanced/OTASketchDownloadWifi/OTASketchDownloadWifi.ino */

#include "variables/variables.h"


#include <ArduinoOTA.h> // only for InternalStorage
#include <ArduinoHttpClient.h>

char urlBuf[256];

void handleCfgDownload(HttpClient client, int version) {
  const char* PATH = "%s/%s-V%d.CFG";  

  snprintf(urlBuf, sizeof(urlBuf), PATH, otaPath, SENSEBOX_ID, version + 1);


#ifdef DEBUG_ENABLED
  Serial.print("OTA: Check for update cfg-file TBD");
  Serial.println(urlBuf);
#endif
}

void handleSketchDownload(HttpClient client, int fwVersion) {
  const char* PATH = "%s/%s-V%d.bin";  
  fwVersion++; // look for newer version
  snprintf(urlBuf, sizeof(urlBuf), PATH, otaPath, SENSEBOX_ID, fwVersion);

#ifdef DEBUG_ENABLED
  Serial.print("OTA: Check for update fw-file [");
  Serial.print(urlBuf);
  Serial.println("]");
#endif

  // Make the GET request
  client.get(urlBuf);

  int statusCode = client.responseStatusCode( );

#ifdef DEBUG_ENABLED
  Serial.print("OTA: Update status code: ");
  Serial.println(statusCode);
#endif

  if (statusCode != 200) {
    client.stop();
    return;
  }

  long length = client.contentLength();
  if (length == HttpClient::kNoContentLengthHeader) {
    client.stop();
#ifdef DEBUG_ENABLED
    Serial.println("OTA: Server didn't provide Content-length header. Can't continue with update.");
#endif
    return;
  }

#ifdef DEBUG_ENABLED
  Serial.print("OTA: Server returned update file of size ");
  Serial.print(length);
  Serial.println(" bytes");
#endif

  if (!InternalStorage.open(length)) {
    client.stop();
#ifdef DEBUG_ENABLED
    Serial.println("OTA: There is not enough space to store the update. Can't continue with update.");
#endif
    return;
  }
  byte b;
  while (length > 0) {
    showRed();
    if (!client.readBytes(&b, 1)) // reading a byte with timeout
      break;
    InternalStorage.write(b);
    length--;
    showBlue();
  }
  InternalStorage.close();
  client.stop();
  if (length > 0) {

#ifdef DEBUG_ENABLED
    Serial.print("OTA: Timeout downloading update file at ");
    Serial.print(length);
    Serial.println(" bytes. Can't continue with update.");
#endif    

    return;
  }

#ifdef DEBUG_ENABLED
  Serial.println("OTA: Sketch update apply and reset.");
  Serial.flush();
#endif

  InternalStorage.apply(); // this doesn't return
}


// WiFiClient    wifiClient;  // HTTP
// WiFiSSLClient wifiClientSSL;  // HTTPS
int status = WL_IDLE_STATUS;

void handleOtaDownload(int version) {
#ifdef DEBUG_ENABLED
  Serial.println("OTA: Initialize WiFi");
#endif

  // attempt to connect to Wifi network:
  int cnt = 0;
  while ((status != WL_CONNECTED) && cnt++ < 4) { //allow 4 attempts
    showRed();
#ifdef DEBUG_ENABLED
    Serial.print("OTA: Attempting to connect to SSID: ");
    Serial.println(ssid);
#endif
    delay(300);
    showBlue();
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
  }
  if (cnt>4) {
#ifdef DEBUG_ENABLED
    Serial.print("OTA: No connection, back to normal.: ");
#endif
    WiFi.end();
    showRed();
    return;
  }
#ifdef DEBUG_ENABLED
  Serial.println("OTA: WiFi connected");
#endif

  HttpClient httpClient(wifiClient, otaURL, atoi(otaPort));  // HTTP
  // HttpClient client(wifiClientSSL, SERVER, SERVER_PORT);  // HTTPS


  handleCfgDownload(httpClient, version);
  handleSketchDownload(httpClient, version); // if successfull it restarts the device
} 
