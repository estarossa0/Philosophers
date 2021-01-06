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

void		fork_available(int id, int hands[2])
{
	int		fork_index[2];
	int		*pick;

	get_fork_indexs(id, fork_index);
	pick = hands[0] ? &hands[1] : &hands[0];
	pthread_mutex_lock(&g_forks_mutex);
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
	pthread_mutex_unlock(&g_forks_mutex);
}

void	get_food(t_philo *me)
{
	int		fork_index[2];

	pthread_mutex_lock(&(me->eat_locker));
	logger(me->id, EATING);
	me->eat_amount--;
	gettimeofday(&(me->last_meal), NULL);
	usleep(g_data[TEAT] * 1000);
	get_fork_indexs(me->id, fork_index);
	g_forks[fork_index[0]] = 1;
	g_forks[fork_index[1]] = 1;
	pthread_mutex_unlock(&(me->eat_locker));
}

void	go_sleep(int id, struct timeval lastmeal)
{
	logger(id, SLEEP);
	usleep(g_data[TSLEEP] * 1000);
}

void		*liveness_thread(void *ptr)
{
	t_philo		*me;

	me = (t_philo *)ptr;
	while (1)
	{
		pthread_mutex_lock(&(me->eat_locker));
		if (must_die(&(me->last_meal)))
		{
			logger(me->id, DIED);
			pthread_mutex_unlock(&g_join_mutex);
			break ;
		}
		pthread_mutex_unlock(&(me->eat_locker));
	}
	return (NULL);
}
