/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/28 14:42:34 by jkang             #+#    #+#             */
/*   Updated: 2020/07/28 14:57:56 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_vars	*get_vars(void)
{
	static t_vars vars;

	return (&vars);
}

void	*monitoring(void *v_philo)
{
	t_vars	*vars;
	t_philo	*philo;

	philo = (t_philo *)v_philo;
	vars = get_vars();
	while (1)
	{
		pthread_mutex_lock(&vars->eats[philo->p_idx]);
		if (get_time() - philo->last_eat_time > (unsigned long)vars->t_die)
		{
			print_status(vars, philo, DIED);
			pthread_mutex_lock(&vars->someone_died);
			vars->flag_died = 1;
			pthread_mutex_unlock(&vars->someone_died);
		}
		pthread_mutex_unlock(&vars->eats[philo->p_idx]);
		ft_usleep(100);
	}
}

int		main(int argc, char **argv)
{
	t_vars	*vars;

	if (argc != 5 && argc != 6)
		return (-1);
	vars = get_vars();
	if (!init(argc, argv) || !create_philo(vars))
		return (-1);
	while (1)
	{
		pthread_mutex_lock(&vars->alive);
		if (vars->n_alive == 0)
			break ;
		pthread_mutex_unlock(&vars->alive);
		pthread_mutex_lock(&vars->someone_died);
		if (vars->flag_died == 1)
			return (free_all(0));
		pthread_mutex_unlock(&vars->someone_died);
		ft_usleep(5);
	}
	if (vars->flag_died == 0)
		ft_putstr("Every philosopher ate enough!\n");
	return (free_all(0));
}
