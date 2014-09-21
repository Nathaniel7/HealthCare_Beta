#include "debug.h"

void D_HBACK_Thermometer(listNode* p)//D_printRawData(p, CURRDATA); , D_printUserData(p, CURRDATA);
{
    int prev_data =p->p_analyzeData[0];
    int prev_data2=p->p_analyzeData[1];
    int curr_data =p->c_analyzeData[0];
    int curr_data2=p->c_analyzeData[1];
    int i;
    //////////////////////////////////
    printf("\n\n---------------< %s > %d\n", p->dev_name, p->fd);
    D_printRawData(p, PREVDATA);
    D_printRawData(p, CURRDATA);
    printf("\n");

    D_printUserData(p, PREVDATA);
    D_printUserData(p, CURRDATA);
    /////////Quilification/////////////
    if(FILTER_OPT_BACKDATA == p->dev_filter_opt)
    {
        printf("\n\n[Physical Qualification]    :  25℃  ~ 35℃ ");
        printf("\n[Arround Qualification]     :  25℃  ~ 35℃  \n");
    }
    else if(FILTER_OPT_DEPLACEMENT == p->dev_filter_opt)
    {
        printf("\n\n[Qualification]     : %d.%d℃ - %d.%d℃  (±2℃)\n",
                prev_data / 100, prev_data % 100, curr_data / 100,
                curr_data % 100);
    }
    else if(FILTER_OPT_AVERAGE == p->dev_filter_opt)
    {
        printf("\n\n[Physical Qualification]     : %d.%d℃ (±2℃)",
        		p->avg_analyzeData[0] / 100, p->avg_analyzeData[0] % 100);
        printf("\n[Arround Qualification]     : %d.%d℃ (±2℃)\n",
                		p->avg_analyzeData[1] / 100, p->avg_analyzeData[1] % 100);
    }
    else if( FILTER_OPT_FILTER == p->dev_filter_opt)
    {
		printf("\n\n[Qualification]     : %d.%d℃ - %d.%d℃  (±2℃)\n",
		                prev_data / 100, prev_data % 100, curr_data / 100,
		                curr_data % 100);

		printf("\n\nFilter Mode [ o ]\n");
    }
    else if(FILTER_OPT_NONFILTERD == p->dev_filter_opt)
    {
    	printf("\n\nFilter Mode [ x ]\n");
    }
}

void D_HBACK_Dioxide(listNode* p)
{
    int prev_data =p->p_analyzeData[0];
    int curr_data =p->c_analyzeData[0];

    ////////
    printf("\n\n---------------< %s > %d\n", p->dev_name, p->fd);
    D_printRawData(p, PREVDATA);
    D_printRawData(p, CURRDATA);

    D_printUserData(p, PREVDATA);
    D_printUserData(p, CURRDATA);
    /////////Quilification/////////////
    if(3 == p->dev_filter_opt)
    {
        printf("\n\n[Qualification]     :  650ppm  (±350ppm)\n");
    }
    else if(4 == p->dev_filter_opt)
    {
        printf("\n\n[Qualification]     : %d%dppm - %d%dppm  (±50ppm)\n",
                prev_data / 100, prev_data % 100, curr_data / 100,
                curr_data % 100);
    }
    else if(5 == p->dev_filter_opt)
    {
        int avg_data  = (prev_data + curr_data) / 2;

        printf("\n\n[Qualification]     : %d%dppm (±50ppm)\n",
                avg_data / 100, avg_data % 100);
    }
}

//(0,1) (2,3) (4,5) (6,7) (8,9) (10,11)(이렇게 6번데이터를 받아서 분석한다 )
void D_HBACK_Weather(listNode* p)
{
    int i=0;
    int curr_data[10];
    for(i=0;i<p->dev_datalen/2+1;i++)//마지막은 lux
        curr_data[i] = p->p_analyzeData[i];

    ////////
    printf("\n\n---------------< %s > %d\n", p->dev_name, p->fd);
    D_printRawData(p, PREVDATA);
    D_printRawData(p, CURRDATA);

    //F_printUserData(p, PREVDATA);
	D_printUserData(p, CURRDATA);
	/////////Quilification/////////////

//	if (p->getErrordata && 0 != strcmp(p->dev_name, "Weather"))
//		printf("\n[Filtering]\t    : [o]");
//	else
//		printf("\n[Filtering]\t    : [x]");
}

void D_HBACK_Dust(listNode* p)
{
		int i = 0;
	    int curr_data[10];
	    for(i=0;i<p->dev_datalen/2+1;i++)//마지막은 lux
	        curr_data[i] = p->p_analyzeData[i];

	    ////////
	    printf("\n\n---------------< %s > %d\n", p->dev_name, p->fd);
	    D_printRawData(p, PREVDATA);
	    D_printRawData(p, CURRDATA);

		D_printUserData(p, CURRDATA);
}

void D_HBACK_VOC(listNode* p)
{
	int i = 0;
	int curr_data[10];
	for (i = 0; i < p->dev_datalen / 2 + 1; i++)
		curr_data[i] = p->p_analyzeData[i];

	////////
    printf("\n\n---------------< %s > %d\n", p->dev_name, p->fd);
	D_printRawData(p, PREVDATA);
	D_printRawData(p, CURRDATA);

	D_printUserData(p, CURRDATA);
}

void D_printRawData(listNode* N,int data_opt)
{
	int i = 0;
	if (data_opt == CURRDATA)
	{
		//printf("\n--------------------Current Raw Data!---------------------\n");
		printf("[Current Raw Data]  :");
		for (i = 0; i < N->dev_pacLen; i++)
			printf("%02X ", N->c_buf[i]);
	}
	else if (data_opt == PREVDATA)
	{
		//printf("\n--------------------Previous Raw Data!---------------------\n");
		printf("[Previous Raw Data] :");
		for (i = 0; i < N->dev_pacLen; i++)
			printf("%02X ", N->p_buf[i]);
	}
	printf("\n");
}

void D_printQueueData(listNode* N,int data_opt)
{
    int i=0,data=0;
    //printf("\n-----------------[[[[[[%s]]]]]]]-----------------\n",N->dev_name);
    printf("\n");
    for (i = 0; i < N->dev_datalen / 2; i++)
    {
        data = getQueuedata(N, data_opt);
        printf("%d-", data);
    }
    printf("\n");
}


void D_printUserData(listNode* N,int data_opt)
{
    int i=0;
    int data=0;

    if(!isEmptyQueuedata(N,data_opt))
        return;

    if(CURRDATA == data_opt)
        printf("\n[Curr Analyzed Data]:");
    else
        printf("\n[Prev Analyzed Data]:");

    if(N->dev_id[0] == HANBACK_WEATHER_FRONT &&
            N->dev_id[1] == HANBACK_WEATHER_REAR)
    {
//        printf("\n---------------< %s >\n",N->dev_name);
        for(i=0; i < N->dev_datalen/2; i++)
        {
            data = getQueuedata(N,data_opt);
            if(0 == i)
            {
                printf("\n[Temp]:%d℃ ",data/10);
            }
            if(1 == i)
            {
                printf("[Press]:%dmpbs",data/10);
            }
            if(2 == i)
            {
                printf("[Acc_X]:%d",data);
            }
            if(3 == i)
            {
                printf("[Acc_Y]:%d",data);
            }
            if(4 == i)
            {
                printf("[L_CH0]:%d",data);
            }
            if(5 == i)
            {
                printf("[L_CH1]:%d",data);
            }
            if(6 == i)
            {
                printf("[lux]:%d",data);
            }
            if(N->getErrordataWeather[i]) printf("\t[Filtering]:o\n");
            else printf("\t[Filtering]:x\n");

            N->getErrordataWeather[i] = 0;
        }
    }
    else if(N->dev_id[0] == HANBACK_THRMMTR_FRONT &&
            N->dev_id[1] == HANBACK_THRMMTR_REAR)
    {
        //printf("\n---------------[[[[[[%s]]]]]]]-----------------\n",N->dev_name);

        for(i=0; i < N->dev_datalen/2; i++)
        {
            data = getQueuedata(N,data_opt);
            if(0 == i)
            {
                printf(" Phsical Temp:");
                if(ERRORDATA == data) printf("####Filtered Data####");
                else		  printf("%d%d.%d%d℃ ",data/1000,
						   	   	   	   	   	   	   (data%1000)/100,
						   	   	   	   	   	   	   (data%100)/10,
						   	   	   	   	   	   	    data%10);
            }
            else if(1 == i)
			{
				printf(" Arround Temp:");
				if(ERRORDATA == data) printf("####Filtered Data####");
				else printf("%d%d.%d%d℃ ", data/1000,
						   	   	   	   	   (data%1000)/100,
						   	   	   	   	   (data%100)/10,
						   	   	   	   	    data%10);
            }
        }
    }
    else if(N->dev_id[0] == HANBACK_DIOXIDE_FRONT &&
            N->dev_id[1] == HANBACK_DIOXIDE_REAR)
    {
        //printf("\n---------------[[[[[[%s]]]]]]]-----------------\n",N->dev_name);

        for(i=0; i < N->dev_datalen/2; i++)
        {
            data = getQueuedata(N,data_opt);
            if(0 == i)
            {
                printf(" Dioxide Density: %d%d%d%dppm ", data/1000,
													(data%1000)/100,
													(data%100)/10,
													 data%10);
            }
        }
    }
    else if(N->dev_id[0] == HANBACK_DUST_FRONT &&////////////////////////////////////////dustalgo
            N->dev_id[1] == HANBACK_DUST_REAR)
    {
		for (i = 0; i < N->dev_datalen / 2; i++)
		{
			data = getQueuedata(N, data_opt);
			if (0 == i)
			{
				printf(" Dust Density: %d%d%d%d ppm ", data/1000,
													   (data%1000)/100,
													   (data%100)/10,
													   data % 10);
			}
		}
    }
    else if(N->dev_id[0] == HANBACK_VOC_FRONT &&////////////////////////////////////////dustalgo
            N->dev_id[1] == HANBACK_VOC_REAR)
    {
		for (i = 0; i < N->dev_datalen / 2; i++)
		{
			data = getQueuedata(N, data_opt);
			if (0 == i)
			{
				printf(" VOC Density: %d%d%d%dppb ", data / 1000,
												(data%1000)/100,
												(data%100)/10,
												data % 10);
			}
		}
    }
}

//DCURFILE//
void D_before_writeDataInfile()
{
	printf("##D_before_writeDataInfile##\n");

	printf("파일 이름:");
	scanf("%s",&writeData_Filename);
}

//DCURFILE//
void D_writeDataInfile(listNode* N)//1개의 센서만 돌아갈때 메인에서 사용 할 수 있다.
{
	printf("##D_writeDataInfile##\n");
	int i=0;
//

		int f_rear=0,f_front=N->q_front;
		printf("###f_front:%d , fileq_front:%d!!\n",f_front,N->fileq_front);

		FILE *f;
		char filepath[20];
		sprintf(filepath,"/sdcard/%s",writeData_Filename);

		f=fopen(filepath,"w");

		////버퍼가 20480 이라는 것을 가정하고 한 것이다////
		while(f_rear < f_front)
		{
			for(i=0;i<N->dev_datalen/2;i++){
				int data = N->fileQdata[f_rear++];
				///////////////Sensor 별로 표기를 다르게한다/////////////
				if(0 == strcmp(N->dev_name,"Weather")){
				if(0 == i){fprintf(f,"\n%d℃-",data/10);}
				if(1 == i){fprintf(f,"%dmpbs-",data/10);}
				if(2 == i){fprintf(f," [ACC_XY]:%d-",data);}
				if(3 == i){fprintf(f,"%d-",data);}
				if(4 == i){fprintf(f," [L_CH0,1]:%d",data);}
				if(5 == i){fprintf(f,"-%d",data);}
				if(6 == i){fprintf(f," [lux]:%d",data);}
				}
				else if(0 == strcmp(N->dev_name,"Dust"))
				{
					fprintf(f,"\n%d%d ppm",data / 100, data % 100);
				}
				else if(0 == strcmp(N->dev_name,"Thermometer"))
				{
					if(0 == i){fprintf(f,"\nPhysical Temp:"); fprintf(f,"%d.%d℃ \n", data/100,data%100);}
				    if(1 == i){fprintf(f,"Arround Temp:");fprintf(f,"%d.%d℃ \n", data/100,data%100);}
				}
				else if( 0 == strcmp(N->dev_name,"VOC"))// 나누기 10해서 넣기
				{
					if(0 == i){fprintf(f,"%d ppm\n",data);}
				}
				else if( 0 == strcmp(N->dev_name,"Dioxide"))
				{
					if(0 == i){fprintf(f,"%d ppm\n",data);}
				}
			}
			fprintf(f,"\n");
		}
		fclose(f);
	printf("\n############Write In File >> %s #################\n",N->dev_name);
}


// Quilification 출력의 예
void thermometer_print()
{
    //thermometer//
    printf("\n Current  Packet data\t: 76 00 18 04 4B A9 F1 B4 88 F0 0F");
    printf("\n Current  Analysis data\t: Physical Temp 28.4℃  / Around Temp 22.91℃");
    printf("\n Qualification\t\t: 28℃  (±3℃ )");
    printf("\n Filtering\t\t: [o]\n");
}
