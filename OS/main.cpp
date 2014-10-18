#include<algorithm>
#include<iostream>
#include<string>
#include"header.h"

using namespace std;

bool createProcess()
{
	pcb p;
	int num = 1 + e() % MAX;				//������ɽ��̵ĸ���
	//int num = 2;
	for (int i = 0; i < num; i++)
	{
		p.pid = 1 + e() % IDRANGE;		//pidȡֵΪ1~100
		while (flag[p.pid])						//��pid�ظ�������ȡֵ
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
		DisplaySelection();				//��ʾ������ʾ
	}
}

void RR()
{
	//��vector�еĽ��̰�˳�����������������
	PushInQueue(pcbarea);

	for (vector<pcb>::iterator it = pcbarea.begin(); it != pcbarea.end(); ++it)		
		cout << it->pid << " ";
	cout << endl;

	//ֻҪ�������зǿգ���һֱ����
	while (!readyQueue.empty())		
	{
		p_running = &readyQueue.front();

		p_running->time -= TIMESLICE;
		p_running->status = RUNNING;

		//����ִ�н������޸���Ϣ
		if (p_running->time < 0)
		{
			p_running->time = 0;
			p_running->status = TERMINATED;
		}

		Traverse( );

		readyQueue.pop_front();				
		if (p_running->time != 0)		//��ʣ��ʱ�䲻Ϊ0���������β
		{
			p_running->status = READY;
			readyQueue.push_back(*p_running);
		}
	}
}

void Priority()
{
	vector<pcb> pcbareaTemp = pcbarea;									//����pcbarea�ĸ���
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

		//����ִ�н������޸���Ϣ
		if (p_running->time < 0)
		{
			p_running->time = 0;
			p_running->status = TERMINATED;
		}

		Traverse();

		readyQueue.pop_front();
		if (p_running->time != 0)		//��ʣ��ʱ�䲻Ϊ0���������β
		{
			p_running->status = READY;
			readyQueue.push_back(*p_running);
		}
	}

}

void SPN()
{
	vector<pcb> pcbareaTemp = pcbarea;			//����pcbarea�ĸ���
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
	int n = 0;		//���½��̳��ִ���;
	vector<pcb> pcbareaTemp = pcbarea;			//����pcbarea�ĸ���
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

		//����ִ�н������޸���Ϣ
		if (p_running->time < 0)
		{
			p_running->time = 0;
			p_running->status = TERMINATED;
		}

		Traverse();

		readyQueue.pop_front();
		if (p_running->time != 0)						//��ʣ��ʱ�䲻Ϊ0���������β
		{
			p_running->status = READY;
			readyQueue.push_back(*p_running);
		}

		InsertNewProcess(pcbareaTemp,n);		//�����½���
		
	}
}

void InsertNewProcess(vector<pcb> pcbareaTemp,int &n)
{

	//30%�������½��̽��룬������5��
	if (1 + (e() % 10) < 4 && n < 5)
	{
		pcb p;
		for (int i = 0; i < e() % 5; i++)
		{
			p.pid = 1 + e() % IDRANGE;		//pidȡֵΪ1~100
			while (flag[p.pid])						//��pid�ظ�������ȡֵ
				p.pid = 1 + e() % IDRANGE;
			flag[p.pid] = 1;
			//p.arrivalTime = e() % 20;
			p.pri = 1 + e() % PRI;
			p.time = 1 + e() % LEFTTIME;
			p.status = READY;

			readyQueue.push_back(p);
		}
		pcbareaTemp.clear();																											//���vector���Ԫ��
		for (deque<pcb>::iterator it = readyQueue.begin(); it != readyQueue.end(); ++it)				//���޸Ĺ��Ľ��̶�����ӽ�vector
			pcbareaTemp.push_back(*it);
		sort(pcbareaTemp.begin(), pcbareaTemp.end(), CmpTime);												//��ʣ��ʱ��Խ�����������
		readyQueue.clear();																											//��վ�������
		PushInQueue(pcbareaTemp);																							//��vector�����Ź��Ľ����������������
		cout << "���½��̽���!!" << endl;
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
	cout << "*******************************���ε��Ⱥ�Ľ��*******************************\n";

	//�������̶���
	for (deque<pcb>::iterator it = readyQueue.begin(); it != readyQueue.end();++it)
	{
		PrintOut(*it);
	}
	cout << endl;

}

void DisplaySelection()
{
	cout << "1:ʱ��Ƭ��ת����" << endl << "2:����������" << endl;
	cout << "3:��̽�������" << endl << "4:���ʣ��ʱ������"<<endl;
	cout << "9:���´�������"<<endl<<"0:�˳�"<<endl;
	cout << "��ѡ������㷨:";
}

void PrintOut( pcb process)
{
	cout << "pid:" << process.pid << "\t\t";
	cout << "������:" << process.pri << "\t\t";
	cout << "ʣ��ʱ��:" << process.time << "\t\t";
	cout << "״̬:" << process.status << endl;
}

int main()
{
	dispatch();
}
