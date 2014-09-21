#include "monitorDevice.h"

// Device Monitoring Function
int M_connectDevice(listNode_h *N)
{
	int i = 0;
	char USB_serial[MAX_DEV_NUM][MAX_DEV_PATH_SIZE] = {
//			"/dev/ttyUSB0", "/dev/ttyUSB1",	"/dev/ttyUSB2", "/dev/ttyUSB3", "/dev/ttyUSB4"};
			"/dev/ttyUSB0", "/dev/ttyUSB2", "/dev/ttyUSB3", "/dev/ttyUSB4",
			"/dev/ttyUSB5", "/dev/ttyUSB6", "/dev/ttyUSB7", "/dev/ttyUSB8", "/dev/ttyUSB9" };

	printf("\n>> Connect Device\n");
	for(i = 0; i < MAX_DEV_NUM; i++){	// Device Search and connect
		int read_size;

		//*Check Device exist
		if(access(USB_serial[i], R_OK & W_OK) == 0) {
			// make Linked List
			M_makeLinkedList(N, USB_serial[i]);
			// check not yet open Device
			if(N->tail->fd == 0) {
				M_openDevice(N);
			}
			if(N->tail->fd != -1 && !strcmp(N->tail->dev_name, "")) {

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
	}
	printCutLine();		// ---------------------------------

	return 0;
}

void M_makeLinkedList(listNode_h *N, char *dev_path)
{
	//	printf(">> START Make Linked List\n");
	if(N->head == NULL)
		insertLastNode(N, dev_path);
	else {
		int i;
		listNode *prev = N->head;

		while(prev != NULL) {
			if(strcmp(prev->dev_path, dev_path) == 0)
				break;
			else if(prev == N->tail)
				insertLastNode(N, dev_path);

			prev = prev->next;
		}
	}
}

void M_openDevice(listNode_h *N)
{
	N->tail->fd = user_uart_open(N->tail->dev_path);
	if(N->tail->fd != -1) {
		user_uart_config(N->tail->fd, 57600, 8, 0, 1);
		printf(">> Open Device: %s, FD: %d\n", N->tail->dev_path, N->tail->fd);
	}
	else
		printf(">> Open Device Fail!!!!!\n");
}

int M_checkDeviceFingerprint(listNode *Node)
{
	int readSize;
	int readTotalSize;
	unsigned char buff[MAX_BUFF_SIZE] = {0};

	int checkCompany = 0;

	// tcflush() 함수는, fd 그리고 참조되는 단말에 쓰기나무 데이터 이지만 아직 단말에 송신되어 있지 않은 모든 데이터,
	//     또는 그 단말로부터 수신한 데이터이지만 아직 읽어들이지 않은 모든 데이터를, action 의 값에 응해 폐기합니다.
	// TCIFLUSH	 수신했지만 읽어들이지 않은 데이터를 버립니다.
	// TCOFLUSH	 쓰기응이지만 송신하고 있지 않는 데이터를 버립니다.
	// TCIOFLUSH 수신했지만 읽어들이지 않은 데이터, 및 쓰기응이지만 송신하고 있지 않는 데이터의 양쪽 모두를 버립니다.

	tcflush(Node->fd, TCIFLUSH);
	printf(" -Analysing Sensor Packet Data...\n");

	// Read data until buff[MAX_BUFF_SIZE] full
	for(readTotalSize = 0; readTotalSize < MAX_BUFF_SIZE; readTotalSize += readSize) {
		if((readSize = user_uart_read(Node->fd, buff, MAX_BUFF_SIZE)) == -1) {
			readSize = 0;
			continue;
		}

//		printHex(buff, readSize);
		strncat_s(Node->q_data[0], buff, readTotalSize, readSize);
	}
	//	printHex(Node->q_data[0], readTotalSize);

	if(M_checkDeviceCompany(Node)) {
	    M_checkSensor(Node);
	    return true;
	}
	else {
	    memset(Node , NULL, sizeof(Node));
	}

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
