#include "philo_one.h"

int error_input(int argc, char **argv)
{
	int		error;
	int		index;
	int		jndex;

	error = 0;
	index = 1;
	jndex = 0;
	if (argc != 5 && argc != 6)
		error = 1;
	else
	{
		while (argv[index] && argv[index][jndex] && !error)
		{
			if (argv[index][jndex] < '0' || argv[index][jndex] > '9')
				error = 1;
			jndex++;
			if (argv[index][jndex] == '\0')
			{
				index++;
				jndex = 0;
			}
		}
	}
	error ? write(STDIN_FILENO, "Error input\n", 12) : 1;
	return error;
}

int		get_ms_diff(struct timeval *a, struct timeval *b)
{
	int		sec;
	int		u_sec;
	int		ms_result;

	sec = b->tv_sec - a->tv_sec;
	u_sec = b->tv_usec - a->tv_usec;
	sec *= 1e6;
	u_sec = sec + u_sec;
	ms_result = u_sec/1000;
	return ms_result;
}

int		must_die(struct timeval *last_meal)
{
	struct	timeval	curr_time;
	int				ms_diff;

	gettimeofday(&curr_time, NULL);
	ms_diff = get_ms_diff(last_meal, &curr_time);
	if (ms_diff >= g_data[TDIE])
		return 1;
	return 0;
}

void	get_fork_indexs(int id,int fork_index[2])
{
	id--;
	fork_index[0] = (id == 0) ? g_data[NPHILO] - 1 : id - 1;
	fork_index[1] = (id == g_data[NPHILO] - 1) ? 0 : id + 1;
}

int		fork_available(int id)
{
	int		fork_index[2];

	get_fork_indexs(id, fork_index);
	printf("<%d %d>", fork_index[0], fork_index[1]);
	if (fork_index[0] != fork_index[1] &&
	g_forks[fork_index[0]] && g_forks[fork_index[1]])
	{
		g_forks[fork_index[0]] = 0;
		g_forks[fork_index[1]] = 0;
		return 1;
	}
	return 0;
}

void	get_food(int id, struct timeval *last_meal)
{
	int		fork_index[2];

	gettimeofday(last_meal, NULL);
	usleep(g_data[TEAT]);
	get_fork_indexs(id, fork_index);
	g_forks[fork_index[0]] = 1;
	g_forks[fork_index[1]] = 1;
}

int		go_sleep(struct timeval lastmeal)
{
	int		sleep_amount;

	sleep_amount = g_data[TSLEEP];
	usleep(sleep_amount);
	return 1;
}

int		all_alive()
{
	int		index;

	index = 0;
	while (index < g_data[NPHILO])
	{
		if (g_liveness[index] == 0)
			return (0);
		index++;
	}
	return (1);
}

void	*Philosophers(void *idptr)
{
	struct timeval	last_meal;
	int				alive;
	int				id;

	id = (long)idptr;
	gettimeofday(&last_meal, NULL);
	while (all_alive())
	{
		if (must_die(&last_meal))
		{
			g_liveness[id - 1] = 0;
			printf("%d dead\n", id);
			break ;
		}
		else if (fork_available(id))
		{
			write(1, "v", 1);
			get_food(id, &last_meal);
			go_sleep(last_meal);
		}
	}
	return (NULL);
}

int main(int argc, char **argv)
{
	pthread_t	*threads;
	// if (error_input(argc, argv))
	// 	return 1;
	g_data[NPHILO] = 2;
	g_data[TDIE] = 1000;
	g_data[TEAT] = 3000;
	g_data[TSLEEP] = 1000;
	g_data[NTPEAT] = -1;
	// while (argc)
	// {
	// 	g_data[argc - 1] = ft_atoi(argv[argc - 1]);
	// 	argc--;
	// }
	g_forks = (int *)malloc(sizeof(int) * g_data[NPHILO]);
	g_liveness = (int *)malloc(sizeof(int) * g_data[NPHILO]);
	threads = (pthread_t *)malloc(sizeof(pthread_t) * g_data[NPHILO]);
	g_forks[0] = 1;
	g_liveness[0] = 1;
	g_liveness[1] = 1;
	g_forks[1] = 1;
	pthread_create(&threads[0], NULL, Philosophers, (void *)1);
	pthread_create(&threads[1], NULL, Philosophers, (void *)2);
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
}
