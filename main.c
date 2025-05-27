#include "CpuScheduling.h"


int main(){

    int process_count = 0;
    int i=0;

    Quantum quantum;
    Process *process;

    printf("프로세스 개수를 입력하세요: ");
    scanf("%d", &process_count);

    process = (Process *)malloc(sizeof(Process) * process_count);
    
    // 사용자로부터 프로세스 정보 입력
    printf("각 프로세스의 id 도착시간 burst time 우선순위를 알려주세요: ex. P0 3 5 2 \n");


    while(i<process_count){
        scanf("%s %d %d %d", process[i].id, &process[i].arrival_time, &process[i].cpu_burst, &process[i].priority);
        i++;
    }

    //사용자로부터 time quantum 입력
    printf("time quantum을 입력하세요: ");
    scanf("%d", &quantum);

    // 정보가 맞게 들어갔는지 확인용 print

    for (int i = 0; i < process_count; i++) {
        printf("ID: %s, Arrival: %d, Burst: %d, Priority: %d\n",
            process[i].id, process[i].arrival_time, process[i].cpu_burst, process[i].priority);
    }
    // 전체 프로세스 초기화
    process_init(process, process_count);

    printf("<FCFS>");
    FCFS(process, process_count);

    printf("\n\n");

    printf("<non preemptive SJF>");
    npSJF(process, process_count);

    printf("\n\n");

    printf("<preemptive SJF>");
    pSJF(process, process_count);

    printf("<preemptive priority>");
    PPS(process, process_count);

    printf("<non preemptive priority>");
    NPPS(process, process_count);

    printf("<Round Robin>");
    RR(process, process_count, quantum);

}