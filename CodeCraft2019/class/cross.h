#ifndef CROSS_H_
#define CROSS_H_
#include <iostream>
#include "string"
#include <vector>
using namespace std;
#define Cross_Num    1000
//你的代码写在这里
class Cross{
 public:
    int cross_id;
    int road_id[4];
    void cross(int cross_num,int car_num,vector<int>&car);
};
#endif