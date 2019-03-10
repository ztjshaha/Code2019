#ifndef ROAD_H_
#define ROAD_H_
#include <iostream>
#include "string"
using namespace std;
#define Road_Num    2000
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
  // void stringTOnum1(string s,  int * pdata);

};
#endif