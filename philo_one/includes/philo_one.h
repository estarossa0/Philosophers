#ifndef FT_PHILO_ONE_H
# define FT_PHILO_ONE_H
# define _DEFAULT_SOURCE
# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# define NPHILO	0
# define TDIE	1
# define TEAT	2
# define TSLEEP	3
# define NTPEAT	4
int		ft_atoi(const char *str);
int		g_data[5];
int		*g_liveness;
int		*g_forks;
#endif
