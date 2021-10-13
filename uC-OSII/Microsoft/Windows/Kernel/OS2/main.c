#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  "app_cfg.h"


// ��
#define TASK_STK_SIZE 64
#define ENABLE_DATA_SHARE

// ����
TCB_EXT_DATA tasks_data[3];
OS_STK TaskStks[3][TASK_STK_SIZE];

// lab�ĵڶ������������������ȼ���������ȼ��������ݹ���������1������3���������ݹ���
// ���ֻҪ1���ź������ɱ�֤�������ٽ���������locked״̬��ʹ�õ���0-1ģʽ��===>stfw!(����֮ǰ����TCP/IP�����̡��˽��)
#ifdef ENABLE_DATA_SHARE
OS_EVENT *sem;					// �ź��������������ٽ�������һ��
INT8U err;
#endif


// ��������
void Task1(void *args);
void Task2(void *args);
void Task3(void *args);



int  main(void)
{
#if OS_TASK_NAME_EN > 0u
	CPU_INT08U  os_err;
#endif


// �����ǰ���������ݹ�������Ҫ�����ź���
// ��Ϊֻ�����������񣬹��ź�����ʼֵΪ1����

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
	args = args;								// ���⾯��
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
		OSTCBCur->start += OSTCBCur->period;	// �����´ο�ʼ��ʱ��Ϊ���ο�ʼ���ϴο�ʼ��ʱ�� + ����
		OSTCBCur->compTime = OSTCBCur->cost;
#ifdef ENABLE_DATA_SHARE
		OSSemPost(sem);
#endif
		OSTimeDly(delay);
	}
}
void Task2(void *args)
{
	args = args;								// ���⾯��
	int start;
	int end;
	int delay;
	while (DEF_ON) {
		start = OSTimeGet();
		while (OSTCBCur->compTime > 0) {}
		end = OSTimeGet();
		delay = OSTCBCur->period - (end - start);
		delay = delay >= 0 ? delay : 0;
		OSTCBCur->start += OSTCBCur->period;	// �����´ο�ʼ��ʱ��Ϊ���ο�ʼ���ϴο�ʼ��ʱ�� + ����
		OSTCBCur->compTime = OSTCBCur->cost;
		OSTimeDly(delay);
	}
}
void Task3(void *args)
{
	args = args;								// ���⾯��
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
		OSTCBCur->start += OSTCBCur->period;	// �����´ο�ʼ��ʱ��Ϊ���ο�ʼ���ϴο�ʼ��ʱ�� + ����
		OSTCBCur->compTime = OSTCBCur->cost;
#ifdef ENABLE_DATA_SHARE
		OSSemPost(sem);
#endif
		OSTimeDly(delay);
	}
}
