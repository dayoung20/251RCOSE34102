#ifndef __ROUND_ROBIN__
#define __ROUND_ROBIN__

/* Round Robin Algorithm */

/* 헤더 */
#include <limits.h>

#include "../functions.h"
#include "../table.h"
#include "../process.h"


/**
 * [rr_calculate_waiting_time RR 알고리즘 대기 시간 계산 함수]
 * @param process   프로세스 배열
 * @param process_count  프로세스 개수
 * @param q   time quantum
 */
void rr_calculate_waiting_time(Process *process, int process_count, Quantum q)
{
	int i;

	// 진행 시간 (현재 시간)
	int time = 0;

	/* 각 프로세스 별 남은 실행 시간을 저장할 배열 동적 할당 */
	int *remain_burst_time = (int *)malloc(sizeof(int) * process_count);
	/* 응답 시간을 확인하는데 사용할 배열 동적 할당 */
	int *calc_response_time = (int *)malloc(sizeof(int) * process_count);

	/* 프로세스의 갯수만큼 반복 */
	for (i = 0; i < process_count; i++)
	{
		// 남은 실행 시간 저장 배열 초기화
		remain_burst_time[i] = process[i].cpu_burst;
		// 응답시간 계산 확인 배열 초기화
		calc_response_time[i] = FALSE;
	}

	/* 모든 프로세스가 완료될때 까지 반복 */
	while (TRUE)
	{
		// 모든 프로세스가 완료되었는지 확인하는 변수
		int check = TRUE;

		/* 프로세스의 갯수만큼 반복 */
		for (i = 0; i < process_count; i++)
		{
			/* 실행시간이 남아있을 경우 */
			if (remain_burst_time[i] > 0)
			{
				// remain burst time이 있는 프로세스가 존재하므로 false로 설정
				check = FALSE;

				/* 아직 응답 시간이 계산되지 않았을 경우 */
				if (calc_response_time[i] == FALSE)
				{
					process[i].response_time = time - process[i].arrival_time;
					// 응답 시간 계산 후 저장
					calc_response_time[i] = TRUE;
					// 응답 시간 계산 처리
				}

				/* 남은 시간이 시간 할당량보다 클 경우 */
				if (remain_burst_time[i] > q)
				{
					time += q;
					// 현재 시간 시간 할당량만큼 증가
					remain_burst_time[i] -= q;
					// 현재 실행 중 프로세스의 남은 시간 감소
				}

				/* 남은 시간이 시간 할당량보다 작을 경우 */
				else
				{
					time += remain_burst_time[i];
					// 현재 시간 남은 시간만큼 증가
					process[i].waiting_time = time - process[i].cpu_burst;
					// 대기 시간 계산
					remain_burst_time[i] = 0;
					// 남은 시간을 0으로 바꾸어줌
				}
			}
		}

		/* 모든 프로세스가 완료되었을 경우 */
		if (check == TRUE)
			break;
			// 무한 루프 탈출
	}

	free(remain_burst_time);
	// 동적 할당한 배열 메모리 할당 해제
}

/**
 * [rr_calculate_turnaround_time 턴어라운드 타임 계산 함수]
 * @param process   프로세스 배열
 * @param process_count 프로세스 개수
 */
void rr_calculate_turnaround_time(Process *process, int process_count)
{
	int i;
	// 반복문에서 사용할 변수 선언

	/* 프로세스의 갯수만큼 반복 */
	for (i = 0; i < process_count; i++)
		process[i].turnaround_time = process[i].cpu_burst + process[i].waiting_time - process[i].arrival_time;
		// 턴어라운드 타임 계산 후 저장
}

/**
 * Round Robin 간트 차트 출력 함수]
 * @param process   프로세스 배열
 * @param process_count 프로세스 개수
 * @param q   time quantum
 */
void rr_gantt(Process *process, int process_count, Quantum q)
{
	int i, j;
	// 반복문에서 사용할 변수 선언
	int curr_time = 0, total_burst_time = 0;
	// 현재 시간과 총 실행 시간을 저장할 변수 선언 및 초기화
	int temp_total_burst_time = 0;
	// 임시로 값을 저장할 변수 선언 및 초기화

	/* 각 프로세스 별 남은 실행 시간을 저장할 배열 동적 할당 */
	int *remain_burst_time = (int *)malloc(sizeof(int) * process_count);

	/* 프로세스의 갯수만큼 반복 */
	for (i = 0; i < process_count; i++)
	{
		remain_burst_time[i] = process[i].cpu_burst;
		// 남은 시간 저장 배열  초기화
		total_burst_time += process[i].cpu_burst;
		// 총 실행 시간 계산
	}

	printf("\n");

	/* 대기 시간 계산 알고리즘과 동일 상단 바 출력 */
	while (TRUE)
	{
		int check = TRUE;

		for (i = 0; i < process_count; i++)
		{
			if (remain_burst_time[i] > 0)
			{
				check = FALSE;

				if (remain_burst_time[i] < q)
				{
					printf(" ");
					for (j = 0; j < remain_burst_time[i]; j++)
						printf("--");
				}

				else
				{
					printf(" ");
					for (j = 0; j <= q; j++)
						printf("--");
				}

				if (remain_burst_time[i] > q)
				{
					curr_time += q;
					remain_burst_time[i] -= q;
				}

				else
				{
					curr_time += remain_burst_time[i];
					process[i].waiting_time = curr_time - process[i].cpu_burst;
					remain_burst_time[i] = 0;
				}


			}
		}

		if (check == TRUE)
			break;
	}

	printf(" \n");

	for (i = 0; i < process_count; i++)
	{
		remain_burst_time[i] = process[i].cpu_burst;
	}

	/* 프로세스 아이디 출력 */
	while (TRUE)
	{
		int check = TRUE;

		for (i = 0; i < process_count; i++)
		{
			if (remain_burst_time[i] > 0)
			{
				check = FALSE;

				if (remain_burst_time[i] < q)
				{
					printf("|");

					if (remain_burst_time[i] != 1)
					{
						for (j = 0; j <= remain_burst_time[i] / 2; j++)
							printf(" ");

						printf("%2s", process[i].id);

						for (j = 0; j <= remain_burst_time[i] / 2; j++)
							printf(" ");
					}

					else
						printf("%2s", process[i].id);
				}

				else
				{
					printf("|");

					for (j = 0; j < q; j++)
						printf(" ");

					printf("%2s", process[i].id);

					for (j = 0; j < q; j++)
						printf(" ");
				}

				if (remain_burst_time[i] > q)
				{
					curr_time += q;
					remain_burst_time[i] -= q;
				}

				else
				{
					curr_time += remain_burst_time[i];
					process[i].waiting_time = curr_time - process[i].cpu_burst;
					remain_burst_time[i] = 0;
				}


			}
		}

		if (check == TRUE)
			break;
	}

	printf("|\n");

	for (i = 0; i < process_count; i++)
	{
		remain_burst_time[i] = process[i].cpu_burst;
	}

	/* 하단 바 출력 */
	while (TRUE)
	{
		int check = TRUE;

		for (i = 0; i < process_count; i++)
		{
			if (remain_burst_time[i] > 0)
			{
				check = FALSE;

				if (remain_burst_time[i] < q)
				{
					printf(" ");
					for (j = 0; j < remain_burst_time[i]; j++)
						printf("--");
				}

				else
				{
					printf(" ");
					for (j = 0; j <= q; j++)
						printf("--");
				}

				if (remain_burst_time[i] > q)
				{
					curr_time += q;
					remain_burst_time[i] -= q;
				}

				else
				{
					curr_time += remain_burst_time[i];
					process[i].waiting_time = curr_time - process[i].cpu_burst;
					remain_burst_time[i] = 0;
				}


			}
		}

		if (check == TRUE)
			break;
	}

	printf("\n");

	for (i = 0; i < process_count; i++)
		remain_burst_time[i] = process[i].cpu_burst;

	curr_time = 0;

	/* 프로세스 시간 출력 */
	while (TRUE)
	{
		int check = TRUE;

		for (i = 0; i < process_count; i++)
		{
			if (remain_burst_time[i] > 0)
			{
				check = FALSE;

				if (remain_burst_time[i] < q)
				{
					printf("%-2d", curr_time);

					for (j = 0; j < remain_burst_time[i] - 1; j++)
						printf("  ");

					printf(" ");
				}

				else
				{
					printf("%-2d", curr_time);

					for (j = 0; j < q; j++)
						printf("  ");

					printf(" ");
				}

				if (remain_burst_time[i] > q)
				{
					curr_time += q;
					remain_burst_time[i] -= q;
				}

				else
				{
					curr_time += remain_burst_time[i];
					process[i].waiting_time = curr_time - process[i].cpu_burst;
					remain_burst_time[i] = 0;
				}
			}
		}

		if (check == TRUE)
			break;
	}

	printf("%-3d\n", total_burst_time);

	printf("\n");

	// 동적 할당한 배열의 메모리 할당 해제
	free(remain_burst_time);
}

/**
 * RoundRobin 알고리즘 함수
 * @param process       프로세스 배열
 * @param process_count     프로세스 개수
 * @param quantum           time quantum
 */
void RR(Process *process, int process_count, Quantum quantum)
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

	// merge_sort_by_arrival_time 함수 호출로 도착 시간을 기준으로 정렬
	merge_sort_by_arrival_time(process, 0, process_count);

	// rr_calculate_waiting_time 함수 호출로 대기 시간, 응답 시간 계산
	rr_calculate_waiting_time(process, process_count, quantum);

	// rr_calculate_turnaround_time 함수 호출로 턴어라운드 타임 계산
	rr_calculate_turnaround_time(process, process_count);

	/* 프로세스의 갯수만큼 반복 */
	for (i = 0; i < process_count; i++)
	{
		process[i].waiting_time = process[i].turnaround_time - process[i].cpu_burst;
		// 대기 시간 계산 후 저장
		process[i].return_time = process[i].arrival_time + process[i].cpu_burst + process[i].waiting_time;
		// 반환 시간 계산 후 저장

		total_waiting_time += process[i].waiting_time;
		// 총 대기 시간 증가
		total_turnaround_time += process[i].turnaround_time;
		// 총 턴어라운드 타임 증가
		total_response_time += process[i].response_time;
		// 총 응답 시간 증가
	}

	printf("\nRound Robin Scheduling Algorithm ( Quantum : %d )\n\n", quantum);

	// rr_gantt 함수 호출로 간트 차트 출력
	rr_gantt(process, process_count, quantum);

	/* 평균 대기시간, 턴어라운드 타임, 응답 시간 출력 */
	printf("\nAverage Waiting Time     : %-2.2lf\n", (double)total_waiting_time / (double)process_count);
	printf("tAverage Turnaround Time  : %-2.2lf\n", (double)total_turnaround_time / (double)process_count);
	// printf("Average Response Time    : %-2.2lf\n\n", (double)total_response_time / (double)process_count);

	print_table(process, process_count);
	// print_table 함수 호출로 데이터 표 출력
}


#endif