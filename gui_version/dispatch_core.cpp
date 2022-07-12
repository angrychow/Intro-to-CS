#include "dispatch_core.h"
#include<cstdio>
#include <iostream>
#include <cstdlib>
#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>





//变量定义
int distance, strategy, totalStation, totalDistance;

//请求列表和当前请求个数
Query QueryList[1000];
int QueryNumber=0;

//当前站点请求情况
int isCounterclockwiseQuery[1001]={0};
int isClockwiseQuery[1001]={0};
int isBusQuery[1001]={0};

//巴士任务执行状态
int isInCharge = 0;
int stationGoto = 0; // station, not position

//巴士变量
int busPosition = 0;
int nowDirection = CLOCKWISE;
int nowClock = 0;

//原config_reader.c
//读取配置文件
int g_TotalStation = 5;
int g_distance = 2;
int g_mode = MODE_FCFS;
int g_totalDistance = g_distance*g_TotalStation;


//配合新函数使用
int s_isNearby;
int s_stationGoto;
int s_isinfinish;


//读取配置文件部分
void g_initDispatchCore(int giveDistance,int giveStrategy,int giveTotalStation){
    s_isNearby = 0;
    s_isinfinish = 0;
    s_stationGoto = 1;
    strategy = giveStrategy;
    distance = giveDistance;
    totalStation = giveTotalStation;
    totalDistance = distance*totalStation;
    g_TotalStation = giveTotalStation;
    g_distance = giveDistance;
    g_totalDistance = distance*totalStation;
}
int g_getTotalStation(){
    return g_TotalStation;
}
//返回总的车站数量
int g_getDispatchMethod(){
    return g_mode;
}
//返回调度模式
int g_getDistance(){
    return g_TotalStation*g_distance;
}
//返回总的距离

//接口实现
Query* g_getNowQueryList(int* retQueryNumber){
    Query* tempQueryList = (Query*)malloc(sizeof(Query)*QueryNumber);
    for(int i=0;i<QueryNumber;i++){
        tempQueryList[i].genTime=QueryList[i+1].genTime;
        tempQueryList[i].type=QueryList[i+1].type;
        tempQueryList[i].station=QueryList[i+1].station;
    }
    *retQueryNumber=QueryNumber;
    return tempQueryList;
}

int g_getNowDirection(){
    return nowDirection;
}

bool g_insertNewQuery(Query newQuery){
    //逆时针任务，且目标站点无请求
    if(newQuery.type == COUNTERCLOCKWISE && isCounterclockwiseQuery[newQuery.station] == 0)
    {
        isCounterclockwiseQuery[newQuery.station] = 1;
        QueryList[++QueryNumber].station = newQuery.station;
        QueryList[QueryNumber].type = COUNTERCLOCKWISE;
        QueryList[QueryNumber].genTime = newQuery.genTime;
        return true;
    }
    //顺时针任务，且目标站点无请求
    else if(newQuery.type == CLOCKWISE&&isClockwiseQuery[newQuery.station]==0)
    {
        isClockwiseQuery[newQuery.station] = 1;
        QueryList[++QueryNumber].station = newQuery.station;
        QueryList[QueryNumber].type = CLOCKWISE;
        QueryList[QueryNumber].genTime = newQuery.genTime;
        return true;
    }
    //车内请求
    else if(newQuery.type == TARGET&&isBusQuery[newQuery.station]==0)
    {
        isBusQuery[newQuery.station] = 1;
        QueryList[++QueryNumber].station = newQuery.station;
        QueryList[QueryNumber].type = TARGET;
        QueryList[QueryNumber].genTime = newQuery.genTime;
        return true;
    }
    //插入失败，返回false
    else
    {
        return false;
    }
}











//原get_next_task.c内容
int getNextTask(
    Query queryList[],
    int queryListNumber,
    int direction,
    int strategyType,
    int totalStation,
    int distance,
    int nowStation)
{
    int ret = 0;
    if (strategyType == MODE_FCFS)
        ret = 1;

    if (strategyType == MODE_SSTF)
    {
        int interval = 0x7fffffff; //当前确定的最近点的距离

        for (int i = 1; i <= queryListNumber; i++)
        {
            int d1; // d1为nowstation到queryList[i].station的顺时针距离
            if (queryList[i].station >= nowStation)
                d1 = queryList[i].station - nowStation;
            else
                d1 = (totalStation - nowStation) + queryList[i].station;

            int d2; // d2为nowstation到queryList[i].station的逆时针距离
            if (queryList[i].station <= nowStation)
                d2 = nowStation - queryList[i].station;
            else
                d2 = nowStation + (totalStation - queryList[i].station);

            if (d1 <= d2)
            { // nowStation顺时针到达queryList[i].station最近
                if (d1 <= interval)
                {                  //如果d1 == interval 直接换掉最省事 不用管原来最优解的方向
                    interval = d1; //因为：若原来最优解为顺时针  此时可以换也可以不换
                    //若原来最优解为逆时针 此时必须换
                    ret = i;
                }
            }

            else
            { // nowStation逆时针到达queryList[i].station最近
                if (d2 < interval)
                {                  //如果d2 == interval  不换最省事
                    interval = d2; //因为: 若原来最优解为顺时针  此时不能换
                    //若原来最优解为逆时针  此时可以换也可以不换
                    ret = i;
                }
            }
        }
    }

    if (strategyType == MODE_SCAN)
    {
        int nearest_clockwise, nearest_counterclockwise; //分别是顺时针到达的最近的车站 和 逆时针到达的最近的车站
        int min1 = 0x7fffffff, min2 = 0x7fffffff;        //分别是顺时针到达的最近的车站的距离 和  逆时针到达的最近的车站的距离
        for (int i = 1; i <= queryListNumber; ++i)
        {
            int d1; // d1为nowstation到queryList[i].station的顺时针距离
            if (queryList[i].station >= nowStation)
                d1 = queryList[i].station - nowStation;
            else
                d1 = (totalStation - nowStation) + queryList[i].station;

            int d2; // d2为nowstation到queryList[i].station的逆时针距离
            if (queryList[i].station <= nowStation)
                d2 = nowStation - queryList[i].station;
            else
                d2 = nowStation + (totalStation - queryList[i].station);

            if (d1 < min1)
            {
                min1 = d1;
                nearest_clockwise = i;
            }

            if (d2 < min2)
            {
                min2 = d2;
                nearest_counterclockwise = i;
            }
        }

        if (direction == CLOCKWISE)
            ret = nearest_clockwise;
        else
            ret = nearest_counterclockwise;
    }

    return ret;
}

int hasNearBy(
    Query queryList[],
    int *queryListNumber,
    int strategyType,
    int totalStation,
    int distance,
    int nowStation,
    int nowTime,
    int direction)
{
    int n = *queryListNumber, cnt = 0;
    Query copy[n + 1]; //**修复了数组长度为n  导致访问copy[n]出错的bug

    for (int i = 1; i <= n; ++i)
    {
        // SSTF的顺便服务判断
        //if (strategyType == MODE_SSTF && queryList[i].genTime <= nowTime - 2 && queryList[i].type == direction && queryList[i].station == nowStation) //在到站前一秒之前；同方向；同位置
                                                                                                                                                      //由于nowTime已经+1,故nowTime-1是到达该点的时间，可以顺便服务的任务必须在nowTime-1之前
        //** SSTF顺便服务判断更新 + 修复了少考虑target请求的bug
        if(strategyType == MODE_SSTF && queryList[i].station == nowStation && (queryList[i].type == TARGET || queryList[i].type == direction)&& queryList[i].genTime <= nowTime - 2 )
            continue;

        //** SCAN顺便服务判断更新 + 修复了少考虑target请求的bug
        //if (strategyType == MODE_SCAN && queryList[i].genTime <= nowTime - 2 && queryList[i].station == nowStation)
        if(strategyType == MODE_SCAN && queryList[i].station == nowStation&& queryList[i].genTime <= nowTime - 2)
            continue;

        copy[++cnt] = queryList[i]; //否则该任务保留
    }

    for (int i = 1; i <= cnt; ++i)
        queryList[i] = copy[i];
    *queryListNumber = cnt;

    return n - cnt;
} /*
 这个函数你要返回的是，如果存在可以顺便服务的请求，并且删除，重排请求；如果没有则返回0。有多种方案全部删除。返回完成的请求数量
 特别注意：如果车辆在某站点本没有停车计划，新的请求要至少要提前1秒钟产生才能享受顺便服务。也就是说不为太近的请求停车。
 */

int fulfillQuery(
    Query queryList[],
    int *queryListNumber,
    int direction,
    int strategyType,
    int totalStation,
    int distance,
    int nowStation)
{
    int n = *queryListNumber, cnt = 0;
    Query copy[n + 1]; //** 修复了数组长度为n,导致访问copy[n]出错的bug
    for (int i = 1; i <= n; ++i)
    {
        if (queryList[i].station == nowStation)
            continue;
        if (strategyType == MODE_FCFS)
               {
                   for (int j = i; j <= n; ++j)
                       copy[++cnt] = queryList[j];
                   break;
               }

        copy[++cnt] = queryList[i];
    }

    for (int i = 1; i <= cnt; ++i)
        queryList[i] = copy[i];
    *queryListNumber = cnt;

    return n - cnt;
} /*
 这个函数里，你要处理汽车到达目的地后的请求列表。汽车一次停靠可能会完成多个任务，请将完成的任务删掉并重新排列，返回完成的任务数。
 */













//原maic.c确定方向，方位，前进，更新列表等一些函数
// MODE_SCAN第一次任务确定方向标志位
int flagFirstTime = 1;

int getClockwiseDist(int targetPos)
{
    return ((targetPos - busPosition + totalDistance) % totalDistance);
}

int getCounterclockwiseDist(int targetPos)
{
    return ((busPosition - targetPos + totalDistance) % totalDistance);
}

int getStationPosition(int stationToCalc)
{
    return ((stationToCalc - 1) * distance);
}

//前进函数
void moveForward()
{
    if (nowDirection == CLOCKWISE)
    {
        busPosition = (busPosition + 1) % totalDistance;
    }
    else
    {
        if(busPosition == 0) //**  修正了公交车逆时针经过零位置 buPosition变为负数的bug
            busPosition = totalDistance - 1;
        else
            busPosition = (busPosition - 1) % totalDistance;
    }
}

//任务被删除后，更新
void updateOutputList()
{
    for (int i = 1; i <= totalStation; i++)
    {
        isBusQuery[i] = 0;
        isClockwiseQuery[i] = 0;
        isCounterclockwiseQuery[i] = 0;
    }
    for (int i = 1; i <= QueryNumber; i++)
    {
        if (QueryList[i].type == CLOCKWISE)
        {
            isClockwiseQuery[QueryList[i].station] = 1;
        }
        else if (QueryList[i].type == COUNTERCLOCKWISE)
        {
            isCounterclockwiseQuery[QueryList[i].station] = 1;
        }
        else if (QueryList[i].type == TARGET)
        {
            isBusQuery[QueryList[i].station] = 1;
        }
    }
}

//先来先服务，最短时间服务调用此函数以确定方向
void chooseDirection()
{
    int clockwiseDist = getClockwiseDist(getStationPosition(stationGoto));
    int counterclockwiseDist = getCounterclockwiseDist(getStationPosition(stationGoto));
    if (clockwiseDist <= counterclockwiseDist)
    {
        nowDirection = CLOCKWISE;
    }
    else
    {
        nowDirection = COUNTERCLOCKWISE;
    }
}

//对于顺便服务，检查时否需要变向
//这里先假定向下取整
void keepDirectionValid()
{
    if (QueryNumber == 0)
        return;
    int needChangeDirection = 1;
    for (int i = 1; i <= QueryNumber; i++)
    {
        if (nowDirection == CLOCKWISE)
        {
            if (getClockwiseDist((QueryList[i].station - 1) * distance) <= totalDistance / 2)
            {
                needChangeDirection = 0;
            }
        }
        else
        {
            if (getCounterclockwiseDist((QueryList[i].station - 1) * distance) <= totalDistance / 2)
            {
                needChangeDirection = 0;
            }
        }
    }
    if (needChangeDirection)
    {
        if (nowDirection == CLOCKWISE)
        {
            nowDirection = COUNTERCLOCKWISE;
        }
        else
        {
            nowDirection = CLOCKWISE;
        }
    }
}

// SCAN模式第一次找任务
void firstTimeScanGetDir()
{
    int dirTemp = CLOCKWISE, disMin = 10000000;
    for (int i = 1; i <= QueryNumber; i++)
    {                                                                      //遍历所有任务
        int pos = getStationPosition(QueryList[i].station);                //计算位置
        int dirClock = getClockwiseDist(pos);                              //计算顺时针距离
        int dirCounter = getCounterclockwiseDist(pos);                     //计算逆时针距离
        int thisDisMin = dirClock > dirCounter ? dirCounter : dirClock;    //计算最短距离
        int dirMin = dirClock > dirCounter ? COUNTERCLOCKWISE : CLOCKWISE; //计算最短距离对应方向
        if (thisDisMin < disMin)
        { //严格小于，变向
            dirTemp = dirMin;
            disMin = thisDisMin;
        }
        else if (thisDisMin == disMin)
        { // 等于，则顺时针优先
            if (dirMin == CLOCKWISE)
                dirTemp = CLOCKWISE;
        }
    }
    nowDirection = dirTemp; //保存变向结果
    flagFirstTime = 0;
}

void outputResult()
{
    FILE * file=fopen("log.txt","a");
    fprintf(file,"TIME:%d\n", nowClock);
    fprintf(file,"BUS:\n");
    fprintf(file,"position:%d\n", busPosition);
    fprintf(file,"target:");
    for (int i = 1; i <= totalStation; i++)
        fprintf(file,"%d", isBusQuery[i]);
    fprintf(file,"\n");
    fprintf(file,"STATION:\n");
    fprintf(file,"clockwise:");
    for (int i = 1; i <= totalStation; i++)
        fprintf(file,"%d", isClockwiseQuery[i]);
    fprintf(file,"\n");
    fprintf(file,"counterclockwise:");
    for (int i = 1; i <= totalStation; i++)
        fprintf(file,"%d", isCounterclockwiseQuery[i]);
    fprintf(file,"\n");
}






//最重要的逻辑函数
int g_nextClock(){
    //初始化变量
    s_isinfinish = 0;
    s_isNearby = 0;

    if (isInCharge == 0)
                { //目前无任务
                    if (QueryNumber > 0)
                    {                   //有需求
                        isInCharge = 1; //设置为有需求了
                        if (strategy == MODE_SCAN)
                        {
                            if (flagFirstTime)
                            {                          //顺便服务第一次，特判
                                firstTimeScanGetDir(); //第一次，找方向
                            }
                            else
                            {
                                keepDirectionValid(); //保证方向正确
                            }
                        }
                            int destGoto = getNextTask(
                            QueryList,
                            QueryNumber,
                            nowDirection,
                            strategy,
                            totalStation,
                            distance,
                            (busPosition / distance) + 1); //找站点编号

                        stationGoto = QueryList[destGoto].station;
                        s_stationGoto = stationGoto;
                        if (strategy == MODE_FCFS || strategy == MODE_SSTF)
                        {
                            chooseDirection(); //前两种模式，变向
                        }
                        //判断任务是否出现在原地，是则直接停车，否则前进
                        if (busPosition == getStationPosition(stationGoto))
                        {
                            int getFinish = fulfillQuery(
                                QueryList,
                                &QueryNumber,
                                nowDirection,
                                strategy,
                                totalStation,
                                distance,
                                stationGoto);   //删除相关任务
                            s_isinfinish = 1;
                            updateOutputList(); //更新
                            isInCharge = 0;     //任务结束，闲置状态
                        }
                        else
                        {
                            moveForward();
                        }
                    }
                }
                else
                { // 有任务
                    if (
                        ((busPosition % distance) == 0) &&
                        strategy != MODE_FCFS &&
                        busPosition != getStationPosition(stationGoto))
                    { //目前，车在某一个车站并且不是先来先服务并且不是终点站 -- 顺便服务判断
                            int isNearby = hasNearBy(
                            QueryList,
                            &QueryNumber,
                            strategy,
                            totalStation,
                            distance,
                            (busPosition / distance) + 1,
                            nowClock, //注意，此时时间已经+1
                            nowDirection);
                            s_isNearby = isNearby;
                        if (isNearby != 0)
                        { //有任务被删除，更新列表
                            updateOutputList();
                        }
                        else
                        { //没有任务被删除，前进
                            moveForward();
                        }
                    }
                    else if (busPosition == getStationPosition(stationGoto))
                    { //到达世界最高城！理塘！
                        int getFinish = fulfillQuery(
                            QueryList,
                            &QueryNumber,
                            nowDirection,
                            strategy,
                            totalStation,
                            distance,
                            stationGoto);   //删除相关任务
                        s_isinfinish = 1;
                        updateOutputList(); //更新
                        isInCharge = 0;     //任务结束，闲置状态
                    }
                    else
                    { //其余情况，前进
                        moveForward();
                    }
                }
                outputResult();
                return busPosition;
}












int g_getNowState(){
    qDebug()<<s_stationGoto<<"*";
    if(busPosition % distance != 0 ) return STATE_RUNNING;//不在任何一个站点
    else
    {
       if(g_mode == MODE_FCFS && s_stationGoto != (busPosition / distance) + 1) return STATE_RUNNING;//即来即服务，不在终点站上
       if(s_stationGoto != (busPosition / distance) + 1 && s_isNearby != 0) return STATE_SERVING;//在站点上，可以顺便服务
       else if(s_stationGoto != (busPosition / distance) + 1 && s_isNearby == 0) return STATE_RUNNING;//在站点上，不可以顺便服务
       else if(s_stationGoto == (busPosition / distance) + 1)
       {
            if(s_isinfinish == 1) return STATE_SERVING;//刚刚到终点站，在服务
            else return STATE_WAITING;//服务完毕还在终点站上，说明正在等待任务
       }
    }
    return STATE_WAITING;
}




