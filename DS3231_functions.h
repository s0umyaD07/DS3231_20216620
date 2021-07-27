#include <iostream>
#include <string>
using namespace std ;

class DS3231_functions
{

    public:

    DS3231_functions();

    

      int Read_Display_Time_Date();
    

    void Read_Display_Temp();

    

    void Set_Time_Date() ;

    void Set_Alarm ();
};

