#ifndef ROAD_H_
#define ROAD_H_
#include <iostream>
#include "string"
#include "vector"
//#include <stdlib.h>
using namespace std;
#define Road_Num    2000
/*
typedef struct CarState
{  
    int flag;
    int car_dirction;
    int car_id;
    int	car_position;
    int car_speed;
    vector <int> car_passed;
    vector <int> car_pass;
};*/
//你的代码写在这里
class Road{
 public:
    int id;
    int length;
    int limit_speed;
    int channel;
    int start;
    int end;
    int flag_bothway;
    
//void set(int channel,int length,int flag_bothway);

   

    
  // void stringTOnum1(string s,  int * pdata);

};
#endif