#ifndef __FUNCTION__
#define __FUNCTION__

// 필요한 함수들 정의

/**
 * merge 분할된 두 배열 합하는 함수
 * @param arr   정렬할 배열
 * @param left  배열의 가장 왼쪽 인덱스
 * @param mid   배열의 가운데 인덱스
 * @param right 배열의 가장 오른쪽 인덱스
 */
void merge(Process arr[], int left, int mid, int right)
{
	int fIdx = left;
	int rIdx = mid + 1;
	int i;

	Process *sortArr = (Process *)malloc(sizeof(Process) * (right + 1));
	int sIdx = left;

	while (fIdx <= mid && rIdx <= right)
	{
		if (arr[fIdx].arrival_time <= arr[rIdx].arrival_time)
			sortArr[sIdx] = arr[fIdx++];
		else
			sortArr[sIdx] = arr[rIdx++];

		sIdx++;
	}

	/* 오른쪽 배열의 데이터가 남아있는 경우 */
	if (fIdx > mid)
	{
		/* 오른쪽 배열의 남은 데이터 만큼 반복 */
		for (i = rIdx; i <= right; i++, sIdx++)
			sortArr[sIdx] = arr[i];
	}

	/* 왼쪽 배열의 데이터가 남아있는 경우 */
	else
	{
		/* 왼쪽 배열의 남은 데이터 만큼 반복 */
		for (i = fIdx; i <= mid; i++, sIdx++)
			sortArr[sIdx] = arr[i];
	}

	/* 배열의 데이터 개수만큼 반복 */
	for (i = left; i <= right; i++)
		arr[i] = sortArr[i];
		// 원본 배열에 복사

	free(sortArr);
}

/**
 * 도착 시간 기준 병합 정렬 함수
 * @param arr   입력된 배열
 * @param left  배열의 가장 왼쪽 인덱스
 * @param right 배열의 가장 오른쪽 인덱스
 */
void merge_sort_by_arrival_time(Process arr[], int left, int right)
{
    // 중간 지점 인덱스
	int mid;

	/* left가 right보다 작으면 실행 */
	if (left < right)
	{
		/* 중간 지점을 계산 */
		mid = (left + right) / 2;

		/* 둘로 나눠서 각각을 정렬한다 */
		merge_sort_by_arrival_time(arr, left, mid);
		merge_sort_by_arrival_time(arr, mid + 1, right);

		/* 정렬된 두 배열을 병합한다 */
		merge(arr, left, mid, right);
	}
}


//반환 시간 기준 비교 함수 
int compare_by_return_time(const void *a, const void *b)
{
	/* const void 형 변수 형 변환 */
	Process *ptA = (Process *)a;
	Process *ptB = (Process *)b;

	/* ptA의 반환 시간이 작을 경우 */
	if (ptA->return_time < ptB->return_time)
		return -1;
		// -1 반환

	/* ptA의 반환 시간이 클 경우 */
	else if (ptA->return_time > ptB->return_time)
		return 1;

	/* 반환 시간이 같을 경우 -> 존재 X */
	else
		return 0;
}

/**
 * 반환 시간 기준 퀵 정렬 함수
 * @param process   프로세스 배열
 * @param process_count 프로세스 개수
 */
void quick_sort_by_return_time(Process process[], int process_count)
{
	qsort(process, process_count, sizeof(Process), compare_by_return_time);
}

#endif