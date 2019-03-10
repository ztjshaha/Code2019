#ifndef ROAD_H_
#define ROAD_H_
#include <iostream>
#include "string"
using namespace std;
#define MAX_ROAD    2000
//你的代码写在这里
class Road{
 public:
    int id;
    int road_length;
    int limit_speed;
    int lane_num;
    int start;
    int end;
    int flag_twoway;
   void stringTOnum1(string s,  int * pdata);

};
#endif