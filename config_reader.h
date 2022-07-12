#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#define MODE_FCFS 1
#define MODE_SSTF 2
#define MODE_SCAN 3

void initConfigReader();
int getTotalStation();
int getDistance();
int getStrategy();

#endif