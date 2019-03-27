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
int out_car=0;	
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
vector<vector<vector<int>>> map3d_copy;
int sys_clk=1;
int pre_roadID = -1;

//data
//0:flag  1:car_dirction  2:car_position  3:car_channel  
//4:car_id  5:car_speed  6:token  7:car_turn  8:is_moved
int car_data[9];
deque <int> passed;
deque <int> pass;
int cars;

int first_channel(Node * car_link ,int dirction , int channel);
void update_car_in_cross(int car_i,int road_num);
int cross_car_update(int car_id,int road_num);
//通过路的id查找哪条路
/*
 * int all_car_status(){
 * for(int i=0;i<car_num;i++)
 * {
 * if(car[i].situation.flag==0)
 * return 1;
 * else
 * return -1;
 * }
 * }
 */

int end_sys(int car_num)
{
  for(int i=0;i<car_num;i++)
  {
    if(!car[i].situation.car_pass.empty())
      return 1;
  }
  return 0;
}

int End_Allcross(int road_num)
{
  for(int roadi=0;roadi<road_num;roadi++)
  {
    Node *p=road[roadi].Cur_Road.Head;
    if(p->next != nullptr)
    {
      p=p->next;
      while(p)
      {    
	if(p->flag == 0)
	  return 1;
	
	p=p->next;
      }
    }
  }
  return 0;
}

int End_cross1(int road_num,int cross_id)
{
  
  for(int roadi=0;roadi<road_num;roadi++)
  {
    if(roadi == cross_id)
    {
      Node *p=road[roadi].Cur_Road.Head;
      if(p->next != nullptr)
      {
	p=p->next;
	while(p)
	{    
	  if(p->flag == 0)
	    return 1;
	  
	  p=p->next;
	}
      }
    }
  }
  return 0;
}


int road_in_to_i(int roadid,int road_num)
{
  if(roadid==-1)
    return -1;
  else{
    for(int i=0;i<road_num;i++)   //road_num:道路数量     
    {
      if(road[i].id==roadid)
	return i;
    }
    return -1;
  }
}
int car_in_to_i(int roadid,int road_num)
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


//插入排序
void linkinsertSort(Node * mylist )
{
  if((mylist -> next == NULL) || (mylist -> next -> next == NULL))
  {   
    return;
  }   
  
  Node * head, * p1, * prep1, * p2, * prep2, * temp;
  head = mylist;
  prep1 = head -> next;
  p1 = prep1 -> next;
  //prep1和p1是否需要手动后移
  bool flag;
  
  while(p1 != NULL)
  {   
    flag = true;
    temp = p1; 
    //由于是单向链表，所以只能从头部开始检索
    for(prep2 = head, p2 = prep2 -> next; p2 != p1; prep2 = prep2 -> next, p2 = p2 -> next)
    {   
      //发现第一个较小值 位置相同看车道
      if((p2->car_position < p1 ->car_position) || (p2->car_position==p1->car_position&&p2->car_channel>p1->car_channel))
      {   
	p1 = p1 -> next;
	prep1 -> next = p1; 
	prep2 -> next = temp;
	temp -> next = p2; 
	flag = false;
	break;
      }   
    }   
    //手动后移prep1和p1
    if(flag)
    {   
      prep1 = prep1 -> next;
      p1 = p1 -> next;
    }   
  }   
}

void update_start(int road_num)
{
  
  for(int roadi=0;roadi<road_num;roadi++)
  {
    Node *p=road[roadi].Cur_Road.Head;
    if(p->next != nullptr)
    {
      p=p->next;
      while(p)
      {    
	car[p->car_id].situation.flag=0;
	p->flag=0;
	//	cout<<"car[p->car_id].situation.flag=  "<<car[p->car_id].situation.flag<<endl;
	p=p->next;
      }
    }
  }
}


//出车库
//车只在一条路上(无阻挡)  //到不了路口  (可以直接到)
void update_car_in_road(int car_i,int road_num,int road_channel,int car_passdist,int T)
{
  //////////////////////////////车状态变化///////////////////////////////////
  //到不了路口
  /* for(int i=0;i<car[car_i].car_path.size();i++){
   *    car[car_i].situation.car_pass.push_back(car[car_i].car_path[i]);
   *    cout<<car[car_i].situation.car_pass.front()<<" ";}*/
  
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
  for(int i=0;i<9;i++)
    car_data[i]=-3;   //清空数组
    passed.clear();
  pass.clear();      
  
  //   car[car_i].situation.flag=-1;			  
  car[car_i].situation.flag=-1;
  car_data[0]=car[car_i].situation.flag;
  
  car[car_i].situation.car_dirction=car[car_i].situation.car_pass[1];//设置方向
  car_data[1]=car[car_i].situation.car_dirction;
  
  car[car_i].situation.car_passed.push_back(car[car_i].situation.car_pass.front());//设置已经走过的路径
  
  car[car_i].situation.car_pass.pop_front();//删除已经走过的路径
  
  pass=car[car_i].situation.car_pass;
  passed=car[car_i].situation.car_passed;
  
  int road_s;//道路编号转换
  //road_s=road_in_to_i(map3d[k][car[car_i].car_path[1]-1][0],road_num);//转换道路编号
  road_s=road_in_to_i(map3d[car[car_i].situation.car_passed.back()-1][car[car_i].situation.car_pass.front()-1][0],road_num);//查询行驶道路编号
  
  car[car_i].situation.car_speed=velocity(car_i,road_s);
  car_data[5]=car[car_i].situation.car_speed;
  
  car[car_i].situation.car_position=car_passdist;//位置变化
  car_data[2]=car[car_i].situation.car_position;
  
  car[car_i].situation.road_id=map3d[car[car_i].situation.car_passed.back()-1][car[car_i].situation.car_pass.front()-1][0];
  cout<<"update_road_id:"<<car[car_i].situation.road_id<<endl;
  car[car_i].situation.car_channel=road_channel;
  car_data[3]=car[car_i].situation.car_channel;
  if(road_channel!=-1)
    car[car_i].situation.is_moved=1;//是否移动标志:置1
    else
      car[car_i].situation.is_moved=0;
    car_data[8]=car[car_i].situation.is_moved;
  
  car_data[4]=car_i;
  car_data[6]=-2;
  car_data[7]=-2;
  cout << "car_id="<< car_i<<endl;
  cout <<"position carpart update="<<car[car_i].situation.car_position<<endl;
  cout <<"dirction carpart update="<<car[car_i].situation.car_dirction<<endl;
  cout <<"flag carpart update="<<car[car_i].situation.flag<<endl;
  cout <<"channel carpart update="<<car[car_i].situation.car_channel<<endl;
  car[car_i].situation.actual_start_time=T;
}




//车到另一条路上(无阻挡)
void update_car_to(int car_i,int road_i,int road_num)
{
  for(int i=0;i<9;i++)
    car_data[i]=-3;   //清空数组
    passed.clear();
  pass.clear();      
  
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
//路状态变化（车在路里面移动） 车-->路  
void update_road_inside(int car_i,int road_num)
{
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
  int roadi = road_in_to_i ( car[car_i].situation.road_id , road_num );//通过路的id查找哪条路（这段路）
  Node * link_i = road[roadi].Cur_Road.Search( car_i ); //这部车在链表哪个位置
  
  if( cars > 0)
  {  
    //跟移动车辆改变的参数一样
    link_i->car_position =  car_data[2]; //位置变化
    //  cout <<"pos:"<<link_i->car_position<<endl;
    link_i->is_moved = car_data[8];               //是否移动标志:置1
    //  car[car_i].situation.token                      //状态变化        //是否在最前面（还需要判断） ?????
  }
  link_i->flag=car_data[0];
  
  if(roadi==1)
    cout<<"link 1 pos"<<link_i->car_position<<endl;
  
  cars=-1;
  for(int i=0;i<9;i++)
    car_data[i]=-100000;   //清空数组
    
}


//路状态变化（有车进入车道） 车-->路  ?????方向如何考虑
void update_road_in(int roadi)
{
  //把车的信息直接加到链表中(车的信息)
  road[roadi].Cur_Road.AddNode(car_data,passed,pass);
  
  //linkinsertSort(road[roadi].Cur_Road.Head);//链表排序
}




//路状态变化（有车出车道） 车-->路   
void update_road_out(int roadi,int car_i)
{
  int link_i = road[roadi].Cur_Road.Search_i( car[car_i].id ); //这部车在链表哪个位置
  //把车的信息直接在链表中删除
  road[roadi].Cur_Road.Delete(link_i);
  
  //  linkinsertSort(road[roadi].Cur_Road.Head);//链表排序
}

//查询道路状态（是否有车）?????方向如何考虑
//?????
Node * lookfor_road(int roadi)
{
  int car_in_road = road[roadi].Cur_Road.ListLength();
  //  cout<<car_in_road<<endl;//car_in_road 
  if((car_in_road-1)==0)
  {
    cout<<"no car"<<endl;
    return nullptr;
  }
  else
  {
    
    return road[roadi].Cur_Road.Head;//返回链表头
  }
}

int road_sche(Node * car_link,int car_id,int road_num)
{
  Node *p;
  int position=0;
  int min_position=-1;
  //  int i=0;
  int flag=-1;
  if(car[car_id].situation.flag==-1)
  {
    car[car_id].situation.flag=-1;
    return -1;
  }
  if(car_link->next==NULL)
  {
    return -1;
  }
  int road_s;//道路编号转换
  road_s=road_in_to_i(car[car_id].situation.road_id,road_num);//查询行驶道路编号
  p=car_link->next;
  while(p)//遍历找车
  {
    if((p->car_position>car[car_id].situation.car_position) && (p->car_channel==car[car_id].situation.car_channel) && (p->car_dirction==car[car_id].situation.car_dirction))//找出前面所有车
    {
      position = (p->car_position - car[car_id].situation.car_position);
      flag=car[p->car_id].situation.flag;
      min_position=position;
      break;
    }
    p=p->next;
  } 
  p=car_link->next;
  while(p)//遍历找车
  {
    if((p->car_position>car[car_id].situation.car_position) && (p->car_channel==car[car_id].situation.car_channel) && (p->car_dirction==car[car_id].situation.car_dirction))//找出前面所有车
    {
      position = (p->car_position - car[car_id].situation.car_position);
      if(min_position>position)
      {
	flag=car[p->car_id].situation.flag;
	min_position=position;
      }
    }
    p=p->next;
  }
  cout << "MIN Position:" << min_position;
  if(min_position>0)//前方有车,判据(此链同一车道,同一方向,位置大于本车,和本车ID不同)
  {
    if((min_position-car[car_id].situation.car_speed)<=0)//(前车距离-目前车位置-此车Vmax)>0,则有阻拦.
    {
      if(flag==0)//判断前车状态,前车等待
      {
	car[car_id].situation.flag=0;
	return -1;	    
      }
      if(flag==-1)//前车终止
      {
	if(car[car_id].situation.flag==0)
	  return (min_position-1);
	else
	  return -1;
      }
    }//是阻拦
    else
    {
      if(car[car_id].situation.flag==0)
	return car[car_id].situation.car_speed;
      else
	return -1;
      
    }//没阻拦
  }
  else//前方没车
  {
    if((car[car_id].situation.car_speed>(road[road_s].length-car[car_id].situation.car_position)))//判断能否出路口//能出路口 
    {
      car[car_id].situation.flag=0;
      return -1;
    }
    else
    {
      return car[car_id].situation.car_speed;
    }
  }
}



void insertSort(int arr[], int n){  
  for(int i = 1;i < n;i++){  
    int temp = arr[i];  
    int j = i - 1;  
    while(temp < arr[j]){  
      arr[j+1] = arr[j];  
      j--;  
      if(j == -1){  
	break;  
      }  
    }  
    arr[j+1] = temp;  
  }  
} 


int Cross_first_car(int cross_id,int road_i)//,int index)//测试  不能通过OK  通过后续还需要测试 //road_i  传入 5000道路就是 0
{
  int cross_road_num=0;
  int road_serial;
  int car_id=-1;
  //road_serial=cross[cross_id].road_id[index];
  //insertSort(road_serial, 4);   //排序后的道路顺序在road_serial数组里面
  //if(road_serial!=-1)
  //  ;
  Node * temp;
  temp=road[road_i].Cur_Road.Head;////-5000
  if(temp->next==nullptr)
  {
    car_id=-1;
    cout << "(没车)car_id()="<<car_id<<endl;
  }
  else
  {
    temp=temp->next;
    int max_position=0;
    while(temp)
    {
      if((temp->car_pass.front()-1)==cross_id)
      {
	
	cout<<"temp[i]->car_id="<<temp->car_id<<"temp[i]->car_pass.front()="<<temp->car_pass.front()<<endl;
	cout<<"temp[i]->flag="<<temp->flag<<endl;
	cout << "temp[i]->position=" << temp->car_position << endl;
	cout << "temp[i]->channel=" << temp->car_channel << endl;
	if((temp->car_position>max_position)&&(temp->flag==0))
	{
	  max_position=temp->car_position;
	  car_id=temp->car_id; ///?????
	}else
	{
	  if((temp->car_position==max_position)&&(temp->flag==0))
	  {
	    if(temp->car_channel<car[car_id].situation.car_channel)
	      car_id=temp->car_id;
	  }
	}
      }
      temp=temp->next;
    }
  }
  return car_id;
}
//     if(car_id.size()>1)
//       {
//       vector<int>::iterator ite;
//       for(ite=car_id.begin();ite!=car_id.end();++ite)
//       {
// 	if(ite==car_id.begin())
// 	{
// 	  sele_car.clear();
// 	  small_channel=car[*ite].situation.car_channel;
// 	  sele_car.push_back(*ite);
// 	}
// 	else{
// 	      if(small_channel>car[*ite].situation.car_channel)
// 		{
// 		  small_channel=car[*ite].situation.car_channel;
// 		  sele_car.clear();
// 		  sele_car.push_back(*ite);
// 		}
// 	    }
//       } 
//       back_car.push_back(sele_car.back());
//       cout << "(有车)back_car.back()="<<back_car.back()<<endl;
//     }
//     else
//     {
//       if(car_id.size()==1)
//       {
// 	back_car.push_back(car_id[0]);
// 	cout << "(有车)back_car.back()="<<back_car.back()<<endl;
//       }
//       else{
//       back_car.push_back(-1);
//       cout << "(没车)back_car.back()="<<back_car.back()<<endl;
//       //return back_car;
//       }
//     }//找最小路口
//     car_id.clear();
//   }
//  }


int roadIndexInCross(int cross_id,int road_id)  //通过cross_id和road_id找到road在cross文件中的下标
{
  int i = 0;
  cout <<"roadIndexInCross_id= "<<cross_id<<endl;
  for(i=0;i<4;i++)
  {
    if(cross[cross_id].road_id[i] == road_id)
      return i;
  }
}



int findTheNextRoad(Car & car)    //找到当前车辆下一条要驶入的路id
{
  int i = 0;//用于存储当前道路在path中的下标
  for(i = 0;i < car.situation.car_pass.size();i++)
  {
    if(car.situation.car_pass[i] == car.situation.car_dirction)
    {
      cout<< "car.situation.car_dirction:"<<car.situation.car_dirction<<endl;
      break;
    }
  }
  return map3d[car.situation.car_pass[i]-1][car.situation.car_pass[i+1]-1][0];
}
int nextOrientation(int cross_id,Car & car)    //得到出当前路口时要转向的方向D:0/L:1/R:2
{
  if(car.situation.car_dirction == car.end)    //if the car is going to arrive its destine in this road. then return 0
    return 0;
  else
  {
    int roadi = car.situation.road_id;
    int roadj = findTheNextRoad(car);
    cout<<"roadj= "<<roadj<<endl;
    int index_i = roadIndexInCross(cross_id,roadi);
    int index_j = roadIndexInCross(cross_id,roadj);
    cout<<"index_i:"<<index_i<<endl;
    cout<<"index_j:"<<index_j<<endl;
    switch ((index_i - index_j + 4) % 4)
    {
      //     case 0:return -1;break;
      case 1:return 2;
      case 2:return 0;
      case 3:return 1;
      default:return -1;
    }
  }
}

int findTheFirstCar(int roadid,int cross_id)//find the car which has the first order,return car_id
{
  Node * ptr = road[roadid].Cur_Road.Head;
  int max_posi = 0;//the maximum position
  int max_channel = 0;
  int carid = 0;
  if(ptr == NULL)
    return -1;
  else
  {
    //ptr = ptr->next;
    
    while(ptr)
    {
      
      if(((max_posi == 0)||(max_posi <= ptr->car_position)) && (ptr->car_dirction == cross_id))
      {
	if(max_posi == 0)
	{
	  max_posi = ptr->car_position;
	  max_channel = ptr->car_channel;
	  carid = ptr->car_id;
	}
	else if(max_posi == ptr->car_position)
	{
	  if(max_channel > ptr->car_channel)
	  {
	    max_posi = ptr->car_position;
	    max_channel = ptr->car_channel;
	    carid = ptr->car_id;
	  }
	}
	else
	{
	  max_posi = ptr->car_position;
	  max_channel = ptr->car_channel;
	  carid = ptr->car_id;
	}
      }
      ptr = ptr->next;
    }
  }
  return carid;
}

int whichRoadToSche(int crossid,int roadid,int road_num)//find out which way to scheduling next in this crossing
{
  cout << "pre_roadid"<<pre_roadID<<endl;
  if(roadid == 0) //roadid=0 means its the first time to call this function
  {
    int min_id;
    for(int i=0;i<4;i++)
    {
      if(cross[crossid].road_id[i]>0)
      {
	min_id = cross[crossid].road_id[i];
	break;
      }
    }
    for(int i=0;i<4;i++)
    {
      if(cross[crossid].road_id[i]>0)
	if(cross[crossid].road_id[i] < min_id)
	  min_id = cross[crossid].road_id[i];
    }
    int roadi = road_in_to_i ( min_id , road_num );//通过路的id查找哪条路（这段路）
    pre_roadID = roadi;
    return pre_roadID;
  }
  else if(roadid==-1)
  {
    int bigger_id = -1;
    int dis = 0;
    //int road_s = road_in_to_i ( cross[crossid].road_id[i] , road_num );
    for(int i=0;i<4;i++)
    {
      int road_s = road_in_to_i ( cross[crossid].road_id[i] , road_num );
      cout<<"road["<<i<<"] = "<<road_s<<endl;
      if(road_s >pre_roadID)
      {
	dis = road_s  - pre_roadID;
	bigger_id =road_s ;
	break;
      }
    }
    if(bigger_id == -1)
      return -1;
    for(int i=0;i<4;i++)
    {
      int road_s = road_in_to_i ( cross[crossid].road_id[i] , road_num );
      cout<<"road["<<i<<"] = "<<road_s << "  crossid= "<< crossid<<endl;
      if((( road_s - pre_roadID) < dis) && (( road_s - pre_roadID) > 0))
      {
	cout<<"mark0"<<endl;
	dis =  road_s - pre_roadID;
	bigger_id =  road_s;
      }
    }
    cout<<"bigger_id= " <<bigger_id<<endl;
    //int roadi = road_in_to_i ( min_id , road_num );
    pre_roadID = bigger_id;
    return pre_roadID;
  }
  else if(roadid > 0)
  {
    //int roadi = road_in_to_i ( pre_roadi , road_num );
    return pre_roadID;
  }
}

int get_index_incross(int cross_id,int scheing_road,int road_num)
{
  for(int i=0;i<4;i++)
  {
    int roadi = road_in_to_i (cross[cross_id].road_id[i] , road_num );//通过路的id查找哪条路（这段路）
    if(roadi==scheing_road)
    {
      cout <<"i= "<<i <<endl;
      return i;
    }
  }
  return 0;
}
int turning(int turn[],int car_id[],int cross_id,int index,int road_num,int road_i)
{
  int back_carid=0;
  int next_road_id=0;
  int nnext_road_id=0;
  if(car_id[index]==-1)
    return -1;
  else
  {
    if(turn[index]==0)
    {
      back_carid= car_id[index];
      return back_carid;
    }
    else if(turn[index]==1)//如果左转
    {
      next_road_id=(index+3) % 4;
      if(cross[cross_id].road_id[next_road_id]==-1)//没路
      {
	back_carid= car_id[index];
	return back_carid;
      }
      else if(turn[next_road_id]==0)
      {
	car[car_id[index]].situation.flag=0;//等待
	int roadi = road_in_to_i ( car[car_id[index]].situation.road_id , road_num );//通过路的id查找哪条路（这段路）
	Node * link_i = road[roadi].Cur_Road.Search( car_id[index]); //这部车在链表哪个位置
	link_i->flag=car[car_id[index]].situation.flag;
	back_carid=-1;//car_id[k];
	return back_carid;
      }
      else
      {
	back_carid= car_id[index];
      }
      
    }
    else if(turn[index]==2)
    { //如果右拐
      next_road_id=(index + 1) % 4;
      nnext_road_id=(index + 2) % 4;
      if(cross[cross_id].road_id[next_road_id]!=-1)
      {
	if(turn[next_road_id]==0)
	{
	  car[car_id[index]].situation.flag=0;//等待	
	  int roadi = road_in_to_i ( car[car_id[index]].situation.road_id , road_num );//通过路的id查找哪条路（这段路）
	  Node * link_i = road[roadi].Cur_Road.Search( car_id[index]); //这部车在链表哪个位置
	  link_i->flag=car[car_id[index]].situation.flag;
	  back_carid=-1;//car_id[k];
	  return back_carid;
	}
	else
	{
	  if(cross[cross_id].road_id[nnext_road_id]!=-1)
	  {
	    if(turn[nnext_road_id]==1)
	    {
	      car[car_id[index]].situation.flag=0;//等待		    
	      int roadi = road_in_to_i ( car[car_id[index]].situation.road_id , road_num );//通过路的id查找哪条路（这段路）
	      Node * link_i = road[roadi].Cur_Road.Search( car_id[index]); //这部车在链表哪个位置
	      link_i->flag=car[car_id[index]].situation.flag;
	      back_carid=-1;//car_id[k];
	      return back_carid;
	    }
	    else
	    {
	      back_carid= car_id[index];
	      return back_carid;
	    }
	    
	  }
	  else
	  {
	    back_carid= car_id[index];
	    return back_carid;
	  }
	}
      }
      else
      {
	if(cross[cross_id].road_id[nnext_road_id]!=-1)
	{
	  if(turn[nnext_road_id]==1)
	  {
	    car[car_id[index]].situation.flag=0;//等待		    
	    int roadi = road_in_to_i ( car[car_id[index]].situation.road_id , road_num );//通过路的id查找哪条路（这段路）
	    Node * link_i = road[roadi].Cur_Road.Search( car_id[index]); //这部车在链表哪个位置
	    link_i->flag=car[car_id[index]].situation.flag;
	    back_carid=-1;//car_id[k];
	    return back_carid;
	  }
	  else
	  {
	    back_carid= car_id[index];
	    return back_carid;
	  }
	}
	else
	{
	  back_carid= car_id[index];
	  return back_carid;
	}
      }
    } 
  }
  return back_carid; 
} 

/*    
 *    if((cross[cross_id].road_id[next_road_id]==-1)&&(cross[cross_id].road_id[nnext_road_id]==-1))//没路
 *    {
 *    back_carid= car_id[index];
 *    return back_carid;
 *    }else if(cross[cross_id].road_id[next_road_id]!=-1)
 *    {
 *    if(turn[next_road_id]==0)
 *    {
 *    car[car_id[index]].situation.flag=0;//等待	
 *    int roadi = road_in_to_i ( car[car_id[index]].situation.road_id , road_num );//通过路的id查找哪条路（这段路）
 *    Node * link_i = road[roadi].Cur_Road.Search( car_id[index]); //这部车在链表哪个位置
 *    link_i->flag=car[car_id[index]].situation.flag;
 *    back_carid=-1;//car_id[k];
 *    return back_carid;
 *    } 
 *    }
 *    
 *    
 *    if(turn[nnext_road_id]==1)
 *    {
 *    car[car_id[index]].situation.flag=0;//等待		    
 *    int roadi = road_in_to_i ( car[car_id[index]].situation.road_id , road_num );//通过路的id查找哪条路（这段路）
 *    Node * link_i = road[roadi].Cur_Road.Search( car_id[index]); //这部车在链表哪个位置
 *    link_i->flag=car[car_id[index]].situation.flag;
 *    back_carid=-1;//car_id[k];
 *    return back_carid;
 *    }
 *    else
 *    {
 *    back_carid=car_id[index];
 *    return back_carid;
 *    }
 *    
 *    }
 *    }*/


int end_cross(int cross_id,int road_num)
{
  for(int roadi=0;roadi<4;roadi++)
  {
    if(cross[cross_id].road_id[roadi]==-1);
    else
    {
      int roadid = road_in_to_i (cross[cross_id].road_id[roadi] , road_num );//通过路的id查找哪条路（这段路）
      Node *p=road[roadid].Cur_Road.Head;
      if(p->next != nullptr)
      {
	p=p->next;
	while(p)
	{
	  if((p->car_dirction-1)==cross_id&&p->flag == 0)
	    return 1;
	  p=p->next;
	}
      }
    } 
  }
  return 0;
}
int Cross_Sche(int cross_id , int road_num,int T)
{
  int first_car_i[4];
  int scheing_road=0;
  int turn[4];
  int Index=0;
  int turn_car_id=0;
  //int update_roadi=0;
  xiayilun: while(end_cross(cross_id,road_num))
  {
    cout<< " T = " << T << " pre_scheing_road = " << scheing_road << " pre_CrossID: " << cross_id << endl;
    scheing_road=whichRoadToSche(cross_id,scheing_road,road_num);
    cout<< " T = " << T << " scheing_road = " << scheing_road << " CrossID: " << cross_id << endl;
    if(scheing_road == -1)
    {
      cout << "this crossing is finished"  << endl;
      break;
    }
    
    
    //scheing_road=whichRoadToSche(cross_id,scheing_road,road_num);
    for(int i=0;i<4;i++)
    {
      //first_car_i[i]=findTheFirstCar(cross[cross_id].road_id[i],cross_id);
      if(cross[cross_id].road_id[i]==-1) first_car_i[i]=-1;
      else
      {
	int roadi = road_in_to_i ( cross[cross_id].road_id[i] , road_num );//通过路的id查找哪条路（这段路）
	cout<<"roadi="<<roadi<<endl;
	first_car_i[i]=Cross_first_car(cross_id,roadi);//, index);
      }
      if(first_car_i[i]==-1)
	turn[i]=-1;
      else
      {
	turn[i]=nextOrientation(cross_id,car[first_car_i[i]]);
	cout<<" car_id = "<<first_car_i[i]<<endl;
	cout<<" road_id = "<<car[first_car_i[i]].situation.road_id<<endl;
	cout<<" turn = "<<turn[i]<<endl;
	cout<<" car.pass= "<<car[first_car_i[i]].situation.car_pass.front()<<endl;
      }
    }
    
    Index=get_index_incross(cross_id,scheing_road,road_num);
    cout<<"INDEX= "<<Index<<endl;
    turn_car_id=turning(turn,first_car_i,cross_id,Index,road_num,scheing_road);
    if(turn_car_id==-1)
    { 
      scheing_road=-1;
      goto xiayilun;
    }
    else
    {
      scheing_road=cross_car_update(turn_car_id,road_num);
    }
  }
}

int car_into_next_road_position(Node * next_car,int channel,int dirction,int &flag,int car_id)
{
  Node * temp;
  int min_position=10000;
  flag = 3;
  if(next_car==nullptr)
  {
    cout<<car_id<<"mark a"<<endl;
    return min_position;
  }
  temp=next_car->next;
  
  while(temp)
  {
    if((temp->car_channel==channel)&&(temp->car_dirction==dirction))//如果等于1 就进入状态更新
    {
      cout<<car_id<<"mark c"<<endl;
      if(min_position>temp->car_position)
      {
	min_position=temp->car_position;
	flag=temp->flag;
	
      }
    }
    temp=temp->next;
  }
  cout<<car_id<<"mark b" << " channel "<< channel << " dirction " << dirction <<endl;
  return min_position;
}
int next_road_cari(Node * car,int channel,int minposition,int dirction)
{
  Node * temp;
  temp=car->next;
  int car_id=0;
  while(temp)
  {
    if((temp->car_channel==channel)&&(temp->car_position==minposition)&&(temp->car_dirction==dirction))//如果等于1 就进入状态更新
    {
      car_id=temp->car_id;
    }
    temp=temp->next;
  }
  return car_id;
  
}

// int cross_car_update(int car_id,int road_num)
// {
// 
//   Node *p;
//   int car_id_roadi=road_in_to_i(car[car_id].situation.road_id,road_num);//车的道路ID
//   int data[9];
//   if(car[car_id].situation.car_pass.size()==1)
//   {
//     int delet_car=road[car_id_roadi].Cur_Road.Search_i(car_id);//删除下一个道路的ID号
//     cout <<"delNODE: "<<car_id<<" car_id_roadi "<<car_id_roadi<<endl;
//     road[car_id_roadi].Cur_Road.Delete(delet_car);
//     
//     //排序遍历
// //    road_sche();
//     
// 
//     linkinsertSort( road[car_id_roadi].Cur_Road.Head );         
//     Node *p_l=road[car_id_roadi].Cur_Road.Head;
//     if(p_l->next != nullptr)
//     {
//       p_l=p_l->next;
//       while(p_l)
//       {
// 	update_car_in_cross(p_l->car_id,road_num);
// 	update_road_inside(p_l->car_id,road_num);
// 	p_l=p_l->next;       
//       }
//     }
//     car[car_id].situation.flag=-1;
//     data[0]=car[car_id].situation.flag;						//flag 终止行驶
//     car[car_id].situation.car_dirction= 0;
//     data[1]=car[car_id].situation.car_dirction; 					//方向
//     car[car_id].situation.car_position= 0;
//     data[2]=car[car_id].situation.car_position; 					//位置
//     car[car_id].situation.car_channel=0;
//     data[3]=car[car_id].situation.car_channel;  					//通道
//     data[4]=car_id;								//ID
//     car[car_id].situation.car_speed=0;
//     
//     data[5]=0;								//速度
//     data[6]=-2;									//优先级
//     data[7]=-2;									//转向
//     data[8]=0;									//移动
//     pass.clear();
//     passed.clear();
//     car[car_id].situation.car_passed.push_back(car[car_id].situation.car_pass.front());//设置已经走过的路径
//     car[car_id].situation.car_pass.pop_front();					//删除已经走过的路径
//     
//     pass=car[car_id].situation.car_pass;						//未来走的节点
//     
//     passed=car[car_id].situation.car_passed;					//已经走过的路程
//     cout <<"car_id="<< car_id <<"car[car_id].situation.car_pass.front="<<car[car_id].situation.car_pass.front()<< "   finished   "<<endl;
//     return car_id_roadi;
//     
//   }
//   else
//   {
//   int next_road_id=road_in_to_i(map3d[car[car_id].situation.car_pass.front()-1][car[car_id].situation.car_pass[1]-1][0],road_num);//查询行驶道路编号
//   cout<<"map3d= "<<map3d[car[car_id].situation.car_pass.front()-1][car[car_id].situation.car_pass[1]-1][0];
//   int sele_channel=first_channel(lookfor_road(next_road_id),car[car_id].situation.car_pass[1],//获取下一条路可以通行的道路
// 				 map3d[car[car_id].situation.car_pass.front()-1][car[car_id].situation.car_pass[1]-1][3]);
//   cout<<"car_id:"<<car_id<<"sele_channel="<<sele_channel<<endl;
//   
//   int car1_length_posotion=road[car_id_roadi].length-car[car_id].situation.car_position;//车在入口前的道路长度-position
//   int car_V1max=velocity(car_id,car_id_roadi);//在入口前道路1的最大速度
//   int car_V2max=velocity(car_id,next_road_id);//在入口后第二条道路的最大速度
//   int carV=min(car_V1max,car_V2max);//车的最大速度
//   int car_position2=car_into_next_road_position(lookfor_road(next_road_id),sele_channel,car[car_id].situation.car_pass[1]-1);//第二条路的最近的车position
//   
//   if(sele_channel==-1)//-1 代表下一个车道没有位置
//   {
//     p=road[next_road_id].Cur_Road.Head;
//     p=p->next;
//     while(p)//找出下一个车道的所有position = 1的车判断状态
//     {
//       if((p->car_position==1)&&(car[car_id].situation.car_pass[1]==p->car_pass.front()))//如果等于1 就进入状态更新
//       {
// 	if(car[p->car_id].situation.flag!=-1)//如果存在不终止
// 	{
// 	  car[car_id].situation.flag=0;//不能过路口终止
// 	}else
// 	{
// 	  car[car_id].situation.flag=-1;
// 	  car_data[0]=car[car_id].situation.flag;
// 	  car[car_id].situation.car_position= road[car_id_roadi].length;
// 	  car_data[2]=car[car_id].situation.car_position;
// 	  update_road_inside(car_id,road_num);
// 	}
//       }
//       p=p->next;
//     }
//     return -1;
//   }else// i 代表能够进入到下一个车道
//   {
//     if(car_position2>carV-car1_length_posotion)
//     {
//       //删除上一条路节点  并添加到下一条路
//       //carV-car1_length_posotion;//加入下一条链的位置
// 	 int delet_car=road[car_id_roadi].Cur_Road.Search_i(car_id);//删除下一个道路的ID号
// 	 cout <<"delNODE: "<<car_id<<" car_id_roadi "<<car_id_roadi<<endl;
// 	 road[car_id_roadi].Cur_Road.Delete(delet_car);
// 	 
// 	 
// 	 linkinsertSort( road[car_id_roadi].Cur_Road.Head );         
// 	 Node *p_l=road[car_id_roadi].Cur_Road.Head;
// 	 if(p_l->next != nullptr)
// 	 {
// 	   p_l=p_l->next;
// 	   while(p_l)
// 	   {
// 	     update_car_in_cross(p_l->car_id,road_num);
// 	     update_road_inside(p_l->car_id,road_num);
// 	     p_l=p_l->next;       
// 	   }
// 	 }
// 	 
// 	 
// 	  /*******************************更新数据*********************************/
// 	 car[car_id].situation.flag=-1;
// 	 data[0]=car[car_id].situation.flag;						//flag 终止行驶
// 	 car[car_id].situation.car_dirction= car[car_id].situation.car_pass[1];
// 	 data[1]=car[car_id].situation.car_dirction; 					//方向
// 	 car[car_id].situation.car_position= carV-car1_length_posotion;;
// 	 data[2]=car[car_id].situation.car_position; 					//位置
// 	 car[car_id].situation.car_channel=sele_channel;
// 	 data[3]=car[car_id].situation.car_channel;  					//通道
// 	 data[4]=car_id;								//ID
// 	 car[car_id].situation.car_speed=car_V2max;
// 	 
// 	 data[5]=car_V2max;								//速度
// 	 data[6]=-2;									//优先级
// 	 data[7]=-2;									//转向
// 	 data[8]=0;									//移动
// 	 pass.clear();
// 	 passed.clear();
// 	 car[car_id].situation.car_passed.push_back(car[car_id].situation.car_pass.front());//设置已经走过的路径
// 	 car[car_id].situation.car_pass.pop_front();					//删除已经走过的路径
// 	 
// 	 pass=car[car_id].situation.car_pass;						//未来走的节点
// 	 passed=car[car_id].situation.car_passed;					//已经走过的路程
// 	 car[car_id].situation.road_id=map3d[car[car_id].situation.car_passed.back()-1][car[car_id].situation.car_pass.front()-1][0];
// 	 /**********************************************************************/
// 	 road[next_road_id].Cur_Road.AddNode(data,passed,pass);  //加入链表
// 	 
// 	 cout <<"ADDNODE: "<<car_id<<" next_road_id "<<next_road_id<<endl;
// 	 /*linkinsertSort( road[car_id_roadi].Cur_Road.Head );
// 	 Node *p=road[car_id_roadi].Cur_Road.Head;
// 	 if(p->next != NULL)
// 	 {
// 	   p=p->next;
// 	   update_car_in_cross(p->car_id,road_num);
// 	   update_road_inside(p->car_id,road_num);
// 	 }*/
// 	 return car_id_roadi;
//       //通过最大速度
//     }
//     else
//     {
//       //判断前车状态
//       if(car[next_road_cari(lookfor_road(next_road_id),sele_channel,car_position2,car[car_id].situation.car_pass[1])].situation.flag==-1){
// 	int delet_car=road[car_id_roadi].Cur_Road.Search_i(next_road_cari(lookfor_road(car_id_roadi),sele_channel,car_position2,car[car_id].situation.car_pass[1]));//删除下一个道路的ID号
// 	road[car_id_roadi].Cur_Road.Delete(delet_car);
// 	  
// 	 //排序遍历
// 	// road_sche();
// 	linkinsertSort( road[car_id_roadi].Cur_Road.Head );         
// 	Node *p_l=road[car_id_roadi].Cur_Road.Head;
// 	 if(p_l->next != nullptr)
// 	 {
// 	   p_l=p_l->next;
// 	   while(p_l)
// 	   {
// 	     update_car_in_cross(p_l->car_id,road_num);
// 	     update_road_inside(p_l->car_id,road_num);
// 	     p_l=p_l->next;       
// 	   }
// 	 }
// 	 
// 	 
// 	 cout <<"delNODE: "<<car_id<<" car_id_roadi "<<car_id_roadi<<endl;
// 	 /*******************************更新数据*********************************/
// 	 car[car_id].situation.flag=-1;
// 	 data[0]=car[car_id].situation.flag;						//flag 终止行驶
// 	 car[car_id].situation.car_dirction= car[car_id].situation.car_pass[1];
// 	 data[1]=car[car_id].situation.car_dirction; 					//方向
// 	 car[car_id].situation.car_position=car_position2-1;
// 	 data[2]=car[car_id].situation.car_position; 					//位置
// 	 car[car_id].situation.car_channel=sele_channel;
// 	 data[3]=car[car_id].situation.car_channel;  					//通道
// 	 data[4]=car_id;								//ID
// 	 car[car_id].situation.car_speed=car_V2max;
// 	 data[5]=car_V2max;								//速度
// 	 data[6]=-2;									//优先级
// 	 data[7]=-2;									//转向
// 	 data[8]=0;									//移动
// 	 pass.clear();
// 	 passed.clear();
// 	 car[car_id].situation.car_passed.push_back(car[car_id].situation.car_pass.front());//设置已经走过的路径
// 	 car[car_id].situation.car_pass.pop_front();					//删除已经走过的路径
// 	
// 	 pass=car[car_id].situation.car_pass;						//未来走的节点
// 	 passed=car[car_id].situation.car_passed;					//已经走过的路程	
// 	 car[car_id].situation.road_id=map3d[car[car_id].situation.car_passed.back()-1][car[car_id].situation.car_pass.front()-1][0];
// 	 /**********************************************************************/
// 	 road[next_road_id].Cur_Road.AddNode(data,passed,pass);  //加入链表
// 	//行驶Vmax-1 car_position2-1
// 	 cout <<"ADDNODE: "<<car_id<<" next_road_id "<<next_road_id<<endl;
// 	 
// 	/* linkinsertSort( road[car_id_roadi].Cur_Road.Head );
// 	  Node *p=road[car_id_roadi].Cur_Road.Head;
// 	  if(p->next != NULL)
// 	  {
// 	    p=p->next;
// 	    update_car_in_cross(p->car_id,road_num);
// 	    update_road_inside(p->car_id,road_num);
// 	  }*/
// 	  return car_id_roadi;
//       }
//       if(car[next_road_cari(lookfor_road(next_road_id),sele_channel,car_position2,car[car_id].situation.car_pass[1])].situation.flag==0)
//       {
// 		
// 	//更新路的状态信息
// 	car[car_id].situation.flag=0;
// 	
// 	int roadi = road_in_to_i ( car[car_id].situation.road_id , road_num );//通过路的id查找哪条路（这段路）
// 	Node * link_i = road[roadi].Cur_Road.Search( car_id ); //这部车在链表哪个位置
// 	link_i->flag=car[car_id].situation.flag;
// 	
// 	return -1;
//       }
//     }
//   }
//   }
// }


int cross_car_update(int car_id, int road_num)
{
  cout << " car_id= " << car_id << endl;
  cout << "cross_car_update CALLED" << endl;
  Node *p;
  int car_id_roadi = road_in_to_i(car[car_id].situation.road_id, road_num);//车的道路ID
  int data[9];
  if (car[car_id].situation.car_dirction == car[car_id].end)//到达
  {
    cout << " MARK=00 " << endl;
    int delet_car = road[car_id_roadi].Cur_Road.Search_i(car_id);//删除下一个道路的ID号
    cout << "delNODE: " << car_id << " car_id_roadi " << car_id_roadi << endl;
    road[car_id_roadi].Cur_Road.Delete(delet_car);
    linkinsertSort(road[car_id_roadi].Cur_Road.Head);
    Node *p_l = road[car_id_roadi].Cur_Road.Head;
    if (p_l->next != nullptr)
    {
      p_l = p_l->next;
      while (p_l)
      {
	if((p_l->car_dirction==car[car_id].situation.car_dirction)&&(p_l->flag==0)&&(p_l->car_channel==car[car_id].situation.car_channel))
	{
	  update_car_in_cross(p_l->car_id, road_num);
	  update_road_inside(p_l->car_id, road_num);
	}
	p_l = p_l->next;
      }
    }
    car[car_id].situation.flag = -1;						//flag 终止行驶
    car[car_id].situation.car_dirction = car[car_id].situation.car_dirction;					//方向
    car[car_id].situation.car_position = 0;					//位置
    car[car_id].situation.car_channel = car[car_id].situation.car_channel;								//ID
    car[car_id].situation.car_speed = 0;									//移动
    pass.clear();
    passed.clear();
    car[car_id].situation.car_passed.push_back(car[car_id].situation.car_pass.front());//设置已经走过的路径
    car[car_id].situation.car_pass.pop_front();					//删除已经走过的路径   
    pass = car[car_id].situation.car_pass;						//未来走的节点  
    passed = car[car_id].situation.car_passed;					//已经走过的路程
    cout << "car_id=" << car_id << "   finished   " << endl;

    return car_id_roadi;
    
  }
  else//未到达   下一节点不是终点
  {
    int next_road_id = road_in_to_i(map3d[car[car_id].situation.car_pass.front() - 1][car[car_id].situation.car_pass[1] - 1][0], road_num);//查询行驶道路编号
    cout << "map3d= " << map3d[car[car_id].situation.car_pass.front() - 1][car[car_id].situation.car_pass[1] - 1][0];
    int sele_channel = first_channel(lookfor_road(next_road_id), car[car_id].situation.car_pass[1],//获取下一条路可以通行的道路
				     map3d[car[car_id].situation.car_pass.front() - 1][car[car_id].situation.car_pass[1] - 1][3]);
    cout << "car_id:" << car_id << "sele_channel=" << sele_channel << endl;
    
    int car1_length_posotion = road[car_id_roadi].length - car[car_id].situation.car_position;//车在入口前的道路长度-position
    int car_V1max = velocity(car_id, car_id_roadi);//在入口前道路1的最大速度
    int car_V2max = velocity(car_id, next_road_id);//在入口后第二条道路的最大速度
    //int carV = min(car_V1max, car_V2max);//车的最大速度
    int next_car_flag=-1;
     cout<<car_id<<"    "<<next_road_id;
    int car_position2 = car_into_next_road_position(lookfor_road(next_road_id), sele_channel, car[car_id].situation.car_pass[1],next_car_flag,car_id);//第二条路的最近的车position
    if (sele_channel == -1)//-1 代表下一个车道没有位置
    {
      cout << " MARK=XX " << endl;
      p = road[next_road_id].Cur_Road.Head;
      p = p->next;
      int wait_flag = 0;
      while (p)//找出下一个车道的所有position = 1的车判断状态
      {
	if ((p->car_position == 1) && (car[car_id].situation.car_pass[1] == p->car_dirction))//如果等于1 就进入状态更新
	{
	  if (car[p->car_id].situation.flag != -1)//如果存在不终止
	  {
	    wait_flag = 1;
	    break;
	  }
	}
	p = p->next;
      }
      if(wait_flag == 1)
      {
	cout << " MARK=01 " << endl;
	car[car_id].situation.flag = 0;//不能过路口终止
	//更新路的状态
	int roadi = road_in_to_i(car[car_id].situation.road_id, road_num);//通过路的id查找哪条路（这段路）
	Node * link_i = road[roadi].Cur_Road.Search(car_id); //这部车在链表哪个位置
	link_i->flag = car[car_id].situation.flag;
      }
      else
      {
	cout << " MARK=02 " << endl;
	car[car_id].situation.flag = -1;
	car[car_id].situation.car_position = road[car_id_roadi].length;
	int roadi = road_in_to_i ( car[car_id].situation.road_id , road_num );//通过路的id查找哪条路（这段路）
	Node * link_i = road[roadi].Cur_Road.Search( car_id ); //这部车在链表哪个位置
	link_i->flag=car[car_id].situation.flag;
	link_i->car_position=car[car_id].situation.car_position;
	linkinsertSort(road[car_id_roadi].Cur_Road.Head);
	Node *p_l = road[car_id_roadi].Cur_Road.Head;
	if (p_l->next != nullptr)
	{
	  p_l = p_l->next;
	  while (p_l)
	  {
	    if((p_l->car_dirction==car[car_id].situation.car_dirction)&&(p_l->flag==0)&&(p_l->car_channel==car[car_id].situation.car_channel))
	    {
	      update_car_in_cross(p_l->car_id, road_num);
	      update_road_inside(p_l->car_id, road_num);
	    }
	    p_l = p_l->next;
	  }
	}
      }
      return -1;
    }
    else// i 代表能够进入到下一个车道
    {
      cout << " MARK=AA " << endl;
      if (car_V1max>car1_length_posotion)
      {
	if(car_V2max>car1_length_posotion)
	{
	  if(((car_V2max-car1_length_posotion)>=car_position2)||(car_position2 == 10000))
	  {
	    if(next_car_flag==0)
	    {
	      cout << " MARK=03 " << endl;
	      //更新路的状态信息
	      car[car_id].situation.flag = 0;
	      int roadi = road_in_to_i(car[car_id].situation.road_id, road_num);//通过路的id查找哪条路（这段路）
	      Node * link_i = road[roadi].Cur_Road.Search(car_id); //这部车在链表哪个位置
	      link_i->flag = car[car_id].situation.flag;
	      return -1;
	    }
	    else if(next_car_flag==3)
	    {
	      cout << " MARK=04 " << endl;
	      //删除上一条路节点  并添加到下一条路
	      int delet_car = road[car_id_roadi].Cur_Road.Search_i(car_id);//删除下一个道路的ID号
	      cout << "delNODE: " << car_id << " car_id_roadi " << car_id_roadi << endl;
	      road[car_id_roadi].Cur_Road.Delete(delet_car);
	      linkinsertSort(road[car_id_roadi].Cur_Road.Head);
	      Node *p_l = road[car_id_roadi].Cur_Road.Head;
	      if (p_l->next != nullptr)
	      {
		p_l = p_l->next;
		while (p_l)
		{
		  if((p_l->car_dirction==car[car_id].situation.car_dirction)&&(p_l->flag==0)&&(p_l->car_channel==car[car_id].situation.car_channel))
		  {
		    cout << " MARK=05 " << endl;
		    update_car_in_cross(p_l->car_id, road_num);
		    update_road_inside(p_l->car_id, road_num);
		  }
		  p_l = p_l->next;
		}
	      }
	      /*******************************更新数据*********************************/
	      car[car_id].situation.flag = -1;
	      data[0] = car[car_id].situation.flag;						//flag 终止行驶
	      car[car_id].situation.car_dirction = car[car_id].situation.car_pass[1];
	      data[1] = car[car_id].situation.car_dirction; 					//方向
	      car[car_id].situation.car_position =  car_V2max-car1_length_posotion;
	      data[2] = car[car_id].situation.car_position; 					//位置
	      car[car_id].situation.car_channel = sele_channel;
	      data[3] = car[car_id].situation.car_channel;  					//通道
	      data[4] = car_id;								//ID
	      car[car_id].situation.car_speed = car_V2max;
	      data[5] = car_V2max;								//速度
	      data[6] = -2;									//优先级
	      data[7] = -2;									//转向
	      data[8] = 0;									//移动
	      pass.clear();
	      passed.clear();
	      car[car_id].situation.car_passed.push_back(car[car_id].situation.car_pass.front());//设置已经走过的路径
	      car[car_id].situation.car_pass.pop_front();					//删除已经走过的路径
	      pass = car[car_id].situation.car_pass;						//未来走的节点
	      passed = car[car_id].situation.car_passed;					//已经走过的路程
	      car[car_id].situation.road_id = map3d[car[car_id].situation.car_passed.back() - 1][car[car_id].situation.car_pass.front() - 1][0];
	      /**********************************************************************/
	      road[next_road_id].Cur_Road.AddNode(data, passed, pass);  //加入链表
	      cout << "00 ADDNODE: " << car_id << " next_road_id " << next_road_id << endl;
	      return car_id_roadi;
	      //通过最大速度
	    }
	    else//下一条路最近的车终止
	    {
	      cout << " MARK=06 " << endl;
	      //删除上一条路节点  并添加到下一条路
	      int delet_car = road[car_id_roadi].Cur_Road.Search_i(car_id);//删除下一个道路的ID号
	      cout << "delNODE: " << car_id << " car_id_roadi " << car_id_roadi << endl;
	      road[car_id_roadi].Cur_Road.Delete(delet_car);
	      linkinsertSort(road[car_id_roadi].Cur_Road.Head);
	      Node *p_l = road[car_id_roadi].Cur_Road.Head;
	      if (p_l->next != nullptr)
	      {
		p_l = p_l->next;
		while (p_l)
		{
		  if((p_l->car_dirction==car[car_id].situation.car_dirction)&&(p_l->flag==0)&&(p_l->car_channel==car[car_id].situation.car_channel))
		  {
		    update_car_in_cross(p_l->car_id, road_num);
		    update_road_inside(p_l->car_id, road_num);
		  }
		  p_l = p_l->next;
		}
	      }
	      /*******************************更新数据*********************************/
	      car[car_id].situation.flag = -1;
	      data[0] = car[car_id].situation.flag;						//flag 终止行驶
	      car[car_id].situation.car_dirction = car[car_id].situation.car_pass[1];
	      data[1] = car[car_id].situation.car_dirction; 					//方向
	      car[car_id].situation.car_position =  car_position2-1;
	      data[2] = car[car_id].situation.car_position; 					//位置
	      car[car_id].situation.car_channel = sele_channel;
	      data[3] = car[car_id].situation.car_channel;  					//通道
	      data[4] = car_id;								//ID
	      car[car_id].situation.car_speed = car_V2max;
	      
	      data[5] = car_V2max;								//速度
	      data[6] = -2;									//优先级
	      data[7] = -2;									//转向
	      data[8] = 0;									//移动
	      pass.clear();
	      passed.clear();
	      car[car_id].situation.car_passed.push_back(car[car_id].situation.car_pass.front());//设置已经走过的路径
	      car[car_id].situation.car_pass.pop_front();					//删除已经走过的路径
	      
	      pass = car[car_id].situation.car_pass;						//未来走的节点
	      passed = car[car_id].situation.car_passed;					//已经走过的路程
	      car[car_id].situation.road_id = map3d[car[car_id].situation.car_passed.back() - 1][car[car_id].situation.car_pass.front() - 1][0];
	      /**********************************************************************/
	      road[next_road_id].Cur_Road.AddNode(data, passed, pass);  //加入链表
	      cout << "01 ADDNODE: " << car_id << " next_road_id " << next_road_id << endl;
	      return car_id_roadi;
	      //通过最大速度
	    }
	  }
	  else//不能能撞到下一条路最近的车
	  {
	    cout << " MARK=07 " << endl;
	    //删除上一条路节点  并添加到下一条路
	    int delet_car = road[car_id_roadi].Cur_Road.Search_i(car_id);//删除下一条路车的ID号
	    cout << "delNODE: " << car_id << " car_id_roadi " << car_id_roadi << endl;
	    road[car_id_roadi].Cur_Road.Delete(delet_car);
	    linkinsertSort(road[car_id_roadi].Cur_Road.Head);
	    Node *p_l = road[car_id_roadi].Cur_Road.Head;
	    if (p_l->next != nullptr)
	    {
	      p_l = p_l->next;
	      while (p_l)
	      {
		if((p_l->car_dirction==car[car_id].situation.car_dirction)&&(p_l->flag==0)&&(p_l->car_channel==car[car_id].situation.car_channel))
		{
		  update_car_in_cross(p_l->car_id, road_num);
		  update_road_inside(p_l->car_id, road_num);
		}
		p_l = p_l->next;
	      }
	    }
	    /*******************************更新数据*********************************/
	    car[car_id].situation.flag = -1;
	    data[0] = car[car_id].situation.flag;						//flag 终止行驶
	    car[car_id].situation.car_dirction = car[car_id].situation.car_pass[1];
	    data[1] = car[car_id].situation.car_dirction; 					//方向
	    car[car_id].situation.car_position = car_V2max-car1_length_posotion;
	    data[2] = car[car_id].situation.car_position; 					//位置
	    car[car_id].situation.car_channel = sele_channel;
	    data[3] = car[car_id].situation.car_channel;  					//通道
	    data[4] = car_id;								//ID
	    car[car_id].situation.car_speed = car_V2max;
	    
	    data[5] = car_V2max;								//速度
	    data[6] = -2;									//优先级
	    data[7] = -2;									//转向
	    data[8] = 0;									//移动
	    pass.clear();
	    passed.clear();
	    car[car_id].situation.car_passed.push_back(car[car_id].situation.car_pass.front());//设置已经走过的路径
	    car[car_id].situation.car_pass.pop_front();					//删除已经走过的路径
	    pass = car[car_id].situation.car_pass;						//未来走的节点
	    passed = car[car_id].situation.car_passed;					//已经走过的路程
	    car[car_id].situation.road_id = map3d[car[car_id].situation.car_passed.back() - 1][car[car_id].situation.car_pass.front() - 1][0];
	    road[next_road_id].Cur_Road.AddNode(data, passed, pass);  //加入链表
	    cout << "02 ADDNODE: " << car_id << " next_road_id " << next_road_id << endl;
	    return car_id_roadi;//通过最大速度
	  }
	}
	else//V2导致不能出路口
	{
	  cout << " MARK=08 " << endl;
	  int roadi = road_in_to_i ( car[car_id].situation.road_id , road_num );//通过路的id查找哪条路（这段路）
	  Node * link_i = road[roadi].Cur_Road.Search( car_id); //这部车在链表哪个位置
	  /*******************************更新数据*********************************/
	  car[car_id].situation.flag = -1;						//flag 终止行驶
	  link_i->flag=car[car_id].situation.flag;
	  link_i->car_dirction=car[car_id].situation.car_dirction;
	  car[car_id].situation.car_position = road[car_id_roadi].length;
	  link_i->car_position=car[car_id].situation.car_position;
	  link_i->car_channel=car[car_id].situation.car_channel;								//ID
	  car[car_id].situation.car_speed = car_V1max;
	  /**********************************************************************/
	  Node *p = road[car_id_roadi].Cur_Road.Head;
	  if (p->next != NULL)
	  {
	    
	    if((p->car_dirction==car[car_id].situation.car_dirction)&&(p->flag==0)&&(p->car_channel==car[car_id].situation.car_channel))
	    {
	      update_car_in_cross(p->car_id, road_num);
	      update_road_inside(p->car_id, road_num);
	    }
	    p = p->next;
	  }
	  return car_id_roadi;
	}
      }
      else//V1导致不能出路口
      {
	cout << " MARK=09 " << endl;
	//linkinsertSort(road[car_id_roadi].Cur_Road.Head);
	int roadi = road_in_to_i ( car[car_id].situation.road_id , road_num );//通过路的id查找哪条路（这段路）
	Node * link_i = road[roadi].Cur_Road.Search( car_id); //这部车在链表哪个位置
	/*******************************更新数据*********************************/
	car[car_id].situation.flag = -1;
	link_i->flag=car[car_id].situation.flag;
	link_i->car_dirction=car[car_id].situation.car_dirction;					//方向
	car[car_id].situation.car_position = car[car_id].situation.car_position+car_V1max;
	link_i->car_position=car[car_id].situation.car_position; 
	car[car_id].situation.car_speed = car_V1max;
	/**********************************************************************/
	linkinsertSort(road[car_id_roadi].Cur_Road.Head);
	Node *p = road[car_id_roadi].Cur_Road.Head;
	if (p->next != NULL)
	{
	  if((p->car_dirction==car[car_id].situation.car_dirction)&&(p->flag==0)&&(p->car_channel==car[car_id].situation.car_channel))	  
	  {
	    update_car_in_cross(p->car_id, road_num);
	    update_road_inside(p->car_id, road_num);
	  }
	  p = p->next;
	}	
	return car_id_roadi;
      }  
    }
  }  
}  


//查询道路状态（最后面的车在哪里）?????方向如何考虑
int first_channel(Node * car_link ,int dirction , int channel)//, int &short_position , int &first//没车返回 0 没位置返回 -1
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

//在一段路上行驶 ?????没有方向
int car_passdistence(int channel,int car_id,Node * car_link,int road_s ,int dirction)
{
  if(channel==-1)
    return 0;
  else{
    Node *temp;
    
    //int vmax=car[car_id].situation.car_speed;
    int vmax=velocity(car_id,road_s);  
    int thiscar_s=0;//car[car_id].situation.car_position
    if(car_link == nullptr)
    {
      cout <<  "car_id="<<car_id <<"mark01"<<endl;
      return vmax;
    }
    temp=car_link->next;
    int cari;
    int car_s=0;      //此通道上最近车辆的距离
    
    int num=0;
    while(temp)
    {
      if(temp->car_id==82)
	cout <<  "car_id="<<car_id<<"dirction="<<dirction<<"channel= "<<channel <<temp->car_id<<" 82 "<<"temp->car_dirction"<<temp->car_dirction <<endl;
      if(temp->car_channel==channel && temp->car_dirction == dirction)
      {
	num++;
      }    
      temp=temp->next;
    }
    if(num==0)
    {
      cout <<  "car_id="<<car_id <<"mark02"<<endl;
      return vmax;
    }
    
    temp=car_link->next; 
    
    while(temp)
    {
      if((car_s==0||temp->car_position<car_s)&&temp->car_channel==channel && temp->car_dirction == dirction)
      {
	car_s=temp->car_position;
	cari=temp->flag;
      }
      
      temp=temp->next;
    }
    
    if(vmax < (car_s - thiscar_s))	//car_s 此通道上最近车辆的距离
      //thiscar_s 正在行驶的车
    {
      cout <<  "car_id="<<car_id <<"mark05"<<endl;
      return vmax;
    }
    else if(vmax >= (car_s - thiscar_s))
    {
      if(cari == -1)
      {
	cout <<  "car_id="<<car_id <<"mark03"<<endl;
	return car_s - thiscar_s - 1;
      }
      else
      {
	cout <<  "car_id="<<car_id <<"mark04"<<endl;
	return -1;
    
      }
    }
  }
}


//车在一条路上走
//车只在一条路上(无阻挡)   //可以到路口
void update_car_in_cross(int car_i,int road_num)
{
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
  
  //car[car_i].situation.car_dirction    //方向不变
  //car[car_i].situation.car_channel   //车道不变 
  //car[car_i].situation.road_id        //道路id不变 
  //car[car_i].situation.car_speed  //速度不变
  //car[car_i].situation.car_pass  //规划的路不变
  //car[car_i].situation.car_passed   //走过的路不变
  
  
  int roadi = road_in_to_i ( car[car_i].situation.road_id , road_num );//通过路的id查找路的编号（这段路）
  
  //  int i=road[roadi].Cur_Road.Search_i( car_i);
  
  //  road[roadi].Cur_Road.GetData(i , car_data , passed , pass);//获取车现在的状态
  Node *p= road[roadi].Cur_Road.Head;
  if(p->next != nullptr)
  {
    
    cars = road_sche(p , car_i , road_num); //lookfor_road(roadi)
    //   cout<<"cars_flag:"<<cars<<endl;
    if(cars >= 0 ) //不是等待状态，可以移动!= -1
    {
      for(int i=0;i<9;i++)
	car_data[i]=-100000;   //清空数组
	//       passed.clear();
	//       pass.clear();    
	
	car[car_i].situation.car_position= car[car_i].situation.car_position+cars;
      
      car_data[2]=car[car_i].situation.car_position;
      if(roadi ==1)
	cout<<"1 car_pos:"<<car_data[2]<<endl;
      //   car[car_i].situation.token = 1;                      //优先级??????  
      car[car_i].situation.is_moved = 1;                  //是否移动标志:置1
      car_data[8]=car[car_i].situation.is_moved; 
      car[car_i].situation.flag=-1; //终止状态
    }
    car_data[0]=car[car_i].situation.flag;
  }
  //linkinsertSort( road[roadi].Cur_Road.Head );
}



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


//查找车库中车的数量
int lookfor_cross(int cross_i ,vector<vector<int>>  car_samestartid)
{
  vector<int>::iterator ite_time;
  int num=0;
  for(ite_time = car_samestartid[cross_i].begin(); ite_time !=car_samestartid[cross_i].end(); ++ite_time)
  {
    if(car[*ite_time].situation.flag == 1)//||car[*ite_time].situation.flag == 2
    {
      num++;
    }
  }
  return num;
}


//判断哪些车可以出发
void car_start_flag(int T , int car_num)
{
  //这个时间点能出发的车
  for(int i=0;i<car_num;i++)
  {
    if(car[i].start_time <= T && car[i].situation.flag ==2)
    {
      car[i].situation.flag=1;
    }
  }
}

/********************车库调度********************/
void car_start_to_road(int cross_i ,  int out , vector<vector<int>>  car_samestartid, int road_num,int T )
{   
  vector<int>::iterator ite_time;	   
  for(ite_time = car_samestartid[cross_i].begin(); ite_time !=car_samestartid[cross_i].end(); ++ite_time)
  {
    if(car[*ite_time].situation.flag == 1)
    {
      int road_s=0;
      road_s=road_in_to_i(map3d[cross_i][car[*ite_time].situation.car_pass[1]-1][0],road_num);//查询行驶道路编号
      int out_channel=first_channel(lookfor_road(road_s),car[*ite_time].situation.car_pass[1],map3d[cross_i][car[*ite_time].situation.car_pass[1]-1][3]);
      
      int out_dist=car_passdistence(out_channel,*ite_time,lookfor_road(road_s),road_s ,  car[*ite_time].situation.car_pass[1]);
      cout<<*ite_time<<" out_channel= "<<out_channel<<" out_dist="<<out_dist<<" road_s= "<<road_s<<endl;
      if(out_dist >0)
      {
	update_car_in_road(*ite_time,road_num,out_channel,out_dist,T);
	update_road_in(road_s);
	out--;
      }
    }
    if(out == 0)
      break;		
  }
}



int count_car(int road_num)
{
  int car_count=0;
  for(int k=0;k<road_num;k++)//道路调度
  {
    car_count=(road[k].Cur_Road.ListLength()-1)+car_count;   
  }  
  //cout<<"count: "<<car_count<<endl;
  return car_count;
}

//重新规划路径
void reload_Dijkstra(int cross_num ,int car_num, int del_start,int del_end,int car_id)
{
  
  map3d_copy=map3d;

  map3d_copy[del_start-1][del_end-1][1]=MAX;
  map3d_copy[del_end-1][del_start-1][1]=MAX;
  for(int i = 0; i < cross_num; i++)
    for (int j = 0; j < cross_num; j++)
	  road_length[i][j]=-1;
  for(int i = 0; i < cross_num; i++)
    for (int j = 0; j < cross_num; j++)
      path[i][j].clear();
  
  for (int i = 0; i < cross_num; i++)
  {
    for (int j = 0; j < cross_num; j++)
    {   
      if(((i!=j)&&(map3d_copy[i][j][1]==0)))
      {
	road_length[i][j]=MAX;	
      }
      else
      {
	road_length[i][j]=map3d_copy[i][j][1];
      }
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
  
  Dijkstra(cross_num);
/*  int begin=car[car_id].situation.car_pass[0];
  car[car_id].situation.car_pass.clear();
  
  int num=0;
  vector<int>::iterator ite;
  for (ite = path[begin-1][car[car_id].end-1].begin(); ite != path[begin-1][car[car_id].end-1].end(); ++ite) {
    if (ite == path[begin-1][car[car_id].end-1].begin())
    {
      car[car_id].situation.car_pass.push_back(*ite);
      cout <<car[car_id].situation.car_pass[num];
      num++;
    }
    else
    {
      car[car_id].situation.car_pass.push_back(*ite);
      cout <<"->"<<car[car_id].situation.car_pass[num];
      num++;
    }
    
  }
  cout<<endl;
  */
  
 // Search_car_path(car_id,begin,car[car_id].end);
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
  ofstream answer_file(argv[4]);
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
	Search_car_path(i,car[i].start,car[i].end);
	cout <<i<<endl;
      }
      
      
      
      
      //reload_Dijkstra(cross_num,car_num,15,16,0);
      /********************************************/
      vector<vector<int>> car_samestartid(cross_num);
      /*car_samestartid.resize(cross_num);
       *	//path_out.resize(cross_num);
       *	for (int k = 0; k < cross_num; ++k){
       *            car_samestartid[k].resize(cross_num);//每行为c列
       *	    //path_out[k].resize(cross_num);
}
*/
      
      
      
    // reload_Dijkstra(cross_num,car_num,15,16,0);
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
      vector<int> next_roadid;
      int first_time=0;
      int car_cross_num=0;
      vector<int> road_channel;
      int cross_car[4];
      int T=0;
      int cross_a=0;
      
      /*	
       *	int x=lookfor_cross(6,car_samestartid);
       *	//for(int i=0;i<cross_num;i++)
       *	car_start_to_road(6 ,3,car_samestartid, road_num) ;
       *	cout<<"left1:"<<x<<endl;
       *	cout<<"left:"<<lookfor_cross(6,car_samestartid)<<endl;
       *	cout<<"out_car: "<<out_car<<endl;
       */	
      
      
      while(end_sys(car_num) || T == 0)
      {
	T++;    
	update_start(road_num);//更新路上的车的flag
	
	for(int k=0;k<road_num;k++)//每个链表排序
	{
	  linkinsertSort( road[k].Cur_Road.Head );
	}
	
	for(int k=0;k<road_num;k++)//道路调度
	{         
	  Node *p_l=road[k].Cur_Road.Head;
	  if(p_l->next != nullptr)
	  {
	    p_l=p_l->next;
	    while(p_l)
	    {
	      update_car_in_cross(p_l->car_id,road_num);
	      update_road_inside(p_l->car_id,road_num);
	      p_l=p_l->next;
	    }
	  }
	}
	
	while(End_Allcross(road_num))//路口调度
	{
	  for(int i=0;i<cross_num;i++)
	  {
	    cout<< "cross_num["<<i<<"]= "<<i<<endl;
	    Cross_Sche(i , road_num , T);
	  }
	}
	car_start_flag(T ,car_num);//判断那一辆车可以出发
	
	for(cross_a=0;cross_a<cross_num;cross_a++)
	{
	  if(count_car(road_num)<300)
	    car_start_to_road(cross_a ,1,car_samestartid, road_num,T) ; //车辆出车库		
	}
	
	
	for(int k=0;k<road_num;k++)
	{
	  if(k == 0)
	    cout << "Enter new flag display!" << endl;
	  Node *p_l=road[k].Cur_Road.Head;
	  // if(p_l->next != nullptr)
	  // {
	  p_l=p_l->next;
	  while(p_l)
	  {
	    cout <<"the flag of car " << p_l->car_id << " is "<< p_l->flag << " the road is "<< k;
	    cout <<" the channel and position of car is " << p_l->car_channel << " and "<<p_l->car_position << endl;
	    p_l=p_l->next;
	  }
	  // }
	}
	
	
	//if(T==3)break;
	cout<<" now T:"<<T<<endl;
	
      }
     
      answer_file << "#(carid,StartTime,RoadId...)" << endl;
      for(int k=0;k<car_num; k++)
      {
	answer_file<<"(";
	answer_file<<car[k].id;	
	answer_file<<",";
	answer_file<<car[k].situation.actual_start_time;
	answer_file<<",";
	
	for(int j=0;j<car[k].situation.car_passed.size()-1;j++)
	{
	  if(j==0)
	  {
	    answer_file<<map3d[car[k].situation.car_passed[j]-1][car[k].situation.car_passed[j+1]-1][0];
	  }
	  else
	  {
	    answer_file<<","<<map3d[car[k].situation.car_passed[j]-1][car[k].situation.car_passed[j+1]-1][0];
	  }
	}
	
	answer_file<<")"<<endl;
      }
     
      cout<<"finish T:"<<T<<endl; 
      cout<<"输出结束"<<endl;
       
      // TODO:write output file
      
      return 0; 
}