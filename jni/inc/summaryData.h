#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <math.h>

#include "linked_list.h"
#include "hanback_device.h"

int S_SummaryHanbackSensor(listNode* p, int index);	//data return
int S_SummarySensorStatus(listNode* p, int data);
