#include "main.h"

#define HANBACK_DIOXIDE_FRONT   0x13
#define HANBACK_DUST_FRONT      0x15
#define HANBACK_VOC_FRONT       0x17
#define HANBACK_THRMMTR_FRONT   0x18
#define HANBACK_WEATHER_FRONT   0x12

/* ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
   ■■■■■■■■ Use Debugging Logcat argument Instead of LOGI ■■■■■■■■
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "", );

    ex) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%d", num);
   ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

JNIEXPORT jint JNICALL Java_com_Nathaniel_healthcare_beta_AbstractionLib_Test(JNIEnv *env, jclass clazz)
{
    //    char buff[100] = " > TEST";
    //    for(; i != 0; i--)
    //        LOGI((buff));
	return 0;
}

JNIEXPORT jint JNICALL Java_com_Nathaniel_healthcare_beta_AbstractionLib_TMconnectDevice(JNIEnv *env, jclass clazz)
{
    Node = createListNode_H();//장치의 링크드리스트를 만들기위해 헤드를 만듦

    ////////////////
    newDevice_cnt = 0; //새로운 장치 연결 개수를 잼 thread_F_makeDevthread 의 curDevice_cnt와 비교했을때 숫자가 같으면 새로운 장치가 연결 된것임
    deviceIndex = 0; //현재 장치 개수를 셈
    ////////////////

    //	extern TTD_Number;
    //	TTD_Number = 10;

//    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, " > Monitor Start");

    while (1) {
        //LOGI(" > Monitoring Start");
//        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, " > Monitor0");
        if (M_connectDevice(Node)) {
//            printf("M-connect!()\n");
//            printCutLine();

            setDevHead(Node);
            newDevice_cnt++;
        }
        usleep(1000 * 100);
    }

    return 0;
}

JNIEXPORT jint JNICALL Java_com_Nathaniel_healthcare_beta_AbstractionLib_TFmakeDevthread(JNIEnv *env, jclass clazz)
{
    int curDevice_cnt = 1;
    int thr_id;
    pthread_t p_thread[5];

    //	extern TTD_Number;

    listNode_h* tmpNode;
    listNode* p;

    //	for(;TTD_Number != 0; TTD_Number--)
    //	    LOGI("   > TTD Number Filtering");

    while(1)
    {
        //LOGI(" > Filtering Start");
        tmpNode  = getDevHead();
        if(tmpNode != NULL)
            p = tmpNode->head;

        if(p == NULL)
        {
//            printf("Sleep! readDataSuit!\n");
//            sleep(1);//장치정보에대한 Make Linked List 만들기전에 생성되는것을 방지
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
        usleep(1000 * 100);
    }//end while

    return 0;
}

JNIEXPORT jint JNICALL Java_com_Nathaniel_healthcare_beta_AbstractionLib_TSpressData(JNIEnv *env, jclass clazz)
{
    //linkedlist 만들기
    listNode_h* Node;
    listNode* p = NULL;
    int i = 0;
    int data[2] = {0};//new

//    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, " > Summary Start");
    while(1)
    {
        //LOGI(" > Summarization Start");
        Node = getDevHead();
        if(Node != NULL)
            p = Node->head;

        while(p != NULL)
        {
            if (p->f_getqueue_cnt > p->dev_datalen / 2 && p->dev_datalen / 2 != 0)
            {
//                for(i = 0; i < p->dev_datalen/2; i++)
            	for(i = 0; i < 1; i++)
                {
                    //#WARNING# FILTER_OPT_FILTER일경우 1만 반환 FILTER_OPTNONFILTERED라면 에러데이터는 0반환
                    // error Data는 -1로 표기되므로 그를 고려하여서 폐기하여야 한다.
                    if(data[i] = S_SummaryHanbackSensor(p, i))
                    {
                        p->summarizedData[i] = data[i]; //current analyzed data를 저장함
                        p->dev_abs.res_summarizedData[i] = p->summarizedData[i];
                        p->D_data.s_Data[i] = data[i];
                    }
                    else
                        p->summarizedData[i] = -1;

                	S_SummarySensorStatus(p, data[i]);
                }
            }
            p = p->next;
        }//end while

        usleep(1000 * 100);
    }//end while

    return 0;
}

JNIEXPORT jintArray JNICALL Java_com_Nathaniel_healthcare_beta_AbstractionLib_getData(JNIEnv *env, jclass clazz)
{
    listNode_h* Node;
    listNode* p = NULL;
    int qData;//new
    int i = 0;
    int j = 0;

    jint fill[1000] = { 0 };
    jintArray result = (*env)->NewIntArray(env, 1000);

    Node = getDevHead();
    if(Node != NULL)
        p = Node->head;

    for(i = 0; p != NULL; i += 100)
    {
        fill[i] = p->dev_abs.res_company;
        fill[i+1] = p->dev_abs.res_sensor;
        fill[i+2] = p->dev_abs.res_sensor_datalen;
        fill[i+3] = p->dev_abs.res_state;

        for(j = 0; j < p->dev_datalen/2; j++)
            fill[i+4+j] = p->dev_abs.res_summarizedData[j]; //getQueuedata(p,PREVDATA);

        p = p ->next;
    }

    (*env)->SetIntArrayRegion(env, result, 0, 1000, fill);
    return result;
}

void* thread_F_readData(void* data) {
    listNode* p = (listNode*)data;
//    printf("****StartReadData(F_readData2()) << dev : %s >> \n",p->dev_name);
    while (1)
    {
        while (p->dev_monitor_status)
            F_readData(p);
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

void printCutLine()
{
    printf("\n--------------------------------------------------------\n");
}
