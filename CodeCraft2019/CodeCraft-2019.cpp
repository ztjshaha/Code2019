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
							&(road[road_num].length),
							&(road[road_num].limit_speed),
							&(road[road_num].channel),
							&(road[road_num].start),
							&(road[road_num].end),
							&(road[road_num].flag_bothway));
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
							&(car[car_num].start),
							&(car[car_num].end),
							&(car[car_num].speed_max),
							&(car[car_num].start_time));
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
	   
	   sscanf(str_ts,"(%d,%d,%d,%d,%d)",   		&(cross[cross_num].cross_id),
							&(cross[cross_num].road_id[0]),
							&(cross[cross_num].road_id[1]),
							&(cross[cross_num].road_id[2]),
							&(cross[cross_num].road_id[3]));
	cross_num++;
         }
       }
       cross_file.close();
       cout<<"All files input finished"<<endl;
       
       /******Test Input Datas******/
       int l=0;
       int m=0;
       int n=0;
       printf("Number %d  car_info: %d  %d  %d  %d  %d \n",l,car[l].id,car[l].start,car[l].end,car[l].speed_max,car[l].start_time);
       printf("Number %d  cross: %d  %d  %d  %d  %d \n",m,cross[m].cross_id,cross[m].road_id[0],cross[m].road_id[1],cross[m].road_id[2],cross[m].road_id[3]);
       printf("Number %d  road: %d  %d  %d  %d  %d  %d  %d\n",n,(road[n].id),
						    (road[n].length),
						    (road[n].limit_speed),
						    (road[n].channel),
						    (road[n].start),
						    (road[n].end),
						    (road[n].flag_bothway));
       
       printf("road_num: %d  car_num: %d  cross_num: %d \n",road_num,car_num,cross_num);
       /******Test Input Datas finished******/
       
       /********************creat a map3d*************************/
       int map3d [road_num][road_num][4];
       for(int j=0;j<road_num;j++)
	{
	 map3d[road[j].start-1][road[j].end-1][0]=road[j].id;
	 map3d[road[j].start-1][road[j].end-1][1]=road[j].length;
	 map3d[road[j].start-1][road[j].end-1][2]=road[j].limit_speed;
	 map3d[road[j].start-1][road[j].end-1][3]=road[j].channel;
	 if(road[j].flag_bothway==1)
	 {
	   map3d[road[j].end-1][road[j].start-1][0]=map3d[road[j].start-1][road[j].end-1][0];
	   map3d[road[j].end-1][road[j].start-1][1]=map3d[road[j].start-1][road[j].end-1][1];
	   map3d[road[j].end-1][road[j].start-1][2]=map3d[road[j].start-1][road[j].end-1][2];
	   map3d[road[j].end-1][road[j].start-1][3]=map3d[road[j].start-1][road[j].end-1][3];
	  }
	  else{
	    map3d[road[j].end-1][road[j].start-1][0]=-1;
	    map3d[road[j].end-1][road[j].start-1][1]=-1;
	    map3d[road[j].end-1][road[j].start-1][2]=-1;
	    map3d[road[j].end-1][road[j].start-1][3]=-1;
	  }
	}
	/***************************creat map3d finished***********************************/
	
	/*******************************Test Map3d*****************************************/
        for(int z=0;z<4;z++)
	   for(int y=0;y<cross_num;y++)
	      for(int x=0;x<cross_num;x++)
	    {
	       cout << map3d[x][y][z]<<' ';
	       if(x==(cross_num-1))
		 cout<<endl;
	    }
	    cout <<map3d [0][1][0]<<endl;
	// TODO:process
	// TODO:write output file
	
	return 0;
}