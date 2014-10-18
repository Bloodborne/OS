#include<algorithm>
#include<iostream>
#include<string>
#include"header.h"

using namespace std;

bool createProcess()
{
	pcb p;
	int num = 1 + e() % MAX;				//随机生成进程的个数
	//int num = 2;
	for (int i = 0; i < num; i++)
	{
		p.pid = 1 + e() % IDRANGE;		//pid取值为1~100
		while (flag[p.pid])						//若pid重复则重新取值
			p.pid = 1 + e() % IDRANGE;
		flag[p.pid] = 1;
		//p.arrivalTime = e() % 20;
		p.pri = 1 + e() % PRI;
		p.time = 1 + e() % LEFTTIME;
		p.status = READY;

		pcbarea.push_back(p);

		//cout << pcbarea[i].pid << endl;
	}
	return true;
}

void dispatch()
{
	int select;
	
	DisplaySelection();

	createProcess();

	while (cin >> select)
	{
		switch (select)
		{
		case 1:
			RR();
			break;
		case 2:
			Priority();
			break;
		case 3:
			SPN();
			break;
		case 4:
			SRT();
			break;
		case 9:
			pcbarea.clear();
			flag.clear();
			flag.assign(IDRANGE + 1, 0);
			cout << "Command Completed\n\n";
			createProcess();
			break;
		case 0:
			return;
		default:
			cout << "Error Input\n\n";
		}
		DisplaySelection();				//显示输入提示
	}
}

void RR()
{
	//将vector中的进程按顺序添加至就绪队列中
	PushInQueue(pcbarea);

	for (vector<pcb>::iterator it = pcbarea.begin(); it != pcbarea.end(); ++it)		
		cout << it->pid << " ";
	cout << endl;

	//只要就绪队列非空，就一直调度
	while (!readyQueue.empty())		
	{
		p_running = &readyQueue.front();

		p_running->time -= TIMESLICE;
		p_running->status = RUNNING;

		//进程执行结束，修改信息
		if (p_running->time < 0)
		{
			p_running->time = 0;
			p_running->status = TERMINATED;
		}

		Traverse( );

		readyQueue.pop_front();				
		if (p_running->time != 0)		//若剩余时间不为0则添加至队尾
		{
			p_running->status = READY;
			readyQueue.push_back(*p_running);
		}
	}
}

void Priority()
{
	vector<pcb> pcbareaTemp = pcbarea;									//创建pcbarea的副本
	sort(pcbareaTemp.begin(), pcbareaTemp.end(), CmpPri);
	PushInQueue(pcbareaTemp);

	for (vector<pcb>::iterator it = pcbareaTemp.begin(); it != pcbareaTemp.end(); ++it)
		cout << it->pri << " ";
	cout << endl;

	while (!readyQueue.empty())
	{
		p_running = &readyQueue.front();

		--p_running->time ;
		--p_running->pri;
		p_running->status = RUNNING;

		//进程执行结束，修改信息
		if (p_running->time < 0)
		{
			p_running->time = 0;
			p_running->status = TERMINATED;
		}

		Traverse();

		readyQueue.pop_front();
		if (p_running->time != 0)		//若剩余时间不为0则添加至队尾
		{
			p_running->status = READY;
			readyQueue.push_back(*p_running);
		}
	}

}

void SPN()
{
	vector<pcb> pcbareaTemp = pcbarea;			//创建pcbarea的副本
	sort(pcbareaTemp.begin(), pcbareaTemp.end(), CmpTime);
	PushInQueue(pcbareaTemp);

	for (vector<pcb>::iterator it = pcbareaTemp.begin(); it != pcbareaTemp.end(); ++it)
		cout << it->time << " ";
	cout << endl;

	while (!readyQueue.empty())
	{
		p_running = &readyQueue.front();

		p_running->time=0;
		p_running->status = TERMINATED;

		Traverse();

		readyQueue.pop_front();
	}
}
void SRT()
{
	int n = 0;		//有新进程出现次数;
	vector<pcb> pcbareaTemp = pcbarea;			//创建pcbarea的副本
	sort(pcbareaTemp.begin(), pcbareaTemp.end(), CmpTime);
	PushInQueue(pcbareaTemp);

	for (vector<pcb>::iterator it = pcbareaTemp.begin(); it != pcbareaTemp.end(); ++it)
		cout << it->time << " ";
	cout << endl;

	while (!readyQueue.empty())
	{
		p_running = &readyQueue.front();

		p_running->time-=TIMESLICE;
		p_running->status = RUNNING;

		//进程执行结束，修改信息
		if (p_running->time < 0)
		{
			p_running->time = 0;
			p_running->status = TERMINATED;
		}

		Traverse();

		readyQueue.pop_front();
		if (p_running->time != 0)						//若剩余时间不为0则添加至队尾
		{
			p_running->status = READY;
			readyQueue.push_back(*p_running);
		}

		InsertNewProcess(pcbareaTemp,n);		//插入新进程
		
	}
}

void InsertNewProcess(vector<pcb> pcbareaTemp,int &n)
{

	//30%几率有新进程进入，最多出现5次
	if (1 + (e() % 10) < 4 && n < 5)
	{
		pcb p;
		for (int i = 0; i < e() % 5; i++)
		{
			p.pid = 1 + e() % IDRANGE;		//pid取值为1~100
			while (flag[p.pid])						//若pid重复则重新取值
				p.pid = 1 + e() % IDRANGE;
			flag[p.pid] = 1;
			//p.arrivalTime = e() % 20;
			p.pri = 1 + e() % PRI;
			p.time = 1 + e() % LEFTTIME;
			p.status = READY;

			readyQueue.push_back(p);
		}
		pcbareaTemp.clear();																											//清空vector里的元素
		for (deque<pcb>::iterator it = readyQueue.begin(); it != readyQueue.end(); ++it)				//将修改过的进程队列添加进vector
			pcbareaTemp.push_back(*it);
		sort(pcbareaTemp.begin(), pcbareaTemp.end(), CmpTime);												//按剩余时间对进程重新排序
		readyQueue.clear();																											//清空就绪队列
		PushInQueue(pcbareaTemp);																							//将vector里重排过的进程添加至就绪队列
		cout << "有新进程进入!!" << endl;
		system("pause");

		++n;																																	
	}
}

void PushInQueue(vector<pcb> pcbBlock)
{
	for (vector<pcb>::iterator it = pcbBlock.begin(); it != pcbBlock.end(); ++it)
		readyQueue.push_back(*it);
}

void Traverse( )
{
	cout << "*******************************本次调度后的结果*******************************\n";

	//遍历进程队列
	for (deque<pcb>::iterator it = readyQueue.begin(); it != readyQueue.end();++it)
	{
		PrintOut(*it);
	}
	cout << endl;

}

void DisplaySelection()
{
	cout << "1:时间片轮转调度" << endl << "2:优先数调度" << endl;
	cout << "3:最短进程优先" << endl << "4:最短剩余时间优先"<<endl;
	cout << "9:重新创建进程"<<endl<<"0:退出"<<endl;
	cout << "请选择调度算法:";
}

void PrintOut( pcb process)
{
	cout << "pid:" << process.pid << "\t\t";
	cout << "优先数:" << process.pri << "\t\t";
	cout << "剩余时间:" << process.time << "\t\t";
	cout << "状态:" << process.status << endl;
}

int main()
{
	dispatch();
}
