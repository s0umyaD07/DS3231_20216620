#include "DS3231_functions.h"
#include<fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<linux/i2c-dev.h>
#include<stdio.h>
#include<iostream>
#include<sstream>
#include<iomanip>
#include<linux/i2c.h>
#include<DS3231.h>
#include<wire.h>

#define BUFFER_SIZE 19

//DS3231 Registers with the address
#define DS3231_SECONDS  0x00
#define DS3231_MINUTES  0x01
#define DS3231_HOURS  0x02
#define DS3231_DAY 0x03
#define DS3231_DATE 0x04
#define DS3231_MONTH 0x05
#define DS3231_YEAR 0x06
#define DS3231_ALARM1_SECONDS 0x07
#define DS3231_ALARM1_MINUTES 0x08
#define DS3231_ALARM1_HOURS 0x09
#define DS3231_ALARM1_DATE 0x0a
#define DS3231_ALARM2_MINUTES 0x0b
#define DS3231_ALARM2_HOURS 0x0c
#define DS3231_ALARM2_DAY 0x0d
#define DS3231_CONTROL 0x0e
#define DS3231_CTL_STATUS 0x0f
#define DS3231_AGING_OFFSET 0x10
#define DS3231_TEMP_MSB 0x11
#define DS3231_TEMP_LSB 0x12 



using namespace std;

int bcdToDec(char b)

 { 
   
   return (b/16)*10 + (b%16); }

char decToBCD(int d)
    {
        return ((d/10 * 16) + (d % 10));
    }

DS3231_functions::DS3231_functions(unsigned int bus, unsigned int device) {
	this->file=-1;
	this->bus = bus;
	this->device = device;
	this->open();
}

int DS3231_functions::open(){
   string name;
    name = "/dev/i2c-1";


   if((this->file=::open(name.c_str(), O_RDWR)) < 0){
      perror("Failed to open the bus\n");
	  return 1;
   }
   if(ioctl(this->file, I2C_SLAVE, this->device) < 0){
      perror("Failed to connect to the DS3231\n");
	  return 1;
   }
   return 0;
}

int DS3231_functions::write_Register(unsigned int registerAddress, unsigned int value){
   unsigned char buffer[2];
   buffer[0] = registerAddress;
   buffer[1] = decToBCD(value);
   if(::write(this->file, buffer, 2)!=2){
      perror(" Failed write to the register\n");
      return 1;
   }
   return 0;
}


int DS3231_functions::write(unsigned char value){
   unsigned char buffer[1];
   buffer[0]=value;
   if (::write(this->file, buffer, 1)!=1){
      perror("Failed to write to the device\n");
      return 1;
   }
   return 0;
}

unsigned char DS3231_functions::read_Register(unsigned int registerAddress){
   this->write(registerAddress);
   unsigned char buffer[1];
   if(::read(this->file, buffer, 1)!=1){
      perror("Failed to read from register.\n");
      return 1;
   }
   return buffer[0];
}

int DS3231_functions::Read_Display_Time_Date(){

   this->open;
   char buffer[6];

   buffer[0]=this->read_Register(DS3231_SECONDS);
   buffer[1]=this->read_Register(DS3231_MINUTES);
   buffer[2]=this->read_Register(DS3231_HOURS);

   buffer[3]=this->read_Register(DS3231_DATE);
   buffer[4]=this->read_Register(DS3231_MONTH);
   buffer[5]=this->read_Register(DS3231_YEAR);
   
   

   printf("The RTC time is %02d:%02d:%02d\n", bcdToDec(buffer[2]),
         bcdToDec(buffer[1]), bcdToDec(buffer[0]));

         printf("The RTC Date is %02d.%02d.%02d\n", bcdToDec(buffer[3]),
         bcdToDec(buffer[4]), bcdToDec(buffer[5]));
   this->close();
   return 0;

}

int DS3231_functions::Read_Display_Temp(){

  this->open;
   char buffer[1];

buffer[0] = this->read_Register(DS3231_TEMP_LSB);

printf("The RTC Temperature in LSB unit is %02d\n", bcdToDec(buffer[0]));


   this->close();
   return 0;

}

int DS3231_functions::Set_Time_Date(unsigned int Date,unsigned int Month,unsigned int Year,unsigned int Hour,unsigned int Minute,unsigned int Second ){

this->open();

this->write_Register(DS3231_DATE,Date);
this->write_Register(DS3231_MONTH,Month);
this->write_Register(DS3231_YEAR,Year);
this->write_Register(DS3231_HOURS,Hour);
this->write_Register(DS3231_MINUTES,Minute);
this->write_Register(DS3231_SECONDS,Second);

this->close();


}

int DS3231_functions::Set_Alarm1(unsigned int Date,unsigned int Hour,unsigned int Minute,unsigned int Second){
this->open;

this->write_Register(DS3231_ALARM1_DATE,Date);
this->write_Register(DS3231_ALARM1_HOURS,Hour);
this->write_Register(DS3231_ALARM1_MINUTES,Minute);
this->write_Register(DS3231_ALARM1_SECONDS,Second);
this->close();

}

int DS3231_functions::Read_Alarm1(){

this->open;
   char buffer[4];

   buffer[0]=this->read_Register(DS3231_ALARM1_DATE);
   buffer[1]=this->read_Register(DS3231_ALARM1_HOURS);
   buffer[2]=this->read_Register(DS3231_ALARM1_MINUTES);

   buffer[3]=this->read_Register(DS3231_ALARM1_SECONDS);
   
   

   printf("The RTC Alarm1 time is %02d:%02d:%02d\n", bcdToDec(buffer[1]),
         bcdToDec(buffer[2]), bcdToDec(buffer[3]));

         printf("The RTC Alarm1 Date is %02d\n", bcdToDec(buffer[0]));
   this->close();
   return 0;

}

int DS3231_functions::Set_Alarm2(unsigned int Day,unsigned int Hour,unsigned int Minute){
this->open;

this->write_Register(DS3231_ALARM2_DAY,Date);
this->write_Register(DS3231_ALARM2_HOURS,Hour);
this->write_Register(DS3231_ALARM2_MINUTES,Minute);

this->close();

}


int DS3231_functions::Read_Alarm2(){

this->open;
   char buffer[3];

   buffer[0]=this->read_Register(DS3231_ALARM2_DAY);
   buffer[1]=this->read_Register(DS3231_ALARM2_HOURS);
   buffer[2]=this->read_Register(DS3231_ALARM2_MINUTES);

 string weekDays[7]= {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};//Declare array to for day
        int day = bcdToDec(buffer[0]) % 10; //Get the unit digit of the decimal value to display day mentioned in array
   
   

   printf("The RTC Alarm2 time is %02d:%02d\n", bcdToDec(buffer[1]),
         bcdToDec(buffer[2]));

         printf("The RTC Alarm2 Date is %02d\n",weekDays[day]);
   this->close();
   return 0;

}

int DS3231_functions::Interrupt_SqwGenerator()
{

   
}
