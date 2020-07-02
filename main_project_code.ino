String cmd = "AT+CIPSTART=\"TCP\",\"api.openweathermap.org\",80\r\n";
String cmd_get = "GET /data/2.5/weather?id=1268510&APPID=ef44abaa3200b2aabac5cc5677e40735&units=metric\r\n";
String recived = "";

String ssid= "\"B2\"";
String password = ",\"\"";

void setup() {
  Serial.begin(9600);
  Serial2.begin(115200);
  Serial3.begin(9600);
  
  AT_Object("AT+RST\r\n",2000);
  AT_Object("AT+CWMODE=1\r\n",1000);

  while(1){
    Serial.println("try connect WiFi");
    if(connectWiFi()){
      break;
    }
    delay(500);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  String Cipsend = "AT+CIPSEND=";
  Serial.println("Loop");

  AT_Object(cmd,1000);
  if(Serial3.find("Error")){ 
    Serial.println("cmd Error");
    return;
    }else{
      Serial.println("Linked!");
    }
  Cipsend += cmd_get.length();
  Cipsend += "\r\n";
  AT_Object(Cipsend,0);
  if(Serial3.find(">")){
    Serial.println(">");
  }else{
    Serial.println("AT+CIPSEND Error");
    Serial3.print("AT+CIPCLOSE\r\n");
    return;
  }
  recived=AT_Object(cmd_get,4000);
  Serial.println(json_parser(recived,"main"));
  Serial.println(json_parser(recived,"temp"));
  Serial.println(json_parser(recived,"speed"));
  Serial2.print(json_parser(recived,"main"));
  Serial2.print(",");
  Serial2.println(json_parser(recived,"temp"));
  Serial2.print(",");
  Serial2.println(json_parser(recived,"speed"));
  delay(3000);
}

boolean connectWiFi(){
  String WiFicommand =  "AT+CWJAP=";
  WiFicommand += ssid;
  WiFicommand += password;
  WiFicommand += "\r\n";
  AT_Object(WiFicommand,0);
  if(Serial3.find("OK")){
    Serial.println("Connected to WiFi");
    return true;
  } else{
    Serial.println("Can not Connected to WiFi");
    return false;
  }
}

String AT_Object(String command, const int timeout){
  String response ="";
  Serial3.print(command);
  while(!Serial3.available()){
    ;}
  long int time_ = millis();
  while((time_+timeout) > millis()){
    while(Serial3.available()){
      char c = Serial3.read();
      response += c;
    }
  }
  return response;
}

String json_parser(String s, String a){ 
  String val; 
  if (s.indexOf(a) != -1){
    int st_index = s.indexOf(a);
    int val_index = s.indexOf(':', st_index);
    if (s.charAt(val_index + 1) == '"'){
      int ed_index = s.indexOf('"', val_index + 2);
      val = s.substring(val_index + 2, ed_index);
  }else{ 
    int ed_index = s.indexOf(',', val_index + 1); 
    val = s.substring(val_index + 1, ed_index);
    }
  }else{ 
    Serial.print(a); 
    Serial.println(F(" is not available")); 
  } 
  return val; 
}
