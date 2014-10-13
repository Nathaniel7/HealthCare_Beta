// <FilterData.c>

#include "filterData.h"
#include "Logcat.h"

void printLogcat(const char *buff, int count,char* dev_name)
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

void printHex2(const char *buff, int count,char* dev_name)
{
    int i;

    printf("-------%s PrintHex--------\n",dev_name);
    for(i = 0; i < count; i++) {
        printf("%02X ", buff[i]);
    }

    printf("\n");
}

int F_checkStartBit(listNode* p, unsigned char* readBuff)
{
    int readSize = 0;
    if(0 == strcmp(p->dev_maker,"HANBACK"))
    {
        if(readBuff[0] == HANBACK_START_BIT_FRONT &&
                readBuff[1] == HANBACK_START_BIT_REAR)
        {
            return 1;
        }
    }
    ///add code
    return 0;
}

void F_readData(listNode* p)
{
    int readSize = 0;
    int readTotalSize = 0;
    unsigned char readBuff[MAX_BUFF_SIZE] = { 0 };
    unsigned char readTemp[MAX_BUFF_SIZE] = { 0 };

    //printf("---before acces F_readData %s\n",p->dev_path);
    if (access(p->dev_path, R_OK & W_OK) == 0)
    {
        //printf("Access OK!\n");
        p->fd = user_uart_open(p->dev_path);
        user_uart_config(p->fd, 57600, 8, 0, 1);

        //start bit가 일치 할 때 까지 계속 data를 받음
        for(readTotalSize = 0; readTotalSize < p->dev_pacLen;) {
            if((readSize = user_uart_read(p->fd, readTemp, p->dev_pacLen)) == -1) {
                readSize = 0;
                continue;
            }

            strncat_s(readBuff, readTemp, readTotalSize, readSize);

            if(F_checkStartBit(p, readBuff)) {
                readTotalSize += readSize;
            }
            else {
                readSize = 0;
                readTotalSize = 0;
            }
        }
        //        printLogcat(readBuff, readTotalSize, p->dev_name);

        //////save CURRDATA,PREVDATA///////////////////////
        strncat_s(p->p_buf, p->c_buf, 0, p->dev_pacLen);
        strncat_s(p->c_buf, readBuff, 0, p->dev_pacLen);
        ///////////////////////////////////////////////////

        ////data/////
        F_filterData(p, p->c_buf, p->dev_pacLen);
        memset(readBuff, 0, sizeof(readBuff));

        user_uart_close(p->fd);
    }
    //    else //node delete
    //    {
    //        user_uart_close(p->fd);
    //        deleteNode(p, p->dev_path);
    //    }
}

void F_filterData(listNode *p, unsigned char *readBuff, int count)
{
    int i;
    int index;
    int value[2] = {0};     // measure data of readBuff
    int data[2] = {0};    //

    int front = p->a_front;
    int rear = p->a_rear;

    if( strcmp(p->dev_maker,"HANBACK") == 0 )
    {
        // data start location
        index = 4;

        //        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "HANBACK Filtering");
        //// 1.make Analyzed Data ////
        for(i = 0; i < p->dev_datalen / 2; i++)
        {
            value[0] = (int)readBuff[index+(i*2)];
            value[1] = (int)readBuff[index+(i*2)+1];

            //상위비트 하위비트 계산해서 하나의 데이터로 analyzed data 생성
            data[i] = (int)( ((0xFF & value[0])<<8) + (0xFF & value[1]) );

            if(data[i] < 0)
                data[i] = 0;

            if(0 == strcmp(p->dev_name,"VOC"))
                data[i] /= 10;
        }

        if( p->dev_id[0] == HANBACK_THRMMTR_FRONT &&
                p->dev_id[1] == HANBACK_THRMMTR_REAR )
        {

            ////  2-1. Data store
            //            for(i = 0; i < p->dev_datalen / 2; i++) // Body Temp, Interior Temp store
            for(i = 0; i < p->dev_datalen / 4; i++) // Body Temp store
            {
                p->c_analyzedData[i] = data[i];
            	p->D_data.a_Data[i] = data[i];

                ////  2-2. Remove Outlier
                // p->c_analyzedData[front] == Body Temperature
                // Fixed value: because body temperature is never high 40...
                if( p->c_analyzedData[i] < 4500 &&
                        p->c_analyzedData[i] > 0)
                {
                    setFilterQdata(p, p->c_analyzedData[i], CURRDATA);
                    p->D_data.f_Data[i] = p->c_analyzedData[i];
                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%d", p->c_analyzedData[front]);
                }
            }
        }//end else if
        else if( p->dev_id[0] == HANBACK_DIOXIDE_FRONT &&
                p->dev_id[1] == HANBACK_DIOXIDE_REAR)
        {
            // p->p_analyzedData & p->c_analyzedData store
            for(i = 0; i < p->dev_datalen / 2; i++)
            {
                if( front < MAX_BUFF_SIZE )
                {
                    // current analyzed data를 저장함
                    p->c_analyzedData[front] = data[i];
                	p->D_data.a_Data[i] = data[i];

                    // previous analyzed data 를 저장함
                    if( front > 0 )
                        p->p_analyzedData[front-1] = p->c_analyzedData[front-1];
                    else if( front == 0 && rear != 0 )
                        p->p_analyzedData[MAX_BUFF_SIZE-1] = p->c_analyzedData[front];

                    front += 1;
                }
                else
                {
                    front = 0;
                    rear += 1;
                }
            }

            if( p->a_flag == 1 ) {
                int tmp_front = front - 1;  // front - 1 == recent p_analyzedData
                int tmp_data = 0;
                int displacement[2] = {0};

                for(i = 0; i < 10; i++)
                {
                    if(tmp_front > 0 && tmp_front < MAX_BUFF_SIZE)
                        tmp_data += abs(p->p_analyzedData[tmp_front]) - abs(p->p_analyzedData[tmp_front-1]);
                    else {
                        tmp_front = 0;
                        tmp_data += abs(p->p_analyzedData[tmp_front]) - abs(p->p_analyzedData[MAX_BUFF_SIZE-1]);
                    }
                    tmp_front++;
                }

                // "i" is window size : n-1 == 9
                tmp_data /= i-1;

                // displacement calculation
                displacement[0] = p->p_analyzedData[front] - tmp_data;
                displacement[1] = p->p_analyzedData[front] + tmp_data;

                // Filter Data
                if( p->c_analyzedData[front] >= displacement[0] &&
                        p->c_analyzedData[front] <= displacement[1] )
                {
                    setFilterQdata(p, p->c_analyzedData[front], CURRDATA);
                    p->D_data.f_Data[0] = p->c_analyzedData[front];
//                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%d", p->c_analyzedData[front]);
                }
            }
        }//end else if
        else if( p->dev_id[0] == HANBACK_DUST_FRONT &&////////////////////////////////////////dustalgo
                p->dev_id[1] == HANBACK_DUST_REAR &&
				(data[i] != 60 && data[i] != 0))
        {
            // p->p_analyzedData & p->c_analyzedData store
            for(i = 0; i < p->dev_datalen / 2; i++)
            {
                if( front < MAX_BUFF_SIZE)
                {
                    // current analyzed data를 저장함
                    p->c_analyzedData[front] = data[i];
                	p->D_data.a_Data[i] = data[i];

                    // previous analyzed data 를 저장함
                    if( front > 0 )
                        p->p_analyzedData[front-1] = p->c_analyzedData[front-1];
                    else if( front == 0 && rear != 0 )
                        p->p_analyzedData[MAX_BUFF_SIZE-1] = p->c_analyzedData[front];

                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%d", p->c_analyzedData[front]);
                    front += 1;
                }
                else
                {
                    front = 0;
                    rear += 1;
                }
            }

            if( p->a_flag == 1 ) {
                int tmp_front = front - 1;  // front - 1 == recent p_analyzedData
                int tmp_data = 0;
                int displacement[2] = {0};

                for(i = 0; i < 10; i++)
                {
                    if(tmp_front > 0 && tmp_front < MAX_BUFF_SIZE)
                        tmp_data += abs(p->p_analyzedData[tmp_front]) - abs(p->p_analyzedData[tmp_front-1]);
                    else {
                        tmp_front = 0;
                        tmp_data += abs(p->p_analyzedData[tmp_front]) - abs(p->p_analyzedData[MAX_BUFF_SIZE-1]);
                    }
                    tmp_front++;
                }

                // "i" is window size : n-1 == 9
                tmp_data /= i-1;

                // displacement calculation
                displacement[0] = p->p_analyzedData[front] - tmp_data;
                displacement[1] = p->p_analyzedData[front] + tmp_data;

                // Filter Data
                if( p->c_analyzedData[front] >= displacement[0] &&
                        p->c_analyzedData[front] <= displacement[1] )
                {
                    setFilterQdata(p, p->c_analyzedData[front], CURRDATA);
                    p->D_data.f_Data[0] = p->c_analyzedData[front];
                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%d", p->c_analyzedData[front]);
                }
            }
        } //end for
        else if(p->dev_id[0] == HANBACK_VOC_FRONT &&////////////////////////////////////////vocalgo
                p->dev_id[1] == HANBACK_VOC_REAR)
        {
            // p->p_analyzedData & p->c_analyzedData store
            for(i = 0; i < p->dev_datalen / 2; i++)
            {
                if( front < MAX_BUFF_SIZE )
                {
                    // current analyzed data를 저장함
                    p->c_analyzedData[front] = data[i];
                	p->D_data.a_Data[i] = data[i];

                    // previous analyzed data 를 저장함
                    if( front > 0 )
                        p->p_analyzedData[front-1] = p->c_analyzedData[front-1];
                    else if( front == 0 && rear != 0 )
                        p->p_analyzedData[MAX_BUFF_SIZE-1] = p->c_analyzedData[front];
                    front += 1;
                }
                else
                {
                    front = 0;
                    rear += 1;
                }
            }

            if( p->a_flag == 1 ) {
                int tmp_front = front - 1;  // front - 1 == recent p_analyzedData
                int tmp_data = 0;
                int displacement[2] = {0};

                for(i = 0; i < 10; i++)
                {
                    if(tmp_front > 0 && tmp_front < MAX_BUFF_SIZE)
                        tmp_data += abs(p->p_analyzedData[tmp_front]) - abs(p->p_analyzedData[tmp_front-1]);
                    else {
                        tmp_front = 0;
                        tmp_data += abs(p->p_analyzedData[tmp_front]) - abs(p->p_analyzedData[MAX_BUFF_SIZE-1]);
                    }
                    tmp_front++;
                }

                // "i" is window size : n-1 == 9
                tmp_data /= i-1;

                // displacement calculation
                displacement[0] = p->p_analyzedData[front] - tmp_data;
                displacement[1] = p->p_analyzedData[front] + tmp_data;

                // Filter Data
                if( p->c_analyzedData[front] >= displacement[0] &&
                        p->c_analyzedData[front] <= displacement[1] )
                {
                    setFilterQdata(p, p->c_analyzedData[front], CURRDATA);
                    p->D_data.f_Data[0] = p->c_analyzedData[front];
//                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%d", p->c_analyzedData[front]);
                }
            }
        }
    }

    p->a_front = front;
    p->a_rear += rear;

    if(p->a_cnt < 10)
        p->a_cnt += 1;
    else if( rear != 0 || front > 9 )
        p->a_flag = 1;

}

int S_SummaryHanbackSensor(listNode* p, int index, int data)//data return
{
    int ESMA = 0;//지수이동평균을 구하는 값입니다. 사용할 데이터의 갯수는 10으로 한다.
    int SMA = 0;//이동평균을 구하는 값입니다. 사용할 데이터의 갯수는 10으로 한다.
    float WMA;//가중 평균을 구하는 값입니다.
    int EWMA = 0;//지수가중이동평균을 구하는 값입니다.
    int ESMA_cnt = 10;
    int totalQueueData[MAX_SENSOR_DATANUM] = {0};//analyzed data가 여러개일경우를 대비

    int data_cnt = p->f_read_cnt-1; //데이터를 받은 횟수입니다 정확도를 위하여 -2를 합니다 (처음 리드시 0이 나오므로)
    int i = 0;
    int front = p->f_cfront-1;//현재 Queue의 front가 가리키는 위치는 빈 공간이기 때문에
    int invalid_data_cnt = 0;//데이터가 0이 들어올 경우를 예외처리하기 위한 변수

    //    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Summary data / %04d", data);

    if( (strcmp(p->dev_name,"Dioxide") == 0 ||
         strcmp(p->dev_name,"VOC") == 0 ||
         strcmp(p->dev_name,"Dust") == 0) ||
         strcmp(p->dev_name,"Thermometer") == 0 && data != 0)
    {
        if( data_cnt > 0)
        {
            if(data_cnt > ESMA_cnt)//데이터분석을 하기위해선 이전의 데이터가 필요하기 때문에
            {
                for(i = 0; i < ESMA_cnt; i++)//SMA를 구하는 과정 현재 recv한 데이터는 더하지 않는다 또 큐에 존재하지 않는다.
                {
                    if(front >= 0)
                    {
                        totalQueueData[0] += p->c_filteredData[front];
                    }
                    else
                    {
                        front = MAX_BUFF_SIZE-1;
                        totalQueueData[0] += p->c_filteredData[front];
                    }

                    front--;
                }
                SMA = totalQueueData[0] / (ESMA_cnt); //전일 데이터이므로

                if (SMA > 0)
                    ESMA = data * (float) 2 / (ESMA_cnt + 1) + SMA * (1 - (float) 2 / (ESMA_cnt + 1));


                return ESMA;
            }
            else
                return data;
        }
    }

    return 0;
}
