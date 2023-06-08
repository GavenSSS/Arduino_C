#include "QhDHT11.h" 

#define TIMEOUT 10000       

// return values:  
//  0 : OK  
// -1 : checksum error  
// -2 : timeout  
int QhDHT11::getTHValue(uint8_t pin){  
	int rv = read(pin);  
	if (rv != 0) return rv;  
	  
	humidity= bits[0];  // bit[1] == 0;  
	temperature = bits[2];  // bits[3] == 0;  
	uint8_t sum = bits[0] + bits[2]; // bits[1] && bits[3] both 0  
	if (bits[4] != sum) return -1;  
	return 0;  
}  
  
  
// return values:  
//  0 : OK  
// -2 : timeout  
int QhDHT11::read(uint8_t pin)  {  
	uint8_t cnt = 7;  
	uint8_t idx = 0;  
	  
	for (int i=0; i< 5; i++) bits[i] = 0;  
	  
	pinMode(pin, OUTPUT);  
	digitalWrite(pin, LOW);  
	delay(20);  
	digitalWrite(pin, HIGH);  
	delayMicroseconds(40);  
	pinMode(pin, INPUT);  
	  
	unsigned int loopCnt = TIMEOUT;  
	while(digitalRead(pin) == LOW)  
		if (loopCnt-- == 0) return -2;  
	  
	loopCnt = TIMEOUT;  
	while(digitalRead(pin) == HIGH)  
	if (loopCnt-- == 0) return -2;  
	  
	for (int i=0; i<40; i++){  
		loopCnt = TIMEOUT;  
		while(digitalRead(pin) == LOW)  
		if (loopCnt-- == 0) return -2;  
		  
		unsigned long t = micros();  
		  
		loopCnt = TIMEOUT;  
		while(digitalRead(pin) == HIGH)  
			if (loopCnt-- == 0) return -2;  
		  
		if ((micros() - t) > 40) bits[idx] |= (1 << cnt);  
		if (cnt == 0){  
			cnt = 7; 
			idx++;
		}  
		else cnt--;  
	}  
	return 0;  
}  
