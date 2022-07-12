#include <stdio.h>
#include <stdlib.h>
#include "get_next_task.h"

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
            if (queryList[i].station > nowStation)
                d1 = queryList[i].station - nowStation;
            else
                d1 = (totalStation - nowStation) + queryList[i].station;

            int d2; // d2为nowstation到queryList[i].station的逆时针距离
            if (queryList[i].station < nowStation)
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
            if (queryList[i].station > nowStation)
                d1 = queryList[i].station - nowStation;
            else
                d1 = (totalStation - nowStation) + queryList[i].station;

            int d2; // d2为nowstation到queryList[i].station的逆时针距离
            if (queryList[i].station < nowStation)
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
        if(strategyType == MODE_SSTF && queryList[i].station == nowStation && (queryList[i].type == TARGET || queryList[i].type == direction) )
            continue;

        //** SCAN顺便服务判断更新 + 修复了少考虑target请求的bug
        //if (strategyType == MODE_SCAN && queryList[i].genTime <= nowTime - 2 && queryList[i].station == nowStation)
        if(strategyType == MODE_SCAN && queryList[i].station == nowStation)
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
        copy[++cnt] = queryList[i];
    }

    for (int i = 1; i <= cnt; ++i)
        queryList[i] = copy[i];
    *queryListNumber = cnt;

    return n - cnt;
} /*
 这个函数里，你要处理汽车到达目的地后的请求列表。汽车一次停靠可能会完成多个任务，请将完成的任务删掉并重新排列，返回完成的任务数。
 */