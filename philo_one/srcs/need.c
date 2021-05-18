#include "philo_one.h"

int	error_input(int argc, char **argv)
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
	write(STDIN_FILENO, "Error input\n", 12 * error);
	return (error);
}

int	get_ms_diff(struct timeval *a, struct timeval *b)
{
	int		sec;
	int		u_sec;
	int		ms_result;

	sec = b->tv_sec - a->tv_sec;
	u_sec = b->tv_usec - a->tv_usec;
	sec *= 1e6;
	u_sec = sec + u_sec;
	ms_result = u_sec / 1000;
	return (ms_result);
}

int	must_die(struct timeval *last_meal)
{
	struct timeval	curr_time;
	int				ms_diff;

	gettimeofday(&curr_time, NULL);
	ms_diff = get_ms_diff(last_meal, &curr_time);
	if (ms_diff >= g_data[TDIE])
		return (1);
	return (0);
}

void	get_fork_indexs(int id, int fork_index[2])
{
	if (id == 0)
		fork_index[0] = g_data[NPHILO] - 1;
	else
		fork_index[0] = id - 1;
	fork_index[1] = id;
}

void	fork_available(int id)
{
	int		fork_index[2];

	get_fork_indexs(id, fork_index);
	pthread_mutex_lock(&g_forks[fork_index[0]]);
	logger(id, FORK);
	pthread_mutex_lock(&g_forks[fork_index[1]]);
	logger(id, FORK);
}
