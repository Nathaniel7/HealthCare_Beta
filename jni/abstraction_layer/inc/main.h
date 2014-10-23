#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <math.h>
#include <pthread.h>
//#include <ncurses.h>

//#include <omp.h>			// OpenMP
#include "uart_api.h"		// UART Serial Communication API
#include "linked_list.h"
#include "defines.h"
#include "Logcat.h"
#include "com_Nathaniel_healthcare_beta_AbstractionLib.h"

int monitorDev_ok;//thread에서 M_deviceConnect와 F_filterData thread의 동기화를 위한 변수
int newDevice_cnt;//새로운 장치 연결 개수를 잼
int deviceIndex;//현재 연결된 장치 갯수를 말함

int TTD_Number;

void getElapsedTime(struct timeval, struct timeval);
void printCutLine();//--------------------------를 찍음
void printHex(const char *, int);//raw data를 찍는 함수

void device_close(listNode_h *N);
void device_reopen(listNode_h *N);

void* thread_M_connectDevice(void* data);
void* thread_F_readData(void* data);
void* thread_F_makeDevthread(void* data);
void* thread_S_pressData(void* data);

void setDevHead(listNode_h*);
listNode_h* getDevHead();
listNode_h* Node;

/*
WARNING
 에러가 났을때는 이 표시가 되어있는 코드들을 중심적으로 보면서 에러를 점검하도록 함

DCUR
차후 수정및보완해야될 코드들이다.표시가 있다면 수정및보완후 지워야한다.

TEST
에러가 났을때 확인하기 위해 임시적으로 사용하는 코드
*/
