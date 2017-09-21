#ifndef IRTransmitter_H
#define IRTransmitter_H

#include <Arduino.h>
#include <stdint.h>

#define CLKFUDGE 5      // fudge factor for clock interrupt overhead
#define CLK 256      // max value for clock (timer 2)
#define PRESCALE 8      // timer2 clock prescale
#define SYSCLOCK 16000000  // main Arduino clock
#define CLKSPERUSEC (SYSCLOCK/PRESCALE/1000000)   // timer clocks per microsecond

//typedef unsigned short int uint16_t;
struct uint16x2
{
	uint16_t a; // usec on
	uint16_t b; // usec off
};

class IRTransmitter
{
public:
	IRTransmitter(uint16x2 _leader, 
				  uint16x2 _trailer,
				  uint16x2 _oneBit,
				  uint16x2 _zeroBit, 
				  int _freq) : leader{_leader.a,_leader.b}, 
							   trailer{_trailer.a,_trailer.b}, 
							   oneBit{_oneBit.a,_oneBit.b}, 
							   zeroBit{_zeroBit.a,_zeroBit.b}, 
							   freq(_freq) {}
	void transmitWords(char* words, int length);
private:
	uint16_t leader[2];
	uint16_t trailer[2];
	uint16_t oneBit[2];
	uint16_t zeroBit[2];
	int freq; //kHz
	
	void sendWord(char b);
	
	void sendOn(int n);
	void sendOff(int n);
	
	void sendLeader();
	void sendTrailer();
	void sendOne();
	void sendZero();
	
	void enableIROut(int khz);
};

#endif