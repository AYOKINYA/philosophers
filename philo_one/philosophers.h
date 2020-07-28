/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/28 15:44:28 by jkang             #+#    #+#             */
/*   Updated: 2020/07/28 16:43:58 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdio.h>

typedef struct		s_philo
{
	int				p_idx;
	pthread_t		thread;
	pthread_t		m_thread;
	int				n_eat;
	unsigned long	start_time;
	unsigned long	last_eat_time;
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
	pthread_mutex_t	*forks;
	pthread_mutex_t	*eats;
	pthread_mutex_t	pickup;
	pthread_mutex_t	putdown;
	pthread_mutex_t	alive;
	pthread_mutex_t	print;
	pthread_mutex_t	someone_died;
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
void				ft_putstr(char *str);
void				ft_putnbr(long n);
char				*ft_strdup(const char *s);
int					ft_atoi(const char *str);

unsigned long		get_time(void);
int					ft_error(char *str);
void				ft_usleep(unsigned long time);
int					free_all(int ret);

int					init(int argc, char **argv);
int					create_philo(t_vars *vars);
int					print_status(t_vars *vars, t_philo *philo, t_status status);
t_vars				*get_vars(void);
void				*monitoring(void *v_philo);

#endif
