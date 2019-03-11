#include "iostream"
#include "string"
#include <sys/time.h> 
#include "fstream"

/*****************************************/
/**********Add myself lib.h&class .h************/
/*****************************************/
#include "road.h"
#include "car.h"
#include "cross.h"
#include "lib/lib.h"
#include "lib/list.h"
#include "map.h"
/*****************************************/
/*************Define lib&class****************/
/*****************************************/
using namespace std;
List<float> ls1;
Road 	road[Road_Num];
Car 	car[Car_Num];
Cross   cross[Cross_Num];
map map1;
#define MAX 100000
//struct  CarState a[1][1][1];
std::vector<std::vector<std::vector<int> > > a(4);
extern int road_num,car_num,cross_num;
vector<vector<int> > road_length;
vector<int> dist;
vector<bool> visited;


void Dijkstra(int number,int cross_num)
{
	//标记为访问过
	visited[number] = true;
	cout << number << endl;
	//dist[i]数组中保存了从原点到i点的距离，dist[number]则表示从原点到number的距离，如果dist[number]+Cost[number][i]<dist[i],则更新。
	for (int i = 1; i < cross_num; i++)//5 road_x or road_y
	{
		if (dist[number]+road_length[number][i]<dist[i])
		{
			dist[i] = dist[number] + road_length[number][i];
		}
	}
	//接下来时找出为访问过的最小值的点a
	int min=MAX, a=0;
	for (int i = 0; i < cross_num; i++)
	{
		if (dist[i]<min&&!visited[i])
		{
			min = dist[i];
			a = i;
		}
	}
	
	//如果这一点没有被访问过，则访问之
	if (!visited[a])
	{
		Dijkstra(a,cross_num);
	}
	
	
}




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
	char str_road[50];
	road_file.getline(str_road,'\r\n');
	if(str_road[0]=='#');
        if(str_road[0]=='(')
         {
	   
	   sscanf(str_road,"(%d,%d,%d,%d,%d,%d,%d)", 	&(road[road_num].id),
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
	char str_car[50];
	car_file.getline(str_car,'\r\n');
	if(str_car[0]=='#') ;
	if(str_car[0]=='(')
	  {
	   
	   sscanf(str_car,"(%d,%d,%d,%d,%d)",   	&(car[car_num].id),
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
       char str_cross[50];
       cross_file.getline(str_cross,'\r\n');
       if(str_cross[0]=='#');
       if(str_cross[0]=='(')
         {
	   
	   sscanf(str_cross,"(%d,%d,%d,%d,%d)",   	&(cross[cross_num].cross_id),
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
	
	//map1.map3d(road_num,&map3d[][][]);
	/***************************creat map3d finished***********************************/
	/**********************************************************************************/
	/***************************creat vector road_length*******************************/
	road_length.resize(cross_num);
	for (int k = 0; k < cross_num; ++k){
            road_length[k].resize(cross_num);//每行为c列
        }
	for (int i = 0; i < cross_num; i++)
	{
	  for (int j = 0; j < cross_num; j++)
	  {
	      
	       if((i!=j)&&(map3d[i][j][1]==0))
		road_length[i][j]=MAX;
	       else
		 road_length[i][j]=map3d[i][j][1];
               // a[k][j][i] = map3d[i][j][k];
	  }
	}
	for (int i = 0; i < cross_num; i++)
	{
	  for (int j = 0; j < cross_num; j++)
	  {
	      cout<<road_length[i][j]<<" ";
	      if(j==(cross_num-1))
		cout<<endl;
               // a[k][j][i] = map3d[i][j][k];
	  }
	}
	
	
	/***********************************************************************************/
	
	/*for (int i = 0; i < road_num; i++)
	{
	  for (int j = 0; j < road_num; j++)
	  {
	      a[i][j].resize(road_num);
	  }//之后才能对三维数组设定大小，否则内存出错
      }
	for (int i = 0; i < road_num; i++)
	{
	  for (int j = 0; j < road_num; j++)
	  {
            for (int k = 0; k < 4; k++)
            {
                a[k][j][i] = map3d[i][j][k];
            }
	  }
	}*/

      /*for (int i = 1; i < 2; i++)
      {
        for (int j = 0; j < cross_num; j++)
        {
            for (int k = 0; k < cross_num; k++)
            {
               std::cout << a[k][j][i] << std::endl;
            }
        }
    }*/
	/*******************************Test Map3d*****************************************/
        /*for(int z=1;z<2;z++)
	   for(int y=0;y<cross_num;y++)
	      for(int x=0;x<cross_num;x++)
	    {
	      if((x!=y)&&(map3d[x][y][z]==0))
		map3d[x][y][z]=MAX;
	       cout << map3d[x][y][z]<<' ';
	       if(x==(cross_num-1))
		 cout<<endl;
	      // road_map[x][y][z]=map3d[x][y][z];
	    }*/
	    //arr.resize(road_num);
	
	   // cout <<map3d [0][1][0]<<endl;
	/**********************************************************************************/
	/*******************************Test OK!*******************************************/
	/**********************************************************************************/
	// struct  CarState a[road.id][road.channel][road.length][road.flag_bothway];
	//List<float> ls1;//creat list
	//ls1.add(5,5000,10,5,3,2,1);
	//ls1.resShow();
	// TODO:process
	

	dist.push_back(0);
	for(int i=1;i<cross_num;i++)
	{
	  dist.push_back(MAX);
	}
	visited.resize(cross_num, false);
	Dijkstra(0,cross_num);
	for(int i=0;i<cross_num;i++)
	cout<<dist[i]<<endl;
	
	// TODO:write output file
	
	return 0;
}