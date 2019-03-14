#ifndef ROAD_H_
#define ROAD_H_
#include <iostream>
#include "string"
#include "vector"
#include "../lib/list.h"
//#include <stdlib.h>
using namespace std;
#define Road_Num    2000

typedef struct CarState
{  
    int flag;
    int car_dirction;
    int car_id;
    int	car_position;
    int car_speed;
    vector <int> car_passed;
    vector <int> car_pass;
};
//你的代码写在这里
/*class Road_status{
   
};*/
class Road{
 public:
    int id;
    int length;
    int limit_speed;
    int channel;
    int start;
    int end;
    int flag_bothway;
    Linklist &Cur_Road;
    Road(Linklist a);
//    Road();
};


//    List<int> *p;
//void set(int channel,int length,int flag_bothway);
    /*20190312 GJL road up car state
   vector<vector<vector<CarState>>> situation;
   void input_data(int x,int y,int z,int flag_data,int car_dirction_data,int car_id_data,int car_position_data,int car_speed_data,int car_passed_data,int car_pass_data);

 
       for(int k=0;k<road_num;k++)
       {
	road[k].situation.resize(road[k].channel);
	  for(int i=0;i<road[k].channel;i++)
	  {
	    road[k].situation[i].resize(road[k].length);
	      for(int j=0;j<road[k].length;j++)
		road[k].situation[i][j].resize(road[k].flag_bothway);
	  }	 	 
	}
       
       //road[1].situation[0][0][0].flag=1;
       //input_data(int channel,int length,int flag_bothway,int flag_data,int car_dirction_data,int car_id_data,int car_position_data,int car_speed_data,int car_passed_data,int car_pass_data);
       road[1].input_data(2,9,1,0,1,2,3,4,11,23);
       */  
   
   
   
    // void stringTOnum1(string s,  int * pdata);
Road::Road(Linklist a) :Cur_Road(a)
{
	//ptr = nullptr;
}
#endif