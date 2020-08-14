#define buffLength 8850
#define sensorInput PA0
#define threshold 8

float pres;float prev1; float prev2; float prev3; float prev4; float prev5;
float localAvg;
unsigned int readValue;
unsigned int timeout = buffLength+1;
byte buff[buffLength*2];

void setup(){
  Serial.begin(230400);
  pres=analogRead(sensorInput);
  pinMode(PB12,OUTPUT);
  digitalWrite(PB12,HIGH);
}

void loop(){
  readValue=analogRead(sensorInput);
  prev5=prev4;prev4=prev3;prev3=prev2;prev2=prev1;prev1=pres;
  pres=readValue*0.1+pres*0.9;
  localAvg=(prev1+prev2+prev3+prev4+prev5)/5;

  // If significant sound occurs,
  if(abs(pres-localAvg)>threshold){
    // Check if buffer recording has started
    if(timeout > buffLength){
      // If buffer recording hasn't started,
      // Reset buffer pointer
      timeout=buffLength;
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
      digitalWrite(PB12,LOW);
      Serial.write('\n');Serial.write('\n');
      Serial.write(buff,buffLength*2);
      byte temp[10] ={highByte((unsigned int)prev4),lowByte((unsigned int)prev4),highByte((unsigned int)prev3),lowByte((unsigned int)prev3),highByte((unsigned int)prev2),lowByte((unsigned int)prev2),highByte((unsigned int)prev1),lowByte((unsigned int)prev1),highByte((unsigned int)pres),lowByte((unsigned int)pres)};
      Serial.write(temp,10);
      // I noticed that sending 0 is much faster than sending other values.
      Serial.write('\0');Serial.write('\0');
      digitalWrite(PB12,HIGH);
      timeout--;
    }
  }
}
