#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "hanback_device.h"

#define ERRORDATA 	-1

#define FILTER_OPT_BACKDATA     3
#define FILTER_OPT_DEPLACEMENT  4
#define FILTER_OPT_AVERAGE      5
#define FILTER_OPT_NONFILTERD	6
#define FILTER_OPT_FILTER		7

void D_HBACK_Thermometer(p);
void D_HBACK_Dioxide(listNode* p);
void D_HBACK_Weather(listNode* p);
void D_HBACK_Dust(listNode* p);

void D_printRawData(listNode* N,int data_opt);
void D_printQueueData(listNode* N,int data_opt);
void D_printUserData(listNode* N,int data_opt);


/////////////File에 읽고 쓸 것들 ////////////////////
/////##WARNING## 센서가 1개씩 연결 되었을때 작동 하도록 만듦
void D_writeDataInfile(listNode* p);//##WARNING####1개의 센서만 돌아갈때 메인에서 사용 할 수 있다.
void D_before_writeDataInfile();

int writeData_Sleeptime;//측정할시간
char writeData_Filename[20];
