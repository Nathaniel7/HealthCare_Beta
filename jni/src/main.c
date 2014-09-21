#include "main.h"

//int main()
//{
//    pthread_t p_thread[10];
//    int thr_id;
//    int status;
//    //Node = createListNode_H();//장치의 링크드리스트를 만들기위해 헤드를 만듦
//
//    listNode* p;
//    printCutLine();     // ---------------------------------
//
//    ////////thread 1--M_connectDevice    Monitoring //////
//    thr_id = pthread_create(&p_thread[0],NULL,thread_M_connectDevice,(void*)Node);
//    if(thr_id < 0){ perror("Thread_0 create error"); exit(0); }
//
//    //////////thread 2 makeDevthread/////////////
//    //##WARNING## 부모 thread가 자식 스레드의 thread_M_connectDevice를 실행하기전 실행시키면p가 NULL 일 수 있다.
//    thr_id = pthread_create(&p_thread[1],NULL,thread_F_makeDevthread,NULL);//장치별로 thread를 만들어주는 thread함수이다.
//    if(thr_id < 0){ perror("Thread_1 create error"); exit(0);}
//    ///////////////////////////////////////////
//
//    //////////thread 3 Summarization//////////
//    thr_id = pthread_create(&p_thread[2],NULL,thread_S_pressData,NULL);//##WARNING## getQueuedata를 사용하는 함수이다.
//    if(thr_id < 0){ perror("Thread_2 create error"); exit(0); }
//    //////////////////////////////////////////
//
//    //wait thread///
//    pthread_join(p_thread[0],(void **)&status);
//    pthread_join(p_thread[1],(void **)&status);
//    pthread_join(p_thread[2],(void **)&status);
//
//
//    return 0;
//}


JNIEXPORT void JNICALL Java_com_Nathaniel_healthcare_beta_AbstractionLib_Test(JNIEnv *env, jclass clazz)
{
    int i = 0;
    for(i = 0; i < 10; i++)
        LOGI(" > TEST");
}

JNIEXPORT void JNICALL Java_com_Nathaniel_healthcare_beta_AbstractionLib_TMconnectDevice(JNIEnv *env, jclass clazz)
{
    LOGI(" > Monitoring Start");

	Node = createListNode_H();//장치의 링크드리스트를 만들기위해 헤드를 만듦

	////////////////
	newDevice_cnt = 0; //새로운 장치 연결 개수를 잼 thread_F_makeDevthread 의 curDevice_cnt와 비교했을때 숫자가 같으면 새로운 장치가 연결 된것임
	deviceIndex = 0; //현재 장치 개수를 셈
	////////////////

	extern TTD_Number;
	TTD_Number = 10;
	
	while (1) {
		if (M_connectDevice(Node)) {

			printf("M-connect!()\n");
			printCutLine();

			setDevHead(Node);
			newDevice_cnt++;
		}
		sleep(3);
	}
}

JNIEXPORT void JNICALL Java_com_Nathaniel_healthcare_beta_AbstractionLib_TFmakeDevthread(JNIEnv *env, jclass clazz)
{
    LOGI(" > Filtering Start");

	int curDevice_cnt=1;
	int thr_id;
	pthread_t p_thread[5];

	extern TTD_Number;

	listNode_h* tmpNode;
	listNode* p;

	for(;TTD_Number != 0; TTD_Number--)
	    LOGI("   > TTD Number Filtering");

    while(1)
    {
    	tmpNode  = getDevHead();
    	if(tmpNode != NULL)
    	p = tmpNode->head;

    	if(p == NULL)
    	{
    		printf("Sleep! readDataSuit!\n");
    		sleep(3);//장치정보에대한 Make Linked List 만들기전에 생성되는것을 방지
    	}
    	else
    	{
    		// newDevice_cnt 는 0부터 시작한다. 즉 새로운 장치가 연결되면 1==1 이어서 아래 구문을 통과한다.
			if (curDevice_cnt == newDevice_cnt)
			{
				if (newDevice_cnt > 1)
				while(p->next != NULL ){ p = p->next;}

				//printf("#############################create F_readData thread! %s\n",p->dev_name);
				thr_id = pthread_create(&p_thread[deviceIndex++], NULL,thread_F_readData, (void*) p);

				if (thr_id < 0)
				{
					perror("Thread_2~4 create error");
					exit(0);
				}
				curDevice_cnt++;
			}
		}
    }//end while
}

JNIEXPORT void JNICALL Java_com_Nathaniel_healthcare_beta_AbstractionLib_TSpressData(JNIEnv *env, jclass clazz)
{
    LOGI(" > Summarization Start");

	//linkedlist 만들기
	listNode_h* Node;
	listNode* p=NULL;
	int qData;//new
	int i=0;

	while(1)
	{
		Node = getDevHead();
		if(Node != NULL)
			p = Node->head;

		while(p != NULL)
		{
			if (p->getqueue_cnt > p->dev_datalen / 2
					&& p->dev_datalen / 2 != 0)
			{
//				printf("\n<<<<<%s'sQueuedata [%d(q-r) %d]>>>>>>>\n",p->dev_name ,
//						(p->q_front-p->q_rear)%MAX_QUEUE_SIZE,p->getqueue_cnt%MAX_QUEUE_SIZE);
//				////////////////new///////////
				//##WARNING##
				if (0 == strcmp(p->dev_name, "Thermometer")) //D_getQueuedata함수가 있습니다.D_getQueuedata사용시 데이터가사리지므로 주의해야함.
				D_HBACK_Thermometer(p);
				else if (0 == strcmp(p->dev_name, "Weather"))
				D_HBACK_Weather(p);
				else if (0 == strcmp(p->dev_name, "Dioxide"))
				D_HBACK_Dioxide(p);
				else if( 0 == strcmp(p->dev_name, "Dust"))
				D_HBACK_Dust(p);
				else if( 0 == strcmp(p->dev_name, "VOC"))
				D_HBACK_VOC(p);
			}
			p = p ->next;
		}//end while


	}//end while
}

void* thread_F_readData(void* data) {
    listNode* p = (listNode*)data;
    printf("****StartReadData(F_readData2()) << dev : %s >> \n",p->dev_name);
    while (1)
    {
        while (p->dev_monitor_status)
        {
            F_readData(p); //모든 LinkedList Node의 데이터를 다 받는다.
//          if (0 == strcmp(p->dev_name, "Thermometer"))//D_getQueuedata함수가 있습니다.D_getQueuedata사용시 데이터가사리지므로 주의해야함.
//              D_HBACK_Thermometer(p);
//          else if (0 == strcmp(p->dev_name, "Weather"))
//              D_HBACK_Weather(p);
//          else if (0 == strcmp(p->dev_name, "Dioxide"))
//              D_HBACK_Dioxide(p);
            ////////////////////////////////////
        } //end while
    }
}

void setDevHead(listNode_h* updatedHead)
{
    Node = updatedHead;
}

listNode_h* getDevHead()
{
    return Node;
}

void getElapsedTime(struct timeval Tstart, struct timeval Tend) {
    Tend.tv_usec = Tend.tv_usec - Tstart.tv_usec;
    Tend.tv_sec  = Tend.tv_sec - Tstart.tv_sec;
    Tend.tv_usec += (Tend.tv_sec*1000000);

    printf("Elapsed Time: %lf sec\n", Tend.tv_usec / 1000000.0);
}

void printHex(const char *buff, int readSize)
{
    int i;
    for(i = 0; i < readSize; i++) {
        printf("%02X ", buff[i]);
    }

//    printf("\n");
}

void printCutLine()
{
    printf("\n--------------------------------------------------------\n");
}

//TEST//
void test()
{
	listNode_h* tmpHead = getDevHead();
	listNode* p = tmpHead->head;

	while(p != NULL)
	{
		printf("%s-",p->dev_name);
		p = p->next;
	}
	printf("\n");
	sleep(1);
}
//END TEST//

