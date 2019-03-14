#include "iostream"
#include "string"
#include <sys/time.h> 
#include "fstream"
#include <limits.h>
#include <valarray>
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
//Linklist ls1;




Car 	car[Car_Num];
Cross   cross[Cross_Num];
map map1;
#define MAX 100000//__INT_MAX__
//int data[7]={10,22,31,45,56,65,70};
//int out_data[7];
//struct  CarState a[1][1][1];
std::vector<std::vector<std::vector<int> > > a(4);
extern int road_num,car_num,cross_num;

vector<vector<int>> road_length;
vector<vector<vector<int>>> path;
//vector<int>::iterator smallest_path;
//vector<int> path[cross_num][cross_num];
//vector<int> dist;
//vector<bool> visited;


/*void Dijkstra(int number,int cross_num)
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
*/


void Dijkstra(int cross_num)//,vector<int>& path[cross_num][cross_num])
{
  int dis[cross_num][cross_num];        //存储源点到各个顶点的最短路径
  //vector<int> path[cross_num][cross_num];
   for (int i = 0; i < cross_num; i++) 
   {
    for (int j = 0; j < cross_num; j++)              //初始化
	{
		dis[i][j] = road_length[i][j];
		path[i][j].push_back(i+1);
		//path[i][j]=(i+1);
		path[i][j].push_back(j+1);
	}
   }
   
   for (int k = 0; k < cross_num; k++)
   {
	for (int i = 1; i < cross_num; i++)
	{
		for (int j = 0; j < cross_num; j++)
		{
				//dis[i] = min(dis[i],dis[j] + L[j][i]);
			if (dis[k][i] > dis[k][j] + road_length[j][i])               //求源点到节点的最短路径，利用现有的L矩阵
			{
				dis[k][i] = dis[k][j] + road_length[j][i];		
				path[k][i].clear();                         //保存并更新路径
				path[k][i].insert(path[k][i].end(), path[k][j].begin(),path[k][j].end());
				path[k][i].push_back(i+1);
			}
 
		}
	
		for (int m = 0; m < i; m++)              //更新节点最短路径
		{
			for (int j = 0; j < cross_num; j++)
				{
					if (dis[k][m] > dis[k][j] + road_length[j][m])
					{
						dis[k][m] = dis[k][j] + road_length[j][m];
						path[k][m].clear();                     //保存并更新路径
						path[k][m].insert(path[k][m].end(), path[k][j].begin(), path[k][j].end());
						path[k][m].push_back(m + 1);
					}
				}
		}
    }
   }
	vector<int>::iterator ite;
	int z;
	for (int k = 0; k < cross_num; k++)
	{
		for (int i = 0; i < cross_num; i++)
		{
			cout << "Source " << k + 1 << " to " << i + 1 << " the shortest " << dis[k][i] << endl << " Path:";			
			for (ite = path[k][i].begin(); ite != path[k][i].end(); ++ite) {
			  if (ite == path[k][i].begin())
				{
					cout << *ite;
					//path_out[k][i].;
				  
				}
					else
					{
					cout << "->" << *ite;
					//path_out[i][z]=*ite;
					}
					//cout<<path[k][i].size()<<endl;
			}
			
			cout << endl;
			
		}
	}

	
	

}
/*
void Search_Path(int start,int end)
{
  vector<int>::iterator ite;
  int z=path[start][end].size();
  int path[z];
  for (ite = path[start][end].begin(); ite != path[start][end].end(); ++ite) {
			  if (ite == path[start][end].begin())
				{
					cout << *ite;
					//path_out[k][i].;
				  
				}
					else
					{
					cout << "->" << *ite;
					//path_out[i][z]=*ite;
					}
			}
}
*/
int main(int argc, char *argv[])
{

     	
    //linkptr[1]->Cur_Road.AddNode
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
	 road_num++;
         }
       }
	road_file.close();
	ifstream road_file_1(argv[2]);
	Road linkptr[road_num];
//	Road * linkptr[road_num];
	
//	Linklist car_list[road_num];
//	Road road(car_list[0],1);
//	Road 	road[Road_Num];
//	Linklist  car_list[Road_Num];
	
/*	for(int i=0;i<road_num;i++)
	{
	  
	  Road road(car_list,i);
	  
	  linkptr[i] = & road;
	  if(i>9)
	    cout<<linkptr[i-10]->id<<"     "<<linkptr[i]->id<<endl;
//	  road[i].Cur_Road = car_list[i];
	}*/
	int num=0;
       	while(!road_file_1.eof())
	{
	char str_road[50];

	road_file_1.getline(str_road,'\r\n');
	if(str_road[0]=='#');
        if(str_road[0]=='(')
         {
	   
	   sscanf(str_road,"(%d,%d,%d,%d,%d,%d,%d)", 	&(linkptr[num].id),
							&(linkptr[num].length),
							&(linkptr[num].limit_speed),
							&(linkptr[num].channel),
							&(linkptr[num].start),
							&(linkptr[num].end),
							&(linkptr[num].flag_bothway));

	
	num++;
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
       } //car[1].situation.car_dirction
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
       int n=1;
       printf("Number %d  car_info: %d  %d  %d  %d  %d \n",l,car[l].id,car[l].start,car[l].end,car[l].speed_max,car[l].start_time);
       printf("Number %d  cross: %d  %d  %d  %d  %d \n",m,cross[m].cross_id,cross[m].road_id[0],cross[m].road_id[1],cross[m].road_id[2],cross[m].road_id[3]);
       printf("Number %d  road: %d  %d  %d  %d  %d  %d  %d\n",n,(linkptr[n].id),
	 					    (linkptr[n].length),
						    (linkptr[n].limit_speed),
						    (linkptr[n].channel),
						    (linkptr[n].start),
						    (linkptr[n].end),
						    (linkptr[n].flag_bothway));
       
       printf("road_num: %d  car_num: %d  cross_num: %d \n",road_num,car_num,cross_num);
       /******Test Input Datas finished******/

       
       
       /********************creat a map3d*************************/
	int map3d [road_num][road_num][4];
	for(int j=0;j<road_num;j++)
	{
	 map3d[linkptr[j].start-1][linkptr[j].end-1][0]=linkptr[j].id;
	 map3d[linkptr[j].start-1][linkptr[j].end-1][1]=linkptr[j].length;
	 map3d[linkptr[j].start-1][linkptr[j].end-1][2]=linkptr[j].limit_speed;
	 map3d[linkptr[j].start-1][linkptr[j].end-1][3]=linkptr[j].channel;
	 if(linkptr[j].flag_bothway==1)
	 {
	   map3d[linkptr[j].end-1][linkptr[j].start-1][0]=map3d[linkptr[j].start-1][linkptr[j].end-1][0];
	   map3d[linkptr[j].end-1][linkptr[j].start-1][1]=map3d[linkptr[j].start-1][linkptr[j].end-1][1];
	   map3d[linkptr[j].end-1][linkptr[j].start-1][2]=map3d[linkptr[j].start-1][linkptr[j].end-1][2];
	   map3d[linkptr[j].end-1][linkptr[j].start-1][3]=map3d[linkptr[j].start-1][linkptr[j].end-1][3];
	  }
	  else{
	    map3d[linkptr[j].end-1][linkptr[j].start-1][0]=MAX;
	    map3d[linkptr[j].end-1][linkptr[j].start-1][1]=MAX;
	    map3d[linkptr[j].end-1][linkptr[j].start-1][2]=MAX;
	    map3d[linkptr[j].end-1][linkptr[j].start-1][3]=MAX;
	  }
	}
	
	//map1.map3d(road_num,&map3d[][][]);
	/***************************creat map3d finished***********************************/
	/**********************************************************************************/
	/***************************creat vector road_length*******************************/
	
	road_length.resize(cross_num);
	//path_out.resize(cross_num);
	for (int k = 0; k < cross_num; ++k){
            road_length[k].resize(cross_num);//每行为c列
	    //path_out[k].resize(cross_num);
        }
	for (int i = 0; i < cross_num; i++)
	{
	  for (int j = 0; j < cross_num; j++)
	  {
	      
	       if((i!=j)&&(map3d[i][j][1]==0))
	       {
		road_length[i][j]=MAX;
		
	       }
	       else
	       {
		 road_length[i][j]=map3d[i][j][1];
	       }
		// path_out[i][j]=0;
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
	
	

	/*******************************Test Map3d*****************************************/
        for(int z=0;z<4;z++)
	   for(int y=0;y<cross_num;y++)
	      for(int x=0;x<cross_num;x++)
	    {
	      if((x!=y)&&(map3d[x][y][z]==0))
		map3d[x][y][z]=MAX;
	       cout << map3d[x][y][z]<<' ';
	       if(x==(cross_num-1))
		 cout<<endl;
	      // road_map[x][y][z]=map3d[x][y][z];
	    }
	    //arr.resize(road_num);
	
	   // cout <<map3d [0][1][0]<<endl;
	/**********************************************************************************/
	/*******************************Test OK!*******************************************/
	/**********************************************************************************/
	// struct  CarState a[road.id][road.channel][road.length][road.flag_bothway];
	//List<float> ls1;//creat list


	//ls1.AddNode(data);
	//ls1.Display();
	//ls1.Search(31);
	//ls1.resShow();
	//vector<int> path[cross_num][cross_num];
	int a1, a2, a3;
	a1 = cross_num; a2 = cross_num; a3 = cross_num;
	path.resize(a1);//定义数组长(第1位)
	for (int i = 0; i < a1; i++)
	{
		path[i].resize(a2);//定义数组宽(第2位)
		//for (int j = 0; j<a2; j++)
			//path[i][j].resize(a3);//定义数组高(第3位)
	}
	//creater car line
	//vector<int> a;
	
	//Road Test(a);
	// TODO:process
	//Dijkstra Test
	Dijkstra(cross_num);
	
	//test of smallest_path
	/*vector<int>::iterator ite;
	for (ite = path[35][5].begin(); ite != path[35][5].end(); ++ite)
	{
	cout<<*ite<<" "; 
	}*/
	//smallest_path=path;
	//test finished
	//cout <<<path[36][36]<endl;
/*	dist.push_back(0);
	for(int i=1;i<cross_num;i++)
	{
	  dist.push_back(MAX);
	}
	visited.resize(cross_num, false);*/
	//
//	for(int i=0;i<cross_num;i++)
//	cout<<dist[i]<<endl;
	
	// TODO:write output file
	
	return 0; 
}