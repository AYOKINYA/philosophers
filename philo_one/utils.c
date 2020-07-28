/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/28 14:42:41 by jkang             #+#    #+#             */
/*   Updated: 2020/07/28 15:01:03 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

unsigned long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int				ft_error(char *str)
{
	write(2, str, ft_strlen(str));
	return (0);
}

void			ft_usleep(unsigned long time)
{
	unsigned long	start;
	unsigned long	time_elapsed;

	start = get_time();
	while (1)
	{
		time_elapsed = get_time() - start;
		if (time_elapsed >= time)
			break ;
		usleep(1);
	}
}

static int		free_struct(void *s)
{
	if (s)
		free(s);
	s = 0;
	return (1);
}

int				free_all(int ret)
{
	t_vars	*vars;
	int		i;

	vars = get_vars();
	i = 0;
	while (i < vars->n_philo)
		pthread_mutex_destroy(&vars->forks[i++]);
	i = 0;
	while (i < vars->n_philo)
		pthread_mutex_destroy(&vars->eats[i++]);
	pthread_mutex_destroy(&vars->alive);
	pthread_mutex_destroy(&vars->print);
	pthread_mutex_destroy(&vars->someone_died);
	pthread_mutex_destroy(&vars->putdown);
	pthread_mutex_destroy(&vars->pickup);
	free_struct((void *)vars->philo);
	free_struct((void *)vars->eats);
	free_struct((void *)vars->forks);
	return (ret);
}
