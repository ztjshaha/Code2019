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

//update version
//vector<Car> car;
vector<Road> road;
Car car[Car_Num];
Cross cross[Cross_Num];
//Car 	car[Car_Num];
//vector<Cross>   cross;

map map1;
#define MAX 100000//__INT_MAX__
//int data[7]={10,22,31,45,56,65,70};
//int out_data[7];
//struct  CarState a[1][1][1];
std::vector<std::vector<std::vector<int> > > a(4);
extern int road_num,car_num,cross_num;

vector<vector<int>> road_length;
vector<vector<vector<int>>> path;
vector<vector<vector<int>>>map3d;
int sys_clk=1;
//vector<int>::iterator smallest_path;
//vector<int> path[cross_num][cross_num];
//vector<int> dist;
//vector<bool> visited;

int car_data[9];
deque <int> passed;
deque <int> pass;


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

//通过路的id查找哪条路
int road_in_to_i(int roadid,int road_num)
{
  for(int i=0;i<road_num;i++)   //road_num:道路数量     
  {
    if(road[i].id==roadid)
      return i;
  }
  return -1;
}

//行驶速度
int velocity(int car_i,int road_i)
{
  return 
        (car[car_i].speed_max>road[road_i].limit_speed)?
                                            road[road_i].limit_speed : car[car_i].speed_max;
}


//车只在一条路上(无阻挡)  //到不了路口  (可以直接到)
void update_car_in_road(int car_i,int road_num,int road_channel)
{
  //////////////////////////////车状态变化///////////////////////////////////
  //到不了路口
 /* for(int i=0;i<car[car_i].car_path.size();i++){
    car[car_i].situation.car_pass.push_back(car[car_i].car_path[i]);
    cout<<car[car_i].situation.car_pass.front()<<" ";}*/
   
//     int flag;	//1 等待出发
			//0 等待行驶
			//-1 终止行驶
			
			
//     int car_dirction;
//     int car_position;
//     int car_channel;// 1 2 3分别三个车道数据
//     int road_id;
//     int car_speed;
//     bool  token;  //第一优先级车辆标志，是则为1，不是则为0
//     deque <int> car_passed;
//     deque <int> car_pass;
//     int car_turn;	//D 	:	 1
			//L	:	 0
			//R	:	-1
			
//     int is_moved;	//0 move 
			//1 no move
			
  car[car_i].situation.flag=1;
  car_data[0]=car[car_i].situation.flag;
  
  car[car_i].situation.car_dirction=car[car_i].situation.car_pass[1];//设置方向
  cout<<"car[car_i].situation.car_dirction=car[car_i].situation.car_pass.front():"<<car[car_i].situation.car_dirction<<endl;
  car_data[1]=car[car_i].situation.car_dirction;
  
  car[car_i].situation.car_passed.push_back(car[car_i].situation.car_pass.front());//设置已经走过的路径
  
  car[car_i].situation.car_pass.pop_front();//删除已经走过的路径
  
  pass=car[car_i].situation.car_pass;
  passed=car[car_i].situation.car_passed;
  
  int road_s;//道路编号转换
  //road_s=road_in_to_i(map3d[k][car[car_i].car_path[1]-1][0],road_num);//转换道路编号
  road_s=road_in_to_i(map3d[car[car_i].situation.car_passed.front()-1][car[car_i].situation.car_pass.front()-1][0],road_num);//查询行驶道路编号
  
  car[car_i].situation.car_speed=velocity(car_i,road_s);
  car_data[5]=car[car_i].situation.car_speed;
  
  car[car_i].situation.car_position=car[car_i].situation.car_position + car[car_i].situation.car_speed; //位置变化
  car_data[2]=car[car_i].situation.car_position;
  
  car[car_i].situation.road_id=map3d[car[car_i].situation.car_passed.front()-1][car[car_i].situation.car_pass.front()-1][0];
  cout<<"update_road_id:"<<car[car_i].situation.road_id<<endl;
  car[car_i].situation.car_channel=road_channel;
  car_data[3]=car[car_i].situation.car_channel;
  
  car[car_i].situation.is_moved=1;                                                      //是否移动标志:置1
  car_data[8]=car[car_i].situation.is_moved;
  
  car_data[4]=car_i;
  car_data[6]=-2;
  car_data[7]=-2;

  
  
}

//车只在一条路上(无阻挡)   //可以到路口
void update_car_in_cross(int car_i,int road_num)
{
  //////////////////////////////车状态变化///////////////////////////////////
   //可以到路口甚至下一段路 
  int roadi = road_in_to_i ( car[car_i].situation.road_id , road_num );//通过路的id查找哪条路（这段路）
  car[car_i].situation.car_position=road[roadi].length; //位置变化(到路口)  
  car[car_i].situation.is_moved = 1;                                                      //是否移动标志:置1
  car[car_i].situation.token = 1;                      //状态变化
  
   //////////////////////////////路状态变化///////////////////////////////////
}


//车到另一条路上(无阻挡)
void update_car_to(int car_i,int road_i,int road_num)
{
       //////////////////////////////车状态变化///////////////////////////////////
  int roadi = road_in_to_i ( car[car_i].situation.road_id , road_num );//通过路的id查找哪条路（这段路）
  car[car_i].situation.car_speed = velocity( car_i , road_i );//下一段速度
  int s = road[roadi].length - car[car_i].situation.car_position;// 这条路还剩的路程
  
  car[car_i].situation.car_position = car[car_i].situation.car_speed - s;//下一段路的位置
  car[car_i].situation.road_id = road[road_i].id;//改变车所在的路的id
  
  car[car_i].situation.car_channel=1;//车道(还需要判断)???????
  
  //  car[car_i].situation.token                      //状态变化        //是否在最前面（还需要判断） ?????
  
  car[car_i].situation.car_passed.push_back( car[car_i].situation.car_pass.front() );
  car[car_i].situation.car_pass.pop_front();
  
  car[car_i].situation.car_dirction=car[car_i].situation.car_pass.front() ;           //方向?????
  
  car[car_i].situation.is_moved=1;     //是否移动标志:置1
  

   //////////////////////////////路状态变化///////////////////////////////////
    
}

//路状态变化比较简单
//路状态变化（车在路里面移动） 车-->路  ?????方向如何考虑
void update_road_inside(int car_i,int road_num)
{
  int roadi = road_in_to_i ( car[car_i].situation.road_id , road_num );//通过路的id查找哪条路（这段路）
  Node * link_i = road[roadi].Cur_Road.Search( car[car_i].id ); //这部车在链表哪个位置
  
  //跟移动车辆改变的参数一样
  link_i->car_position= car[car_i].situation.car_position; //位置变化
  link_i->is_moved=1;                                                      //是否移动标志:置1
  //link_i->car_dirction                  ///方向
 //  car[car_i].situation.token                      //状态变化        //是否在最前面（还需要判断） ?????
}


//路状态变化（有车进入车道） 车-->路  ?????方向如何考虑
void update_road_in(int roadi)
{
 //把车的信息直接加到链表中(车的信息)
  road[roadi].Cur_Road.AddNode(car_data,passed,pass);
 
}




//路状态变化（有车出车道） 车-->路   ?????方向如何考虑
void update_road_out(int roadi,int car_i)
{
  int link_i = road[roadi].Cur_Road.Search_i( car[car_i].id ); //这部车在链表哪个位置
 //把车的信息直接在链表中删除
  road[roadi].Cur_Road.Delete(link_i);
 
}

//查询道路状态（是否有车）?????方向如何考虑
//?????
Node * lookfor_road(int roadi)
{
  int car_in_road = road[roadi].Cur_Road.ListLength();
  //cout<<car_in_road;//car_in_road 
  if((car_in_road-1)==0)
  {
    cout<<"no car"<<endl;
    return NULL;
  }
  else
  {
    
    return road[roadi].Cur_Road.Head;//返回链表头
  }
}

//查询道路状态（最后面的车在哪里）?????方向如何考虑
int first_channel(Node * car_link ,int dirction , int channel)//, int &short_position , int &first
{
  Node *temp; 
  int flag=0;
  if(car_link==NULL)
  {
    return 0;
  }else
  {
 for(int i=0;i<channel;i++)
 {
   flag=0;
   temp = car_link->next; 
   while(temp)
   {
     if(temp->car_position==1 && temp->car_channel==i && temp->car_dirction==dirction)
     {
       flag=0;
       break;
     }
     temp = temp->next;
     flag=1;
   }
   if(flag)
     return i;
 }
 return -1;
  }
}

//在一段路上行驶
int car_passdistence(int channel,int car_id,Node * car_link)
{
  Node *temp;
    int vmax=car[car_id].situation.car_speed;
  int thiscar_s=car[car_id].situation.car_position;
  if(car_link==NULL)
  {
    return vmax;
  }
  temp=car_link->next;
  int cari;
  int car_s=0;      //此通道上最近车辆的距离
  while(temp)
  {
    if((car_s==0||temp->car_position<car_s)&&temp->car_channel==channel)
    {
      car_s=temp->car_position;
      cari=temp->is_moved;
    }
    temp=temp->next;
  }

  if(vmax < (car_s - thiscar_s))	//car_s 此通道上最近车辆的距离
					//thiscar_s 正在行驶的车
    return vmax;
  else if(vmax >= (car_s - thiscar_s))
  {
    if(cari)
      return car_s - thiscar_s - 1;
    else
      return -1;
  }
}

//前方是否有车  ?????方向如何考虑
void is_car()
{
  
}

//判断前方是否有阻挡
//void resist()
//{}

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
						path[k][m].push_back(m+1);
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
	//int data = 100;
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


void decent_car()
{
  
}



void Search_car_path(int car_num,int &car_start,int &car_end)
  {
    int num=0;
    int car_path=0;
    vector<int>::iterator ite;
    for (ite = path[car_start-1][car_end-1].begin(); ite != path[car_start-1][car_end-1].end(); ++ite) {
			  if (ite == path[car_start-1][car_end-1].begin())
				{
				  car_path=*ite;
				  car[car_num].situation.car_pass.push_back(car_path);
				  cout <<car[car_num].situation.car_pass[num];
				  num++;
				}
				else
				{
					car_path=*ite;
					car[car_num].situation.car_pass.push_back(car_path);
					cout <<"->"<<car[car_num].situation.car_pass[num];
					num++;
				}

			}
			cout<<endl;
  }

int main(int argc, char *argv[])
{

     	
    //road[1]->Cur_Road.AddNode
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
	/*
	std::string s;
	int road_num=0,car_num=0,cross_num=0;
	std::cout<<"read start"<<std::endl;度为：1

	while(!road_file.eof())
	{
	char str_road[50];
	road_file.getline(str_road,'\r\n');
	if(str_road[0]=='#');
        if(str_road[0]=='(')100000
         {sscanf(str_road,"(%d,%d,%d,%d,%d,%d,%d)", 	&(road[road_num].id),
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
	//ifstream road_file_1(argv[2]);
	road.resize(road_num);*/
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
	road.resize(road_num);
	int num=0;
       	while(!road_file_1.eof())
	{
	char str_road[50];

	road_file_1.getline(str_road,'\r\n');
	if(str_road[0]=='#');
        if(str_road[0]=='(')
         {
	   
	   sscanf(str_road,"(%d,%d,%d,%d,%d,%d,%d)", 	&(road[num].id),
							&(road[num].length),
							&(road[num].limit_speed),
							&(road[num].channel),
							&(road[num].start),
							&(road[num].end),
							&(road[num].flag_bothway));

	
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
       //car.resize(car_num);
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
      // cross.resize(cross_num);
       cout<<"All files input finished"<<endl;
       
       /******Test Input Datas******/
       int l=0;
       int m=0;
       int n=1;
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
	//int map3d [road_num][road_num][4];
	map3d.resize(road_num);//定义数组长(第1位)
	for (int i = 0; i < road_num; i++)
	{
	  map3d[i].resize(road_num);//定义数组宽(第2位)
	    for (int j = 0; j<road_num; j++)
		map3d[i][j].resize(4);
	}
	
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
	    map3d[road[j].end-1][road[j].start-1][0]=MAX;
	    map3d[road[j].end-1][road[j].start-1][1]=MAX;
	    map3d[road[j].end-1][road[j].start-1][2]=MAX;
	    map3d[road[j].end-1][road[j].start-1][3]=MAX;
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
	//vector<int> path[cross_num][cross_num];[i])
	int a1, a2, a3;
	a1 = cross_num; a2 = cross_num; a3 = cross_num;
	path.resize(a1);//定义数组长(第1位)
	vector<int>::iterator ite;
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
	//D function
	/********************************************/
	Dijkstra(cross_num);
	//input smallest_path
	for(int i=0;i<car_num;i++)
	{
	//int i=5;
	  Search_car_path(i,car[i].start,car[i].end);
	  cout <<i<<endl;
	}
	//test out_smallest_path
	/*for(int i=0;i<car[0].car_path.size();i++)
	{
	  cout<<car[0].car_path[i]<<" ";
	}*/
	/********************************************/
	vector<vector<int>> car_samestartid(cross_num);
	/*car_samestartid.resize(cross_num);
	//path_out.resize(cross_num);
	for (int k = 0; k < cross_num; ++k){
            car_samestartid[k].resize(cross_num);//每行为c列
	    //path_out[k].resize(cross_num);
        }
        */
	/******************************************************/
	/*****************cross all car*************************/
	/******************************************************/
	  for(int j=0;j<cross_num;j++)
	  {
	    cout<<"cross_num:"<<(j+1)<<endl;
	    for(int i=0;i<car_num;i++)
	      {
		if(j==(car[i].start-1)) //all cars start cross_num
		  {
		    car_samestartid[j].push_back(i); 
			
		    cout<<"car_id:"<<car_samestartid[j].back()<<" ";
		  }
	
	      }
	      cout <<endl;
	  }
	  
	
	/******************************************************/   
	/********************车库调度********************/
	/******************************************************/
	vector<int>::iterator ite_time;
	vector<int> next_roadid;
	int first_time=0;
	int car_cross_num=0;
	vector<int> road_channel;
	
	//car_cross_num=car_samestartid[1].size();
	for(int k=0;k<cross_num;k++){
	   cout<<endl<<"路口数:"<<k<<endl;
	    for(ite_time = car_samestartid[k].begin(); ite_time !=car_samestartid[k].end(); ++ite_time){// 遍历所有路口的所有车辆  并根据相应逻辑进行车辆初始化
 		//将所有的车在初始化时sys_clk=1 时准备时刻,进行所有仓库的车标记为SET_OUT=1
	      if (ite_time == car_samestartid[k].begin())//每个节点的第一辆车初始化
		{	
		      //if(car[*ite_time].start_time==sys_clk)
		     // {
// 			判断是否有阻挡
			int road_s=0;
			//cout<<"road[0].id"<<road[0].id<<endl;
			//cout<<*ite_time<<endl<<car[*ite_time].situation.car_pass[1]<<endl;
			cout<<map3d[k][car[*ite_time].situation.car_pass[1]-1][0]<<endl;
			road_s=road_in_to_i(map3d[k][car[*ite_time].situation.car_pass[1]-1][0],road_num);//查询行驶道路编号
			cout <<"road_s"<<road_s<<endl;
			int aa=first_channel(lookfor_road(road_s),car[*ite_time].situation.car_pass[1],map3d[k][car[*ite_time].situation.car_pass[1]-1][3]);
			cout<<"map3d[k][car[*ite_time].situation.car_pass[1]-1][3]:"<<map3d[k][car[*ite_time].situation.car_pass[1]-1][3]<<endl;
			cout<<car[*ite_time].situation.car_pass[1]<<endl;
			cout <<aa<<endl;
			
			//car[*ite_time].situation.flag=;//等待出发
			//cout<<"car_id:"<<*ite_time<<"flag:"<<car[*ite_time].situation.flag<<"  ";
			//cout<<endl;
			//cout<<map3d[k][car[*ite_time].car_path[1]-1][0]<<endl;
			
			//cout<<road_s;
			//查找车道是否有车
			//cout<<car[*ite_time].car_path[1]<<map3d[k][car[*ite_time].car_path[1]-1][3]<<endl;
			//int aa=first_channel(lookfor_road(road_s),car[*ite_time].car_path[1],map3d[k][car[*ite_time].car_path[1]-1][3]);			
		        //cout<<aa<<endl;
			//int Car_S=car_passdistence(aa,*ite_time,lookfor_road(road_s));
			//cout<<Car_S<<endl;
			update_car_in_road(*ite_time,road_num,aa);
			update_road_in(road_s);
			Node* n=lookfor_road(road_s);
			
			cout<<"car_passed.front()="<<n->next->car_passed.back()<<endl;
			cout<<"car_position="<<n->next->car_position<<endl;
			cout<<"car_channel="<<n->next->car_channel<<endl;
		      //}
			//else
			 // {
			    //car[*ite_time].situation.flag=0;
			    //cout<<"car_id:"<<*ite_time<<"flag"<<car[*ite_time].situation.flag<<"  ";
			    
			 // }
		      
		 }
			else
			  {
				  
			    int road_s=0;
			    cout<<map3d[k][car[*ite_time].situation.car_pass[1]-1][0]<<endl;
			    road_s=road_in_to_i(map3d[k][car[*ite_time].situation.car_pass[1]-1][0],road_num);//查询行驶道路编号
			    cout <<"road_s"<<road_s<<endl;
			    int aa=first_channel(lookfor_road(road_s),car[*ite_time].situation.car_pass[1],map3d[k][car[*ite_time].situation.car_pass[1]-1][3]);
			    int bb=car_passdistence(aa,*ite_time,lookfor_road(road_s));
			    cout <<"aa:  "<<aa<<"  bb:"<<bb<<endl;

			    update_car_in_road(*ite_time,road_num,aa);
			    update_road_in(road_s);
			    Node* n=lookfor_road(road_s);
			
			    cout<<"car_position="<<n->next->car_position<<endl;		
			    cout<<"car_channel="<<n->next->car_channel<<endl;
			    cout << endl;
			  }
	  
	    }
	    /*
	    cout<<"set_out标记结束"<<endl;
	    cout<<"寻找路口车辆的下一个节点"<<endl;
	   // ite_time.clear();
	    for(ite_time = car_samestartid[k].begin(); ite_time !=car_samestartid[k].end(); ++ite_time){
	      next_roadid.push_back(car[*ite_time].car_path[1]);
	      cout <<"next_roadid: "<<next_roadid.back()<<"  ";
	      road_channel.push_back(map3d[k][car[*ite_time].car_path[1]-1][3]);
	      cout <<"road_channel: "<<road_channel.back()<<"  ";
	      cout <<"car_speed: "<<car[*ite_time].speed_max<<"  ";
	      //car_cross_num=car_samestartid[k].size();//在这个时刻出发的车辆的数量
	     // car[ite_time].speed_max;
	      //if(car_cross_num>2){
		 
	      }
	      //cout<<"road_channel.size(): "<<road_channel.size()<<"road_channel.max_size: "<<road_channel.max_size()<<endl;
	      road_channel.clear();
	    }
	*/
	}
	cout<<"输出结束"<<endl;
	
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
