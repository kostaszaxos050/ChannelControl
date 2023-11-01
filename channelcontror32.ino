#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ezButton.h>
#include "index.h"

//network credentials
const char *ssid = "WindWiFi_B214"; //"WIND_2.4G_138351";
const char *password = "69773075"; //"8N4LLEH4";

// Set web server port number to 80
WebServer server(80);

//static ip adress
IPAddress local_IP(192, 168, 1, 155);
IPAddress gateway(192, 168, 1, 254);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

//Button and channel config
ezButton button(2);
ezButton button2(4); 
ezButton button3(35);
ezButton button4(12); 
const int channel = 14, channel2 = 15, channel3 = 36, channel4 = 39;

//Channel state array
int channelState[] = {LOW, LOW, LOW, LOW}; // 0 = LOW / 1 = HIGH

//Bool variable to refresh the page
bool buttonPress = false;

//Total hours wasted on this porject: inf+1


void handleRoot() {

  //loading html code to a string
  String stateUpdate = MAIN_page;

  //Strings that store the correct html class depending on channel state
  String channelSt;
  if(channelState[0] == LOW) channelSt = "<a href=\"Channel\" class=\"red-link\">Channel</a>";
  else channelSt = "<a href=\"Channel\" class=\"green-link\">Channel</a>";

  String channelSt2;
  if(channelState[1] == LOW) channelSt2 = "<a href=\"Channel2\" class=\"red-link\">Channel2</a>";
  else channelSt2 = "<a href=\"Channel2\" class=\"green-link\">Channel2</a>";

  String channelSt3;
  if(channelState[2] == LOW) channelSt3 = "<a href=\"Channel3\" class=\"red-link\">Channel3</a>";
  else channelSt3 = "<a href=\"Channel3\" class=\"green-link\">Channel3</a>";

  String channelSt4;
  if(channelState[3] == LOW) channelSt4 = "<a href=\"Channel4\" class=\"red-link\">Channel4</a>";
  else channelSt4 = "<a href=\"Channel4\" class=\"green-link\">Channel4</a>";

  //replacing the placeholders on the html page with the modified strings
  stateUpdate.replace("%C1%", channelSt);
  stateUpdate.replace("%C2%", channelSt2);
  stateUpdate.replace("%C3%", channelSt3);
  stateUpdate.replace("%C4%", channelSt4);

  server.send (200, "text/html", stateUpdate); //displays the main page

}

//handling each chanel seperatly by reversing the previous state and redirecting back to the main page
void handleChannel() {

  channelState[0] = !channelState[0];
  digitalWrite(channel, channelState[0]);

  server.sendHeader("Location", "/", true); // Redirect to the root page
  server.send(302, "text/plain", ""); // 302 Found response

}

void handleChannel2() {

  channelState[1] = !channelState[1];
  digitalWrite(channel2, channelState[1]);

  server.sendHeader("Location", "/", true); // Redirect to the root page
  server.send(302, "text/plain", ""); // 302 Found response

}

void handleChannel3() {

  channelState[2] = !channelState[2];
  digitalWrite(channel3, channelState[2]);

  server.sendHeader("Location", "/", true); // Redirect to the root page
  server.send(302, "text/plain", ""); // 302 Found response

}

void handleChannel4() {

  channelState[3] = !channelState[3];
  digitalWrite(channel4, channelState[3]);

  server.sendHeader("Location", "/", true); // Redirect to the root page
  server.send(302, "text/plain", ""); // 302 Found response

}

void handleUpdate() {

  if (buttonPress){ 
    buttonPress = false;
    server.send (200, "text/html", "true");
  }
  else server.send (200, "text/html", "false");
}

void setup() {

  Serial.begin(115200);

  //Outputs setup
  pinMode(channel, OUTPUT);
  pinMode(channel2, OUTPUT);
  pinMode(channel3, OUTPUT);
  pinMode(channel4, OUTPUT);

  digitalWrite(channel, LOW);
  digitalWrite(channel2, LOW);
  digitalWrite(channel3, LOW);
  digitalWrite(channel4, LOW);

  //Button debounce setup
  button.setDebounceTime(50);
  button2.setDebounceTime(50);
  button3.setDebounceTime(50);
  button4.setDebounceTime(50); 

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) Serial.println("STA Failed to configure");

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/Channel", HTTP_GET, handleChannel);
  server.on("/Channel2", HTTP_GET, handleChannel2);
  server.on("/Channel3", HTTP_GET, handleChannel3);
  server.on("/Channel4", HTTP_GET, handleChannel4);
  server.on("/update", HTTP_GET, handleUpdate);

  server.begin();

}

void loop(){

  button.loop();
  button2.loop();
  button3.loop();
  button4.loop();

  if (button.isPressed()) {
    channelState[0] = !channelState[0];
    buttonPress = true;
    }
      else if (button2.isPressed()) {
       channelState[1] = !channelState[1];
       buttonPress = true;
      }
        else if (button3.isPressed()) {
        channelState[2] = !channelState[2];
        buttonPress = true;
        }
          else if (button4.isPressed()) {
          channelState[3] = !channelState[3];
          buttonPress = true;
          }

  server.handleClient();
  
}
