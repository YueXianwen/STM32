#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "delay.h"
#include "exti.h"
#include "timer.h"

//�������ȼ�
#define START_TASK_PRIO     1
#define TASK1_PRIO          2
#define TASK2_PRIO          3

//�����ջ��С
#define START_STK_SIZE      128
#define TASK1_STK_SIZE      128
#define TASK2_STK_SIZE      128

//������
TaskHandle_t AppTaskCreate_Handle;
TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;

//��������б�
List_t TestList;
ListItem_t ListItem1;       //�б���1
ListItem_t ListItem2;       //�б���2
ListItem_t ListItem3;       //�б���3

void task1(void *pvParameters)
{
    u32 task1_num = 0;
    while(1)
    {
        task1_num++;
        LED0=!LED0;
        printf("Task1 Running: %lu!!\r\n", task1_num);
        delay_ms(1000);
    }
}

void task2(void *pvParameters)
{
    //��ʼ���б��б���
    vListInitialise(&TestList);
    vListInitialiseItem(&ListItem1);
    vListInitialiseItem(&ListItem2);
    vListInitialiseItem(&ListItem3);
    ListItem1.xItemValue = 40;
    ListItem2.xItemValue = 60;
    ListItem3.xItemValue = 50;

    /* �ڶ�������ӡ�б�������б���ĵ�ַ */
    printf("/**************�ڶ�������ӡ�б���б���ĵ�ַ**************/\r\n");
    printf("��Ŀ\t\t\t��ַ\r\n");
    printf("TestList\t\t0x%p\t\r\n", &TestList);
    printf("TestList->pxIndex\t0x%p\t\r\n", TestList.pxIndex);
    printf("TestList->xListEnd\t0x%p\t\r\n", (&TestList.xListEnd));
    printf("ListItem1\t\t0x%p\t\r\n", &ListItem1);
    printf("ListItem2\t\t0x%p\t\r\n", &ListItem2);
    printf("ListItem3\t\t0x%p\t\r\n", &ListItem3);
    printf("/**************************����***************************/\r\n");

    printf("\r\n/*****************���������б���1�����б�******************/\r\n");
    vListInsert((List_t*    )&TestList,         /* �б� */
                (ListItem_t*)&ListItem1);       /* �б��� */
    printf("��Ŀ\t\t\t\t��ַ\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("/**************************����***************************/\r\n");

    /* ���Ĳ����б���2�����б� */
    printf("\r\n/*****************���Ĳ����б���2�����б�******************/\r\n");
    vListInsert((List_t*    )&TestList,         /* �б� */
                (ListItem_t*)&ListItem2);       /* �б��� */
    printf("��Ŀ\t\t\t\t��ַ\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
    printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
    printf("/**************************����***************************/\r\n");

    /* ���岽���б���3�����б� */
    printf("\r\n/*****************���岽���б���3�����б�******************/\r\n");
    vListInsert((List_t*    )&TestList,         /* �б� */
                (ListItem_t*)&ListItem3);       /* �б��� */
    printf("��Ŀ\t\t\t\t��ַ\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
    printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
    printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
    printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
    printf("/**************************����***************************/\r\n");

    /* ���������Ƴ��б���2 */
    printf("\r\n/*******************���������Ƴ��б���2********************/\r\n");
    uxListRemove((ListItem_t*   )&ListItem2);   /* �Ƴ��б��� */
    printf("��Ŀ\t\t\t\t��ַ\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
    printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
    printf("/**************************����***************************/\r\n");

    /* ���߲����б�ĩβ����б���2 */
    printf("\r\n/****************���߲����б�ĩβ����б���2****************/\r\n");
    TestList.pxIndex = &ListItem1;
    vListInsertEnd((List_t*     )&TestList,     /* �б� */
                   (ListItem_t* )&ListItem2);   /* �б��� */
    printf("��Ŀ\t\t\t\t��ַ\r\n");
    printf("TestList->pxIndex\t\t0x%p\r\n", TestList.pxIndex);
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
    printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
    printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
    printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
    printf("/************************ʵ�����***************************/\r\n");
    while(1)
    {
        LED1=!LED1;
        printf("Task2 Running!!\r\n");
        delay_ms(1000);
    }
}

void AppTaskCreate(void *pvParameters)
{
    taskENTER_CRITICAL();
    printf("AppTaskCreate���񴴽���......\r\n");
    //��������1
    xTaskCreate((TaskFunction_t )task1,
                (const char*    )"task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t *) &Task1_Handle);
    if (Task1_Handle)
        printf("Task1���񴴽��ɹ���\r\n");

    //��������2
    xTaskCreate((TaskFunction_t )task2,
                (const char*    )"task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t *) &Task2_Handle);
    if (Task2_Handle)
        printf("Task2���񴴽��ɹ���\r\n");

    vTaskDelete(AppTaskCreate_Handle);
    taskEXIT_CRITICAL();
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    uart_init(115200);
    PrintfInit(USART1);
    led_init();
    delay_init();

//    TIM2_Int_Init(9999,7199);
//    TIM3_Int_Init(9999,7199);

    printf("��������.....\r\n");
    xTaskCreate((TaskFunction_t ) AppTaskCreate,
                (const char*    )"AppTaskCreate",
                (uint16_t       )START_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t*   )AppTaskCreate_Handle);
    vTaskStartScheduler();
}
