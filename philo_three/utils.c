/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/28 14:41:39 by jkang             #+#    #+#             */
/*   Updated: 2020/07/28 16:52:58 by jkang            ###   ########.fr       */
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

int				ft_error(char *str, int ret)
{
	t_vars *vars;

	vars = get_vars();
	sem_wait(vars->print_error);
	write(2, str, ft_strlen(str));
	sem_post(vars->print_error);
	return (ret);
}

void			ft_usleep(unsigned long time)
{
	unsigned long start;
	unsigned long time_elapsed;

	start = get_time();
	while (1)
	{
		time_elapsed = get_time() - start;
		if (time_elapsed >= time)
			break ;
		usleep(1);
	}
}

int				ft_unlink(int ret)
{
	if ((sem_unlink("/forks") == -1))
		ft_error("error: sem_unlink\n", 1);
	if ((sem_unlink("/eats") == -1))
		ft_error("error: sem_unlink\n", 1);
	if ((sem_unlink("/pickup") == -1))
		ft_error("error: sem_unlink\n", 1);
	if ((sem_unlink("/putdown") == -1))
		ft_error("error: sem_unlink\n", 1);
	if ((sem_unlink("/print") == -1))
		ft_error("error: sem_unlink\n", 1);
	if ((sem_unlink("/print_error") == -1))
		ft_error("error: sem_unlink\n", 1);
	if ((sem_unlink("/alive") == -1))
		ft_error("error: sem_unlink\n", 1);
	if ((sem_unlink("/someone_died") == -1))
		ft_error("error: sem_unlink\n", 1);
	return (ret);
}

int				free_all(char *str, int ret)
{
	t_vars	*vars;

	if (str != 0)
		ft_putstr_fd(str, 2);
	vars = get_vars();
	sem_close(vars->forks);
	sem_close(vars->eats);
	sem_close(vars->pickup);
	sem_close(vars->putdown);
	sem_close(vars->alive);
	sem_close(vars->print);
	sem_close(vars->someone_died);
	sem_close(vars->print_error);
	ft_unlink(0);
	free(vars->philo);
	return (ret);
}
