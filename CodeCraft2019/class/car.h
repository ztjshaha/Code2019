#ifndef CAR_H_
#define CAR_H_
#include <iostream>
#include "string"
#include <vector>
#include <deque>
using namespace std;
#define Car_Num    10000
typedef struct Car_Situation
{  
   // int set_out;        //1 已经出发 
			//0 还未出发
  
    int flag;		//1 等待出发
			//0 等待行驶
			//-1 终止行驶
			
			
    int car_dirction;
    int	car_position;
    int car_channel;// 1 2 3分别三个车道数据
    int road_id;
    int car_speed;
    bool token;  //第一优先级车辆标志，是则为1，不是则为0
    deque <int> car_passed;
    deque <int> car_pass;
    int car_turn;	//D 	:	 1
			//L	:	 0
			//R	:	-1
			
    int is_moved;	//0 move 
			//1 no move
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
    //vector<int> car_path;
    Car()
    {
      //situation.set_out=NULL;
      situation.flag=NULL;
      situation.car_dirction=NULL;
      situation.car_position=NULL;
      situation.car_channel=NULL;
      situation.road_id=NULL;
      situation.car_speed=NULL;
      situation.token=NULL;
      //situation.car_passed=NULL;
      //situation.car_pass=NULL;
      situation.car_turn=NULL;
      situation.is_moved=NULL;
    }
    void car_init();
    
};

#endif