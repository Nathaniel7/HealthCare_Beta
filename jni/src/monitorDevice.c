#include "monitorDevice.h"
#include "Logcat.h"
#include "parson.h"

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
			"/dev/ttyUSB0", "/dev/ttyUSB1",	"/dev/ttyUSB2", "/dev/ttyUSB3", "/dev/ttyUSB4",
			"/dev/ttyUSB5", "/dev/ttyUSB6", "/dev/ttyUSB7", "/dev/ttyUSB8", "/dev/ttyUSB9" };


	// LOGI("  3. Monitor Devices");
	for(i = 0; i < MAX_DEV_NUM; i++){	// Device Search and connect
		int read_size;

		//         LOGI("   4. Monitor Devices");
		//*Check Device exist
		if(access(USB_serial[i], R_OK & W_OK) == 0) {
			//		    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "  ▲!!!!! access available / %s", USB_serial[i]);
			//             LOGI("    5. Monitor Devices");
			// make Linked List
			M_makeLinkedList(N, USB_serial[i]);
			// check not yet open Device
			if(N->tail->fd == 0) {
				//                LOGI("        9. Monitor open device");
				M_openDevice(N);
			}
			if(N->tail->fd != -1 && !strcmp(N->tail->dev_name, "")) {
				//		         LOGI("         10. Monitor fd, name check");
				// analysing sensor packet data

				if(M_checkDeviceFingerprint(N->tail))
				{
					//                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, " > Monitor1");
					// just display device node info
					// M_printDeviceInfo(*N->tail);
					user_uart_close(N->tail->fd);

					// Sensor Data store //
					N->tail->dev_abs.res_sensor          = N->tail->dev_id[0];
					N->tail->dev_abs.res_company         = N->tail->dev_maker_id[0];
					N->tail->dev_abs.res_sensor_datalen  = N->tail->dev_datalen;
					// Sensor Data store //

					N->tail->dev_monitor_status = 1;

					return 1;
				}
				else
				{
					N->tail->dev_monitor_status = 0;
					return 0;
				}
			}
			usleep(1000 * 100);
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

			}

			prev = prev->next;
		}
	}
}

void M_openDevice(listNode_h *N)
{
	N->tail->fd = user_uart_open(N->tail->dev_path);

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
	//    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, " > Monitor2");

	// Read data until buff[MAX_BUFF_SIZE] full
	for(readTotalSize = 0; readTotalSize < MAX_BUFF_SIZE; readTotalSize += readSize) {
		//LOGI("         10-2. Monitor read for loop");
		if((readSize = user_uart_read(Node->fd, buff, MAX_BUFF_SIZE)) == -1) {

			//LOGI("         10-3. Monitor read data");

			readSize = 0;
			continue;
		}
		//		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, " > Monitor Read");

		//		printHex(buff, readSize);
		strncat_s(Node->q_data[0], buff, readTotalSize, readSize);
	}
	//LOGI("         10-4. Monitor read End");
	//	printHex(Node->q_data[0], readTotalSize);

	if(M_checkDevice(Node))
		return true;
	else
		memset(Node, '\0', sizeof(Node));


	//	LOGI("         10-6. Monitor End");
	return false;
}



int M_checkDevice(listNode *Node)
{
	int i = 0;
	int find_flag = 0;
    const char *filename = "/sdcard/Healthcare/sensors.json";

	size_t	root_array_cnt		= 0;
	size_t	sensor_array_cnt	= 0;

	JSON_Value  *root_value			= NULL;		// 제일 처음 JSON파일을 담을 변수
	JSON_Array  *root_array			= NULL;		// JSON에서 회사별 오브젝트를 얻기 위한 배열 변수
	JSON_Object *root_object		= NULL;		// JSON에서 회사별 오브젝트를 담을 변수
	JSON_Array  *brand_array[2]		= {NULL};	// 회사별 고유번호가 담긴 오브젝트를 담을 변수
	JSON_Array  *sensor_array		= NULL;		// 회사의 센서 오브젝트를 얻기 위한 배열 변수
	JSON_Object *sensor_info_object	= NULL;		// 센서 정보가 담긴  오브젝트를 담을 변수
	JSON_Array  *sensor_id_array	= NULL;		// 센서의 고유번호를 담을 변수

	unsigned int brand_start_bit[2]	= {0};		// 회사 고유번호 Start Bit
	unsigned int brand_end_bit[2]	= {0};		// 회사 고유번호 End Bit
	const char	*brand_name			= NULL;		// 회사 이름

	unsigned int sensor_id[2]		= {0};		// 센서 고유번호
	const char 	*sensor_type		= NULL;		// 센서 고유번호
	unsigned int sensor_pac_len		= 0;		// 센서 고유번호
	unsigned int sensor_data_len	= 0;		// 센서 고유번호
	// JSON에 사용 END

    root_value = json_parse_file_with_comments(filename);
    root_array = json_value_get_array(root_value);
    root_array_cnt = json_array_get_count(root_array);

	for(i = 0, find_flag = 0; i < root_array_cnt; i++) {
        root_object = json_array_get_object(root_array, i);
        brand_array[0] = json_object_get_array(json_object_get_object(root_object, "brand"), "start bit");
        brand_array[1] = json_object_get_array(json_object_get_object(root_object, "brand"), "end bit");

        brand_name			= json_object_get_string(json_object_get_object(root_object, "brand"), "name");
        brand_start_bit[0]	= json_array_get_number(brand_array[0], 0);	// Brand Start Bit High
        brand_start_bit[1]	= json_array_get_number(brand_array[0], 1);	// Brand Start Bit Low
        brand_end_bit[0]	= json_array_get_number(brand_array[1], 0);	// Brand End Bit High
        brand_end_bit[1]	= json_array_get_number(brand_array[1], 1);	// Brand End Bit Low

        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "\tNo. %d, Name: %s, SH: %02X, SL: %02X, EH: %02X, EL: %02X", i, brand_name, brand_start_bit[0], brand_start_bit[1], brand_end_bit[0], brand_end_bit[1]);

    	if (Node->q_data[0][0] == brand_start_bit[0] &&
    		Node->q_data[0][1] == brand_start_bit[1]) {

    		int i;
    		for(i = 0; i < MAX_BUFF_SIZE; i++) {

    			if (Node->q_data[0][i] == brand_end_bit[0] &&
   					Node->q_data[0][i+1] == brand_end_bit[1]) {

    				///Add information to node
    				strcpy(Node->dev_maker, brand_name);
    				Node->dev_maker_id[0] = brand_start_bit[0];
    				Node->dev_maker_id[1] = brand_start_bit[1];
    				Node->dev_end_bit[0] = brand_end_bit[0];
    				Node->dev_end_bit[1] = brand_end_bit[1];

    				__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "\t\tName: %s, SH: %02X, SL: %02X, EH: %02X, EL: %02X", Node->dev_maker, Node->dev_maker_id[0], Node->dev_maker_id[1], Node->dev_end_bit[0], Node->dev_end_bit[1]);
    				find_flag = 1;
    				break;
    			}
    		}

    		if(find_flag)
    			break;
    	}
	}

	if(find_flag == 0) {
		strcpy(Node->dev_maker, "#Unknown");
		strcpy(Node->dev_name, "#Unknown");
		return 0;
	}

	sensor_array = json_object_get_array(root_object, "sensors");
	sensor_array_cnt = json_array_get_count(sensor_array);
	for(i = 0, find_flag = 0; i < sensor_array_cnt; i++) {
		sensor_info_object = json_array_get_object(sensor_array, i);

		sensor_id_array = json_object_get_array(sensor_info_object, "id");
		sensor_id[0] = json_array_get_number(sensor_id_array, 0);
        sensor_id[1] = json_array_get_number(sensor_id_array, 1);

        sensor_type = json_object_get_string(sensor_info_object, "type");
        sensor_pac_len = json_object_get_number(sensor_info_object, "pac len");
        sensor_data_len = json_object_get_number(sensor_info_object, "data len");

		if (Node->q_data[0][2] == sensor_id[0] &&
			Node->q_data[0][3] == sensor_id[1]) {
			strcpy(Node->dev_name, sensor_type);

			Node->dev_id[0]=sensor_id[0];
			Node->dev_id[1]=sensor_id[1];
			Node->dev_pacLen = sensor_pac_len;
			Node->dev_datalen = sensor_data_len;

			find_flag = 1;
			break;
		}
	}

	if(find_flag == 1)
		return 1;

	return 0;
}

void M_printDeviceInfo(listNode Node)
{
	printf("  Device Maker\t: %s\n", Node.dev_maker);
	printf("  Device Name\t: %s\n", Node.dev_name);
	printf("  Device Path\t: %s\n", Node.dev_path);
	printf("  Device paclen\t: %d\n",Node.dev_pacLen);
	printf("\n");
}
