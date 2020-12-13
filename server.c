#include <ESP8266WiFi.h>

const char* ssid = "LChayim2";
const char* password = "01096286028";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
//HTTP 통신은 80번 포트를 사용합니다.
#define RINGNUM 2

int* (tableState[RINGNUM]);
int one = 1;
int zero = 0;
int two = 2;

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  
  // Connect to WiFi network
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
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  tableState[0] = &zero;
  tableState[1] = &zero;
  tableState[2] = &zero;
}

void loop() {
  // Check if a client has connected
  // 클라이언트(사용자)가 접속을 했을
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
//  Serial.print("newClinet : ");

  int req = client.readStringUntil('\r').toInt();
  int table = req/10;
  int state = req%10;
//  Serial.print(table);
//  Serial.print(",");
//  Serial.println(state);
  if(state == 1){
    tableState[table] = &one;
  }
  else{
    tableState[table] = &zero;
  }
  
  // Prepare the response
  String webPage ="<html><head><meta http-equiv='refresh' content='2; URL=http://192.168.43.241'><meta name='viewport' content='width=device-width, initialscale=1.0'></head><div style='width:100%;height:15%;background-color:#525f78;text-align:center;font-size:50pt;font-family:sans-serif;color:white;'>Seat Status</div><div id='content' style='font-size:15pt; text-align:center; vertical-align:center; padding:60px 0px 60px 0px;'>";

  int cnt = 0;
  for(int i = 1 ;i <= RINGNUM; i ++){
    int tmp = *tableState[i];
//    Serial.print(tmp);
//    Serial.print(" , ");
    if(tmp == 0){
      cnt++;
      webPage += "<div id='area1' style='width : 45%; border:1px solid black; display: inline-block; background-color:lawngreen;'>Available</div>";
    }
    else if(tmp ==1 ){
      webPage += "<div id='area2' style='width : 45%; border:1px solid black; display: inline-block; background-color:red;'>Not available</div>";
    }
  }
//  Serial.println(" ");


  webPage += "</div><div id='footer' style='font-family: sans-serif; font-size: 20pt; margin:0; padding:0; text-align: center; color:white;'><ul style='list-style: none;width:100%; background-color:#525f78; padding:10px 0px 10px 0px; border: 1px solid #33435e;'><li><div id='total'>Total : ";
  webPage += RINGNUM;
  webPage += "</div></li><li><div id='remaining'>Remaining Seat : ";
  webPage += cnt;
  webPage += "</div></li></ul></div></div></body></html>";
  // Send the response to the client
  client.print(webPage);
  delay(1000);
//  Serial.println("Client disonnected");


  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
