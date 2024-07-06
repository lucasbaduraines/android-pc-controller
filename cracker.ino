#include <SoftwareSerial.h>
SoftwareSerial BTSerial(11, 10); // RX | TX
#include <Keyboard.h>
#include <Mouse.h>


void setup() 
{
    Keyboard.begin();
    Mouse.begin();
    Serial.begin(9600);
    Serial.println("Arduino with HC-06 is ready");
 
    // HC-06 default baud rate is 9600
    BTSerial.begin(9600);  
    Serial.println("BTserial started at 9600");
}


String messageBuffer = "";
String message = "";
 
String split_cmd[8];
int StringCount = 0;

void SplitString(){
    // Split the string into substrings
  while (message.length() > 0)
  {
    int index = message.indexOf(' ');
    if (index == -1) // No space found
    {
      split_cmd[StringCount++] = message;
      break;
    }
    else
    {
      split_cmd[StringCount++] = message.substring(0, index);
      message = message.substring(index+1);
    }
  }
}

void show_split(){
  for(int i = 0; i < 5; i++){
    Serial.print(split_cmd[i]);
  } 
}

void wipeString(){
  message = "";
  messageBuffer = "";
  for(int i = 0; i < 5; i++){
    split_cmd[i] = "";
  } 
  StringCount = 0;
}

void CommandHandler(){
  SplitString();
  delay(20);
  //Serial.print(split_cmd[0]);

  if(split_cmd[0].equals("type")){
    TypeOnScreen(split_cmd[1]);
  }
  if(split_cmd[0].equals("move")){
    MouseOnScreen(split_cmd);
  }
}

void TypeOnScreen(String text){
  for(int i = 0; i < text.length() - 1; i++){

    if((int)text.charAt(i) == -118){
      Keyboard.write(' ');
      continue;
    }
    if((int)text.charAt(i) == -117){
      Keyboard.write((char)176);
      continue;
    }
    if((int)text.charAt(i) < 0){
      continue;
    }
    Keyboard.write(text.charAt(i));
    //Serial.print(String((int)text.charAt(i)) + " ");
    delay(10);
  }
  wipeString();
}

void MouseOnScreen(String text[]){
  show_split();
  delay(10);
  Mouse.move(text[1].toInt(),text[2].toInt(),text[3].toInt());
  Serial.print(text[1].toInt());
  Mouse.click(text[4].toInt());
  delay(10);
  wipeString();
  //1 left click
  //2 and 3 right click
  //4 middle click
}


void proccessMessage(){
  for(int i = 0; i < message.length() -1 ; i++){
    if(message.charAt(i) == (char)13){
      message.remove(i,1);
    }
    if(message.charAt(i) == (char)10){
      message.remove(i,1);
    }
  }
}

void charPrint(){
  for(int i = 0; i < message.length() -1 ; i++){

    Serial.print(String((int)message.charAt(i)) + " / ");

  }
}

void loop() {

  while (BTSerial.available() > 0) {
    char data = (char) BTSerial.read();
    messageBuffer += data;
    //Serial.print(messageBuffer);
    if (data == ';'){
      message = messageBuffer;
      messageBuffer = "";

      proccessMessage();
      Serial.print(message); // send to serial monitor
      charPrint();
      delay(10);
      CommandHandler();
      // message = "You sent " + message;
      BTSerial.print(message); // send back to bluetooth terminal
    }
  }
}

