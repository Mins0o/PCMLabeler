#define sensorInput PA0
#define threshold 5
#define durationWindow 200

unsigned int pres;unsigned int prev1;unsigned int prev2;unsigned int prev3;unsigned int prev4;
unsigned int timeout=durationWindow+1;
unsigned int readValue;
unsigned int localAvg;

void setup(){
  Serial.begin(230400);
  pinMode(sensorInput,INPUT_ANALOG);
}

void loop(){
  readValue=analogRead(sensorInput);
  pres=readValue;
  prev4=prev3;prev3=prev2;prev2=prev1;prev1=pres;
  localAvg=(prev1+prev2+prev3+prev4)/4;
  if(abs((int)(pres-localAvg)) > threshold){
    if(timeout > durationWindow){
      // New data transmission begins
      Serial.write(0);
    }
    // Renew timeout
    timeout = durationWindow;
  }
  if(timeout < durationWindow){
    byte singlePackage[3]={highByte(prev4),lowByte(prev4),'\n'};
    Serial.write(singlePackage,3);
    timeout--;
  }
  if(timeout==0){
      Serial.write(0);Serial.write(0);Serial.write(0);Serial.write(0);Serial.write('\n');
      timeout--;
  }
}
