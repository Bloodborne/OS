#pragma once
#include <vector>
#include<deque>
#include<random>
#include<ctime>

#define MAX 10		//����ϵͳ������̸���ΪMAX
#define PRI 10			//�������ȶȷ�Χ
#define LEFTTIME	20		//ʣ������ʱ�䷶Χ	
#define IDRANGE	100	//pid�ķ�Χ

#define RUNNING "RUNNING"	//����̬
#define READY "READY"		//����̬
#define TERMINATED "TERMINATED"	//��ֹ̬

#define TIMESLICE 2 //ʱ��ƬΪ2

//int curTime = 0;	//��ʾ��ǰϵͳʱ��

struct pcb
{
	int pid;		//���̱�ʶ��
	std::string status;		//����״̬
	int pri;			//����������
	int time;		//ʣ������ʱ��
	//int arrivalTime;	//����ʱ��
};

std::deque<pcb> readyQueue;
std::deque<pcb> freeQueue;

pcb *p_running;		//����ָ���������н��̵Ľ��̿��ƿ��ָ��

std::vector<int> flag(IDRANGE+1, 0);		//����ȷ��pidû���ظ�

std::vector<pcb> pcbarea;

bool createProcess();		//��������
void dispatch();				//���Ƚ���

void RR();		//ʱ��Ƭ��ת
void Priority();	//���ȼ�����
void SPN();		//��̽�������
void SRT();		//���ʣ��ʱ������
	
void InsertNewProcess(std::vector<pcb>pcbareaTemp,int &n);		//���½�������
void PushInQueue(std::vector<pcb> pcbBlock);		//��vector�е�pcb������
void Traverse( );				//���������е�pcb
void DisplaySelection();		//��ʾ�û�ѡ����ֵ����㷨
void PrintOut( pcb process);		//���pcb�е�����

std::default_random_engine e(time(NULL));		//eΪ���������

bool CmpPri(pcb t1, pcb t2)			//�ȽϽ������ȶ�
{
	return t1.pri > t2.pri;
}

bool CmpTime(pcb t1, pcb t2)		//�ȽϽ���ʣ��ʱ��
{
	return t1.time < t2.time;
}

