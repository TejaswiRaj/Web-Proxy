#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>


// constant declaration that can be modified for testing
// Station Credentials
const char *newssid = "Tejaswi";
const char *newpassword = "";

const char* ssid = "SCU-Guest";
const char* password = "";

WebServer server(80);

void Root() {}

//Method for getting all the response from the Station
void NotFound()
{ 
  int counter = 0;

  //Getting the whole URL
  String requestUri = server.uri();
  if (server.args() > 0) {
     requestUri += "?";
     for (int i=0; i<server.args(); i++) {
        requestUri += server.argName(i);
        requestUri += "=";
        requestUri += server.arg(i);
        if (i+1<server.args()) {
           requestUri += "&";
        }
     }
  }
  Serial.println(requestUri); 
  const char* host="esqsoft.com";
  const char* url="https://esqsoft.com/examples/troubleshooting-http-using-telnet.htm";
  WiFiClient client;
  
//  Logic for serving as complete web server
//  const char* host="esqsoft.com";
//  const char* url="/javascript/free-online-time-sheet.htm";
//  Serial.println(requestUri.charAt(0));
//  if(requestUri.charAt(0)=='w')
//  {
//    uri_break = 4;
//  }
//  while(requestUri.charAt(uri_break) != '/')
//  {
//    host += requestUri.charAt(uri_break);
//    uri_break++;
//  }
//  while(requestUri.length() != uri_break)
//  {
//    url +=requestUri.charAt(uri_break);
//    uri_break++;
//  }
//  Serial.println("HOST:"+host);
//  Serial.println("URL:"+url);

 
  

//Once we have all the data from WebPage we just forward it to the Access point

  while (!client.connect(host, 80)) 
  {
    Serial.println("Connection Failed");  
  }

  client.print(String("GET ") + url);

  client.print(String(" HTTP/1.1\r\n") + "Host: " + host + "\r\n" +  "Connection: close\r\n\r\n");
           
  while(!client.available())
  {
    yield();
    Serial.println("yielding");
  }

  String w;
  while(client.available())
  {
    Serial.println("client available");
    w += client.readStringUntil('\r');
    w.replace("HTTP", "What-is-this");
    Serial.println(w);
  }
  server.send(200, "text/html", w);
  client.stop();
}

void setup()
{
  Serial.begin(115200);
  WiFi.softAP(newssid, newpassword);
  server.on("/", Root);
  Serial.println( "uri :"+server.uri());
  
  //Serial.println( "arg :"+server.arg());
  
  //Serial.println( "argName :"+server.argName);

  server.onNotFound(NotFound);

  server.begin();
  Serial.println("HTTP Server connected");
  WiFi.begin(ssid, password);
}

void loop()
{
  server.handleClient();
}
