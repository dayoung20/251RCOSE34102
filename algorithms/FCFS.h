#ifndef __FCFS__
#define __FCFS__

/* FCFS (First Come First Served) */

/* 헤더 */
#include "../functions.h"
#include "../table.h"

/**
 * fcfs 간트 차트 출력 함수
 * @param process 프로세스 배열
 * @param process_count 프로세스 개수
 */
void fcfs_gantt(Process *process, int process_count)
{
	int i, j;

    // 프로세스 시간 출력
	printf("0");

	for (i = 0; i < process_count; i++)
	{
		for (j = 0; j < process[i].cpu_burst; j++)
			printf("  ");

		if (process[i].return_time > 9)
			printf("\b");

		printf("%d", process[i].return_time);

	}

    printf("\n");

	printf(" ");

	// 간트 차트 상단 라인
	for (i = 0; i < process_count; i++)
	{
		for (j = 0; j < process[i].cpu_burst; j++)
			printf("--");

		printf(" ");
	}

	printf("\n|");

	// 프로세스 이름 
	for (i = 0; i < process_count; i++)
	{
		for (j = 0; j < process[i].cpu_burst - 1; j++)
			printf(" ");

		printf("%s", process[i].id);

		for (j = 0; j < process[i].cpu_burst - 1; j++)
			printf(" ");

		printf("|");
	}

	printf("\n ");

	// 간트 차트 하단 라인
	for (i = 0; i < process_count; i++)
	{
		for (j = 0; j < process[i].cpu_burst; j++)
			printf("--");

		printf(" ");
	}

	printf("\n");
}

/**
 * FCFS 실행 함수
 * @param process   process 배열
 * @param process_count process 개수 
 */
void FCFS(Process *process, int process_count)
{
	int i;
    
    // 총 waiting time 선언 및 초기화
	int total_waiting_time = 0;
	
    // 총 turn aroung time 선언 및 초기화
	int total_turnaround_time = 0;
	
    // 총 response time 선언 및 초기화
	int total_response_time = 0;
	
    // 총 return time 선언 및 초기화
	int total_return_time = 0;

    // 프로세스 초기화
	process_init(process, process_count);

    // 도착 시간을 기준으로 정렬
	// left : 0, right : process_count
	merge_sort_by_arrive_time(process, 0, process_count);
	
	// 1. 첫 프로세스 먼저 실행
    // 2. process 배열의 해당 인덱스 항목 update
	process[0].return_time = process[0].cpu_burst;
	process[0].turnaround_time = process[0].return_time - process[0].arrive_time;
	process[0].response_time = 0;
	process[0].waiting_time = 0;

	// 3. 실행된 프로세스만큼 total 변수들 update
	total_response_time += process[0].response_time;
	total_waiting_time += process[0].waiting_time;
	total_turnaround_time += process[0].turnaround_time;
	total_return_time += process[0].cpu_burst;

	// 이후 process 반복
	for (i = 1; i < process_count; i++)
	{
		// 1. 각 프로세스 먼저 실행
        // 2. process 배열의 해당 인덱스 항목 update
		process[i].waiting_time = total_return_time - process[i].arrive_time;
		process[i].return_time = total_return_time + process[i].cpu_burst;
		process[i].turnaround_time = process[i].return_time - process[i].arrive_time;
		process[i].response_time = process[i].waiting_time;

		// 3. 실행된 프로세스만큼 total 변수들 update
		total_return_time += process[i].cpu_burst;
		total_waiting_time += process[i].waiting_time;
		total_turnaround_time += process[i].turnaround_time;
		total_response_time += process[i].response_time;
	}

	printf("\nFirst Come First Served\n\n");

    // 간트 차트 출력
	fcfs_gantt(process, process_count);
	
	/* 평균 대기시간, 턴어라운드 타임, 응답 시간 출력 */
	printf("\nAverage Waiting Time     : %-2.2lf\n", (double)total_waiting_time / (double)process_count);
	printf("Average Turnaround Time  : %-2.2lf\n", (double)total_turnaround_time / (double)process_count);
	// printf("Average Response Time    : %-2.2lf\n\n", (double)total_response_time / (double)process_count);

    //각 프로세스 별 정보 출력
	print_table(process, process_count);
}

#endif