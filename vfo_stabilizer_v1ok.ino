#include <FreqPeriodCounter.h>  //
#include <FreqCount.h>   //

#include <AD9833.h>     // Include the library
#define FNC_PIN 8       // Can be any digital IO pin
AD9833 gen(FNC_PIN); 

unsigned long myTime = 0;
float freq_dds = 5000000;
float freq = 0;
const byte counterPin = 2;
const byte counterInterrupt =  0; // 0 to d2, 1 to d3
FreqPeriodCounter counter(counterPin, micros);

float total = 0;                  // the running total
float average = 0;                // the average
int readIndex = 0;
   int i = 0;
void setup(void)
{
   gen.Begin(); 
   gen.EnableOutput(true); 
   //gen.ApplySignal(SQUARE_WAVE,REG0,freq_dds);
   gen.ApplySignal(SINE_WAVE,REG0,freq_dds);
  // 
  attachInterrupt(counterInterrupt, counterISR, CHANGE);
  pinMode(10, OUTPUT);  // DOWN 13 verde
  pinMode(9, OUTPUT);  // up  11  red
  pinMode(7, OUTPUT);  // reset voltage
  pinMode(3, OUTPUT);
   //analogWrite(6,254);
  //  analogWrite(5,254);
  //TCCR2B = TCCR2B & B11111000 | B00000001;
  //TCCR0B = TCCR0B & B11111000 | B00000101;  // 61.04hz
 //TCCR0B = TCCR0B & B11111000 | B00000001;  // 
  //TCCR1A = 0b00000001; 
  //TCCR1B = 0b00001011;  //976 8bits
 //digitalWrite(7, HIGH);
 digitalWrite(3, LOW); // signal pin 2 off
  Serial.begin(57600);
  FreqCount.begin(1000);
 
}

void loop(void)
{ 
 myTime = millis();
 

//Serial.println(myTime);
if ( i <  3){
   if (FreqCount.available()) { 
    unsigned long count = FreqCount.read();
    //freq_dds =  count/.9222;
    freq  =  count/1.000192 +freq;
    i = i + 1;
    Serial.println(i,DEC);
    //Serial.println(freq);
    
    //freq_dds = freq_dds ;
   }
}else {
  if ( i  == 3 ){
   FreqCount.end();
   freq_dds = freq/3.;
   freq_dds = freq_dds-1000;
   //digitalWrite(7, LOW);
  //gen.ApplySignal(SINE_WAVE,REG0,freq_dds);
  //gen.ApplySignal(SQUARE_WAVE,REG1,freq_dds);
  Serial.println(freq_dds);
  
  digitalWrite(3, HIGH);
  i = i + 1;
  }else{
    
 // gen.SetFrequency(REG0,freq_dds);
  //gen.ApplySignal(SQUARE_WAVE,REG1,freq_dds);
  gen.ApplySignal(SINE_WAVE,REG0,freq_dds);
   
  float total = 0;                  // the running total
float average = 0;                // the average
int readIndex = 0;
float fo;
   
  
    if(counter.ready()) {   // no pin 2
    
  while (readIndex < 10) { // util fazer media para freq exata
    float total_m = 0;                  // the running total
float average_m = 0;                // the average
long readIndex_m = 0;
  
  while (readIndex_m < 11) {
    //if(counter.ready()) 
    fo = (float)1000000/counter.period;
    readIndex_m = readIndex_m + 1;
    total_m = total_m + fo ;
  }
  average_m = total_m / readIndex_m;
  total = total + average_m;
  readIndex= readIndex + 1;

  }
  average = total / readIndex;
  }
  
  //average = average;
  
  //average = average/16;
//Serial.print("freq: ");
//Serial.println(total,10);
Serial.println(average,2);
  //float fo = (float)1000000/counter.period;
  //float fo = counter.hertz(10);
  // fo = fo/16;
//Serial.print("freq: ");
//Serial.println(fo,10);
//Serial.print(fo/1000);
//Serial.print('.');
//Serial.println(fo%1000);
  
  //Serial.println(f);
  //delay(1000);
  delay(500);
if (average < 500.0){
  analogWrite(10,184);  // H
  analogWrite(9,8);  // L
  }
  else{
   analogWrite(10,8);  // H
   analogWrite(9,184);  // L
    }
   if (average > 1000.0)  { 
    FreqCount.begin(1000);
    analogWrite(10,255);  // H
  analogWrite(9,255);  // L
  i = 0;
  freq = 0 ;
 digitalWrite(3, LOW); 
 //freq_dds = freq_dds+average;
  //gen.ApplySignal(SINE_WAVE,REG0,freq_dds);
  Serial.println("RESET");
//Serial.println(average,2);
 }  
  }
}

} 
  
void counterISR()
{ counter.poll();
}
