#ifndef __P_PRIORITY__
#define __P_PRIORITY__

/* Preemptive priority Algorithm */

/* 헤더 */
#include <limits.h>

#include "../functions.h"
#include "../table.h"
#include "../process.h"


/**
 * preemptive priority 알고리즘 시간 계산 함수
 * @param process   프로세스 배열
 * @param process_count 프로세스 개수
 */
void pps_calculate_time(Process *process, int process_count)
{
	int i;
	// 반복문에서 사용할 변수 선언
	int priority;
	// 우선순위를 저장할 변수 선언
	int current_time = 0;
	// 현재 시간을 저장할 변수 선언 및 초기화
	int total_burst_time = 0;
	// 총 실행 시간을 저장할 변수 선언
	int k = 0;
	// 현재 실행할 프로세스 번호를 저장할 번수 선언 및 초기화

	/* 각 프로세스 별 남은 실행 시간을 저장할 배열 동적 할당 */
	int *remain_burst_time = (int *)malloc(sizeof(int) * process_count);
	/* 응답 시간을 확인하는데 사용할 배열 동적 할당 */
	int *count = (int *)malloc(sizeof(int) * process_count);

	/* 프로세스의 갯수만큼 반복 */
	for (i = 0; i < process_count; i++)
	{
		count[i] = 0;
		// count 배열 초기화
		remain_burst_time[i] = process[i].cpu_burst;
		// remain_burst_time 배열 초기화
		total_burst_time += process[i].cpu_burst;
		// 총 남은 실행 시간 초기화
	}

	/* 현재 시간이 총 실행 시간이 되기 전까지 반복 */
	while (current_time < total_burst_time)
	{
		priority = INT_MAX;
		// 우선순위를 INT_MAX로 초기화

		/* 가장 마지막에 들어온 프로세스의 도착시간 보다 작을 경우 */
		if (current_time <= process[process_count - 1].arrive_time)
		{
			/* 프로세스의 갯수만큼 반복 */
			for (i = 0; i < process_count; i++)
			{
				/* 완료되지 않았으며 도착시간이 현재시간보다 작거나 같으며
				   현재 우선순위보다 우선순위가 작을 경우 */
				if ((process[i].completed == FALSE)
						&& (process[i].arrive_time <= current_time)
							&& (priority > process[i].priority))
				{
					priority = process[i].priority;
					// 우선 순위 갱신
					k = i;
					// 프로세스 인덱스 갱신
				}
			}
		}

		/* 더 이상 새로운 프로세스가 들어오지 않는 경우 */
		else
		{
			/* 프로세스의 갯수만큼 반복 */
			for (i = 0; i < process_count; i++)
			{
				/* 완료되지 않았으며 현재 우선순위간보다
				   우선순위가 작을 경우 */
				if ((process[i].completed == FALSE)
						&& (priority > process[i].priority))
				{
					priority = process[i].priority;
					// 우선 순위 갱신
					k = i;
					// 프로세스 인덱스 갱신
				}
			}
		}

		/* 선택된 프로세스가 처음 시작될 경우 */
		if (count[k] == 0)
		{
			count[k]++;
			// 초기 실행이 아님을 표시
			process[k].response_time = current_time;
			// 실행중인 프로세스의 응답시간 저장
		}

		remain_burst_time[k]--;
		// 실행된 프로세스의 남은 시간 감소
		current_time++;
		// 현재 시간 증가

		/* 프로세스의 남은 실행 시간이 0이될 경우 */
		if (remain_burst_time[k] == 0)
		{
			process[k].completed = TRUE;
			// 완료 상태로 변경
			process[k].waiting_time = current_time - process[k].cpu_burst - process[k].arrive_time;
			// 대기 시간 계산
			process[k].return_time = current_time;
			// 반환 시간 계산
		}
	}

	/* 동적 할당한 배열의 메모리 할당 해제 */
	free(remain_burst_time);
	free(count);
}


/**
 * preemptive priority 간트 차트 출력 함수
 * @param process   프로세스 배열
 * @param process_count 프로세스 개수
 */
void pps_print_gantt_chart(Process *process, int process_count)
{
	int i;
	int total_burst_time = 0;
	int current_time = 0, previous_time = 0;
	// 이전 프로세스가 실행된 시간을 저장할 변수 추가 선언
	int k, pre_k = 0;
	// 이전 프로세스 번호를 저장할 변수 추가 선언
	int priority, num;
	// 새로 실행된 프로세스 사이 거리를 저장할 변수 선언

	int *count = (int *)malloc(sizeof(int) * process_count);
	int *remain_burst_time = (int *)malloc(sizeof(int) * process_count);

	for (i = 0; i < process_count; i++)
	{
		remain_burst_time[i] = process[i].cpu_burst;
		total_burst_time += process[i].cpu_burst;
		process[i].completed = FALSE;
		count[i] = 0;
	}

	printf(" ");

	/* 동일 알고리즘을 실행하며 상단 바 출력 */
	while (current_time < total_burst_time)
	{
		priority = INT_MAX;

		if (current_time <= process[process_count - 1].arrive_time)
		{
			for (i = 0; i < process_count; i++)
			{
				if ((process[i].completed == FALSE)
						&& (process[i].arrive_time <= current_time))
				{
					if (priority > process[i].priority)
					{
						priority = process[i].priority;
						k = i;
					}
				}
			}
		}

		else
		{
			for (i = 0; i < process_count; i++)
			{
				if (process[i].completed == FALSE)
				{
					if (priority > process[i].priority)
					{
						priority = process[i].priority;
						k = i;
					}
				}
			}
		}

		/* 이전에 실행된 프로세스와 다른 프로세스일 경우 */
		if (pre_k != k)
			printf(" ");
			// 공백 출력

		printf("--");
		remain_burst_time[k]--;
		current_time++;
		pre_k = k;
		// 이전 프로세스 저장

		if (remain_burst_time[k] == 0)
			process[k].completed = TRUE;
	}

	for (i = 0; i < process_count; i++)
	{
		remain_burst_time[i] = process[i].cpu_burst;
		process[i].completed = FALSE;
	}

	printf("\n|");
	current_time = 0;

	/* 동일 알고리즘을 실행하며 프로세스 아이디 출력
	   이전 프로세스와 비교하며 \b 사용하여 간격 조절 */
	while (current_time < total_burst_time)
	{
		priority = INT_MAX;

		if (current_time <= process[process_count - 1].arrive_time)
		{
			for (i = 0; i < process_count; i++)
			{
				if ((process[i].completed == FALSE)
						&& (process[i].arrive_time <= current_time))
				{
					if (priority > process[i].priority)
					{
						priority = process[i].priority;
						k = i;
					}
				}
			}
		}

		else
		{
			for (i = 0; i < process_count; i++)
			{
				if (process[i].completed == FALSE)
				{
					if (priority > process[i].priority)
					{
						priority = process[i].priority;
						k = i;
					}
				}
			}
		}

		if (current_time == 0)
		{
			count[k]++;
			printf("  ");
		}

		else
		{
			/* 이전 프로세스와 다른 프로세스일 경우 */
			if (pre_k != k)
			{
				num = count[pre_k] + 1;
				// 두 프로세스 시간 차이 저장
				count[pre_k] = 0;
				// 이전 프로세스 카운트 초기화
				count[k]++;
				// 현재 프로세스 카운트 증가

				/* 두 프로세스 차이만큼 \b 출력 */
				for (i= 0; i < num; i++)
					printf("\b");

				/* 이전 프로세스 ID 출력 */
				printf("%2s", process[pre_k].id);

				/* 간격을 맞추어 공백 출력 */
				for (i = 0; i < num - 2; i++)
					printf(" ");

				printf("|  ");
			}

			/* 같은 프로세스일 경우 */
			else
			{
				// 현재 프로세스 카운트 증가
				count[k]++;

				printf("  ");
				// 공백 출력

				/* 마지막 프로세스 실핼일 경우 */
				if (current_time == total_burst_time - 1)
				{
					num = count[pre_k] + 1;
					count[pre_k] = 0;
					count[k]++;

					for (i = 0; i < num; i++)
						printf("\b");

					printf("%2s", process[pre_k].id);

					for (i = 0; i < num - 2; i++)
						printf(" ");
				}
			}
		}

		pre_k = k;
		remain_burst_time[k]--;
		current_time++;

		if (remain_burst_time[k] == 0)
			process[k].completed = TRUE;
	}

	for (i = 0; i < process_count; i++)
	{
		remain_burst_time[i] = process[i].cpu_burst;
		process[i].completed = FALSE;
	}

	printf("|\n");
	current_time = 0;

	/* 동일 알고리즘을 사용하여 하단 바 출력 */
	while (current_time < total_burst_time)
	{
		priority = INT_MAX;

		if (current_time <= process[process_count - 1].arrive_time)
		{
			for (i = 0; i < process_count; i++)
			{
				if ((process[i].completed == FALSE)
						&& (process[i].arrive_time <= current_time))
				{
					if (priority > process[i].priority)
					{
						priority = process[i].priority;
						k = i;
					}
				}
			}
		}

		else
		{
			for (i = 0; i < process_count; i++)
			{
				if (process[i].completed == FALSE)
				{
					if (priority > process[i].priority)
					{
						priority = process[i].priority;
						k = i;
					}
				}
			}
		}

		if (pre_k != k)
			printf(" ");

		printf("--");

		remain_burst_time[k]--;
		current_time++;
		pre_k = k;

		if (remain_burst_time[k] == 0)
			process[k].completed = TRUE;
	}

	for (i = 0; i < process_count; i++)
	{
		remain_burst_time[i] = process[i].cpu_burst;
		process[i].completed = FALSE;
	}

	current_time = 0;
	num = 0;
	printf("\n");

	/* 프로세스 ID 출력과 같은 방법으로 실행하며 시간 출력 */
	while (current_time <= total_burst_time)
	{
		if (total_burst_time != current_time)
		{
			priority = INT_MAX;

			if (current_time <= process[process_count - 1].arrive_time)
			{
				for (i = 0; i < process_count; i++)
				{
					if ((process[i].completed == FALSE)
							&& (process[i].arrive_time <= current_time))
					{
						if (priority > process[i].priority)
						{
							priority = process[i].priority;
							k = i;
						}
					}
				}
			}

			else
			{
				for (i = 0; i < process_count; i++)
				{
					if ((process[i].completed == FALSE)
							&& (priority > process[i].priority))
					{
						priority = process[i].priority;
						k = i;
					}
				}
			}


			if (pre_k != k)
			{
				for (i = 0; i < num && current_time != 0; i++)
					printf("  ");

				if (current_time != 0)
					printf(" ");

				printf("%-2d", current_time);
				num = 0;

				previous_time = current_time;
			}

			else
				num++;

			remain_burst_time[k]--;
			current_time++;
			pre_k = k;

			if (remain_burst_time[k] == 0)
				process[k].completed = TRUE;
		}

		else
		{
			for (i = 0; i < current_time - previous_time - 1; i++)
				printf("  ");
			printf(" ");

			printf("%-2d", current_time);

			break;
		}
	}

	printf("\n");

	free(count);
	free(remain_burst_time);
}


/**
 * preemptive priority 알고리즘 실행 함수
 * @param process   프로세스 배열
 * @param len 프로세스 개수
 */
void PPS(Process *process, int process_count)
{
	int i;
	// 반복문에서 사용할 변수 선언
	int total_waiting_time = 0;
	// 총 대기 시간을 저장할 변수 선언 및 초기화
	int total_turnaround_time = 0;
	// 총 턴어라운드 타임을 저장할 변수 선언 및 초기화
	int total_response_time = 0;
	// 총 응답 시간을 저장할 변수 선언 및 초기화

	process_init(process, process_count);
	// process_init 함수 호출로 프로세스 초기화

	merge_sort_by_arrive_time(process, 0, process_count);
	// merge_sort_by_arrive_time 함수 호출로 도착 시간을 기준으로 정렬

	pps_calculate_time(process, process_count);
	// pps_calculate_time 함수 호출로 프로세스 시간 계산

	/* 프로세스의 갯수 만큼 반복 */
	for (i = 0; i < process_count; i++)
	{
		process[i].turnaround_time = process[i].return_time - process[i].arrive_time;
		// 턴어라운드 타임 계산
		total_waiting_time += process[i].waiting_time;
		// 총 대기 시간 증가
		total_turnaround_time += process[i].turnaround_time;
		// 총 턴어라운드 타임 증가
		total_response_time += process[i].response_time;
		// 총 응답 시간 증가
	}

	printf("\nPreemptive Priority Scheduling Algorithm\n\n");

	pps_print_gantt_chart(process, process_count);
	// pps_print_gantt_chart 함수 호출로 간트 차트 출력

	/* 평균 대기시간, 턴어라운드 타임, 응답 시간 출력 */
	printf("\nAverage Waiting Time     : %-2.2lf\n", (double)total_waiting_time / (double)process_count);
	printf("Average Turnaround Time  : %-2.2lf\n", (double)total_turnaround_time / (double)process_count);
	// printf("Average Response Time    : %-2.2lf\n\n", (double)total_response_time / (double)process_count);

	print_table(process, process_count);
	// print_table 함수 호출로 데이터 표 출력
}
#endif