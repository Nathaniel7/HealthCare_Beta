#include "defines.h"

////////장치의 상태를 나타낸다 ////////////////
enum sensor_status
{
    STATE_WORST,
    STATE_GOOD,
    STATE_WAIT
};

//////센서가 출저회사를 나타낸다 ////////////////
#define COMPANY_HANBACK             0x76

///////센서의 종류를 나타낸다///////
#define HANBACK_DIOXIDE_FRONT       0x13
#define HANBACK_MONOXIDE_FRONT      0x14
#define HANBACK_DUST_FRONT          0x15
#define HANBACK_VOC_FRONT           0x17
#define HANBACK_THRMMTR_FRONT       0x18
#define HANBACK_WEATHER_FRONT       0x12

typedef struct Abstract
{
    int res_sensor;
    int res_company;
    int res_sensor_datalen;
    int res_state;
    int res_analyzeData[10];
} AbstNode;

typedef struct ListNode
{
    int fd;
    int dev_pacLen; // device total packet Length
    int dev_datalen;//전체패킷중 데이터에 해당되는 길이
    int dev_filter_opt;
    int dev_monitor_status;

    char dev_path[MAX_DEV_PATH_SIZE];
    char dev_maker[MAX_DEV_NAME_SIZE];
    char dev_name[MAX_DEV_NAME_SIZE];
    char dev_id[2];
    char dev_maker_id[2];
    char dev_end_bit[2];

    AbstNode dev_abs;

    ///////////////Read data////////////////////
    unsigned char q_data[5][MAX_BUFF_SIZE];//DCUR//q_data대신 다른 것으로 바꾸어야함.

    ///////Analyzed data Queue////////////
    int q_rdata[MAX_QUEUE_SIZE];//analyzed data save q_rdata(current data)
    int q_front;//삽입 queue index
    int q_rear;//삭제 queue index
    ////
    int q_prdata[MAX_QUEUE_SIZE];//previous data
    int q_pfront;
    int q_prear;

    ////////Raw data/////////////
    unsigned char c_buf[MAX_BUFF_SIZE];
    unsigned char p_buf[MAX_BUFF_SIZE];

    ////////Analyzed data////////
    int c_analyzedData[MAX_BUFF_SIZE];
    int p_analyzedData[MAX_BUFF_SIZE];
    int a_front;
    int a_rear;
    int a_flag;
    int a_cnt;

    int c_filteredData[MAX_BUFF_SIZE];
    int p_filteredData[MAX_BUFF_SIZE];
    int f_cfront;
    int f_crear;
    int f_pfront;
    int f_prear;
    int f_getqueue_cnt;
    int f_read_cnt;

    int summarizedData[MAX_BUFF_SIZE];

//    ///////////filtering 시에 사용되는 값들////////////
    /*Debug Using*/ int avg_analyzeData[MAX_BUFF_SIZE];//DCUR//
    /*Debug Using*/ int total_analyzeData[MAX_BUFF_SIZE];//DCUR//
    int read_cnt;//총 read한 횟수를 셉니다.

    int getErrordataThermometer[2];//DCUR//
    int getErrordataWeather[6];//DCUR//
    int getqueue_cnt;//queue에서 꺼낸 총 횟수를 말합니다.

    int circumstance;         // status value
    //////////////DCURFILE입력을위함//////////////
    int fileQdata[MAX_QUEUE_SIZE];//##WARNING### 파일입출력시 데이터크기가 20을 넘어도 에러가 나지않음
                        //컴파일러에서 잡아주는 것 같다.
    int fileq_front;

    //////////filtering 시 고정값 찾는것//////////
    int getCircumstanceMode;

    /////////지수평균필터 && 이동평균필터////////

    //struct ListNode *prev;
    struct ListNode *next;
} listNode;

typedef struct ListNode_h
{
    struct ListNode *head;
    struct ListNode *tail;
} listNode_h;

#define CURRDATA  0
#define PREVDATA  1
#define PRINTDATA 2
//////////여기에 있어야 linkedlist.h가 여러군데에 include되어서 다른 헤더나 .c에 사용 될 수 있음////////////
#define FILTER_OPT_BACKDATA     3
#define FILTER_OPT_DEPLACEMENT  4
#define FILTER_OPT_AVERAGE      5
#define FILTER_OPT_FILTER		6
#define FILTER_OPT_NONFILTERD	7

listNode_h *createListNode_H(); //contain tail
void initNode(listNode*, const char*);
void insertLastNode(listNode_h *, const char *);  //node add Head and Tail
void deleteNode(listNode_h *, char *);      //dev not exist
void displayNode(listNode_h *);
void freeAllNode(listNode_h *);

void setFilterQdata(listNode *, int, int); // Filter Data set
int getFilterQdata(listNode *, int);

void setQueuedata(listNode* N, int data, int data_opt);//analyzed data를 큐에다 저장합니다
int getQueuedata(listNode* N, int data_opt);
//int* getQueuedata(listNode*,int);//analyzed data를 큐에서 가져온 후 삭제합니다

int isEmptyQueuedata(listNode*, int data_opt);

void strncat_s(unsigned char *target, unsigned char *buff, int target_size, int buff_size);
//use strncat_s(rdata,N->c_buf,0[start string],N->dev_pacLen[total string]);
