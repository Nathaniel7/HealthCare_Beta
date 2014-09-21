// <FilterData.c>

#include "filterData.h"

void printHex2(const char *buff, int count,char* dev_name)
{
    int i;

    printf("-------%s PrintHex--------\n",dev_name);
    for(i = 0; i < count; i++) {
        printf("%02X ", buff[i]);
    }

    printf("\n");
}
//////

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
	    for(readTotalSize = 0; readTotalSize <= p->dev_pacLen;) {
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

	    //////save CURRDATA,PREVDATA///////////////////////
	    strncat_s(p->p_buf, p->c_buf, 0, p->dev_pacLen);
	    strncat_s(p->c_buf, readBuff, 0, p->dev_pacLen);
	    ///////////////////////////////////////////////////

	    ////data/////
	    //printHex2(readBuff, readSize, p->dev_name);
	    F_analyzeData(p, readBuff, p->dev_pacLen);
	    memset(readBuff, 0, sizeof(readBuff));

	    user_uart_close(p->fd);
	}
	else //node delete
	{
	    user_uart_close(p->fd);
	    deleteNode(p, p->dev_name);
	}
}

void F_analyzeData(listNode* p, unsigned char* readBuff, int count)//queuing
{
    int data[10]={-999};
    int index,i;//modify
    //////Thermometer///////
    int value[2]={0};//센서의 데이터길이가 2인경우 이 변수를 사용합니다.
    int analyzedData[10]={-999};//**modify**
    int stateErrData=1; //err 1 normal 0
    ///////Weather//////
    int temp[p->dev_pacLen];
    int L_CH0, L_CH1;
    int lux;
    float R2;
    double R;
    ///////////////////////////////////////////
    //setRawData(p,readBuff,CURRDATA);
    ////////////////////////////analyze Data///////////////////////////////////////
    if( 0 == strcmp(p->dev_maker,"HANBACK"))
    {
        index=4;//except MAKERID,DEVICEID

        /////1.make Analyzed Data/////
        for(i=0;i<p->dev_datalen/2;i++)
        {
        	value[0] =(int)readBuff[index+(i*2)];
        	value[1] =(int)readBuff[index+(i*2)+1];

        	data[i] = (int)( ((0xFF & value[0])<<8) + (0xFF & value[1]) );//상위비트 하위비트 계산해서 하나의 데이터로 analyzed data 생성
        	if(data[i]<0) data[i]=0;

       	    ///////////save Analyzed data///////////(분석된 anlyzed data를 저장한다)
			p->p_analyzeData[i] = p->c_analyzeData[i]; //previous analyzed data 를 저장함
			p->c_analyzeData[i] = data[i]; //current analyzed data를 저장함
			////////////////////////////////////////
        }
        //////2.센서별로 처리할 데이터코드 //////
        if(p->dev_id[0] == HANBACK_WEATHER_FRONT &&
           p->dev_id[1] == HANBACK_WEATHER_REAR)//analyzed_data:packetlen + 1(lux)
        {
        	stateErrData=0;

            for(i=0;i<p->dev_datalen/2;i++)//(0,1) (2,3) (4,5) (6,7) (8,9) (10,11)(이렇게 6번데이터를 받아서 분석한다 )
            {
                //weather sensor는 센서에서 보낸 데이터를 가지고 계산하여 다른 데이터를 만들어야 하기 때문에 temp를 사용한다
                temp[(i*2)] =(int)readBuff[index+(i*2)];
                temp[(i*2)+1] =(int)readBuff[index+(i*2)+1];
                //              printf("High:[%02X]-->>%d+Low:[%02X]-->>%d"
                //                      ,readBuff[index+i],temp[i],readBuff[index+i+1],temp[i+1]);

                data[i] = (int)( ((0xFF & temp[(i*2)])<<8) + (0xFF & temp[(i*2)+1]) );//상위비트 하위비트 계산해서 하나의 데이터로 analyzed data 생성
                //printf(" [[[%d]]]\n",data);
                if(data[i]<0) data[i]=0;
                ///////////save Analyzed data///////////(분석된 anlyzed data를 저장한다)
                p->p_analyzeData[i] = p->c_analyzeData[i];//previous analyzed data 를 저장함
                p->c_analyzeData[i] = data[i];       //current analyzed data를 저장함
                ////////////////////////////////////////
                //lux 는 L_CH1 즉 (8,9)번째 데이터를 가지고 분석해여 만들 수 있다
                if(i == 5)//ADDCODE
                {	L_CH0 = data[i];    }
                else if(i == 6)
                {
                	L_CH1 = data[i];
					//문서화에 나와 있는 lux 계산 공식
					/*
					 double R = 0;
					 If(L_CH0 != 0)    R = L_CH1 / L_CH0;
					 //여기영역에서  L_CH0 은 p->p_analyzeData[i-1] 이고 L_CH1 은 p->p_analyzeData[i] or data 이다
					 lux = (int)(L_CH0 * 0.46 * Math.pow(2.71828, -3.13 * R))
					 */
					//lux 구한뒤
					if (L_CH0 != 0)
						R = L_CH1 / L_CH0;
					printf("#####R:%lf####\n");
//
					//lux = (int)pow(2.71828, -3.13 * R);
//					p->p_analyzeData[6] = lux;
					//에 저장
                }
                if(F_filterHanbackWeather(p, i,data[i]))
                {
                	analyzedData[i] = data[i];
                }
                else
                {
                    //DCURRENT
                	analyzedData[i] = -1;
                    ////WEATHERSTATE
                    p->getErrordataWeather[i] = 1;
                }
            }//end for
        }///end Weather
        else if(p->dev_id[0] == HANBACK_THRMMTR_FRONT &&////////////////////////////////////////thermometeralgo
                p->dev_id[1] == HANBACK_THRMMTR_REAR)//필터링안됨
        {

        	 for(i=0;i<p->dev_datalen/2;i++)
             if(F_filterHanbackThermometer(p,i))//#WARNING# FILTER_OPT_FILTER일경우 1만 반환 FILTER_OPTNONFILTERED라면 에러데이터는 0반환
                {								   //에러데이터는 -1로 표기되므로 그를 고려하여서 폐기하여야 한다.
//            	 printf("\ndata! -> %d  read_cnt:%d\n",data[i],p->read_cnt);

            	 analyzedData[i] = data[i];//우선 분석된데이터 중 1개라도
                    //에러데이터가 있는지 검사하기위해 analyzedData에 저장합니다.
              }
              else
              {
                	analyzedData[i] = -1;
                    printf("\n[Error Filtering] : O\n");
              }
                /////////////////////////////////////////////
            ////////////////////////////////////////////
        }//end else if
        else if(p->dev_id[0] == HANBACK_DIOXIDE_FRONT &&
                p->dev_id[1] == HANBACK_DIOXIDE_REAR)
        {
                ////////////Filter Analyzed data//////////////
        		for(i=0;i<p->dev_datalen/2;i++)
        		if(data[i] = F_filterHanbackSensor(p,i,data[i]))
        		{
        			analyzedData[i] = data[i];
        		}
        		else
        		{
        			//DCURRENT
        			analyzedData[i] = -1;
        		}
        		/////////////////////////////////////////////
        }//end else if
		else if(p->dev_id[0] == HANBACK_DUST_FRONT &&////////////////////////////////////////dustalgo
                p->dev_id[1] == HANBACK_DUST_REAR)
		{
				////////////Filter Analyzed data//////////////
				for(i=0;i<p->dev_datalen/2;i++)
				if(data[i] = F_filterHanbackSensor(p,i,data[i]))
				{
					//rintf("\ndata! -> %d read_cnt:%d total:%d \n",data[i],p->read_cnt,p->total_analyzeData[0]);

					if(data[i] <= 0) data[i] = 0;//처음데이터를 60으로시작
					else if(data[i] > 3000) data[i]=3000;

					analyzedData[i] = data[i];
					//에러데이터가 있는지 검사하기위해 analyzedData에 저장합니다.
				}
				else
				{
					//DCURRENT
					analyzedData[i] = -1;
					/////////
					printf("\n[Error Filtering] : O\n");
				}
				/////////////////////////////////////////////
		} //end for
		else if(p->dev_id[0] == HANBACK_VOC_FRONT &&////////////////////////////////////////vocalgo
                p->dev_id[1] == HANBACK_VOC_REAR)
		{
			////////////Filter Analyzed data//////////////
			for(i=0;i<p->dev_datalen/2;i++)
			if (F_filterHanbackVOC(p, i)) //filter error data
			{
				analyzedData[i] = data[i]/10; //우선 분석된데이터 중 1개라도
				//에러데이터가 있는지 검사하기위해 analyzedData에 저장합니다.
				printf("\n###Data:%d###\n",data[i]);
			}
			else
			{
				//DCURRENT
				analyzedData[i] = -1;
			}
		/////////////////////////////////////////////
		}
    /////////////3.분석된 큐에 데이터를 집어 넣음//////////////
    //////////save Queue Analyzed data//////////
    for (i = 0; i < p->dev_datalen / 2; i++)
       setQueuedata(p, analyzedData[i], CURRDATA);
    }//end if if( 0 == strcmp(p->dev_maker,"HANBACK"))
}

int F_filterHanbackSensor(listNode* p,int index,int data)//data return
{
	int EMA=0;//지수평균을 구하는 값입니다. 사용할 데이터의 갯수는 10으로 한다.
	int SMA=0;//이동평균을 구하는 값입니다. 사용할 데이터의 갯수는 10으로 한다.
	int ESMA_cnt = 10;
	int totalQueueData[MAX_SENSOR_DATANUM]={0};//analyzed data가 여러개일경우를 대비
	float dustTotalQueueData;

	int data_cnt = p->read_cnt-1; //데이터를 받은 횟수입니다 정확도를 위하여 -2를 합니다 (처음 리드시 0이 나오므로)
	int i=0;
	int front = p->q_front-1;//현재 Queue의 front가 가리키는 위치는 빈 공간이기 때문에

	if( 0 == strcmp(p->dev_name,"Dioxide") ||
		0 == strcmp(p->dev_name,"VOC")		)//지수이동평균 그리고 모두 분석대상 데이터는 1개
	{
		if( data_cnt > 0)
		{
			if(data_cnt > ESMA_cnt)//데이터분석을 하기위해선 이전의 데이터가 필요하기 때문에
			{
				for(i=0;i<ESMA_cnt;i++)//SMA를 구하는 과정 현재 recv한 데이터는 더하지 않는다 또 큐에 존재하지 않는다.
				{
					totalQueueData[0] += p->q_rdata[front];
					front--;
				}
				printf("\n##total:%d/ %d\n",totalQueueData[0],(ESMA_cnt));



				SMA = totalQueueData[0] / (ESMA_cnt); //전일 데이터이므로
				printf("\n###data_cnt : %d recv_Data : %d   SMA : %d\n", data_cnt,
						data, SMA);

				if (SMA > 0)
					EMA = data * (float) 2 / (ESMA_cnt + 1)
							+ SMA * (1 - (float) 2 / (ESMA_cnt + 1));
				printf("###EMA : %d\n", EMA);

				return EMA;

			}
			else
				return data;
		}
	}
	else if( 0 == strcmp(p->dev_name,"Dust") )//가중 이동평균을 구한다.
	{
		float dustTotalQueueData=0;

		if(data < 0) data = 0;
		else if(data > 3000) data = 3000;//dust에러값처리


		if(data_cnt>0)
		{
			if( data_cnt < 10)//10으로 설정
			{
				ESMA_cnt = data_cnt;
				for (i = 0; i < ESMA_cnt - 1; i++)
				{
					totalQueueData[0] += p->q_rdata[front];
					front--;
				}

				totalQueueData[0] += data;
				SMA = totalQueueData[0]/data_cnt; //

				printf("\n###data_cnt : %d recv_Data : %d   SMA : %d\n", data_cnt, data, SMA);

				return SMA;
			}
			else
			{
				ESMA_cnt = 10;
				for (i=ESMA_cnt-2;i>=1;i--)//8개를 더해줌
				{
					float value_rate = i*0.02;
					dustTotalQueueData += p->q_rdata[front]*value_rate;
					//printf("\n###plus data:%f\n",p->q_rdata[front]*value_rate);
					front--;
				}

				dustTotalQueueData += data*0.18;
				printf("\n##data_cnt : %d recv_Data : %d  가중평균: %d\n", data_cnt, data,(int)dustTotalQueueData);

				return (int)dustTotalQueueData;
			}

			return SMA;
		}
		else
			return data;
	}

	return 0;
}



int F_filterHanbackWeather(listNode *p, int i,int data)
{
    int curr_data = p->p_analyzeData[i];
    int prev_data = p->c_analyzeData[i];
    //(0,1) (2,3) (4,5) (6,7) (8,9) (10,11)(이렇게 6번데이터를 받아서 분석한다 )
    //////modify
    if(0 == i)//(0,1) 현재 온도 측정치 (Temp), 아래의 if 문 조건을 모두 만족하면 필터링 대상이 되어서 0을 리턴한다.
    {
        if(curr_data > 290+50 || curr_data < 290-50)//weather 센서의 온도는 analyzedData/10 이다.
        {
            return 0;
        }
    }
    else if(1 == i)//(2,3) 현재 기압 측정치 (Press)
    {
        //ADDCODE
        //이부분에 대해서는 정보가 필요 하다. 또, 측정환경에서 기압에 변화를 줄 수 있는 방법이 있을지가 의문.
        if( abs(curr_data-prev_data) > 50 )
        {
            return 0;   // filtering [o]
        }
    }
    else if(2 == i || 3 == i)
    {
        //ADDCODE
        //가속도계의 X 축 변화 Y 축 변화 (어플 측정을 통해서 알아야 함 형식은 위의 Temp 형식과 같이 써주면 된다.)
        if( abs(curr_data-prev_data) > 50 )
        {
            return 0;   // filtering [o]
        }
    }
    else if(4 == i)
    {
        if(abs(curr_data-prev_data) > 50) {
            return 0;
        }
        //LCH_0
    }
    else if( 5 == i)
    {
        if(abs(curr_data-prev_data) > 50) {
            return 0;
        }
        //LCH_1
    }
    else if( 6 == i)
    {
        ///조도
    }

    return 1;   // filtering [x]
}

int F_filterHanbackThermometer(listNode* p,int index)
{
    //printf("\n[Filterling]-->> prev:%d current:%d\n",p->p_analyzeData[index],p->c_analyzeData[index]);
    int curr_data =p->c_analyzeData[index];
    int prev_data =p->p_analyzeData[index];

    //int filter_num=200;//+- 2'C

    //background 데이터를 기준으로 측정된 평균 데이터 (28'C)를 기준으로 +-3 이상 측정시 필터링됩니다(Background data)
    if(p->dev_filter_opt == FILTER_OPT_BACKDATA)
    {
        if(curr_data > 2800+400 || curr_data < 2800-400)
            return 0;
    }
    //현재 분석된 데이터가 이전 데이터와 비교했을때 2도 이상 차이가 나면 필텉링 됩니다 (변위를 이용한 필터링)
    else if(p->dev_filter_opt == FILTER_OPT_DEPLACEMENT)
    {
        if(abs(curr_data - prev_data) > 200)
            return 0;
    }
    //현재 분석된 데이터와 이전 데이터의 평균을 기준으로 3도 이상 차이나면 필티렁 됩니다(평균을 이용한 필터링)
    else if(p->dev_filter_opt == FILTER_OPT_AVERAGE)
    {
        if( curr_data > (curr_data+prev_data)/2 +300 ||
                curr_data < (curr_data+prev_data)/2 -300)
            return 0;
    }
    /*캡스톤용*/
    //필터링을 사용할 경우
    else if(p->dev_filter_opt == FILTER_OPT_FILTER)
    {
    	//NOW//
    	//if(abs(curr_data - prev_data) > 200)
    	      //return 0;
    }
    //사용하지 않을경우
    return 1;
}

int F_filterHanbackDioxide(listNode* p, int index)
{
    //printf("\n[Filterling]-->> prev:%d current:%d\n",p->p_analyzeData[index],p->c_analyzeData[index]);
    int curr_data =p->c_analyzeData[index];
    int prev_data =p->p_analyzeData[index];

    //int filter_num=200;//+- 2'C

    //background 데이터를 기준으로 측정된 평균 데이터 (28'C)를 기준으로 +-3 이상 측정시 필터링됩니다(Background data)
    if(p->dev_filter_opt == FILTER_OPT_BACKDATA)
    {
        if(curr_data < 300 || curr_data > 1000)
            return 0;
    }
    //현재 분석된 데이터가 이전 데이터와 비교했을때 2도 이상 차이가 나면 필텉링 됩니다 (변위를 이용한 필터링)
    else if(p->dev_filter_opt == FILTER_OPT_DEPLACEMENT)
    {
        if(abs(curr_data - prev_data) > 50)
            return 0;
    }
    //현재 분석된 데이터와 이전 데이터의 평균을 기준으로 3도 이상 차이나면 필티렁 됩니다(평균을 이용한 필터링)
    else if(p->dev_filter_opt == FILTER_OPT_AVERAGE)
    {
        if( curr_data > (curr_data+prev_data)/2 +50 ||
                curr_data < (curr_data+prev_data)/2 -50)
            return 0;
    }
    return 1;
}

int F_filterHanbackOzone(listNode* p, int index)
{
	int curr_data =p->c_analyzeData[index];
	int prev_data =p->p_analyzeData[index];

	if (p->dev_filter_opt == FILTER_OPT_BACKDATA)
	{

	}
	//현재 분석된 데이터가 이전 데이터와 비교했을때 2도 이상 차이가 나면 필텉링 됩니다 (변위를 이용한 필터링)
	else if (p->dev_filter_opt == FILTER_OPT_DEPLACEMENT)
	{

	}
	//현재 분석된 데이터와 이전 데이터의 평균을 기준으로 3도 이상 차이나면 필티렁 됩니다(평균을 이용한 필터링)
	else if (p->dev_filter_opt == FILTER_OPT_AVERAGE)
	{

	}

	return 1;
}

int F_filterHanbackDust(listNode* p, int index)
{
	int curr_data =p->c_analyzeData[index];
	int prev_data =p->p_analyzeData[index];

	if(p->dev_filter_opt == FILTER_OPT_FILTER)
	{
		if( curr_data < 60)
			return 0;

	}

	return 1;
}

int F_filterHanbackVOC(listNode* p, int index)
{
	int curr_data =p->c_analyzeData[index];
	int prev_data =p->p_analyzeData[index];

	if(p->dev_filter_opt == FILTER_OPT_FILTER)
	{
		if( curr_data < 0 || curr_data > 1000)
			return 0;
	}

	return 1;
}
