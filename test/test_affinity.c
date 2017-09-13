#include <stdio.h>
#include <math.h>
#include <pthread.h>

cpu_set_t cpuset,cpuget;

double waste_time(long n)
{
	double res = 0;
	long i = 0;
	while (i <n * 200000000){
		i++;
		res += sqrt(i);
	}
	return res;
}

void *thread_func(void *param)
{   
	CPU_ZERO(&cpuset);
	CPU_SET(0, &cpuset); /* cpu 0 is in cpuset now */
	/* bind process to processor 0 */
	if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) !=0) 
		perror("pthread_setaffinity_np");
	printf("Core 0 is running!\n");
	printf("result: %f\n", waste_time(5));
	/* waste some time so the work is visible with "top" */
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{ 
	pthread_t my_thread;
	time_t startwtime, endwtime;
	startwtime = time (NULL); 
	if (pthread_create(&my_thread, NULL, thread_func,NULL) != 0) {
		perror("pthread_create");
	}
	pthread_join(my_thread,NULL);
	endwtime = time (NULL);
	printf ("wall clock time = %d\n", (endwtime - startwtime));
	return 0;
}

