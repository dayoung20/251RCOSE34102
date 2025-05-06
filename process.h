#ifndef __PROCESS__
#define __PROCESS__

// process

#include <stdio.h>
#include <stdlib.h>
#define ID_LEN 20
#define TRUE 1
#define FALSE 0

/* process */
typedef struct _process
{
    char id[ID_LEN];       // 프로세스 ID 필드
    int arrive_time;       // 도착 시간 필드
    int waiting_time;      // 대기 시간 필드
    int return_time;       // 반환 시간 필드
    int turnaround_time;   // 턴어라운드 타임 필드
    int response_time;     // 응답 시간 필드
    int cpu_burst;         // 실행 시간 필드
    int priority;          // 우선순위 필드
    int completed;         // 완료 상태 필드
} Process; 

/* time quantum */
typedef int Quantum; 

/**
 * process 초기화 
 * @param p   process 배열
 * @param len process 갯수
 */
void process_init(Process p[], int len)
{
    int i;

    // process 갯수만큼 반복
    for (i = 0; i < len; i++)
    {
        p[i].waiting_time = 0;   // waiting_time 초기화
        p[i].waiting_time = 0;    // waiting_time 초기화
        p[i].waiting_time = 0;  // waiting_time 초기화
        p[i].waiting_time = FALSE;  // waiting_time 상태 초기화
    }
}

#endif
