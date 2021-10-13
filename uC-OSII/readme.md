# Ƕ��ʽʵ��---uC/OS-II������

191250044 ��ΰ

[ʵ��Դ����](https://github.com/hewei-nju/Multi-Task-Lab)

## 1. ʵ��Ŀ��

��uC/OS-II�Ͻ��ж��������ʵ�飬���MoodlePDF��

## 2. ʵ������

��uC/OS-II��VS��ֲ�汾�Ͻ�����������ĵ��ȣ������������

* ��������֮��û������ԣ�
* ������ȼ���������ȼ�����������ݹ���

## 3. ʵ��׼��

### 1. ��ν������������ķ������⣿

* ͨ������ѭ�� + �������������������������ִ�У���ʾ���£�

```c
void TaskN(void *args)
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
```

### 2. ��ν������֮��������ݹ������⣿

* ͨ��uC/OS-II�ṩ���ź�����ʵ������ִ�й����У��ٽ��������ݷ���Ψһ�ԡ�
* ����C/C++��������һ��ʽ��ʵ��ʵ����Ҫ��������������ʾ���£�

```c
void TaskN(void *args)
{
    ...
	while (DEF_ON) {
#ifdef ENABLE_DATA_SHARE			// ���ݸú��Ƿ񱻶�����ѡ���Ƿ��������ݹ���
		OSSemPend(sem, 0, &err);
#endif
		...
#ifdef ENABLE_DATA_SHARE
		OSSemPost(sem);
#endif
		OSTimeDly(delay);
	}
}
```

### 3. ���ú��ֵ����㷨��

* ����ʵ��Ҫ��**����RMS�����㷨**����������ȼ��̶�����ʵ���б���Ϊ������������ڳ��̣��ڴ�������ʱ���ֶ�����������ȼ���

ʵ��Ҫ������Ϊ��

```
Task Set {t1(1, 4), t2(2, 5), t3(2, 10)}	// ����ʱ��	����
```

�����ȼ�t1 > t2 > t3���ڴ���������Ϊ����prio1 < prio2 < prio3��

����Ϊ����Ϊ�˷������������ִ�У����ڻ���Ҫͨ����չOS_TCB����˲���OSTaskCreateExt�����������������Բ������������ȼ� = ����š�

t1, t2, t3�ֱ�Ϊ1��2��3.

## 3. ʵ�����

**˵��**��ʵ���е����޸ĵĵط��Ҿ������ע��`// hewei`��ͨ����Դ�ļ�������`hewei`�ܿ��ٶ�λ�������/�޸ĵĵط���

### 1. �����������������Ҫ�󣬶�OS_TCB�ṹ�����ݽ�����չ

**�������**����ucos_ii.h�еĽṹ��struct os_tcb������������ݣ�

```c
typedef struct os_tcb {
	// hewei
	INT32U start;		// ����ִ�еĿ�ʼʱ��
	INT32U cost;		// ����ִ������Ҫ��ʱ��
	INT32U period;		// ����ִ�е�����
	INT32U compTime;	// ִ���걾����������Ҫ��ʱ��

	....				// ԭ�е�����
} OS_TCB;
```
**Ϊ���ܹ���os_core.c�е�OS_TCBInit�ж�OS_TCB��չ���ݽ��г�ʼ�����Լ�����OSTaskCreateExt�е�pextָ���������ucos_ii.h��������½ṹ������**��

```c
// hewei
typedef struct tcb_ext_data {
	INT32U start;		// ����ִ�еĿ�ʼʱ��
	INT32U cost;		// ����ִ������Ҫ��ʱ��
	INT32U period;		// ����ִ�е�����
	INT32U compTime;	// ִ���걾����������Ҫ��ʱ��
} TCB_EXT_DATA;
```
### 2. ����Ĵ���������OSTaskCreateExt�������У���ʹ��pextָ���OS_TCB������չ

```c
	// task_dataΪTCB_EXT_DATA�Ľṹ�����飬Ŀ���ǳ�ʼ��OS_TCB����չ����
	tasks_data[0].cost = 1;
	tasks_data[0].period = 4;
	tasks_data[0].start = 0;
	tasks_data[0].compTime = 1;

	OSTaskCreateExt(
		Task1,
		NULL,
		&TaskStks[0][TASK_STK_SIZE - 1],		// ����������ջջ��
		1,										// ���ȼ�
		1,										// �����
		&TaskStks[0][0],						// ����������ջջ��
		TASK_STK_SIZE,							// ����ջ��С
		&tasks_data[0],							// os_tcb�ṹ����չ��ʼ������ָ��
		0
		);
	OSTaskNameSet(1, "Task 1", &os_err);
```

### 3. ��ʼ��OS_TCB��չ����

**����OSTaskCreateExt�����д�����pextָ���os_tcb�ṹ�������չ��ʼ������os_core.c�е�OS_TCBInit�������������**��

```c
INT8U  OS_TCBInit (INT8U    prio,
                   OS_STK  *ptos,
                   OS_STK  *pbos,
                   INT16U   id,
                   INT32U   stk_size,
                   void    *pext,
                   INT16U   opt)
{
		.....
        ptcb->OSTCBDly           = 0u;                     /* Task is not delayed                      */

		// hewei ͨ��pext��Ϊtcb��չ��ָ�룬����ʼ����os_tcb����ӵ�����
		if (pext != NULL) {
			ptcb->cost = ((TCB_EXT_DATA*)pext)->cost;
			ptcb->period = ((TCB_EXT_DATA*)pext)->period;
			ptcb->start = ((TCB_EXT_DATA*)pext)->start;
			ptcb->compTime = ((TCB_EXT_DATA*)pext)->compTime;
		}
		....
}
```
### 4. ����������ķ���

**��Task 1Ϊ��**��

```c
void Task1(void *args)
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
```

### 5. ģ�������ִ�У���os_core.c��OSTimeTick�������������

```c
void  OSTimeTick (void)
{	...
    { ...
        {
		 ...
		// hewei
		if (OSTCBCur->compTime > 0) {
			OSTCBCur->compTime -= 1;
		}
    }
}
```

### 6. Ϊ����������ɺ͵���ʱ����ӡ��Ӧ������任�������os_core.c�е�OS_Sched��OSIntExit������������ݣ�

![image-20211013230806687](img/image-20211013230806687.png)

![image-20211013230936403](img/image-20211013230936403.png)

### 7. Ϊ��ʵ��������ȼ���������ȼ�����֮��������ݹ���������ʹ���ź���������������£�

**�ź����Ĵ���**��

```c
// �����ǰ���������ݹ�������Ҫ�����ź���
// ��Ϊֻ�����������񣬹��ź�����ʼֵΪ1����

    OSInit();   

#ifdef ENABLE_DATA_SHARE
	sem = OSSemCreate(1);		// OS_EVENT *OSSemCreate(INT16U cnt)
#endif
```

> * С�ӣ��ź���������OSInit����ִ�к󴴽�����������д�����
> * ԭ����ʵ��������ܼ򵥣��ź���������ǲ���ϵͳ��Դ������ϵͳ��δ��ʼ������������ź����أ�
> * Ϊʲô�����ź�������1����Ϊ2^1 = 2������0-1ģʽ.

**��Task 1Ϊ����Task 3ͬ��**��

```c
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
```

## 4. ʵ����

**ע��**����ϸ�Ա����ν���Ĳ��죡

### 1. ����֮���໥����

![image-20211013234711821](img/image-20211013234711821.png)

### 2. ������ȼ������������ȼ�����������ݹ�������

* ע�⣺ִ��ʱ�����`ENABLE_DATA_SHARE`.

![image-20211013234824971](img/image-20211013234824971.png)