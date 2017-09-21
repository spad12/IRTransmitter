#include "IRTransmitter.h"

void IRTransmitter::transmitWords(char* words, int length)
{
	enableIROut(freq);
	sendLeader();
	for(int i=0;i<length;i++)
		sendWord(words[i]);
		
	sendTrailer();
}

void IRTransmitter::sendWord(char b)
{
  for(char i=0;i<8;i++)
  {
    if(b & (1 << i))
      sendZero();
    else
      sendOne();
  }
}

void IRTransmitter::sendOn(int n)
{
  TCCR2A |= _BV(COM2B1); // Enable pin 3 PWM output
  delayMicroseconds(n);
}

void IRTransmitter::sendOff(int n)
{
  TCCR2A &= ~(_BV(COM2B1)); // Disable pin 3 PWM output
  delayMicroseconds(n);
}

void IRTransmitter::sendLeader()
{
	sendOn(leader[0]);
	sendOff(leader[1]);
}

void IRTransmitter::sendTrailer()
{
	sendOn(trailer[0]);
	sendOff(trailer[1]);
}

void IRTransmitter::sendOne()
{
	sendOn(oneBit[0]);
	sendOff(oneBit[1]);
}

void IRTransmitter::sendZero()
{
	sendOn(zeroBit[0]);
	sendOff(zeroBit[1]);
}


void IRTransmitter::enableIROut(int khz) {
  // Enables IR output.  The khz value controls the modulation frequency in kilohertz.
  // The IR output will be on pin 3 (OC2B).
  // This routine is designed for 36-40KHz; if you use it for other values, it's up to you
  // to make sure it gives reasonable results.  (Watch out for overflow / underflow / rounding.)
  // TIMER2 is used in phase-correct PWM mode, with OCR2A controlling the frequency and OCR2B
  // controlling the duty cycle.
  // There is no prescaling, so the output frequency is 16MHz / (2 * OCR2A)
  // To turn the output on and off, we leave the PWM running, but connect and disconnect the output pin.
  // A few hours staring at the ATmega documentation and this will all make sense.
  // See my Secrets of Arduino PWM at http://arcfn.com/2009/07/secrets-of-arduino-pwm.html for details.

  
  // Disable the Timer2 Interrupt (which is used for receiving IR)
  TIMSK2 &= ~_BV(TOIE2); //Timer2 Overflow Interrupt
  
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW); // When not sending PWM, we want it low
  
  // COM2A = 00: disconnect OC2A
  // COM2B = 00: disconnect OC2B; to send signal set to 10: OC2B non-inverted
  // WGM2 = 101: phase-correct PWM with OCRA as top
  // CS2 = 000: no prescaling
  TCCR2A = _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS20);

  // The top value for the timer.  The modulation frequency will be SYSCLOCK / 2 / OCR2A.
  OCR2A = SYSCLOCK / 2 / khz / 1000;
  OCR2B = OCR2A / 3; // 33% duty cycle
}