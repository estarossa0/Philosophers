#include "philo_one.h"

void	*Philosophers(void *idptr)
{
	t_philo		me;

	me.id = (long)idptr;
	me.alive = 1;
	me.eat_amount = g_data[NTPEAT];
	pthread_mutex_init(&(me.eat_locker), NULL);
	gettimeofday(&(me.last_meal), NULL);
	pthread_create(&(me.checker), NULL, liveness_thread, (void *)&me);
	while (!g_stop_threads)
	{
		if (me.eat_amount == 0 && g_data[NTPEAT] != -1)
			break ;
		fork_available(me.id);
		get_food(&me);
		go_sleep(me.id);
	}
	me.alive = 0;
	g_eat_amount--;
	if (g_eat_amount == 0)
		g_stop_threads = 1;
	pthread_join(me.checker, NULL);
	pthread_mutex_destroy(&(me.eat_locker));
	return (NULL);
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
	pthread_mutex_unlock(&g_forks[fork_index[0]]);
	pthread_mutex_unlock(&g_forks[fork_index[1]]);
	pthread_mutex_unlock(&(me->eat_locker));
}

void	go_sleep(int id)
{
	logger(id, SLEEP);
	usleep(g_data[TSLEEP] * 1000);
}

void	*liveness_thread(void *ptr)
{
	t_philo		*me;

	me = (t_philo *)ptr;
	while (me->alive && !g_stop_threads)
	{
		usleep(100);
		pthread_mutex_lock(&(me->eat_locker));
		if (must_die(&(me->last_meal)))
		{
			logger(me->id, DIED);
			me->alive = 0;
		}
		pthread_mutex_unlock(&(me->eat_locker));
	}
	return (NULL);
}
