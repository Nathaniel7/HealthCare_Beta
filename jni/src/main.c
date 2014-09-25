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
static int flag = 0;
JNIEXPORT void JNICALL Java_com_Nathaniel_healthcare_beta_AbstractionLib_Test(JNIEnv *env, jclass clazz)
{
    //    char buff[100] = " > TEST";
    //    for(; i != 0; i--)
    //        LOGI((buff));
}

JNIEXPORT void JNICALL Java_com_Nathaniel_healthcare_beta_AbstractionLib_TMconnectDevice(JNIEnv *env, jclass clazz)
{
    Node = createListNode_H();//장치의 링크드리스트를 만들기위해 헤드를 만듦

    ////////////////
    newDevice_cnt = 0; //새로운 장치 연결 개수를 잼 thread_F_makeDevthread 의 curDevice_cnt와 비교했을때 숫자가 같으면 새로운 장치가 연결 된것임
    deviceIndex = 0; //현재 장치 개수를 셈
    ////////////////

    //	extern TTD_Number;
    //	TTD_Number = 10;

    while (1) {
        //LOGI(" > Monitoring Start");
        if (M_connectDevice(Node)) {
            printf("M-connect!()\n");
//            printCutLine();

            setDevHead(Node);
            newDevice_cnt++;
        }
        sleep(1);
    }
}

JNIEXPORT void JNICALL Java_com_Nathaniel_healthcare_beta_AbstractionLib_TFmakeDevthread(JNIEnv *env, jclass clazz)
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
            printf("Sleep! readDataSuit!\n");
            //sleep(1);//장치정보에대한 Make Linked List 만들기전에 생성되는것을 방지
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
        sleep(1);
    }//end while
}

JNIEXPORT void JNICALL Java_com_Nathaniel_healthcare_beta_AbstractionLib_TSpressData(JNIEnv *env, jclass clazz)
{
    //linkedlist 만들기
    listNode_h* Node;
    listNode* p=NULL;
    int qData;//new
    int i=0;

    while(1)
    {
        //LOGI(" > Summarization Start");
        Node = getDevHead();
        if(Node != NULL)
            p = Node->head;

        while(p != NULL)
        {
//            __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", p->dev_name);

            if (p->getqueue_cnt > p->dev_datalen / 2
                    && p->dev_datalen / 2 != 0)
            {
                // printf("\n<<<<<%s'sQueuedata [%d(q-r) %d]>>>>>>>\n",p->dev_name ,
                //                       (p->q_front-p->q_rear)%MAX_QUEUE_SIZE,p->getqueue_cnt%MAX_QUEUE_SIZE);
                //               ////////////////new///////////
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

                ///////////////////Summaryzation Information/////////////////
                p->dev_abs.res_sensor          = p->dev_id[0];
                p->dev_abs.res_company         = p->dev_maker_id[0];
                p->dev_abs.res_sensor_datalen  = p->dev_datalen;

                for(i=0; i<p->dev_datalen/2; i++)
                    p->dev_abs.res_analyzeData[i] = p->c_analyzeData[i];//getQueuedata(p,PREVDATA);

                if(p->Circumstate)
                    p->dev_abs.res_state = STATE_GOOD;
                else
                    p->dev_abs.res_state = STATE_WORST;


//                printf("\n\n#####sensor:%d company:%d\n res_sensor_datalen:%d res_state: %d#####",
//                        tmpAbstNode.res_sensor, tmpAbstNode.res_company,
//                        tmpAbstNode.res_sensor_datalen, tmpAbstNode.res_state);
//
//                printf("\n<<<<recvdata:");
//                for(i=0; i<tmpAbstNode.res_sensor_datalen/2; i++)
//                    printf("%d-",tmpAbstNode.res_analyzeData[i]);
//                printf(">>>>>\n");

                /////////////////////////////////////////////////////////////
            }
            p = p ->next;
        }//end while

        sleep(1);
    }//end while
}

JNIEXPORT jintArray JNICALL Java_com_Nathaniel_healthcare_beta_AbstractionLib_getData(JNIEnv *env, jclass clazz)
{
    listNode_h* Node;
    listNode* p=NULL;
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
            fill[i+4+j] = p->dev_abs.res_analyzeData[j]; //getQueuedata(p,PREVDATA);

//        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%02X, %02X, %02X, %02X ", fill[i], fill[i+1], fill[i+2], fill[i+3]);
        if(fill[i+1] == HANBACK_DIOXIDE_FRONT)
                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "DIO / %02d, %02d, %02d, %02d ", fill[i+4], fill[i+5], fill[i+6], fill[i+7]);
        if(fill[i+1] == HANBACK_DUST_FRONT)
                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "DUS / %02d, %02d, %02d, %02d ", fill[i+4], fill[i+5], fill[i+6], fill[i+7]);
        if(fill[i+1] == HANBACK_THRMMTR_FRONT)
                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "THR / %02d, %02d, %02d, %02d ", fill[i+4], fill[i+5], fill[i+6], fill[i+7]);
        if(fill[i+1] == HANBACK_VOC_FRONT)
                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "VOC / %02d, %02d, %02d, %02d ", fill[i+4], fill[i+5], fill[i+6], fill[i+7]);


        p = p ->next;
    }

    (*env)->SetIntArrayRegion(env, result, 0, 1000, fill);
    return result;
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

