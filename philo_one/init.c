/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/28 14:42:29 by jkang             #+#    #+#             */
/*   Updated: 2020/07/28 14:59:42 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	init_mutexes_two(t_vars *vars)
{
	if (pthread_mutex_init(&vars->pickup, NULL))
		return (0);
	if (pthread_mutex_init(&vars->putdown, NULL))
		return (0);
	if (pthread_mutex_init(&vars->alive, NULL))
		return (0);
	if (pthread_mutex_init(&vars->print, NULL))
		return (0);
	if (pthread_mutex_init(&vars->someone_died, NULL))
		return (0);
	return (1);
}

static int	init_mutexes(t_vars *vars)
{
	int i;

	vars->n_alive = vars->n_philo;
	if (!(vars->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) *\
															vars->n_philo)))
		return (0);
	if (!(vars->eats = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) *\
															vars->n_philo)))
		return (0);
	i = 0;
	while (i < vars->n_philo)
		if (pthread_mutex_init(&vars->forks[i++], NULL))
			return (0);
	i = 0;
	while (i < vars->n_philo)
		if (pthread_mutex_init(&vars->eats[i++], NULL))
			return (0);
	if (!init_mutexes_two(vars))
		return (0);
	return (1);
}

int			init(int argc, char **argv)
{
	t_vars	*vars;

	vars = get_vars();
	vars->n_philo = ft_atoi(argv[1]);
	vars->t_die = ft_atoi(argv[2]);
	vars->t_eat = ft_atoi(argv[3]);
	vars->t_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		if ((vars->n_must_eat = ft_atoi(argv[5])) <= 0)
			return (0);
	}
	else
		vars->n_must_eat = -1;
	if (vars->n_philo < 2)
		return (0);
	if (vars->t_die < 0 || vars->t_eat < 0 || vars->t_sleep < 0)
		return (0);
	if (!(init_mutexes(vars)))
		return (0);
	return (1);
}
