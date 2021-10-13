#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  "app_cfg.h"


// 宏
#define TASK_STK_SIZE 64
#define ENABLE_DATA_SHARE

// 变量
TCB_EXT_DATA tasks_data[3];
OS_STK TaskStks[3][TASK_STK_SIZE];

// lab的第二种情况：允许最高优先级和最低优先级进行数据共享，即任务1和任务3存在有数据关联
// 因此只要1个信号量即可保证两方的临界区都处于locked状态！使用的是0-1模式！===>stfw!(我是之前看《TCP/IP网络编程》了解的)
#ifdef ENABLE_DATA_SHARE
OS_EVENT *sem;					// 信号量，用来保持临界区数据一致
INT8U err;
#endif


// 函数声明
void Task1(void *args);
void Task2(void *args);
void Task3(void *args);



int  main(void)
{
#if OS_TASK_NAME_EN > 0u
	CPU_INT08U  os_err;
#endif


// 如果当前是允许数据共享，则需要创建信号量
// 因为只区分两个任务，故信号量初始值为1即可

    OSInit();   

#ifdef ENABLE_DATA_SHARE
	sem = OSSemCreate(1);		// OS_EVENT *OSSemCreate(INT16U cnt)
#endif

	tasks_data[0].cost = 1;
	tasks_data[0].period = 4;
	tasks_data[0].start = 0;
	tasks_data[0].compTime = 1;

	tasks_data[1].cost = 2;
	tasks_data[1].period = 5;
	tasks_data[1].start = 0;
	tasks_data[1].compTime = 2;

	tasks_data[2].cost = 2;
	tasks_data[2].period = 10;
	tasks_data[2].start = 0;
	tasks_data[2].compTime = 2;

	OSTaskCreateExt(
		Task1,
		NULL,
		&TaskStks[0][TASK_STK_SIZE - 1],
		1,
		1,
		&TaskStks[0][0],
		TASK_STK_SIZE,
		&tasks_data[0],
		0
		);
	OSTaskNameSet(1, "Task 1", &os_err);

	OSTaskCreateExt(
		Task2,
		NULL,
		&TaskStks[1][TASK_STK_SIZE - 1],
		2,
		2,
		&TaskStks[1][0],
		TASK_STK_SIZE,
		&tasks_data[1],
		0
	);
	OSTaskNameSet(2, "Task 2", &os_err);

	OSTaskCreateExt(
		Task3,
		NULL,
		&TaskStks[2][TASK_STK_SIZE - 1],
		3,
		3,
		&TaskStks[2][0],
		TASK_STK_SIZE,
		&tasks_data[2],
		0
	);
	OSTaskNameSet(3, "Task 3", &os_err);


    OSStart();                                               
}


void Task1(void *args)
{
	args = args;								// 避免警告
	int start;
	int end;
	int delay;

	while (DEF_ON) {
#ifdef ENABLE_DATA_SHARE
		OSSemPend(sem, 0, &err);
#endif
		start = OSTimeGet();
		while (OSTCBCur->compTime > 0) {}
		end = OSTimeGet();
		delay = OSTCBCur->period - (end - start);
		delay = delay >= 0 ? delay : 0;
		OSTCBCur->start += OSTCBCur->period;	// 任务下次开始的时间为本次开始的上次开始的时间 + 周期
		OSTCBCur->compTime = OSTCBCur->cost;
#ifdef ENABLE_DATA_SHARE
		OSSemPost(sem);
#endif
		OSTimeDly(delay);
	}
}
void Task2(void *args)
{
	args = args;								// 避免警告
	int start;
	int end;
	int delay;
	while (DEF_ON) {
		start = OSTimeGet();
		while (OSTCBCur->compTime > 0) {}
		end = OSTimeGet();
		delay = OSTCBCur->period - (end - start);
		delay = delay >= 0 ? delay : 0;
		OSTCBCur->start += OSTCBCur->period;	// 任务下次开始的时间为本次开始的上次开始的时间 + 周期
		OSTCBCur->compTime = OSTCBCur->cost;
		OSTimeDly(delay);
	}
}
void Task3(void *args)
{
	args = args;								// 避免警告
	int start;
	int end;
	int delay;
	while (DEF_ON) {
#ifdef ENABLE_DATA_SHARE
		OSSemPend(sem, 0, &err);
#endif
		start = OSTimeGet();
		while (OSTCBCur->compTime > 0) {}
		end = OSTimeGet();
		delay = OSTCBCur->period - (end - start);
		delay = delay >= 0 ? delay : 0;
		OSTCBCur->start += OSTCBCur->period;	// 任务下次开始的时间为本次开始的上次开始的时间 + 周期
		OSTCBCur->compTime = OSTCBCur->cost;
#ifdef ENABLE_DATA_SHARE
		OSSemPost(sem);
#endif
		OSTimeDly(delay);
	}
}
