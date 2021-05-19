#include "philo_two.h"

void	*Philosophers(void *idptr)
{
	t_philo		me;
	char		name[15];

	me.id = (long)idptr;
	me.alive = 1;
	me.eat_amount = g_data[NTPEAT];
	ft_itoa(me.id + 1, name);
	sem_unlink(name);
	me.eat_locker = sem_open(name, O_CREAT, 0644, 1);
	gettimeofday(&(me.last_meal), NULL);
	pthread_create(&(me.checker), NULL, liveness_thread, (void *)&me);
	while (!g_stop_threads)
	{
		if (me.eat_amount == 0 && g_data[NTPEAT] != -1)
			break ;
		fork_available(me.id);
		get_food(&me);
	}
	me.alive = 0;
	g_eat_amount--;
	if (g_eat_amount == 0)
		g_stop_threads = 1;
	pthread_join(me.checker, NULL);
	sem_close(me.eat_locker);
	return (NULL);
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
	go_sleep(me->id);
}

void	go_sleep(int id)
{
	logger(id, SLEEP);
	usleep(g_data[TSLEEP] * 1000);
	logger(id, THINK);
}

void	*liveness_thread(void *ptr)
{
	t_philo		*me;

	me = (t_philo *)ptr;
	while (me->alive && !g_stop_threads)
	{
		sem_wait(me->eat_locker);
		if (must_die(&(me->last_meal)))
		{
			logger(me->id, DIED);
			me->alive = 0;
		}
		sem_post(me->eat_locker);
		usleep(100);
	}
	return (NULL);
}
