#define buffLength 870
#define sensorInput A3
#define threshold 5

unsigned int pres;unsigned int prev1; unsigned int prev2; unsigned int prev3; unsigned int prev4; unsigned int prev5;
unsigned int localAvg;
unsigned int readValue;
unsigned int timeout = buffLength+1;
byte buff[buffLength*2];
//unsigned long frequencyTimer;

void setup(){
  Serial.begin(230400);
  //pinModee(sensorInput,INPUT);
  pres=analogRead(sensorInput);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}

void loop(){
  readValue=analogRead(sensorInput);
  prev5=prev4;prev4=prev3;prev3=prev2;prev2=prev1;prev1=pres;
  pres=readValue;//*0.3+pres*0.7;
  localAvg=(prev1+prev2+prev3+prev4+prev5)/5;

  // If significant sound occurs,
  if(abs((int)pres-localAvg))>threshold){
    // Check if buffer recording has started
    if(timeout > buffLength){
      // If buffer recording hasn't started,
      // Reset buffer pointer
      timeout=buffLength;
      //frequencyTimer=millis();
    }
  }
  // If the buffer is being filled
  if(timeout<buffLength+1){
    // Fill buffer
    buff[ (buffLength-timeout)*2 ]=highByte((unsigned int)prev5);
    buff[ (buffLength-timeout)*2+1 ]=lowByte((unsigned int)prev5);
    timeout--;
    
    // If the last buffer was filled
    // Start transmission and proceed one more step in timeout counter
    if (timeout==0){
      /*
      float interval=(millis()-frequencyTimer);
      Serial.print(interval);
      Serial.println("ms");
      Serial.print((float)(buffLength)/interval);
      Serial.println("kHz");
      */
      digitalWrite(13,HIGH);
      Serial.write('\n');Serial.write('\n');
      Serial.write(buff,buffLength*2);
      byte temp[10] ={highByte((unsigned int)prev4),lowByte((unsigned int)prev4),highByte((unsigned int)prev3),lowByte((unsigned int)prev3),highByte((unsigned int)prev2),lowByte((unsigned int)prev2),highByte((unsigned int)prev1),lowByte((unsigned int)prev1),highByte((unsigned int)pres),lowByte((unsigned int)pres)};
      Serial.write(temp,10);
      // I noticed that sending 0 is much faster than sending other values.
      Serial.write(0);Serial.write(0);
      digitalWrite(13,LOW);
      timeout--;
    }
  }
}
