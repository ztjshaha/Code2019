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

//data
//0:flag  1:car_dirction  2:car_position  3:car_channel  
//4:car_id  5:car_speed  6:token  7:car_turn  8:is_moved
int car_data[9];
deque <int> passed;
deque <int> pass;

    int cars;

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




//出车库
//车只在一条路上(无阻挡)  //到不了路口  (可以直接到)
void update_car_in_road(int car_i,int road_num,int road_channel,int car_passdist)
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
  road_s=road_in_to_i(map3d[car[car_i].situation.car_passed.front()-1][car[car_i].situation.car_pass.front()-1][0],road_num);//查询行驶道路编号
  
  car[car_i].situation.car_speed=velocity(car_i,road_s);
  car_data[5]=car[car_i].situation.car_speed;
  
  car[car_i].situation.car_position=car[car_i].situation.car_position + car_passdist;//位置变化
  car_data[2]=car[car_i].situation.car_position;
  
  car[car_i].situation.road_id=map3d[car[car_i].situation.car_passed.front()-1][car[car_i].situation.car_pass.front()-1][0];
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
    return NULL;
  }
  else
  {
    
    return road[roadi].Cur_Road.Head;//返回链表头
  }
}

int road_sche(Node * car_link,int car_id,int road_num)
{
  Node *temp; 
  Node *p;
  int position=0;
  int min_position=-1;
//  int i=0;
  int flag=-1;
  if(car_link==NULL)
  {
    return 0;
  }
  int road_s;//道路编号转换
  road_s=road_in_to_i(car[car_id].situation.road_id,road_num);//查询行驶道路编号
  p=car_link->next;
  while(p)//遍历找车
  {
    if(p->car_position>car[car_id].situation.car_position&&p->car_channel==car[car_id].situation.car_channel&&p->car_dirction==car[car_id].situation.car_dirction)//找出前面所有车
    {
      position=p->car_position-car[car_id].situation.car_position;
 //     if(i==0)
 //     {
	flag=car[p->car_id].situation.flag;
	min_position=position;
//      }else{
      if(min_position>position)
      {
	flag=car[p->car_id].situation.flag;
	min_position=position;
      }
//    }
    
    }
    p=p->next;  
  }

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
	      return (min_position-1);
	  }
	}//是阻拦
	else
	{
	  return car[car_id].situation.car_speed;
	}//没阻拦
    }
    else//前方没车
    {
      int roadidnext=road_in_to_i(map3d[car[car_id].situation.car_pass.front()][car[car_id].situation.car_pass[1]][0] , road_num);
      
      if((car[car_id].situation.car_speed>(road[road_s].length-car[car_id].situation.car_position))&& //road[road_s].limit_speed
	((road[road_s].length-car[car_id].situation.car_position)<
	velocity(car_id ,roadidnext)))//判断能否出路口//能出路口 
      {
	car[car_id].situation.flag=0;
	return -1;
      }else
      {
	if( road[road_s].length-car[car_id].situation.car_position >= car[car_id].situation.car_speed)
	{
//	  cout<<"return "<<car[car_id].situation.car_speed<<endl;
	  return car[car_id].situation.car_speed; 
	}
	else
	  return road[road_s].length-car[car_id].situation.car_position;
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


vector<int> Cross_first_car(int cross_id)//测试  不能通过OK  通过后续还需要测试
{
  int cross_road_num=0;
  int road_serial[4];
  vector<int> car_id;
  vector<int> back_car;
  vector<int> sele_car;
  int small_channel=0;
  int j=0;
  for(int i=0;i<4;i++)
    road_serial[i]=cross[cross_id].road_id[i];
  insertSort(road_serial, 4);   //排序后的道路顺序在road_serial数组里面
  for(int i=0;i<4;i++)
  {
    if(road_serial[i]!=-1)
    cross_road_num++; 
  }
  Node * temp[cross_road_num];
  for(int i=0;i<4;i++)
  {
    
     
    if(road_serial[i]!=-1)
    {
    temp[j]=road[road_serial[i]-5000].Cur_Road.Head;////-5000
    j++; 
    }else
    {
      temp[j]=NULL;
      j++; 
    } 
    }
  for(int i=0;i<4;i++)
  {
    if(temp[i]==NULL)
    {
      back_car.push_back(-1);
      cout << "(没车)back_car.back()="<<back_car.back()<<endl;
    }else{
    temp[i]=temp[i]->next;
    int max_position=0;
    while(temp[i])
    {
      //.clear();
      //back_car.clear();
      
	if((temp[i]->car_pass.front()-1)==cross_id)
	{
	  cout<<"temp[i]->car_id="<<temp[i]->car_id<<"temp[i]->car_pass.front()="<<temp[i]->car_pass.front()<<endl;
	    if((temp[i]->car_position>max_position)&&(temp[i]->flag==0))
	      {
		car_id.clear();
		max_position=temp[i]->car_position;
		car_id.push_back(temp[i]->car_id); ///?????
		
	      }else
	      {
		if((temp[i]->car_position=max_position)&&(temp[i]->flag==0))
		{
		  car_id.push_back(temp[i]->car_id);
		}
	      }
	}
    temp[i]=temp[i]->next;
    }
    if(car_id.size()>1)
      {
      vector<int>::iterator ite;
      for(ite=car_id.begin();ite!=car_id.end();++ite)
      {
	if(ite==car_id.begin())
	{
	  sele_car.clear();
	  small_channel=car[*ite].situation.car_channel;
	  sele_car.push_back(*ite);
	}
	else{
	      if(small_channel>car[*ite].situation.car_channel)
		{
		  small_channel=car[*ite].situation.car_channel;
		  sele_car.clear();
		  sele_car.push_back(*ite);
		}
	    }
      } 
      back_car.push_back(sele_car.back());
      cout << "(有车)back_car.back()="<<back_car.back()<<endl;
    }
    else
    {
      if(car_id.size()==1)
      {
	back_car.push_back(car_id[0]);
	cout << "(有车)back_car.back()="<<back_car.back()<<endl;
	//return back_car;
	
      }
      else{
      back_car.push_back(-1);
      cout << "(没车)back_car.back()="<<back_car.back()<<endl;
      //return back_car;
      }
      
      
    }//找最小路口
    car_id.clear();
  }
 }
  
  return back_car;
 // back_car.clear();
}

int roadIndexInCross(int cross_id,int road_id)  //通过cross_id和road_id找到road在cross文件中的下标
{
  int i = 0;
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
      break;
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
    int index_i = roadIndexInCross(cross_id,roadi);
    int index_j = roadIndexInCross(cross_id,roadj);
    switch ((index_i - index_j + 4) % 4)
    {
      case 0:break;
      case 1:return 2;
      case 2:return 0;
      case 3:return 1;
    }
  }
}



int Cross_Sche(int cross_id)
{
  vector<int> car_id;
  int next_road_id=0;
  int turn[4];
  car_id=Cross_first_car(cross_id);
  int back_carid=-1;
  for(int i=0;i<4;i++)
  {
  cout<<" car_id.back= "<<car_id[i]<<endl;
  //insertSort(cross[cross_id].road_id, 4);
  if(car_id[i]==-1)
    turn[i]=-1;
  else
  {
    turn[i]=nextOrientation(cross_id,car[car_id[i]]);
    cout<<"turn ="<<turn[i]<<endl;
    cout<<car[car_id[i]].situation.car_pass.front();
  }
  }
  for(int i=0;i<4;i++)
  {
    //cout<<"turn["<<i<<"] ="<<turn[i]<<endl;
    if(turn[i]==0)
    {
      back_carid= car_id[i];
	return back_carid;
    }
      else
      {
	if(turn[i]==1)
	{	//如果左转
		  for(int j=0;j<4;j++)//找出上一个路口
		  {
		    if(cross[cross_id].road_id[j]==car[car_id[i]].situation.road_id)//找到正在行驶车的道路
		    {
			next_road_id=(j-1 + 4) % 4;
		    }
		  }
		    for(int k=0;k<4;k++)
		    {
		      if(cross[cross_id].road_id[next_road_id]==-1)//没路
		      {
			back_carid= car_id[i];
			return back_carid;
		      }
		      else if(car_id[k]==-1)
		      back_carid=-1; //有路没车
		      else//有路有车
		      {
			if(cross[cross_id].road_id[next_road_id]==car[car_id[k]].situation.road_id)//找到了上一个路口的车,如果这条路有车
			{
			  if(turn[k]==0)
			  {
			    car[car_id[i]].situation.flag=0;//等待
			    back_carid=-1;
			    return back_carid;
			  }
			  else
			  {
			    back_carid= car_id[i];
			    return back_carid;
			  }
			}
		      }
		    }
		return back_carid;
		}
		else if(turn[i]==2)
		{ //如果右拐
		  for(int j=0;j<4;j++)//找出上一个路口
		  {
		    if(cross[cross_id].road_id[j]==car[car_id[i]].situation.road_id)//找到上一个路口
		    {
		      next_road_id=(j + 1) % 4;
		    }
		  }
		  for(int k=0;k<4;k++)//找直行
		  {
		    if(car_id[k]==-1);
		      else
		      {
			if(cross[cross_id].road_id[next_road_id]==car[car_id[k]].situation.road_id)//找到了上一个路口的车
			{
			  if(turn[i]==0)
			  car[car_id[i]].situation.flag=0;//等待
			}
			//else
			  //return car_id[i];
		      } 
		  }
		    //////////////////////////////////////////////////////////////////////////////////////////////
		  for(int j=0;j<4;j++)//找左拐
		  {
		    if(cross[cross_id].road_id[j]==car[car_id[i]].situation.road_id)
		    {
		      next_road_id=(j + 2) % 4;
		    }
		  }
		  for(int g=0;g<4;g++)
		  {
		    if(car_id[g]==-1);//没车
		      else
		      {//有车
			if(cross[cross_id].road_id[next_road_id]==car[car_id[g]].situation.road_id)//找到了上一个路口的车
			{
			  if(turn[g]==1)
			  car[car_id[i]].situation.flag=0;//等待
			}
			  else
			  {
			     back_carid=car_id[i];
			      return back_carid;
			  }
			
		      }
		      back_carid=-1;
		      //return back_carid;
		  }
		}else if(turn[i]==-1) back_carid=-1;
		//return back_carid;
      }
  }
   return back_carid;
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

//在一段路上行驶 ?????没有方向
int car_passdistence(int channel,int car_id,Node * car_link,int road_s ,int dirction)
{
  if(channel==-1)
    return 0;
  else{
  Node *temp;
  
   //int vmax=car[car_id].situation.car_speed;
  int vmax=velocity(car_id,road_s);  
  int thiscar_s=car[car_id].situation.car_position;
  if(car_link==NULL)
  {
    return vmax;
  }
  temp=car_link->next;
  int cari;
  int car_s=0;      //此通道上最近车辆的距离
  
  int num=0;
  while(temp)
  {
     if(temp->car_channel==channel && temp->car_dirction == dirction)
    {
	num++;
    }    
    temp=temp->next;
  }
  if(num==0)
    return vmax;
  
   temp=car_link->next; 
  
  while(temp)
  {
    if((car_s==0||temp->car_position<car_s)&&temp->car_channel==channel && temp->car_dirction == dirction)
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
    if(cars > 0 ) //不是等待状态，可以移动!= -1
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
	int out_car=0;
	int cross_car[4];
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
			int out_channel=first_channel(lookfor_road(road_s),car[*ite_time].situation.car_pass[1],map3d[k][car[*ite_time].situation.car_pass[1]-1][3]);
			cout<<"map3d[k][car[*ite_time].situation.car_pass[1]-1][3]:"<<map3d[k][car[*ite_time].situation.car_pass[1]-1][3]<<endl;
			cout<<car[*ite_time].situation.car_pass[1]<<endl;
			cout <<"out_channel="<<out_channel<<endl;
			int out_dist=car_passdistence(out_channel,*ite_time,lookfor_road(road_s),road_s , car[*ite_time].situation.car_pass[1]);
			cout<<"out_dist="<<out_dist<<endl;
			if(out_dist >0)
			{
			update_car_in_road(*ite_time,road_num,out_channel,out_dist);
			update_road_in(road_s);
			}
			Node* n=lookfor_road(road_s);
			if(n!=NULL){
			  
			
			cout<<"car_passed.front()="<<n->next->car_passed.back()<<endl;
			cout<<"car_position="<<n->next->car_position<<endl;
			cout<<"car_channel="<<n->next->car_channel<<endl;
			if(out_dist>0)
			{
			  out_car++;
			  
			}
			cout << "out_car= "<<out_car<<endl;
			}
			
			cout << endl;
		      
		 }
			else
			  {
			    cout<<"car_id= "<<*ite_time<<endl;
			    int road_s=0;
			    cout<<map3d[k][car[*ite_time].situation.car_pass[1]-1][0]<<endl;
			    road_s=road_in_to_i(map3d[k][car[*ite_time].situation.car_pass[1]-1][0],road_num);//查询行驶道路编号
			    cout <<"road_s="<<road_s<<endl;
			    int out_channel=first_channel(lookfor_road(road_s),car[*ite_time].situation.car_pass[1],map3d[k][car[*ite_time].situation.car_pass[1]-1][3]);
			    cout<<"map3d[k][car[*ite_time].situation.car_pass[1]-1][3]:"<<map3d[k][car[*ite_time].situation.car_pass[1]-1][3]<<endl;
			    cout<<"car[*ite_time].situation.car_pass[1]="<<car[*ite_time].situation.car_pass[1]<<endl;
			    //int bb=car_passdistence(aa,*ite_time,lookfor_road(road_s));
			    cout <<"out_channel:  "<<out_channel<<endl;
			    int out_dist=car_passdistence(out_channel,*ite_time,lookfor_road(road_s),road_s ,  car[*ite_time].situation.car_pass[1]);
			    cout<<" out_dist="<<out_dist<<endl;
			    
			    if(out_dist >0)
			    {
			    update_car_in_road(*ite_time,road_num,out_channel,out_dist);
			    update_road_in(road_s);

			    }
			    //update_road_in
			    Node* n=lookfor_road(road_s);
			    //int road_listlenth=road[road_s].Cur_Road.();
			   // for(int i=0;i<road_listlenth-1;i++){
			     // n=n->next;
			    //}
			   
			    cout<<"car_passed.back()="<<n->next->car_passed.back()<<endl;
			    cout<<"car_position="<<n->next->car_position<<endl;
			    cout<<"car_channel="<<n->next->car_channel<<endl;
			      
			    if(out_dist>0)
			      {
				out_car++;
				  
			      }
			      cout << "out_car= "<<out_car<<endl;
			    

			    cout << endl;
			  }	  
	    }

	}

for(int i=0;i<car_num;i++)
{
  if(car[i].situation.flag==-1)
  {
    car[i].situation.is_moved=0;
  }
}
	

for(int roadi=0;roadi<road_num;roadi++)
{
  cout<<"before road:"<<roadi<<endl;
  Node *p=road[roadi].Cur_Road.Head;
    if(p->next != nullptr)
  {
    p=p->next;
    while(p)
    {    
       cout<<"p->flag="<<p->flag<<endl;
       cout<<"p->id="<<p->car_id<<endl;
       cout<<"p->car_position="<<p->car_position<<endl;  
       cout <<"p->car_speed="<<p->car_speed<<endl;
       cout <<"p->car_dirction="<<p->car_dirction<<endl;
           p=p->next;
    }
  }
     cout<<endl;
}

  Node *p2=road[1].Cur_Road.Head;
   p2=p2->next;
    while(p2)
    {
             cout<<" before 1 p->car_position="<<p2->car_position<<endl;  
	        p2=p2->next;
    }

for(int j=0;j<2;j++)
{
  for(int k=0;k<road_num;k++)
  {   
      
    Node *p_l=road[k].Cur_Road.Head;
    if(p_l->next != nullptr)
    {
      p_l=p_l->next;
      while(p_l)
      {
	update_car_in_cross(p_l->car_id,road_num);
	if(k==1)
	    cout<<"carid "<<p_l->car_id<<endl;
	update_road_inside(p_l->car_id,road_num);
	p_l=p_l->next;       
      }
    }
  //  linkinsertSort( road[roadi].Cur_Road.Head );
  }
}

for(int k=0;k<road_num;k++)
{
 // linkinsertSort( road[k].Cur_Road.Head );
}


for(int roadi=0;roadi<road_num;roadi++)
{
  cout<<"road:"<<roadi<<endl;
  Node *p=road[roadi].Cur_Road.Head;
    if(p->next != nullptr)
  {
    p=p->next;
    while(p)
    {    
       cout<<"p->flag="<<p->flag<<endl;
       cout<<"p->id="<<p->car_id<<endl;
       cout<<"p->car_position="<<p->car_position<<endl;  
       cout <<"p->car_speed="<<p->car_speed<<endl;
       cout <<"p->car_dirction="<<p->car_dirction<<endl;
           p=p->next;
    }
  }
     cout<<endl;
}
/*
 Node *p1=road[1].Cur_Road.Head;
 p1=p1->next;
 cout<<p1->car_position<<endl;

*/
deque<int>::iterator it;
 //for(ite_time = car_samestartid[k].begin(); ite_time !=car_samestartid[k].end(); ++ite_time)

//int bcar[car_num];
for(int i=0;i<car_num;i++)
{
  cout<<"bcar:"<<i;
  for(it=car[i].situation.car_pass.begin(); it != car[i].situation.car_pass.end(); ++it)
                       cout<<"   "<<*it;
 // cout<<"   "<<car[i].situation.road_id;
//  cout<<"   "<<car[i].situation.car_dirction;
 // cout<<"   "<<car[i].situation.car_speed;
 // cout<<"   "<<car[i].situation.car_position;
 // cout<<"   "<<car[i].situation.is_moved<<endl;
  cout<<endl;

}


 int id;
  for(int i=0;i<cross_num;i++)
  {
    id=Cross_Sche(i);
    cout<<"i:"<<i;
    cout <<"  id:"<<id<<endl;
}
	cout<<"输出结束"<<endl;
	

	
	// TODO:write output file
	
	return 0; 
}
