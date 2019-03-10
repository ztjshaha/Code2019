#ifndef __LIB_H__
#define __LIB_H__



typedef struct RoadState
{
    int position_state;
    int direction;
    int ran_road;
}RoadState;




typedef struct CrossState
{
    int id;
    int road_id[4];
}CrossState;




#endif
