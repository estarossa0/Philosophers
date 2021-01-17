#ifndef FT_PHILO_ONE_H
# define FT_PHILO_ONE_H
# define _DEFAULT_SOURCE
# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <string.h>
# define NPHILO	0
# define TDIE	1
# define TEAT	2
# define TSLEEP	3
# define NTPEAT	4
# define FORK	0
# define EATING	1
# define SLEEP	2
# define THINK	3
# define DIED	4
typedef	struct	s_philo	t_philo;
int				ft_atoi(const char *str);
void			ft_putnbr(int n);
void			*liveness_thread(void *ptr);
void			go_sleep(int id, struct timeval lastmeal);
void			get_food(t_philo *me);
void			fork_available(int id, int hands[2]);
int				get_ms_diff(struct timeval *a, struct timeval *b);
void			logger(int id, int type);
int				g_data[5];
int				*g_forks;
int				g_eat_amount;
struct	timeval	g_save;
pthread_mutex_t	g_logger_mutex;
pthread_mutex_t	g_forks_mutex;
pthread_mutex_t	g_join_mutex;
typedef	struct	s_philo
{
	int				id;
	struct	timeval	last_meal;
	pthread_t		checker;
	pthread_mutex_t	eat_locker;
	int				eat_amount;
}				t_philo;
#endif
