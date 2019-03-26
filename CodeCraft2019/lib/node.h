#ifndef NODE_H
#define NODE_H
#include <deque>
using namespace std;
/*class CarState
{  
public:
    int flag;
    int car_dirction;
    int car_id;
    int	car_position;
    int car_speed;
    vector <int> car_passed;
    vector <int> car_pass;
};*/
/*
template <class T>
class Node
{
public:
    Node<T> *next;
    Node<T> *prev;
    T 	flag;
    T 	car_dirction;
    T 	car_id;
    T	car_position;
    T 	car_speed;
    T 	car_passed;
    T 	car_pass;
};*/ 
struct Node {
	//int data = 100;
	Node *next;

	
	 int flag;		//1 等待出发
			//0 等待行驶
			//-1 终止行驶
			
			
        int car_dirction;
        int car_position;
        int car_channel;// 1 2 3分别三个车道数据
        int car_id;
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
 
#endif //TEST1_NODE_H
