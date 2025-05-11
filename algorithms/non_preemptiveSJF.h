#ifndef __NP_SJF__
#define __NP_SJF__

/* (np SJF) Non Preemptive Shortest Job First Algorithm */

/* 헤더 */
#include "../functions.h"
#include "../table.h"

/**
 * np SJF 알고리즘 시간 계산 함수
 * @param process   프로세스 배열
 * @param process_count 프로세스 개수
 */
void np_sjf_calculate_time(Process *process, int process_count)
{
	int i, j;

    // 현재 시간을 저장할 변수 선언 및 초기화
	int time = 0;
	
    // 최소 시간을 갖는 인덱스를 저장할 변수 선언 및 초기화
	int min = 0;
	
	// 1. 첫 프로세스 먼저 실행
    // 2. process 배열의 해당 인덱스 항목 update
	process[0].completed = TRUE;
	process[0].return_time = process[0].cpu_burst;
	process[0].turnaround_time = process[0].cpu_burst - process[0].arrive_time;
	process[0].waiting_time = 0;
	
    // 현재 시간을 첫번째 프로세스의 cpu burst로
	time = process[0].cpu_burst;
	
	for(i = 1; i < process_count; i++)
	{
		/* 프로세스의 갯수 -1 만큼 반복 */
		for (j = 1; j < process_count; j++)
		{
			/* 이미 완료된 프로세스일 경우 */
			if (process[j].completed == TRUE)
                // 다음 인덱스로 이동
				continue;

			/* 아직 완료되지 않은 프로세스일 경우 */
			else
			{
                // min 변수를 해당 프로세스의 인덱스로 변경
				min = j;

                // 반복문 탈출
				break;
			}
		}

		/* 프로세스의 갯수 -1 만큼 반복 */
		for (j = 1; j < process_count; j++)
		{
			/* 최소 작업 시간을 갖는 조건에 맞는 프로세스 탐색 */
			if ((process[j].completed == FALSE)
					&& (process[j].arrive_time <= time)
						&& (process[j].cpu_burst < process[min].cpu_burst))
			{
                // 최소 작업 프로세스 갱신
				min = j;
			}
		}

        // 실행할 프로세스 대기 시간 계산
		process[min].waiting_time = time - process[min].arrive_time;
		// 실행 프로세스 완료 상태 변경
		process[min].completed = TRUE;
        // 현재 시간 프로세스의 실행 시간만큼 증가
		time += process[min].cpu_burst;
        // 프로세스 반환 시간 계산
		process[min].return_time = time;
		// 프로세스 턴어라운드 타임 계산
		process[min].turnaround_time = process[min].return_time - process[min].arrive_time;
		
	}
}

/**
 * np sjf 간트 차트 출력 함수
 * @param process   프로세스 배열
 * @param process_count 프로세스 개수
 */
void np_sjf_gantt(Process *process, int process_count)
{
	int i, j;

	printf("0");

	// 프로세스 시간 출력
	for (i = 0; i < process_count; i++)
	{
		for (j = 0; j < process[i].cpu_burst; j++)
			printf("  ");

		if (process[i].turnaround_time > 9)
			printf("\b");

		printf("%d", process[i].return_time);
	}
	printf(" \n");

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
 * np Shortest Job First 
 * @param process   프로세스 배열
 * @param process_count 프로세스 개수
 */
void npSJF(Process *process, int process_count)
{
	int i;

    // 총 waiting time 선언 및 초기화
	int total_waiting_time = 0;

    // 총 turn aroung time 선언 및 초기화
	int total_turnaround_time = 0;

	// 총 response time 선언 및 초기화
	int total_response_time = 0;

    // 프로세스 초기화
	process_init(process, process_count);

    // 도착 시간을 기준으로 정렬
	merge_sort_by_arrive_time(process, 0, process_count);

	np_sjf_calculate_time(process, process_count);

    // turnaround time, waiting time 계산
	for (i = 0; i < process_count; i++)
	{
        // 프로세스의 반환 시간 계산 후 저장
		process[i].return_time = process[i].turnaround_time + process[i].arrive_time;
		// 프로세스의 응답 시간 저장
		process[i].response_time = process[i].waiting_time;
		// 총 대기 시간 증가
		total_waiting_time += process[i].waiting_time;
		// 총 턴어라운드 타임 증가
		total_turnaround_time += process[i].turnaround_time;
		// 총 응답 시간 증가
		total_response_time += process[i].response_time;
	}

	printf("\nShortest Job First\n\n");

    // quick_sort_by_return_time 함수 호출로 반환 시간으로 정렬
	quick_sort_by_return_time(process, process_count);

    //간트 차트 출력
	np_sjf_gantt(process, process_count);
	
	/* 평균 대기시간, 턴어라운드 타임, 응답 시간 출력 */
	printf("\nAverage Waiting Time     : %-2.2lf\n", (double)total_waiting_time / (double)process_count);
	printf("Average Turnaround Time  : %-2.2lf\n", (double)total_turnaround_time / (double)process_count);
	// printf("Average Response Time    : %-2.2lf\n\n", (double)total_response_time / (double)len);

    //각 프로세스 별 정보 출력
	print_table(process, process_count);
}

#endif