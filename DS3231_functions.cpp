#include "DS3231_functions.h"



DS3231_functions::bcd_Dec(char b) 

{ return (b/16)*10 + (b%16); }

DS3231_functions::Read_Display_Time_Date()

{
int file;
   printf("Starting the DS3231 test application\n");
   if((file=open("/dev/i2c-1", O_RDWR)) < 0){
      perror("failed to open the bus\n");
      return 1;
   }
   if(ioctl(file, I2C_SLAVE, 0x68) < 0){
      perror("Failed to connect to the sensor\n");
      return 1;
   }
   char writeBuffer[1] = {0x00};
   if(write(file, writeBuffer, 1)!=1){
      perror("Failed to reset the read address\n");
      return 1;
   }
   char buf[BUFFER_SIZE];
if(read(file, buf, BUFFER_SIZE)!=BUFFER_SIZE){
      perror("Failed to read in the buffer\n");
      return 1;
   }
   printf("The RTC time is %02d:%02d:%02d\n", bcdToDec(buf[2]),
         bcd_Dec(buf[1]), bcd_Dec(buf[0]));
   close(file);
   return 0;


}

DS3231_functions::DS3231_functions(){

    
}
