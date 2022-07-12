#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "config_reader.h"
#include "get_next_task.h"

// 定义指令
#define CLOCK 1
#define NEW_QUERY 2
#define END 3

// 任务列表
Query queryList[10001];
int queryNumber;

//配置文件读取存储变量
int distance, strategy, totalStation, totalDistance;

//巴士状态变量
int busPosition = 0;
int nowDirection = CLOCKWISE;
int nowClock = 0;

//巴士任务执行状态
int isInCharge = 0;
int stationGoto = 0; // station, not position

//输出列表
int isCounterclockwiseQuery[1001];
int isClockwiseQuery[1001];
int isBusQuery[1001];

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

void moveForward()
{ //前进函数
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

void updateOutputList()
{ //任务被删除后，更新
    for (int i = 1; i <= totalStation; i++)
    {
        isBusQuery[i] = 0;
        isClockwiseQuery[i] = 0;
        isCounterclockwiseQuery[i] = 0;
    }
    for (int i = 1; i <= queryNumber; i++)
    {
        if (queryList[i].type == CLOCKWISE)
        {
            isClockwiseQuery[queryList[i].station] = 1;
        }
        else if (queryList[i].type == COUNTERCLOCKWISE)
        {
            isCounterclockwiseQuery[queryList[i].station] = 1;
        }
        else if (queryList[i].type == TARGET)
        {
            isBusQuery[queryList[i].station] = 1;
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
    if (queryNumber == 0)
        return;
    int needChangeDirection = 1;
    for (int i = 1; i <= queryNumber; i++)
    {
        if (nowDirection == CLOCKWISE)
        {
            if (getClockwiseDist((queryList[i].station - 1) * distance) <= totalDistance / 2)
            {
                needChangeDirection = 0;
            }
        }
        else
        {
            if (getCounterclockwiseDist((queryList[i].station - 1) * distance) <= totalDistance / 2)
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

int getCommand()
{
    char ch[100];
    scanf("%s", ch);
    if (strcmp(ch, "end") == 0)
    {
        return END;
    }
    else if (strcmp(ch, "clock") == 0)
    {
        nowClock++;
        return CLOCK;
    }
    else
    {
        int getQueryStation;
        scanf("%d", &getQueryStation);
        if (
            strcmp(ch, "counterclockwise") == 0 &&
            isCounterclockwiseQuery[getQueryStation] == 0 //没有相同的任务
        )
        {
            isCounterclockwiseQuery[getQueryStation] = 1;
            queryList[++queryNumber].genTime = nowClock;
            queryList[queryNumber].type = COUNTERCLOCKWISE;
            queryList[queryNumber].station = getQueryStation;
        }
        else if (
            strcmp(ch, "clockwise") == 0 &&
            isClockwiseQuery[getQueryStation] == 0)
        {
            isClockwiseQuery[getQueryStation] = 1;
            queryList[++queryNumber].genTime = nowClock;
            queryList[queryNumber].type = CLOCKWISE;
            queryList[queryNumber].station = getQueryStation;
        }
        else if (
            strcmp(ch, "target") == 0 &&
            isBusQuery[getQueryStation] == 0)
        {
            isBusQuery[getQueryStation] = 1;
            queryList[++queryNumber].genTime = nowClock;
            queryList[queryNumber].type = TARGET;
            queryList[queryNumber].station = getQueryStation;
        }
        return NEW_QUERY;
    }
}

void firstTimeScanGetDir()
{ // SCAN模式第一次找任务
    int dirTemp = CLOCKWISE, disMin = 10000000;
    for (int i = 1; i <= queryNumber; i++)
    {                                                                      //遍历所有任务
        int pos = getStationPosition(queryList[i].station);                //计算位置
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
    printf("TIME:%d\n", nowClock);
    printf("BUS:\n");
    printf("position:%d\n", busPosition);
    printf("target:");
    for (int i = 1; i <= totalStation; i++)
        printf("%d", isBusQuery[i]);
    printf("\n");
    printf("STATION:\n");
    printf("clockwise:");
    for (int i = 1; i <= totalStation; i++)
        printf("%d", isClockwiseQuery[i]);
    printf("\n");
    printf("counterclockwise:");
    for (int i = 1; i <= totalStation; i++)
        printf("%d", isCounterclockwiseQuery[i]);
    printf("\n");
}

int main()
{
    // freopen("test.in", "r", stdin);
    // freopen("test.out", "w", stdout);

    initConfigReader();
    distance = getDistance();
    strategy = getStrategy();
    totalStation = getTotalStation();
    totalDistance = distance * totalStation;
    int nowCommand;

    outputResult();
    while (true)
    {
        nowCommand = getCommand();
        if (nowCommand == END) {
            printf("end\n");//**输出末尾补上end+换行
            break;
        }
        if (nowCommand == CLOCK)
        {
            if (isInCharge == 0)
            { //目前无任务
                if (queryNumber > 0)
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
                        queryList,
                        queryNumber,
                        nowDirection,
                        strategy,
                        totalStation,
                        distance,
                        (busPosition / distance) + 1); //找站点编号
                    stationGoto = queryList[destGoto].station;
                    if (strategy == MODE_FCFS || strategy == MODE_SSTF)
                    {
                        chooseDirection(); //前两种模式，变向
                    }
                    //判断任务是否出现在原地，是则直接停车，否则前进
                    if (busPosition == getStationPosition(stationGoto))
                    {
                        int getFinish = fulfillQuery(
                            queryList,
                            &queryNumber,
                            nowDirection,
                            strategy,
                            totalStation,
                            distance,
                            stationGoto);   //删除相关任务
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
                        queryList,
                        &queryNumber,
                        strategy,
                        totalStation,
                        distance,
                        (busPosition / distance) + 1,
                        nowClock, //注意，此时时间已经+1
                        nowDirection);
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
                        queryList,
                        &queryNumber,
                        nowDirection,
                        strategy,
                        totalStation,
                        distance,
                        stationGoto);   //删除相关任务
                    updateOutputList(); //更新
                    isInCharge = 0;     //任务结束，闲置状态
                }
                else
                { //其余情况，前进
                    moveForward();
                }
            }
            outputResult();
        }
    }

    //puts("END SUCCESSFULLY");
    return 0;
}