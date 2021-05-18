#include "philo_one.h"

void	logger(int id, int type)
{
	int				ms;
	struct timeval	now;

	gettimeofday(&now, NULL);
	ms = get_ms_diff(&g_save, &now);
	pthread_mutex_lock(&g_logger_mutex);
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
	pthread_mutex_unlock(&g_logger_mutex);
}

void	init(pthread_t	**threads)
{
	int	i;

	i = 0;
	g_forks = (pthread_mutex_t *)
		malloc(sizeof(pthread_mutex_t) * g_data[NPHILO]);
	g_eat_amount = g_data[NPHILO];
	*threads = (pthread_t *)malloc(sizeof(pthread_t) * g_data[NPHILO]);
	pthread_mutex_init(&g_logger_mutex, NULL);
	while (i < g_data[NPHILO])
		pthread_mutex_init(&g_forks[i++], NULL);
	g_stop_threads = 0;
}

void	clear(pthread_t	*threads)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&g_logger_mutex);
	while (i < g_data[NPHILO])
		pthread_mutex_destroy(&g_forks[i++]);
	free(threads);
	free(g_forks);
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
