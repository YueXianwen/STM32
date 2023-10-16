#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "delay.h"
#include "exti.h"
#include "timer.h"

//任务优先级
#define START_TASK_PRIO     1
#define TASK1_PRIO          2
#define TASK2_PRIO          3

//任务堆栈大小
#define START_STK_SIZE      128
#define TASK1_STK_SIZE      128
#define TASK2_STK_SIZE      128

//任务句柄
TaskHandle_t AppTaskCreate_Handle;
TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;

//定义测试列表
List_t TestList;
ListItem_t ListItem1;       //列表项1
ListItem_t ListItem2;       //列表项2
ListItem_t ListItem3;       //列表项3

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
    //初始化列表、列表项
    vListInitialise(&TestList);
    vListInitialiseItem(&ListItem1);
    vListInitialiseItem(&ListItem2);
    vListInitialiseItem(&ListItem3);
    ListItem1.xItemValue = 40;
    ListItem2.xItemValue = 60;
    ListItem3.xItemValue = 50;

    /* 第二步：打印列表和其他列表项的地址 */
    printf("/**************第二步：打印列表和列表项的地址**************/\r\n");
    printf("项目\t\t\t地址\r\n");
    printf("TestList\t\t0x%p\t\r\n", &TestList);
    printf("TestList->pxIndex\t0x%p\t\r\n", TestList.pxIndex);
    printf("TestList->xListEnd\t0x%p\t\r\n", (&TestList.xListEnd));
    printf("ListItem1\t\t0x%p\t\r\n", &ListItem1);
    printf("ListItem2\t\t0x%p\t\r\n", &ListItem2);
    printf("ListItem3\t\t0x%p\t\r\n", &ListItem3);
    printf("/**************************结束***************************/\r\n");

    printf("\r\n/*****************第三步：列表项1插入列表******************/\r\n");
    vListInsert((List_t*    )&TestList,         /* 列表 */
                (ListItem_t*)&ListItem1);       /* 列表项 */
    printf("项目\t\t\t\t地址\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("/**************************结束***************************/\r\n");

    /* 第四步：列表项2插入列表 */
    printf("\r\n/*****************第四步：列表项2插入列表******************/\r\n");
    vListInsert((List_t*    )&TestList,         /* 列表 */
                (ListItem_t*)&ListItem2);       /* 列表项 */
    printf("项目\t\t\t\t地址\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
    printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
    printf("/**************************结束***************************/\r\n");

    /* 第五步：列表项3插入列表 */
    printf("\r\n/*****************第五步：列表项3插入列表******************/\r\n");
    vListInsert((List_t*    )&TestList,         /* 列表 */
                (ListItem_t*)&ListItem3);       /* 列表项 */
    printf("项目\t\t\t\t地址\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
    printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
    printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
    printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
    printf("/**************************结束***************************/\r\n");

    /* 第六步：移除列表项2 */
    printf("\r\n/*******************第六步：移除列表项2********************/\r\n");
    uxListRemove((ListItem_t*   )&ListItem2);   /* 移除列表项 */
    printf("项目\t\t\t\t地址\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
    printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
    printf("/**************************结束***************************/\r\n");

    /* 第七步：列表末尾添加列表项2 */
    printf("\r\n/****************第七步：列表末尾添加列表项2****************/\r\n");
    TestList.pxIndex = &ListItem1;
    vListInsertEnd((List_t*     )&TestList,     /* 列表 */
                   (ListItem_t* )&ListItem2);   /* 列表项 */
    printf("项目\t\t\t\t地址\r\n");
    printf("TestList->pxIndex\t\t0x%p\r\n", TestList.pxIndex);
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
    printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
    printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
    printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
    printf("/************************实验结束***************************/\r\n");
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
    printf("AppTaskCreate任务创建中......\r\n");
    //创建任务1
    xTaskCreate((TaskFunction_t )task1,
                (const char*    )"task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t *) &Task1_Handle);
    if (Task1_Handle)
        printf("Task1任务创建成功！\r\n");

    //创建任务2
    xTaskCreate((TaskFunction_t )task2,
                (const char*    )"task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t *) &Task2_Handle);
    if (Task2_Handle)
        printf("Task2任务创建成功！\r\n");

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

    printf("正在启动.....\r\n");
    xTaskCreate((TaskFunction_t ) AppTaskCreate,
                (const char*    )"AppTaskCreate",
                (uint16_t       )START_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t*   )AppTaskCreate_Handle);
    vTaskStartScheduler();
}
