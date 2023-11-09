const byte CLOCKOUT = 9;   // Uno, Duemilanove, etc.

void setup ()
{
  // set up 8 MHz timer on CLOCKOUT (OC1A)
  pinMode (CLOCKOUT, OUTPUT); 
  
  TCCR1A = bit (COM1A0);
  TCCR1B = bit (WGM12) | bit (CS10);
  OCR1A =  0;
  

}

void loop ()
{

}