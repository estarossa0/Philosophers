#include "philo_two.h"

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

void		fork_available(int id, int hands[2])
{
	int		*pick;

	pick = hands[0] ? &hands[1] : &hands[0];
	sem_wait(g_forks_sema);
	*pick = 1;
	logger(id, FORK);
}

void	get_food(t_philo *me)
{
	sem_wait(me->eat_locker);
	logger(me->id, EATING);
	me->eat_amount--;
	gettimeofday(&(me->last_meal), NULL);
	usleep(g_data[TEAT] * 1000);
	sem_post(g_forks_sema);
	sem_post(g_forks_sema);
	sem_post(me->eat_locker);
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
		sem_wait(me->eat_locker);
		if (must_die(&(me->last_meal)))
		{
			logger(me->id, DIED);
			sem_post(g_join_sema);
			break ;
		}
		sem_post(me->eat_locker);
	}
	return (NULL);
}
