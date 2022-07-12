#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// 定义指令
#define CLOCK 1
#define NEW_QUERY 2
#define END 3

#define MODE_FCFS 1
#define MODE_SSTF 2
#define MODE_SCAN 3

#define CLOCKWISE 1
#define COUNTERCLOCKWISE 2
#define TARGET 3

typedef struct Query
{
    int type;
    int station;
    int genTime;
} Query;

int a_TotalStation = 5;
int a_Strategy = MODE_FCFS;
int a_Distance = 2;

void initConfigReader()
{
    // int flag_station = 1;
    //方案二的一部分

    FILE *file = fopen("dict.dic", "r");

    //一个个读取字符
    char ch;
    ch = fgetc(file);

    //有限状态自动机信号
    int flag = 0;   //总体信号
    int flag_s = 0; //策略信号
    int flag_t = 0;

    //循环到文件尾
    while (!feof(file))
    {
        //待读取行头状态
        if (flag == 0)
        {
            if (ch == '#')
            {
                flag = 1;
            }
            else if (ch == 'T')
            {
                flag = 2;
                a_TotalStation = 0;
                //方案一的改动
            }
            else if (ch == 'S')
            {
                flag = 3;
            }
            else if (ch == 'D')
            {
                flag = 4;
            }
        }
        else if (flag == 1)
        { //读取注释状态
            if (ch == '\n')
                flag = 0;
        }
        else if (flag == 2)
        { //读取站台数状态
            // if (ch > '1' && ch <= '9')
            //     a_TotalStation =  ch - '0';
            // else if (ch == '1')
            //     a_TotalStation = 10;
            if (ch >= '0' && ch <= '9') //** 站台数上限改为20  对应修改代码
                // if(flag_station==1){
                //     a_TotalStation = 0;
                //     flag_station = 0;
                // }
                //方案二的改动，打算等问清楚之后再决定
                a_TotalStation = a_TotalStation * 10 + ch - '0';
            if (ch == '\n')
                flag = 0;
        }
        else if (flag == 3)
        { //读取策略状态
            if (ch == '=')
                flag_s = 1;
            if (flag_s == 1)
            {
                if (ch == 'F')
                {
                    a_Strategy = MODE_FCFS;
                    flag_s = 0;
                }
                else if (ch == 'S')
                    flag_s = 2;
            }
            else if (flag_s == 2)
            {
                if (ch == 'S')
                {
                    a_Strategy = MODE_SSTF;
                    flag_s = 0;
                }
                else if (ch == 'C')
                {
                    a_Strategy = MODE_SCAN;
                    flag_s = 0;
                }
            }
            if (ch == '\n')
                flag = 0;
        }
        else if (flag == 4)
        { //读取车站距离
            if (ch > '0' && ch < '6')
                a_Distance = ch - '0';
            else if (ch == '\n')
                flag = 0;
        }

        ch = fgetc(file);
    }

    fclose(file);
}

int getTotalStation()
{
    return a_TotalStation; //返回车站数量
}
int getDistance()
{
    return a_Distance; //返回车站间距离
}
int getStrategy()
{
    return a_Strategy; //返回策略
}

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
            int d1;                                 // d1为nowstation到queryList[i].station的顺时针距离
            if (queryList[i].station >= nowStation) //**加上等于号  正确判断nowStation到queryList[i].station的顺逆时针距离
                d1 = queryList[i].station - nowStation;
            else
                d1 = (totalStation - nowStation) + queryList[i].station;

            int d2;                                 // d2为nowstation到queryList[i].station的逆时针距离
            if (queryList[i].station <= nowStation) //**
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
            int d1;                                 // d1为nowstation到queryList[i].station的顺时针距离
            if (queryList[i].station >= nowStation) //**
                d1 = queryList[i].station - nowStation;
            else
                d1 = (totalStation - nowStation) + queryList[i].station;

            int d2;                                 // d2为nowstation到queryList[i].station的逆时针距离
            if (queryList[i].station <= nowStation) //**
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
        //** SSTF顺便服务判断更新 + 修复了少考虑target请求的bug
        if(strategyType == MODE_SSTF && queryList[i].station == nowStation && (queryList[i].type == TARGET || queryList[i].type == direction) && queryList[i].genTime <= nowTime - 2)
            continue;

        //** SCAN顺便服务判断更新 + 修复了少考虑target请求的bug
        //if (strategyType == MODE_SCAN && queryList[i].genTime <= nowTime - 2 && queryList[i].station == nowStation)
        if(strategyType == MODE_SCAN && queryList[i].station == nowStation && queryList[i].genTime <= nowTime - 2)
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
    int nowStation
    )
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

int getPositionStation(int positionToCalc)
{
    return (positionToCalc / distance) + 1;
}

void moveForward()
{ //前进函数
    if (nowDirection == CLOCKWISE)
    {
        busPosition = (busPosition + 1) % totalDistance;
    }
    else
    {
        if (busPosition == 0) //**  修正了公交车逆时针经过零位置 buPosition变为负数的bug
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
        if (nowCommand == END)
        {
            printf("end\n"); //**输出末尾补上end+换行
            break;
        }
        if (nowCommand == CLOCK)
        {
            if (isInCharge == 0)
            { //目前无任务
                if (queryNumber > 0)
                {                   //有需求
                    int getFinish = fulfillQuery(//**车处于停止状态开始一次新调度时（空闲状态或者完成上一次服务后），如果本站有请求且根据规则可以为该请求服务，则该请求立即完成，不再停1秒钟
                    queryList,
                    &queryNumber,
                    nowDirection,
                    strategy,
                    totalStation,
                    distance,
                    getPositionStation(busPosition)
                    );
                    updateOutputList();//更新输出列表
                    if(!queryNumber)//如果没有任务了
                    {
                        outputResult();
                        continue;
                    }

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
                            stationGoto
                            );   //删除相关任务
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

    // puts("END SUCCESSFULLY");
    return 0;
}
