#ifndef _EZ_TARGET_CONFIG_H
#define _EZ_TARGET_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define EZ_LOGGING			1
#define EZ_LOGGING_COLOR	0
#define EZ_LINKEDLIST		1
#define EZ_HEXDUMP			1
#define EZ_RING_BUFFER		1
#define EZ_ASSERT			0
#define EZ_STATIC_ALLOC		1
#define EZ_SYS_ERROR		1
#define EZ_QUEUE			1			

#define EZ_EVENT_NOTIFIER    0
#define EZ_KERNEL            0
#define EZ_TASK_WORKER       1
#define EZ_STATE_MACHINE     1

#define DATA_MODEL			0
#define EZ_CLI				1
#define EZ_RPC				1
#define EZ_IPC				1
#define EZ_HAL_DRIVER		1
#define EZ_UART				1

#define EZ_HAL_ECHO			0
#define EZ_HAL_UART			0


#define LITTLE_FS			0
#define THREADX				0
#define FREERTOS			0


#ifdef __cplusplus
}
#endif

#endif