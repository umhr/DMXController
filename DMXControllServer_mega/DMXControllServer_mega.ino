#include <SPI.h>
#include <Ethernet.h>
#include <DmxSimple.h>
#include "DMX.h"

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x33
};
IPAddress ip(192, 168, 1, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
String pathname = "";
String query = "";
DMX dmx = DMX();

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(74880);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ethernet WebServer Example");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

    lcd.init();
  lcd.backlight();
  lcd.print(Ethernet.localIP());
}

String getQuery(String command)
{
  if(query.indexOf(command + "=") > -1){
    int num = query.indexOf(command + "=");
    int start = num + command.length() + 1;
    int end = query.indexOf("&", start);
    if(end == -1){
      end = query.length();
    }
    return query.substring(start, end);
  }
  return ""; 
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    boolean isGETLine = true;
    String header = "";
    String headerGET = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        if(isGETLine){
          if(c == '\n'){
            isGETLine = false;
            Serial.println(header);
          }else{
            header += String(c);
          }
        }
        
        if (c == '\n' && currentLineIsBlank) {
          headerGET = header.substring(header.indexOf(" ")+1, header.lastIndexOf(" "));
          client.println("HTTP/1.1 200 OK");
          if(headerGET == "/favicon.ico"){
            break;
          }
          pathname = headerGET.substring(0, headerGET.indexOf("?"));
          query = headerGET.indexOf("?") > -1?headerGET.substring(headerGET.indexOf("?")+1):"";
          //Serial.println("pathname=" + pathname + ", query=" + query);
          if(pathname == "/dmx/"){
            String c = getQuery("c");
            String w = getQuery("w");
            Serial.println("c=" + c + ", w=" + w);
            if(c != "" && w != ""){
              DmxSimple.write(c.toInt(), w.toInt());
              client.println("Content-Type: application/json");
              client.println("Access-Control-Allow-Origin: *");
              client.println("Connection: close");
              client.println();
              client.println("{}");
            }else{
              client.println("Content-Type: text/html");
              client.println("Access-Control-Allow-Origin: *");
              client.println("Connection: close");
              //client.println("Refresh: 5");
              client.println();
              client.println(dmx.getHTML());
            }
            break;            
          }
          client.println("Content-Type: text/html");
          client.println("Access-Control-Allow-Origin: *");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<body>");
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("Analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
            client.println("<div><a href='./dmx/'>dmx</a></div>");
          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
