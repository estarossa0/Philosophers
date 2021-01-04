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
	struct timeval	last_meal;
	int				alive;
	int				id;
	int				hands[2];

	id = (long)idptr;
	gettimeofday(&last_meal, NULL);
	while (all_alive())
	{
		if (must_die(&last_meal))
		{
			g_liveness[id] = 0;
			logger(id, DIED);
			break ;
		}
		fork_available(id, hands);
		if (hands[0] && hands[1])
		{
			hands[0] = 0;
			hands[1] = 0;
			get_food(id, &last_meal);
			go_sleep(id, last_meal);
		}
	}
	return (NULL);
}

void	logger(int id, int type)
{
	int				ms;
	struct	timeval	now;

	ms = get_ms_diff(&g_save, &now);
	// printf("%d\n", ms);
	ft_putnbr(ms);
	write(1, " ", 1);
	ft_putnbr(id);
	type == FORK ? write(1, "has taken a fork", 16) : 1;
	type == EATING ? write(1, "is eating", 9) : 1;
	type == SLEEP ? write(1, " is sleeping", 12) : 1;
	type == THINK ? write(1, "is thinking", 11) : 1;
	type == DIED ? write(1, "died", 4) : 1;
}

int main(int argc, char **argv)
{
	pthread_t	*threads;
	// if (error_input(argc, argv))
	// 	return 1;
	g_data[NPHILO] = 2;
	g_data[TDIE] = 1000;
	g_data[TEAT] = 500;
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
	for (size_t i = 0; i < g_data[NPHILO]; i++)
	{
		g_forks[i] = 1;
		g_liveness[i] = 1;
	}
	gettimeofday(&g_save, NULL);
	for (size_t i = 0; i < g_data[NPHILO]; i++)
	{
		pthread_create(&threads[0], NULL, Philosophers, (void *)i);
	}
}
