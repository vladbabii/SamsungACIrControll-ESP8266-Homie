#define COMMANDCOOL 1
#define COMMANDDRY 1
#define COMMANDFAN 1
#define COMMANDHEAT 1
#define COMMANDAUTO 1
#define COMMANDOFF !

#define MAX_SIG_SIZE 400

#include <Homie.h>
#include <IRremoteESP8266.h>

IRsend irsend(5);

unsigned int signal[MAX_SIG_SIZE];
int khz;
int len;
int pos;

HomieNode irNode("raw", "ir");


String getStringPartByNr(String data, char separator, int index) {
    int stringData = 0;        //variable to count data part nr 
    String dataPart = "";      //variable to hole the return text

    for(int i = 0; i<data.length(); i++) {    //Walk through the text one letter at a time

        if(data[i]==separator) {
            //Count the number of times separator character appears in the text
            stringData++;

        }else if(stringData==index) {
            //get the text when separator is the rignt one
            dataPart.concat(data[i]);

        }else if(stringData>index) {
            //return text and stop if the next separator appears - to save CPU-time
            return dataPart;
            break;

        }

    }
    //return text if this is the last part
    return dataPart;
}

bool rawOnHandler(String value) {
  Serial.println("!");
  Serial.println(value);

  Serial.println("---");
  String st;
  st = getStringPartByNr(value,',',0);
  int maxi = st.toInt();
  int i;
  for(i=1;i<=maxi;i++){
    st=getStringPartByNr(value,',',i);
    signal[pos]=st.toInt();
    Serial.print(pos);
    Serial.print(" - ");
    Serial.println(signal[pos]);
    pos++;
  }
  Serial.print("Size: ");
  Serial.println((pos+1));
  return true;
}


bool khzOnHandler(String value) {
  khz = value.toInt();
  Serial.print("khz = ");
  Serial.println(khz);
  return true;
}

bool lengthOnHandler(String value) {
  len = value.toInt();
  if(len> MAX_SIG_SIZE){
    len=MAX_SIG_SIZE;
  }
  pos=0;
  Serial.print("length = ");
  Serial.println(len);
  return true;
}

bool sendOnHandler(String value) {
  Serial.println("Sending...");
  int i;
  for(i=0;i<len;i++){
    Serial.print(signal[i]);
    Serial.print(",");
  }
  Serial.println();
  irsend.sendRaw(signal,len,khz);
  Serial.println("Sent!");
  return true;
}

int sendBatch(const unsigned int* Storage){
  unsigned int Size;
  int counter;
  Size = pgm_read_word_near(Storage);
  if(MAX_SIG_SIZE<Size){
    Size = MAX_SIG_SIZE;
  }
  for(counter=2;counter<Size;counter++){
    signal[counter-2]=pgm_read_word_near(Storage+counter+1);
    #ifdef DEBUGUNCODE
    Serial.print(counter);
    Serial.print(" - ");
    Serial.println(signal[counter-2]);
    #endif
  }
  #ifdef DEBUGUNCODE
    Serial.print(F("Sending "));
    Serial.print(Size);
    Serial.println(" array elements");
  #endif
  irsend.sendRaw(signal, Size-2, 38);
  return 0;
}

bool cmdOnHandler(String value) {
  String st;
  String fan;
  String temp;  
  int fani;
  int tempi;
  st=getStringPartByNr(value,',',0);
  Serial.println(st);
  if(st=="cool" || st=="heat"){
    fan=getStringPartByNr(value,',',1);
    temp=getStringPartByNr(value,',',2);
    if(fan.length()>0 && temp.length()>0){
        Serial.println(fan);
        Serial.println(temp);
        fani=fan.toInt();
        tempi=temp.toInt();
        if(st=="cool"){
          Cmd_Cool_Fan_Temp(fani,tempi);
        }
        if(st=="heat"){
          Cmd_Heat_Fan_Temp(fani,tempi);
        }
    }
    return true;
  }
  if(st=="dry" || st=="auto"){
    temp=getStringPartByNr(value,',',1);
    if(temp.length()>0){
        Serial.println(temp);
        tempi=temp.toInt();
        if(st=="dry"){
          Cmd_Dry_Temp(tempi);
        }
        if(st=="auto"){
          Cmd_Auto_Temp(tempi);
        }
    }
    return true;
  }
  if(st=="fan"){
    fan=getStringPartByNr(value,',',1);
    if(fan.length()>0){
        Serial.println(fan);
        fani=fan.toInt();
        Cmd_Fan(fani);
    }
    return true;
  }
  if(st=="off"){
    Cmd_off();
    return true;
  }
}

void setup() {
  irsend.begin();
  Homie.setFirmware("ircontroller", "1.2.0");
  irNode.subscribe("raw",     rawOnHandler);
  irNode.subscribe("length",  lengthOnHandler);
  irNode.subscribe("khz",     khzOnHandler);
  irNode.subscribe("send",     sendOnHandler);
  irNode.subscribe("cmd",     cmdOnHandler);
  Homie.registerNode(irNode);
  Homie.setup();
}

void loop() {
  Homie.loop();
}
