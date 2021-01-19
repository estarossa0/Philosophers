#ifndef FT_PHILO_TWO_H
# define FT_PHILO_TWO_H
# define _DEFAULT_SOURCE
# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <string.h>
# include <semaphore.h>
# include <fcntl.h>
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
typedef	struct	s_philo
{
	int				id;
	struct	timeval	last_meal;
	pthread_t		checker;
	sem_t			*eat_locker;
	int				eat_amount;
	short			alive;
}				t_philo;
int				ft_atoi(const char *str);
void			ft_putnbr(int n);
void			*liveness_thread(void *ptr);
void			go_sleep(int id, struct timeval lastmeal);
void			get_food(t_philo *me);
void			fork_available(int id, int hands[2]);
int				get_ms_diff(struct timeval *a, struct timeval *b);
void			logger(int id, int type);
char			*ft_itoa(int n, char result[15]);
int				g_data[5];
int				g_eat_amount;
short			g_stop_threads;
struct	timeval	g_save;
sem_t			*g_logger_sema;
sem_t			*g_forks_sema;
#endif
