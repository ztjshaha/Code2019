#ifndef NODE_H
#define NODE_H
#include "vector"
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
};
 
 
#endif //TEST1_NODE_H
