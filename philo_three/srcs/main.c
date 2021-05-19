#include "philo_three.h"

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
		print_state(type);
		write(1, "\n", 1);
	}
	if (type == DIED)
		exit(1);
	sem_post(g_logger_sema);
}

void	init(pid_t	**pids)
{
	g_eat_amount = g_data[NPHILO];
	*pids = (pid_t *)malloc(sizeof(pid_t) * g_data[NPHILO]);
	sem_unlink("logger");
	g_logger_sema = sem_open("logger", O_CREAT, 0644, 1);
	g_stop_threads = 0;
	sem_unlink("forks");
	g_forks_sema = sem_open("forks", O_CREAT, 0664, g_data[NPHILO]);
}

void	clear(pid_t	*pids)
{
	free(pids);
	sem_close(g_forks_sema);
	sem_close(g_logger_sema);
}

void	join_proc(pid_t *pids)
{
	int	status;
	int	i;

	i = 0;
	while (i < g_data[NPHILO])
	{
		waitpid(0, &status, 0);
		if (WEXITSTATUS(status))
		{
			i = 0;
			while (i < g_data[NPHILO])
				kill(pids[i++], SIGINT);
			break ;
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	pid_t	*pids;
	size_t	i;

	i = 0;
	if (error_input(argc, argv))
		return (1);
	g_data[NTPEAT] = -1;
	argc = 0;
	while (argv[++argc])
		g_data[argc - 1] = ft_atoi(argv[argc]);
	init(&pids);
	gettimeofday(&g_save, NULL);
	while ((int)i < g_data[NPHILO])
	{
		pids[i] = fork();
		if (pids[i] == -1)
			exit(1);
		if (pids[i] == 0)
			Philosophers((void *)i);
		i++;
	}
	join_proc(pids);
	clear(pids);
}
