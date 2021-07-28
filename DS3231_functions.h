  
#include <iostream>
#include <string>
using namespace std ;

class DS3231_functions
{   private:
	    unsigned int bus;
	    unsigned int device;
	    int file;


    public:

    DS3231_functions(unsigned int bus, unsigned int device);
	virtual int open();
	virtual int write(unsigned char value);
	virtual unsigned char read_Register(unsigned int registerAddress);
    virtual int write_Register(unsigned int registerAddress, unsigned int value);
    virtual void close();
    virtual int Read_Display_Time_Date();
    virtual int Read_Display_Temp();
    virtual int Set_Time_Date(unsigned int Date,unsigned int Month,unsigned int Year,unsigned int Hour,unsigned int Minute,unsigned int Second ) ;
    virtual int Set_Alarm1(unsigned int Date,unsigned int Hour,unsigned int Minute,unsigned int Second );
    virtual int Read_Alarm1();

      virtual int Set_Alarm2(unsigned int Day,unsigned int Hour,unsigned int Minute );
    virtual int Read_Alarm2();
    virtual int Interrupt_SqwGenerator();



    virtual ~DS3231_functions();
};
