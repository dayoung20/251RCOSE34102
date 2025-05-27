#ifndef __NP_PRIORITY__
#define __NP_PRIORITY__

/* non preemptive priority Algorithm */

/* 헤더 */
#include <limits.h>

#include "../functions.h"
#include "../table.h"
#include "../process.h"


/**
 * npps_calculate NPPS 알고리즘 시간 계산 함수
 * @param process   프로세스 배열
 * @param process_count 프로세스 개수
 */
void npps_calculate(Process *process, int process_count)
{
	int i;
	int check;
	int priority_idx;
	int time = 0;

	/* 가장 먼저 들어온 프로세스 실행 및 시간 계산 */
	process[0].return_time = process[0].cpu_burst;
	process[0].turnaround_time = process[0].return_time - process[0].arrival_time;
	process[0].response_time = 0;
	process[0].waiting_time = 0;
	process[0].completed = TRUE;
	time = process[0].return_time;

	/* 모든 프로세스가 완료될 때까지 반복 */
	while (TRUE)
	{
		// default index 설정
		priority_idx = -1;
		check = FALSE;

		for (i = 1; i < process_count; i++)
		{
			// 실행되지 않은 프로세스 중에서 
			if ((process[i].completed == FALSE) && (process[i].arrival_time <= time))
			{
				// 프로세스 우선순위 높은 것 찾기 (priority는 작은 숫자가 높은 우선순위)
				if (priority_idx == -1 || process[i].priority < process[priority_idx].priority)
				{
					priority_idx = i;
					check = TRUE;
				}
			}
		}

		if (check == FALSE)
			break;

		// 현재 가장 우선순위가 높은 프로세스 수행
		process[priority_idx].response_time = time - process[priority_idx].arrival_time;
		process[priority_idx].waiting_time = time - process[priority_idx].arrival_time;
		process[priority_idx].return_time = time + process[priority_idx].cpu_burst;
		process[priority_idx].turnaround_time = process[priority_idx].return_time - process[priority_idx].arrival_time;
		process[priority_idx].completed = TRUE;

		time += process[priority_idx].cpu_burst;
	}
}


/**
 * npps_print_gantt_chart 간트 차트 출력 함수
 * @param process   프로세스 배열
 * @param process_count 프로세스 개수
 */
void npps_print_gantt_chart(Process *process, int len)
{
	int i, j;

	printf(" ");

	/* 상단 바 출력 */
	for (i = 0; i < len; i++)
	{
		for (j = 0; j < process[i].cpu_burst; j++)
			printf("--");

		printf(" ");
	}

	printf("\n|");

	/* 프로세스 ID 출력 */
	for (i = 0; i < len; i++)
	{
		for (j = 0; j < process[i].cpu_burst - 1; j++)
			printf(" ");

		printf("%s", process[i].id);

		for (j = 0; j < process[i].cpu_burst - 1; j++)
			printf(" ");

		printf("|");
	}

	printf("\n ");

	/* 하단 바 출력 */
	for (i = 0; i < len; i++)
	{
		for (j = 0; j < process[i].cpu_burst; j++)
			printf("--");

		printf(" ");
	}

	printf("\n");

	/* 프로세스 시간 출력 */
	printf("0");

	for (i = 0; i < len; i++)
	{
		for (j = 0; j < process[i].cpu_burst; j++)
			printf("  ");

		if (process[i].return_time > 9)
			printf("\b");

		printf("%d", process[i].return_time);
	}

	printf("\n");
}

/**
 * non preemptive priority 알고리즘 시간 계산 함수
 * @param process   프로세스 배열
 * @param process_count 프로세스 개수
 */
void NPPS(Process *process, int process_count)
{
	int i;

	// 총 대기 시간을 저장할 변수 선언 및 초기화
	int total_waiting_time = 0;
	// 총 턴어라운드 타임을 저장할 변수 선언 및 초기화
	int total_turnaround_time = 0;
	// 총 응답 시간을 저장할 변수 선언 및 초기화
	int total_response_time = 0;
	
	// process_init 함수 호출로 프로세스 초기화
	process_init(process, process_count);

	// merge_sort_by_arrive_time 함수 호출로 도착 시간을 기준으로 정렬
	merge_sort_by_arrival_time(process, 0, process_count);

	// npps_calculate 함수 호출로 시간 계산
	npps_calculate(process, process_count);

	/* 프로세스의 갯수 만큼 반복 */
	for (i = 0; i < process_count; i++)
	{	
		// 총 대기 시간 증가
		total_waiting_time += process[i].waiting_time;
		// 총 턴어라운드 타임 증가
		total_turnaround_time += process[i].turnaround_time;
		// 총 응답 시간 증가
		total_response_time += process[i].response_time;
		
	}

	// quick_sort_by_return_time 함수 호출로 반환 시간으로 정렬
	quick_sort_by_return_time(process, process_count);

	printf("\nNon-preemptive Priority Scheduling Algorithm\n\n");

	// npps_print_gantt_chart 함수 호출로 간트 차트 출력
	npps_print_gantt_chart(process, process_count);

	/* 평균 대기시간, 턴어라운드 타임, 응답 시간 출력 */
	printf("\nAverage Waiting Time     : %-2.2lf\n", (double)total_waiting_time / (double)process_count);
	printf("Average Turnaround Time  : %-2.2lf\n", (double)total_turnaround_time / (double)process_count);
	// printf("Average Response Time    : %-2.2lf\n\n", (double)total_response_time / (double)len);

	print_table(process, process_count);
	// print_table 함수 호출로 데이터 표 출력
}

#endif