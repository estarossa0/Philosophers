#include "philo_one.h"

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
	fork_index[0] = (id == 0) ? g_data[NPHILO] - 1 : id - 1;
	fork_index[1] = id;
}

int		fork_available(int id, int hands[2])
{
	int		fork_index[2];
	int		*pick;

	get_fork_indexs(id, fork_index);
	pick = hands[0] ? &hands[1] : &hands[0];
	if (fork_index[0] == fork_index[1])
		return 0;
	if (g_forks[fork_index[0]])
	{
		g_forks[fork_index[0]] = 0;
		*pick = 1;
		logger(id, FORK);
	}
	else if (g_forks[fork_index[1]])
	{
		g_forks[fork_index[1]] = 0;
		*pick = 1;
		logger(id, FORK);
	}
	return (*pick);
}

void	get_food(int id, struct timeval *last_meal)
{
	int		fork_index[2];

	logger(id, EATING);
	gettimeofday(last_meal, NULL);
	usleep(g_data[TEAT] * 1000);
	get_fork_indexs(id, fork_index);
	g_forks[fork_index[0]] = 1;
	g_forks[fork_index[1]] = 1;
}

int		go_sleep(int id, struct timeval lastmeal)
{
	int		sleep_amount;

	logger(id, SLEEP);
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
