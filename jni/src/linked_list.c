#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

listNode_h *createListNode_H()//contain tail
{
//	printf(">> START Create Node Head\n");
	listNode_h *newNode_h;
	newNode_h = (listNode_h *)malloc(sizeof(listNode_h));
	newNode_h->head = NULL;
	newNode_h->tail = NULL;
	// printf(">> create Head!\n");
	return newNode_h;
}

int isEmptyQueuedata(listNode* N,int data_opt)
{
	if(data_opt == CURRDATA)
	{
		if(N->q_front == N->q_rear)
			return 0;
		else
			return 1;
	}
	else if(data_opt == PREVDATA)
	{
		if(N->q_pfront == N->q_prear)
			return 0;
		else
			return 1;
	}

	return 0;
}

void setQueuedata(listNode* N,int data,int data_opt)
{
	if (data_opt == CURRDATA) {
		N->q_rdata[N->q_front] = data;
		N->q_front++; //printf("######FRONT++!#####\n");//DCUR
		N->q_front %= MAX_QUEUE_SIZE;
		//printf("[CURRENT]setCURRDATA:%d front:%d rear:%d\n",data,N->q_front,N->q_rear);
		//printf("[CURRENT]QUEUEDATA:%d\n",N->q_rdata[N->q_rear]);
		N->getqueue_cnt++;
		N->read_cnt++;
	} else if (data_opt == PREVDATA) {
		N->q_prdata[N->q_pfront] = data;
		N->q_pfront++;
		N->q_pfront %= MAX_QUEUE_SIZE;
		//printf("[PREVIOUS]setPREVDATA:%d front:%d rear:%d\n",data,N->q_pfront,N->q_prear);
	}
}//end void


int getQueuedata(listNode* N,int data_opt)
{
	int ret=0,i=0;

	if(N->getqueue_cnt > 0 && data_opt == CURRDATA)
		N->getqueue_cnt--;
	else if(N->getqueue_cnt == 0 && data_opt != PREVDATA)
		return 0;

	//Exception
	if(data_opt == CURRDATA)
	{
		if (N->q_front == N->q_rear)
			return 0;
	}
	else// PREVDATA
	{
		if (N->q_pfront == N->q_prear)
			return 0;
	}

	if(data_opt == CURRDATA)
	{
		ret = N->q_rdata[N->q_rear];

		setQueuedata(N,ret,PREVDATA);

		//N->q_rdata[N->q_rear] = -9999; //init N->q_rdata  ##WARNING### 나중에 큐 이상의 데이터를 저장 할때 문제가 되는지 테스트해보아야함.
		N->q_rear++; //printf("######REAR++!#####\n");//DCUR
		N->q_rear %= MAX_QUEUE_SIZE;

		//N->fileQdata[N->fileq_front++]=ret;
		N->fileQdata[N->fileq_front++]=ret;//##WARNING### segmantfault의 원인이었음 버퍼크기
	}
	else if(data_opt == PREVDATA)
	{
		ret = N->q_prdata[N->q_prear];
		//printf("ret = :%d rear:%d\n",N->q_prdata[N->q_prear],N->q_prear);
		//N->q_prdata[N->q_prear] = -9999; //##WARNING###
		N->q_prear++;
		N->q_prear %= MAX_QUEUE_SIZE;
	}
	else if(data_opt == PRINTDATA)//delete
	{
		printf("[Previous Analyzed Data]:");
		for(i=N->q_prear;i<N->q_pfront;i++)
			printf("%d-",N->q_prdata[i]);

		printf("\n");

		printf("[Current Analyzed Data]:");
		for(i=N->q_rear;i<N->q_front;i++)
			printf("%d-",N->q_rdata[i]);

	}
	///save previous data
	//setQueue_pdata(N,ret);
	return ret;
}

void initNode(listNode* newNode,char* dev_path)
{
	int i=0;

	memset(newNode, 0, sizeof(listNode));
	strcpy(newNode->dev_path, dev_path);
	newNode->next = NULL;
	newNode->q_front = 0;
	newNode->q_rear  = 0;

	newNode->q_pfront= 0;
	newNode->q_prear = 0;

	newNode->dev_filter_opt=-1;

	newNode->dev_monitor_status = 0;

	///DCURFILE///
	for(i=0;i<MAX_BUFF_SIZE;i++)
	{
		newNode->avg_analyzeData[i] = 0;
		newNode->total_analyzeData[i] = 0;
	}

	newNode->fileq_front=0;
	newNode->read_cnt=0;
}

void insertLastNode(listNode_h *N, char *dev_path)//node add Head and Tail
{
	listNode *newNode = (listNode *)malloc(sizeof(listNode));
	initNode(newNode,dev_path);

	if(N->head == NULL) {	//node not exist
		//set head
		N->head = newNode;
		N->tail = newNode;
		printf(">> Insert Node: %s\n", N->head->dev_path);
		return;
	}
	else {					//node exist
		N->tail->next = newNode;
		N->tail = newNode;
		printf(">> Insert Node: %s\n", N->tail->dev_path);
		return;
	}

	printf("---Insert Node Fail!!!!!");
}

void deleteNode(listNode_h *N, char *dev_path)//dev not exist
{
	listNode* p = N->head;
	listNode* prev = p;

	if(N->head == NULL)
		return;

	printf("\n>> Delete Node\n");

	//isn't lastNode
	while(p != NULL) {
		//dev_path exist
//		printf(" ## debug1\n");
		if(strcmp(p->dev_path, dev_path) == 0) {
			printf("--Delete Device: %s\n\n", p->dev_path);
			break;
		}
		else {
			prev = p;
			p = p->next;
		}
	}//search delete node

//	printf(" ## debug2\n");
	if(N->head == p) {	//first Node
//		printf(" ## first node\n");
		N->head = N->head->next;
		free(p);
	}
	else if(N->tail != p) {	//middle Node if is not Head,Tail
//		printf(" ## middle note\n");
		prev->next = p->next;
		free(p);
	}
	else if(N->tail == p) {	//Last Node
//		printf(" ## last node\n");
		N->tail = prev;
		prev->next = NULL;
		free(p);
	}
}

void displayNode(listNode_h *N)
{
	listNode *p = N->head;

	if(N->head == NULL)
		return;

	while(p != NULL) {
		printf("%s-", p->dev_path);
		p = p->next;
	}
	printf("\n");
}

void freeAllNode(listNode_h* N)
{
	listNode* p = N->head;
	listNode* next=p;

	if(N->head == NULL)
		return;

	while(p != NULL) {
		next = p->next;
		free(p);
		p = next;
	}
	free(N);
}

void strncat_s(unsigned char *target, unsigned char *buff, int target_size, int buff_size)
{
	int i, j;

	for(i = 0, j = target_size; j < MAX_BUFF_SIZE && i < buff_size; i++, j++) {
		target[j] = buff[i];
//		printf("%02X ", buff[i]);
	}
}
