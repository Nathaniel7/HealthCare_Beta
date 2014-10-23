#include "summaryData.h"
#include "Logcat.h"

int S_SummaryHanbackSensor(listNode* p, int index)//data return
{
	int ESMA = 0;//지수이동평균을 구하는 값입니다. 사용할 데이터의 갯수는 10으로 한다.
	int SMA = 0;//이동평균을 구하는 값입니다. 사용할 데이터의 갯수는 10으로 한다.
	float WMA;//가중 평균을 구하는 값입니다.
	int EWMA = 0;//지수가중이동평균을 구하는 값입니다.
	int ESMA_cnt = 10;
	int totalQueueData[MAX_SENSOR_DATANUM] = {0};//analyzed data가 여러개일경우를 대비

	int data_cnt = p->f_read_cnt-1; //데이터를 받은 횟수입니다 정확도를 위하여 -2를 합니다 (처음 리드시 0이 나오므로)
	int i = 0;
	int invalid_data_cnt = 0;//데이터가 0이 들어올 경우를 예외처리하기 위한 변수

	int front = 0;//현재 Queue의 front가 가리키는 위치는 빈 공간이기 때문에
	int tmp_front = 0;

	if(p->f_cfront > 1) {
		front = p->f_cfront-1;
		tmp_front = p->f_cfront-2;
	}
	else if(p->f_cfront == 1) {
		front = p->f_cfront-1;
		tmp_front = MAX_BUFF_SIZE-1;
	}
	else {
		front = MAX_BUFF_SIZE-1;
		tmp_front = MAX_BUFF_SIZE-2;
	}

	//    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Summary data / %04d", data);

	if( (strcmp(p->dev_name,"Dioxide") == 0 ||
			strcmp(p->dev_name,"VOC") == 0 ||
			strcmp(p->dev_name,"Dust") == 0) ||
			strcmp(p->dev_name,"Thermometer") == 0)
	{
		if( data_cnt > 0)
		{
			if(data_cnt > ESMA_cnt)//데이터분석을 하기위해선 이전의 데이터가 필요하기 때문에
			{
				for(i = 0; i < ESMA_cnt; i++)//SMA를 구하는 과정 현재 recv한 데이터는 더하지 않는다 또 큐에 존재하지 않는다.
				{
					if(tmp_front >= 0)
					{
						totalQueueData[0] += p->c_filteredData[tmp_front];
					}
					else
					{
						tmp_front = MAX_BUFF_SIZE-1;
						totalQueueData[0] += p->c_filteredData[tmp_front];
					}

					tmp_front--;
				}
				SMA = totalQueueData[0] / (ESMA_cnt); //전일 데이터이므로

				if (SMA > 0)
					ESMA = p->c_filteredData[front] * (float)2 / (ESMA_cnt + 1) + SMA * (1 - (float)2 / (ESMA_cnt + 1));

				//                __android_log_print(ANDROID_LOG_INFO, "Summary", "c_Data: %d\t SMA: %d\t ESMA: %d", p->c_filteredData[front], SMA, ESMA);

				return ESMA;
			}
			else
				return p->c_filteredData[front];
		}
	}

	return 0;
}


int S_SummarySensorStatus(listNode* p, int data)
{
	if(strcmp(p->dev_name,"Dioxide") == 0) {
		if(data >= 2000)						p->dev_abs.res_state = STATE_WORST;
		else if(data >= 1000 &&	data < 2000)	p->dev_abs.res_state = STATE_BAD;
		else if(data >= 350 &&	data < 1000)	p->dev_abs.res_state = STATE_NORMAL;
		else if(data < 350)						p->dev_abs.res_state = STATE_GOOD;
	}
	else if(strcmp(p->dev_name,"VOC") == 0) {
		if(data >= 10000)						p->dev_abs.res_state = STATE_WORST;
		else if(data >= 1000 &&	data < 10000)	p->dev_abs.res_state = STATE_BAD;
		else if(data >= 500 &&	data < 1000)	p->dev_abs.res_state = STATE_NORMAL;
		else if(data < 500)						p->dev_abs.res_state = STATE_GOOD;
	}
	else if(strcmp(p->dev_name,"Dust") == 0) {
		if(data >= 800)							p->dev_abs.res_state = STATE_WORST;
		else if(data >= 500 &&	data < 800)		p->dev_abs.res_state = STATE_BAD;
		else if(data >= 300 &&	data < 500)		p->dev_abs.res_state = STATE_NORMAL;
		else if(data < 300)						p->dev_abs.res_state = STATE_GOOD;
	}
	else if(strcmp(p->dev_name,"Thermometer") == 0) {
		if(data >= 4000 || data < 3000)			p->dev_abs.res_state = STATE_WORST;
		else if(data >= 3800 &&	data < 4000)	p->dev_abs.res_state = STATE_BAD;
		else if(data >= 3600 &&	data < 3800)	p->dev_abs.res_state = STATE_NORMAL;
		else if(data >= 3000 && data < 3600)	p->dev_abs.res_state = STATE_GOOD;
	}
}
