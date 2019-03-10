#include "car.h"
#include "math.h"
void Car::distance(int id,int Cross_num)
{
  int Dis,a;
  int flag_updown;
  int Road_Side;
  
  Dis=end-start;
  
  Road_Side=sqrt(Cross_num);
  
  if(Dis<0)
  {
    flag_updown=-1;
    cout<<"move up"<<endl;
  }
  else 
   {
      flag_updown=1;  
      cout<<"move down"<<endl;
   }
  if(Dis%Road_Side==0)
  {
    a=Dis/Road_Side;
  }
  else{
    
  }
}