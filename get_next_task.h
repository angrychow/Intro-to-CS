#ifndef GET_NEXT_TASK_H
#define GET_NEXT_TASK_H

#define MODE_FCFS 1
#define MODE_SSTF 2
#define MODE_SCAN 3

#define CLOCKWISE 1
#define COUNTERCLOCKWISE 2
#define TARGET 3

typedef struct Query {
    int type;
    int station;
    int genTime;
} Query ;


int getNextTask (
    Query queryList[],
    int queryListNumber,
    int direction,
    int strategyType,
    int totalStation,
    int distance,
    int nowStation
);


int hasNearBy (
    Query queryList[],
    int *queryListNumber,
    int strategyType,
    int totalStation,
    int distance,
    int nowStation,
    int nowTime,
    int direction
);


int fulfillQuery (
    Query queryList[],
    int *queryListNumber,
    int direction,
    int strategyType,
    int totalStation,
    int distance,
    int nowStation
);

#endif