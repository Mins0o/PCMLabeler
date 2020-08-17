#define sensorInput A3
#define threshold 5
#define durationWindow 200

unsigned int pres;unsigned int prev1;unsigned int prev2;unsigned int prev3;unsigned int prev4;
unsigned int timeout=durationWindow+1;
unsigned int readValue;
unsigned int localAvg;

void setup(){
  Serial.begin(230400);

}

void loop(){
  readValue=analogRead(sensorInput);
  prev4=prev3;prev3=prev2;prev2=prev1;prev1=pres;
  pres=readValue;//You could add filtering here
  localAvg=(prev1+prev2+prev3+prev4)/4;
  
  // When significant sound occurs
  if(abs((int)(pres-localAvg)) > threshold){
	  
	// Transmission has terminated before this trigger
    if(timeout > durationWindow){
      // New data transmission begins
      Serial.write(0);
    }
	
    // Renew timeout
    timeout = durationWindow;
  }
  
  // While the time hasn't ran out(When it runs out, it overflows)
  if(timeout < durationWindow){
	
	// Printout the reading.
	// prev4 is selected to give a slightly broader picture of the signal.
    byte singlePackage[3]={highByte(prev4),lowByte(prev4),'\n'};
    Serial.write(singlePackage,3);
    timeout--;
  }
  
  // End of transmission stream.
  if(timeout==0){
      Serial.write(0);Serial.write(0);Serial.write(0);Serial.write(0);Serial.write('\n');
      timeout--;
  }
}
