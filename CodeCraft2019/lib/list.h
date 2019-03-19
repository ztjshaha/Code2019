#ifndef LIST_H
#define LIST_H
#include <iostream>
#include "node.h"
using namespace std;


class Linklist {
public:
	Linklist();
	int i;
	void AddNode(int data[7]);        //尾插入法创建链表
	int Delete(int i);             //删除表中元素
	int GetData(int i,int data[7]);            //取得表中元素值
        int Search_i(int obj);           //在表中寻找匹配项
	Node * Search(int obj);
	int ListLength();              //获得表的长度
	void Display();                //遍历整个链表
	Node * Head;
};

Linklist::Linklist() 
{                   //构造函数
	Head = new Node;
	Head->next = nullptr;
}

void Linklist::AddNode(int data[7]) 
{      //尾插入法创建链表
	Node *p;
	Node *temp;
	p = Head;
	temp = new Node;
	temp->flag=data[0];
	temp->car_dirction=data[1];
	temp->car_id=data[2];
	temp->car_position=data[3];
	temp->car_speed=data[4];
//	temp->car_passed=data[5];
//	temp->car_pass=data[6];
	p->next = temp;
	p = temp;
	p->next = nullptr;
}

int Linklist::Delete(int i) {          //删除i处的数据
	Node *temp;
	temp = Head;
	int j = 0;
	while (temp&&j < i - 1) {
		temp = temp->next;
		j++;
	}
	if (!temp || j > i - 1) {
		cout << "删除位置错误";
		return -1;
	}
	else {
		Node *s;
		s = temp->next;
		temp->next = s->next;
		delete s;
	}
}

int Linklist::GetData(int i,int data[7]){         //得到i处的元素
	Node *temp;
	temp = Head;
	int j = 0;
	//int data[7];
	while (temp&&j < i - 1) {
		temp = temp->next;
		j++;
	}
	if (!temp || j > i - 1) {
		cout << "寻找位置错误\n";
		return -1;
	}
	else {
		cout << i << "处的数据为：" << "flag:"<<temp->flag << "\n"
					  << "car_dirction:"<<temp->car_dirction << "\n"
					  << "car_id:"<<temp->car_id << "\n"
					  << "car_position:"<<temp->car_position << "\n"
					  << "car_speed:"<<temp->car_speed << "\n";
//					  << "car_passed:"<<temp->car_passed << "\n"
//					  << "car_pass:"<<temp->car_pass << "\n";
					  data[0]=temp->flag;
					  data[1]=temp->car_dirction;
					  data[2]=temp->car_id;
					  data[3]=temp->car_position;
					  data[4]=temp->car_speed;
//					  data[5]=temp->car_passed;
//					  data[6]=temp->car_pass;
		return 0;
	}
}

//input  obj:car_id
int Linklist::Search_i(int obj) {       //寻找链表中有无与obj匹配的元素
	int j = 1;
	Node *temp;
	temp = Head->next;
	while (temp && temp->next->car_id != obj) {
		temp = temp->next;
		j++;
	}
	if (temp == NULL) {
		cout << "该链表中无此car" << "\n";
		return 0;
	}
	else {
		cout << "在该链表中的第" << j << "car等于" << obj << "\n";
		return j;
	}
	//temp->next= NULL;
}

//input  obj:car_id
Node * Linklist::Search(int obj) {       //寻找链表中有无与obj匹配的元素
//	int j = 1;
	Node *temp;
	temp = Head->next;
	while (temp && temp->next->car_id != obj) {
		temp = temp->next;
//		j++;
		return temp;
	}
	
	if (temp == NULL) {
		cout << "该链表中无此car" << "\n";
		return NULL;
	}
	//temp->next= NULL;
}


int Linklist::ListLength() {                //计算链表长度
	Node *temp;
	temp = Head;
	int j = 0;
	while (temp) {
		temp = temp->next;
		j++;
	}
	cout << "该链表的长度为：" << j - 1 << "\n";
	return j;
}

void Linklist::Display(){
	Node *temp;
	temp = Head->next;
	int e;
	cout << "该链表的遍历依次为：";
	while (temp) {
		//e = temp->data;
		cout << temp->flag << " "<<temp->car_dirction<< " "<<temp->car_id<< " "<<temp->car_position<< " "<<temp->car_speed<< " "<<endl;
		temp = temp->next;
	}
	cout << "\n";
}
//temp->car_passed<< " "<<temp->car_pass<<
#endif
