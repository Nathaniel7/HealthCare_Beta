#include "monitorDevice.h"
#include "Logcat.h"
#include <string.h>

void printLogcat2(const char *buff, int count,char* dev_name)
{
    int i;
    char temp[1024];
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "  ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼");
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "  ▼ <%s> %d", dev_name, count);

    for(i = 0; i < count * 3; i+=3) {
        sprintf(&temp[i], "%02X", buff[i/3]);
        sprintf(&temp[i+2], " ");
    }

    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "  ▲ %s", temp);
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "  ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲");
}

// Device Monitoring Function
int M_connectDevice(listNode_h *N)
{
	int i = 0;
	char USB_serial[MAX_DEV_NUM][MAX_DEV_PATH_SIZE] = {
//			"/dev/ttyUSB0", "/dev/ttyUSB1",	"/dev/ttyUSB2", "/dev/ttyUSB3", "/dev/ttyUSB4"};
			"/dev/ttyUSB0", "/dev/ttyUSB2", "/dev/ttyUSB3", "/dev/ttyUSB4",
			"/dev/ttyUSB5", "/dev/ttyUSB6", "/dev/ttyUSB7", "/dev/ttyUSB8", "/dev/ttyUSB9" };

	printf("\n>> Connect Device\n");

    // LOGI("  3. Monitor Devices");
	for(i = 0; i < MAX_DEV_NUM; i++){	// Device Search and connect
		int read_size;

        // LOGI("   4. Monitor Devices");
		//*Check Device exist
		if(access(USB_serial[i], R_OK & W_OK) == 0) {
		    //__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "  ▲!!!!! access available / %s", USB_serial[i]);
            // LOGI("    5. Monitor Devices");
		    // make Linked List
		    M_makeLinkedList(N, USB_serial[i]);
		    // check not yet open Device
		    if(N->tail->fd == 0) {
                //LOGI("        9. Monitor open device");
		        M_openDevice(N);
		    }
		    if(N->tail->fd != -1 && !strcmp(N->tail->dev_name, "")) {
		        // LOGI("         10. Monitor fd, name check");
                // analysing sensor packet data
		        if(M_checkDeviceFingerprint(N->tail)){

		            // just display device node info
		            M_printDeviceInfo(*N->tail);
		            user_uart_close(N->tail->fd);

		            N->tail->dev_monitor_status = 1;
		            return 1;
		        }
		        else
		        {
		            N->tail->dev_monitor_status = 0;
			        return 0;
			    }
			}
		}//end *Check Device exist
//		else
//		    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "  ▲!!!!! Sensor access Fail !! %s", USB_serial[i]);
	}
	printCutLine();		// ---------------------------------

	return 0;
}

void M_makeLinkedList(listNode_h *N, const char *dev_path)
{
	//	printf(">> START Make Linked List\n");
	if(N->head == NULL) {

//	    if(strcmp(dev_path, "/dev/ttyUSB0") == 0)
//	        LOGI("     6. makeLinkedList head == NULL / dev_path == /dev/ttyUSB0");
//	    else if(strcmp(dev_path, "/dev/ttyUSB2") == 0)
//	        LOGI("     6. makeLinkedList head == NULL / dev_path == /dev/ttyUSB2");
//	    else if(strcmp(dev_path, "/dev/ttyUSB3") == 0)
//	        LOGI("     6. makeLinkedList head == NULL / dev_path == /dev/ttyUSB3");
//	    else if(strcmp(dev_path, "/dev/ttyUSB4") == 0)
//	        LOGI("     6. makeLinkedList head == NULL / dev_path == /dev/ttyUSB4");
//	    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, ">> M_LinkedList Device: %s / head NULL", dev_path);
		insertLastNode(N, dev_path);
	}
	else {
		int i;
		listNode *prev = N->head;

		while(prev != NULL) {
			if(strcmp(prev->dev_path, dev_path) == 0) {
	            break;
			}
			else if(prev == N->tail) {
				insertLastNode(N, dev_path);
//				__android_log_print(ANDROID_LOG_INFO, LOG_TAG, ">> M_LinkedList Device: %s / head NOT NULL", dev_path);

//				if(strcmp(dev_path, "/dev/ttyUSB0") == 0)
//				     LOGI("     6. makeLinkedList dev_path == /dev/ttyUSB0");
//				else if(strcmp(dev_path, "/dev/ttyUSB2") == 0)
//				     LOGI("     6. makeLinkedList dev_path == /dev/ttyUSB2");
//				else if(strcmp(dev_path, "/dev/ttyUSB3") == 0)
//				     LOGI("     6. makeLinkedList dev_path == /dev/ttyUSB3");
//				else if(strcmp(dev_path, "/dev/ttyUSB4") == 0)
//				     LOGI("     6. makeLinkedList dev_path == /dev/ttyUSB4");
			}

			prev = prev->next;
		}
	}
}

void M_openDevice(listNode_h *N)
{
	N->tail->fd = user_uart_open(N->tail->dev_path);
	//    if(strcmp(N->tail->dev_path, "/dev/ttyUSB0") == 0)
//        LOGI("        9-1. makeLinkedList dev_path == /dev/ttyUSB0");
//    else if(strcmp(N->tail->dev_path, "/dev/ttyUSB2") == 0)
//        LOGI("        9-1. makeLinkedList dev_path == /dev/ttyUSB2");
//    else if(strcmp(N->tail->dev_path, "/dev/ttyUSB3") == 0)
//        LOGI("        9-1. makeLinkedList dev_path == /dev/ttyUSB3");
//    else if(strcmp(N->tail->dev_path, "/dev/ttyUSB4") == 0)
//        LOGI("        9-1. makeLinkedList dev_path == /dev/ttyUSB4");

	if(N->tail->fd != -1) {
		user_uart_config(N->tail->fd, 57600, 8, 0, 1);
//		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, ">> Open Device: %s, FD: %d\n", N->tail->dev_path, N->tail->fd);
	}
//	else
//	    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, ">> Fail! Open Device: %s, FD: %d\n", N->tail->dev_path, N->tail->fd);

}

int M_checkDeviceFingerprint(listNode *Node)
{
    int readSize;
	int readTotalSize;
	unsigned char buff[MAX_BUFF_SIZE] = {0};

	int checkCompany = 0;

	printf(" -Analysing Sensor Packet Data...\n");

//	LOGI("         10. checkDevicesFingerprint");

//    if(strcmp(Node->dev_path, "/dev/ttyUSB0") == 0)
//        LOGI("         10-1. /dev/ttyUSB0");
//    else if(strcmp(Node->dev_path, "/dev/ttyUSB2") == 0)
//        LOGI("         10-1. /dev/ttyUSB2");
//    else if(strcmp(Node->dev_path, "/dev/ttyUSB3") == 0)
//        LOGI("         10-1. /dev/ttyUSB3");
//    else if(strcmp(Node->dev_path, "/dev/ttyUSB4") == 0)
//        LOGI("         10-1. /dev/ttyUSB4");

	// Read data until buff[MAX_BUFF_SIZE] full
	for(readTotalSize = 0; readTotalSize < MAX_BUFF_SIZE; readTotalSize += readSize) {
	    //LOGI("         10-2. Monitor read for loop");
		if((readSize = user_uart_read(Node->fd, buff, MAX_BUFF_SIZE)) == -1) {

		    //LOGI("         10-3. Monitor read data");

			readSize = 0;
			continue;
		}
//		printHex(buff, readSize);
		strncat_s(Node->q_data[0], buff, readTotalSize, readSize);
	}
//	printLogcat2(Node->q_data[0], readTotalSize, Node->dev_name);
	//LOGI("         10-4. Monitor read End");
	//	printHex(Node->q_data[0], readTotalSize);

	if(M_checkDeviceCompany(Node)) {
	    M_checkSensor(Node);

//	    LOGI("         10-5. Monitor checkSensor");
	    return true;
	}
	else {
	    memset(Node, NULL, sizeof(Node));
	}


//	LOGI("         10-6. Monitor End");
	return false;
}



int M_checkDeviceCompany(listNode *Node)
{
    // HANBACK COMPANY
    if(Node->q_data[0][0] == HANBACK_START_BIT_FRONT &&
            Node->q_data[0][1] == HANBACK_START_BIT_REAR) {
        int i;
        for(i = 0; i < MAX_BUFF_SIZE; i++) {
            if(Node->q_data[0][i] == HANBACK_END_BIT_FRONT &&
                    Node->q_data[0][i+1] == HANBACK_END_BIT_REAR) {

                ///Add information to node
                strcpy(Node->dev_maker, "HANBACK");
                Node->dev_maker_id[0] = HANBACK_START_BIT_FRONT;
                Node->dev_maker_id[1] = HANBACK_START_BIT_REAR;
                Node->dev_end_bit[0] = HANBACK_END_BIT_FRONT;
                Node->dev_end_bit[1] = HANBACK_END_BIT_REAR;

                break;
                //printHex(Node->q_data[0],MAX_BUFF_SIZE);
            }
        }//end for
        return 1;
    }
    else
    {
        strcpy(Node->dev_maker, "#Unknown");
        return 0;
    }

    return 0;
}

void M_checkSensor(listNode *Node)
{
	if(!strcmp(Node->dev_maker, "HANBACK")) {
		if( Node->q_data[0][2] == HANBACK_BLD_PRSR_FRONT &&
				Node->q_data[0][3] == HANBACK_BLD_PRSR_REAR) {
			strcpy(Node->dev_name, "Blood_Pressure");

			Node->dev_id[0]=HANBACK_BLD_PRSR_FRONT;
			Node->dev_id[1]=HANBACK_BLD_PRSR_REAR;
            Node->dev_pacLen = 15;
            Node->dev_datalen = 4;
		}
		else if(Node->q_data[0][2] == HANBACK_DIOXIDE_FRONT &&
				Node->q_data[0][3] == HANBACK_DIOXIDE_REAR) {
			strcpy(Node->dev_name, "Dioxide");

			Node->dev_id[0]=HANBACK_DIOXIDE_FRONT;
			Node->dev_id[1]=HANBACK_DIOXIDE_REAR;
            Node->dev_pacLen = 9;
            Node->dev_datalen = 2;
		}
		else if(Node->q_data[0][2] == HANBACK_DUST_FRONT &&
				Node->q_data[0][3] == HANBACK_DUST_REAR) {
			strcpy(Node->dev_name, "Dust");

			Node->dev_id[0]=HANBACK_DUST_FRONT;
			Node->dev_id[1]=HANBACK_DUST_REAR;
            Node->dev_pacLen = 9;
            Node->dev_datalen = 2;
		}
		else if(Node->q_data[0][2] == HANBACK_MONOXIDE_FRONT &&
				Node->q_data[0][3] == HANBACK_MONOXIDE_REAR) {
			strcpy(Node->dev_name, "Monoxide");

			Node->dev_id[0]=HANBACK_MONOXIDE_FRONT;
			Node->dev_id[1]=HANBACK_MONOXIDE_REAR;
            Node->dev_pacLen = 9;
            Node->dev_datalen = 2;
		}
		else if(Node->q_data[0][2] == HANBACK_NITROGEN_FRONT &&
				Node->q_data[0][3] == HANBACK_NITROGEN_REAR) {
			strcpy(Node->dev_name, "Nitrogen");

			Node->dev_id[0]=HANBACK_NITROGEN_FRONT;
			Node->dev_id[1]=HANBACK_NITROGEN_REAR;
            Node->dev_pacLen = 9;
            Node->dev_datalen = 2;
		}
		else if(Node->q_data[0][2] == HANBACK_OZONE_FRONT &&
				Node->q_data[0][3] == HANBACK_OZONE_REAR) {
			strcpy(Node->dev_name, "Ozone");

			Node->dev_id[0]=HANBACK_OZONE_FRONT;
			Node->dev_id[1]=HANBACK_OZONE_REAR;
            Node->dev_pacLen = 9;
            Node->dev_datalen = 2;
		}
		else if(Node->q_data[0][2] == HANBACK_THRMMTR_FRONT &&
				Node->q_data[0][3] == HANBACK_THRMMTR_REAR) {
			strcpy(Node->dev_name, "Thermometer");

			Node->dev_id[0]=HANBACK_THRMMTR_FRONT;
			Node->dev_id[1]=HANBACK_THRMMTR_REAR;
			Node->dev_pacLen = 11;
			Node->dev_datalen = 4;
		}
		else if(Node->q_data[0][2] == HANBACK_VOC_FRONT &&
				Node->q_data[0][3] == HANBACK_VOC_REAR) {
			strcpy(Node->dev_name, "VOC");

			Node->dev_id[0]=HANBACK_VOC_FRONT;
			Node->dev_id[1]=HANBACK_VOC_REAR;
            Node->dev_pacLen = 9;
            Node->dev_datalen = 2;
		}
		else if(Node->q_data[0][2] == HANBACK_WEATHER_FRONT &&
				Node->q_data[0][3] == HANBACK_WEATHER_REAR) {
			strcpy(Node->dev_name, "Weather");

			Node->dev_id[0]=HANBACK_WEATHER_FRONT;
			Node->dev_id[1]=HANBACK_WEATHER_REAR;
			Node->dev_pacLen = 19;
			Node->dev_datalen = 12;
		}
	}
	else
		strcpy(Node->dev_name, "#Unknown");
}

void M_printDeviceInfo(listNode Node)
{
	printf("  Device Maker\t: %s\n", Node.dev_maker);
	printf("  Device Name\t: %s\n", Node.dev_name);
	printf("  Device Path\t: %s\n", Node.dev_path);
	printf("  Device paclen\t: %d\n",Node.dev_pacLen);
	printf("\n");
}
