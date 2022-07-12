#ifndef DISPATCH_CORE_H
#define DISPATCH_CORE_H
typedef struct Query {
    int type;
    int station;
    int genTime;
} Query ;

#define MODE_FCFS 1
#define MODE_SSTF 2
#define MODE_SCAN 3

#define CLOCKWISE 1
#define COUNTERCLOCKWISE 2
#define TARGET 3

#define STATE_WAITING 1
#define STATE_RUNNING 2
#define STATE_SERVING 3

int g_getNowState();

int g_getTotalStation();
//返回总的车站数量
int g_getDispatchMethod();
//返回调度模式
int g_getDistance();
//返回总的距离

Query* g_getNowQueryList(int*);
//返回当前的所有请求，作为数组返回
bool g_insertNewQuery(Query newQuery);
//向请求数组插入新的请求，成功插入返回true,否则返回false
int g_nextClock();
//时钟向下拨一秒，返回当前车辆所在位置
int g_getNowDirection();
//返回当前车辆行进方向
void g_initDispatchCore(int giveDistance,int giveStrategy,int giveTotalStation);
//用于初始化Dispatch Core
#endif // DISPATCH_CORE_H
