#pragma once
#include <vector>
#include<deque>
#include<random>
#include<ctime>

#define MAX 10		//假设系统允许进程个数为MAX
#define PRI 10			//进程优先度范围
#define LEFTTIME	20		//剩余运行时间范围	
#define IDRANGE	100	//pid的范围

#define RUNNING "RUNNING"	//运行态
#define READY "READY"		//就绪态
#define TERMINATED "TERMINATED"	//终止态

#define TIMESLICE 2 //时间片为2

//int curTime = 0;	//表示当前系统时间

struct pcb
{
	int pid;		//进程标识符
	std::string status;		//进程状态
	int pri;			//进程优先数
	int time;		//剩余运行时间
	//int arrivalTime;	//到达时间
};

std::deque<pcb> readyQueue;
std::deque<pcb> freeQueue;

pcb *p_running;		//定义指向正在运行进程的进程控制块的指针

std::vector<int> flag(IDRANGE+1, 0);		//用于确保pid没有重复

std::vector<pcb> pcbarea;

bool createProcess();		//创建进程
void dispatch();				//调度进程

void RR();		//时间片轮转
void Priority();	//优先级调度
void SPN();		//最短进程优先
void SRT();		//最短剩余时间优先
	
void InsertNewProcess(std::vector<pcb>pcbareaTemp,int &n);		//有新进程生成
void PushInQueue(std::vector<pcb> pcbBlock);		//将vector中的pcb进队列
void Traverse( );				//遍历队列中的pcb
void DisplaySelection();		//提示用户选择何种调度算法
void PrintOut( pcb process);		//输出pcb中的内容

std::default_random_engine e(time(NULL));		//e为随机数引擎

bool CmpPri(pcb t1, pcb t2)			//比较进程优先度
{
	return t1.pri > t2.pri;
}

bool CmpTime(pcb t1, pcb t2)		//比较进程剩余时间
{
	return t1.time < t2.time;
}

