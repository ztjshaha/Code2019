#include "iostream"
#include "string"
#include <sys/time.h> 
#include "fstream"
/*****************************************/
/**********Add myself class .h************/
/*****************************************/
#include "road.h"
#include "car.h"
#include "cross.h"
using namespace std;
/*****************************************/
/*************Define class****************/
/*****************************************/
Road 	road[Road_Num];
Car 	car[Car_Num];
Cross   cross[Cross_Num];
int main(int argc, char *argv[])
{
	std::cout << "Begin"<< std::endl;
	if(argc < 5){
		std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
	}
	/*************input three files,car.txt,road.txt,cross.txt****************/
	ifstream car_file(argv[1]);
	ifstream road_file(argv[2]);
	ifstream cross_file(argv[3]);
	/**********Decent input files************/
	if(!car_file.is_open())  {cout<<"/**ERROR: Road open file**/"<<endl;return 0;} 
	if(!road_file.is_open())   {cout<<"/**ERROR:Car open file**/"<<endl;return 0;}
	if(!cross_file.is_open())   {cout<<"/**ERROR:Cross open file**/"<<endl;return 0;}	
	std::cout << "carPath is :" << car_file << std::endl;
	std::cout << "roadPath is :" << road_file << std::endl;
	std::cout << "crossPath is :" << cross_file << std::endl;
	
	// TODO:read input filebuf
	std::string s;
	int road_num=0,car_num=0,cross_num=0;
	std::cout<<"read start"<<std::endl;
	while(!road_file.eof())
	{
	char str_tr[50];
	road_file.getline(str_tr,'\r\n');
	if(str_tr[0]=='#');
        if(str_tr[0]=='(')
         {
	   
	   sscanf(str_tr,"(%d,%d,%d,%d,%d,%d,%d)", 	&(road[road_num].id),
							&(road[road_num].road_length),
							&(road[road_num].limit_speed),
							&(road[road_num].lane_num),
							&(road[road_num].start),
							&(road[road_num].end),
							&(road[road_num].flag_twoway));
	  road_num++;
         }
       }
       road_file.close();
	while(!car_file.eof())
	{
	char str_tc[50];
	car_file.getline(str_tc,'\r\n');
	if(str_tc[0]=='#') ;
	if(str_tc[0]=='(')
	  {
	   
	   sscanf(str_tc,"(%d,%d,%d,%d,%d)",   		&(car[car_num].id),
							&(car[car_num].set),
							&(car[car_num].goal),
							&(car[car_num].max_speed),
							&(car[car_num].set_time));
	  car_num++;
         }
       }
       car_file.close();
    while(!cross_file.eof())
      {
       char str_ts[50];
       cross_file.getline(str_ts,'\r\n');
       if(str_ts[0]=='#');
       if(str_ts[0]=='(')
         {
	   
	   sscanf(str_ts,"(%d,%d,%d,%d,%d)",   		&(cross[cross_num].id),
							&(cross[cross_num].road_id[0]),
							&(cross[cross_num].road_id[1]),
							&(cross[cross_num].road_id[2]),
							&(cross[cross_num].road_id[3]));
	cross_num++;
         }
       }
	// TODO:process
	// TODO:write output file
	
	return 0;
}