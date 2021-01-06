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


void	*Philosophers(void *idptr)
{
	t_philo		me;
	int			hands[2];

	me.id = (long)idptr;
	pthread_mutex_init(&(me.eat_locker), NULL);
	gettimeofday(&(me.last_meal), NULL);
	pthread_create(&(me.checker), NULL, liveness_thread, (void *)&me);
	while (1)
	{
		fork_available(me.id, hands);
		if (hands[0] && hands[1])
		{
			hands[0] = 0;
			hands[1] = 0;
			get_food(&me);
			go_sleep(me.id, me.last_meal);
		}
	}
	return (NULL);
}

void	logger(int id, int type)
{
	int				ms;
	struct	timeval	now;

	gettimeofday(&now, NULL);
	ms = get_ms_diff(&g_save, &now);
	pthread_mutex_lock(&g_logger_mutex);
	ft_putnbr(ms);
	write(1, " ", 1);
	ft_putnbr(id + 1);
	type == FORK ? write(1, " has taken a fork", 17) : 1;
	type == EATING ? write(1, " is eating", 10) : 1;
	type == SLEEP ? write(1, " is sleeping", 12) : 1;
	type == THINK ? write(1, " is thinking", 12) : 1;
	type == DIED ? write(1, " died", 5) : 1;
	write(1, "\n", 1);
	if (type != DIED)
		pthread_mutex_unlock(&g_logger_mutex);
}

void	init(pthread_t	**threads, pthread_t	*liveness_thread)
{
	g_forks = (int *)malloc(sizeof(int) * g_data[NPHILO]);
	g_eat_amount = (int *)malloc(sizeof(int) * g_data[NPHILO]);
	*threads = (pthread_t *)malloc(sizeof(pthread_t) * g_data[NPHILO]);
	memset(g_forks, 1,g_data[NPHILO] * sizeof(int));
	memset(g_eat_amount, 0,g_data[NPHILO] * sizeof(int));
	pthread_mutex_init(&g_logger_mutex, NULL);
	pthread_mutex_init(&g_forks_mutex, NULL);
	pthread_mutex_init(&g_join_mutex, NULL);
	pthread_mutex_lock(&g_join_mutex);
}

int main(int argc, char **argv)
{
	pthread_t	*threads;
	pthread_t	liveness_thread;

	if (error_input(argc, argv))
		return 1;
	g_data[NTPEAT] = -1;
	argc = 0;
	while (argv[++argc])
		g_data[argc - 1] = ft_atoi(argv[argc]);
	init(&threads, &liveness_thread);
	gettimeofday(&g_save, NULL);
	for (size_t i = 0; i < g_data[NPHILO]; i++)
	{
		pthread_create(&threads[i], NULL, Philosophers, (void *)i);
	}
	pthread_mutex_lock(&g_join_mutex);
}
