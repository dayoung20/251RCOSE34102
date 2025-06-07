#ifndef __PROCESS__
#define __PROCESS__

#include <stdio.h>
#include <stdlib.h>
#define ID_LEN 20
#define TRUE 1
#define FALSE 0

// process 
typedef struct _process
{
    char id[ID_LEN];       // 프로세스 ID 필드
    int arrival_time;       // 도착 시간 필드
    int waiting_time;      // 대기 시간 필드
    int return_time;       // 반환 시간 필드
    int turnaround_time;   // 턴어라운드 타임 필드
    int response_time;     // 응답 시간 필드
    int cpu_burst;         // 실행 시간 필드
    int priority;          // 우선순위 필드
    int completed;         // 완료 상태 필드
} Process; 

// Round robin 스케줄링에서만 사용하는 time quantum
typedef int Quantum; 

/**
 * process 초기화 
 * @param process   process 배열
 * @param process_count process 갯수
 */
void process_init(Process process[], int process_count)
{
    int i;

    for (i = 0; i < process_count; i++)
    {
        process[i].waiting_time = 0;  
        process[i].return_time = 0;    
        process[i].response_time = 0;  
        process[i].completed = FALSE;
    }
}

#endif
