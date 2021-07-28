
#include<iostream>
#include "DS3231_functions.h"

using namespace std ;

int main()
{
   
DS3231_functions d;



d.Read_Display_Time_Date();
return 0;
}

void DS3231::togglealarm(int num, unsigned char toggle){
	unsigned char temp;
	temp = (0x04 | (toggle << (num-1)));
	setregister(0x0E, temp);
}

void DS3231::sqwgen(unsigned char temp){
	temp = (0x40 | temp<<3);
	setregister(0x0E, temp);
}

void DS3231::controlregister(){
	char buf[BUFFER_SIZE];
	if(read(this->file, buf, BUFFER_SIZE)!=BUFFER_SIZE){
		perror("IC2: Failed to read in the full buffer.\n");
	}
	cout<<"The control ";
	bytetobin(buf[14]);
	cout<<endl;
	cout<<"The status ";
	bytetobin(buf[15]);
	cout<<endl;
}

void DS3231::alarmturnoff(){
	unsigned char temp;
	char buf[BUFFER_SIZE];
	if(read(this->file, buf, BUFFER_SIZE)!=BUFFER_SIZE){
		perror("IC2: Failed to read in the full buffer.\n");
	}
	temp = (buf[15] & 0xFC);
	setregister(0x0F, temp);
}

