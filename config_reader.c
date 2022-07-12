#include <stdio.h>
#include "config_reader.h"

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
            if(ch >= '0' && ch <= '9')//** 站台数上限改为20  对应修改代码
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
