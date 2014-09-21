#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#include "linked_list.h"
#include "hanback_device.h"

typedef enum {false, true} bool;

enum COMPANY_NUM {
	HANBACK
};

int M_connectDevice(listNode_h *);
void M_makeLinkedList(listNode_h *, char *);
void M_openDevice(listNode_h *);
int  M_checkDeviceFingerprint(listNode *);
void M_strncat_s(char *, char *, int, int );

int M_checkDeviceCompany(listNode *);
void M_checkSensor(listNode *);
void M_printDeviceInfo(listNode);
