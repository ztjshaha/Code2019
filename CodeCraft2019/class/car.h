#ifndef CAR_H_
#define CAR_H_
#include <iostream>
#include "string"
#include <vector>
using namespace std;
#define Car_Num    10000
typedef struct Car_Situation
{  
   // int flag;
    int car_dirction;
    //int car_id;
    int	car_position;
    int car_channel;
    int road_id;
    int car_speed;
    bool token;  //第一优先级车辆标志，是则为1，不是则为0
    vector <string> car_passed;
    vector <string> car_pass;
    int car_turn;	//D 	:	1
			//L	:	0
			//R	:	-1
};
//你的代码写在这里
class Car{
 public:  
    int id;
    int start;
    int end;
    int speed_max;
    int start_time;  
    Car_Situation situation;
    vector<int> car_path;
    Car()
    {
      situation.car_dirction=NULL;
      situation.car_position=NULL;
      situation.car_channel=NULL;
      situation.road_id=NULL;
      situation.car_speed=NULL;
      situation.token=NULL;
      //situation.car_passed=NULL;
      //situation.car_pass=NULL;
      situation.car_turn=NULL;
    }
    
};
#endif