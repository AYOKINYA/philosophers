/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/28 14:41:34 by jkang             #+#    #+#             */
/*   Updated: 2020/07/28 16:56:13 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>
# include <stdio.h>
# include <signal.h>

typedef struct		s_philo
{
	int				p_idx;
	pthread_t		m_thread;
	int				n_eat;
	unsigned long	start_time;
	unsigned long	last_eat_time;
	pid_t			pid;
}					t_philo;

typedef struct		s_vars
{
	int				n_philo;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				n_must_eat;
	int				n_alive;
	int				flag_died;
	t_philo			*philo;
	sem_t			*forks;
	sem_t			*eats;
	sem_t			*pickup;
	sem_t			*putdown;
	sem_t			*alive;
	sem_t			*print;
	sem_t			*print_error;
	sem_t			*someone_died;
}					t_vars;

typedef enum		e_status
{
	THINKING,
	EATING,
	SLEEPING,
	FORK_TAKEN,
	DIED
}					t_status;

int					ft_strlen(char *s);
void				ft_putchar(char c);
void				ft_putstr_fd(char *str, int fd);
void				ft_putnbr(long n);
char				*ft_strdup(const char *s);
int					ft_atoi(const char *str);

unsigned long		get_time(void);
int					ft_error(char *str, int ret);
void				ft_usleep(unsigned long time);
int					ft_unlink(int ret);
int					free_all(char *str, int ret);

int					init(int argc, char **argv);
int					create_philo(t_vars *vars);
int					print_status(t_vars *vars, t_philo *philo,\
									t_status status, int exit_status);
t_vars				*get_vars(void);

#endif
