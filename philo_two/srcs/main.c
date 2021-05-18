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
		go_sleep(me.id, me.last_meal);
	}
	me.alive = 0;
	g_eat_amount--;
	if (g_eat_amount == 0)
		g_stop_threads = 1;
	pthread_join(me.checker, NULL);
	sem_close(me.eat_locker);
	return (NULL);
}

void	logger(int id, int type)
{
	int				ms;
	struct timeval	now;

	gettimeofday(&now, NULL);
	ms = get_ms_diff(&g_save, &now);
	sem_wait(g_logger_sema);
	if (!g_stop_threads)
	{
		ft_putnbr(ms);
		write(1, " ", 1);
		ft_putnbr(id + 1);
		type == FORK ? write(1, " has taken a fork", 17) : 1;
		type == EATING ? write(1, " is eating", 10) : 1;
		type == SLEEP ? write(1, " is sleeping", 12) : 1;
		type == THINK ? write(1, " is thinking", 12) : 1;
		type == DIED ? write(1, " died", 5) : 1;
		write(1, "\n", 1);
	}
	if (type == DIED)
		g_stop_threads = 1;
	sem_post(g_logger_sema);
}

void	init(pthread_t	**threads)
{
	g_eat_amount = g_data[NPHILO];
	*threads = (pthread_t *)
		malloc(sizeof(pthread_t) * g_data[NPHILO]);
	sem_unlink("logger");
	g_logger_sema = sem_open("logger", O_CREAT, 0644, 1);
	g_stop_threads = 0;
	sem_unlink("forks");
	g_forks_sema = sem_open("forks", O_CREAT, 0664, g_data[NPHILO]);
}

void	clear(pthread_t	*threads)
{
	free(threads);
	sem_close(g_forks_sema);
	sem_close(g_logger_sema);
}

int	main(int argc, char **argv)
{
	pthread_t	*threads;
	size_t		i;

	i = -1;
	if (error_input(argc, argv))
		return (1);
	g_data[NTPEAT] = -1;
	argc = 0;
	while (argv[++argc])
		g_data[argc - 1] = ft_atoi(argv[argc]);
	init(&threads);
	gettimeofday(&g_save, NULL);
	while (++i < g_data[NPHILO])
	{
		pthread_create(&threads[i], NULL, Philosophers, (void *)i);
		usleep(100);
	}
	i = 0;
	while (i < g_data[NPHILO])
		pthread_join(threads[i++], NULL);
	clear(threads);
}
