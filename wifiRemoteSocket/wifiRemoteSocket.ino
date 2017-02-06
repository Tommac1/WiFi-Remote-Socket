#include <ESP8266WiFi.h>

#define pinD1 D1
#define pinD2 D2
#define pinD3 D3
#define pinD4 D4

// your wifi network preferences
const char* ssid = "ssid";
const char* password = "pass";

int pinD1state = 0, pinD2state = 0, pinD3state = 0, pinD4state = 0;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(pinD1, OUTPUT);
  digitalWrite(pinD1, 0);

  pinMode(pinD2, OUTPUT);
  digitalWrite(pinD2, 0);

  pinMode(pinD3, OUTPUT);
  digitalWrite(pinD3, 0);

  pinMode(pinD4, OUTPUT);
  digitalWrite(pinD4,0);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // read the get method
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  if(req.indexOf("GET / HTTP/1.1") != -1)
    delay(1);
  else if (req.indexOf("/pinD1ON") != -1)
    pinD1state = 1;
  else if (req.indexOf("/pinD1OFF") != -1)
    pinD1state = 0;
  else if (req.indexOf("/pinD2ON") != -1)
    pinD2state = 1;
  else if (req.indexOf("/pinD2OFF") != -1)
    pinD2state = 0;
  else if(req.indexOf("/pinD3ON") != -1)
    pinD3state = 1;
  else if(req.indexOf("/pinD3OFF") != -1)
    pinD3state = 0;
  else if(req.indexOf("/pinD4ON") != -1)
    pinD4state = 1;
  else if(req.indexOf("/pinD4OFF") != -1)
    pinD4state = 0;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  digitalWrite(pinD1, pinD1state);
  digitalWrite(pinD2, pinD2state);
  digitalWrite(pinD3, pinD3state);
  digitalWrite(pinD4, pinD4state);

  
  client.flush();

  // html + css of webpage 
  String pageCode = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\
<html lang='pl'>\
<head>\
<title>Czesc, przyjacielu!</title>\
<style>\
body {\
background-color: #333333;\
font-weight: bold;\
color: white;\
font-family: Arial;\
font-size: 15px;\
max-width: 400px;\
text-align: center;\
}\
a {\
color: white;\
}\
.gpio {\
border: 1px solid white;\
padding: 10px 20px 10px 20px;\
display: inline-block;\
}\
.gpioNum {\
margin: 5px;\
border: 1px solid white;\
float: left;\
width: 30px;\
height: 20px;\
vertical-align: middle;\
padding: 10px 20px 10px 20px;\
background: #0022ff;\
}\
.gpioStatus {\
margin: 5px;\
border: 1px solid white;\
float: left;\
padding: 10px 20px 10px 20px;\
width: 30px;\
height: 20px;\
vertical-align: middle;\
}\
.buttonON {\
margin: 5px;\
border: 1px solid white;\
background: #00ff11;\
padding: 10px 20px 10px 20px;\
width: 30px;\
height: 20px;\
float: left;\
vertical-align: middle;\
}\
.buttonOFF {\
margin: 5px;\
border: 1px solid white;\
background: #ff0000;\
padding: 10px 20px 10px 20px;\
width: 30px;\
height: 20px;\
float: left;\
}\
</style>\
</head>\
<body>\
<div id='main'>\
<div id='container'>\
<div class='gpio'>\
<div class='gpioNum'>\
D1\
</div>\
<div class='gpioStatus'>";
  pageCode += (pinD1state) ? "ON" : "OFF";
  pageCode += "</div>\
<a href='pinD1ON'>\
<div class='buttonON'>\
ON\
</div>\
</a>\
<a href='pinD1OFF'>\
<div class='buttonOFF'>\
OFF\
</div>\
</a>\
<div style='clear: both;'></div>\
</div>\
<div class='gpio'>\
<div class='gpioNum'>\
D2\
</div>\
<div class='gpioStatus'>";
  pageCode += (pinD2state) ? "ON" : "OFF";
  pageCode += "</div>\
<a href='pinD2ON'>\
<div class='buttonON'>\
ON\
</div>\
</a>\
<a href='pinD2OFF'>\
<div class='buttonOFF'>\
OFF\
</div>\
</a>\
<div style='clear: both;'></div>\
</div>\
<div class='gpio'>\
<div class='gpioNum'>\
D3\
</div>\
<div class='gpioStatus'>";
  pageCode += (pinD3state) ? "ON" : "OFF";
  pageCode += "</div>\
<a href='pinD3ON'>\
<div class='buttonON'>\
ON\
</div>\
</a>\
<a href='pinD3OFF'>\
<div class='buttonOFF'>\
OFF\
</div>\
</a>\
<div style='clear: both;'></div>\
</div>\
<div class='gpio'>\
<div class='gpioNum'>\
D4\
</div>\
<div class='gpioStatus'>";
  pageCode += (pinD4state) ? "ON" : "OFF";
  pageCode += "</div>\
<a href='pinD4ON'>\
<div class='buttonON'>\
ON\
</div>\
</a>\
<a href='pinD4OFF'>\
<div class='buttonOFF'>\
OFF\
</div>\
</a>\
<div style='clear: both;'></div>\
</div>\
</div>\
</div>\
</body>\
</html>";

  client.print(pageCode);
  delay(1);
  Serial.println("Client disonnected");
}

