/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/28 15:41:04 by jkang             #+#    #+#             */
/*   Updated: 2020/07/28 15:47:02 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	clean_shm(void)
{
	sem_unlink("/forks");
	sem_unlink("/eats");
	sem_unlink("/pickup");
	sem_unlink("/putdown");
	sem_unlink("/print");
	sem_unlink("/alive");
	sem_unlink("/someone_died");
	sem_unlink("/print_error");
}

static int	init_semaphore(t_vars *vars)
{
	vars->n_alive = vars->n_philo;
	clean_shm();
	if ((vars->forks = sem_open("/forks", O_CREAT, 0660, vars->n_philo))\
	== SEM_FAILED)
		return (0);
	if ((vars->eats = sem_open("/eats", O_CREAT, 0660, vars->n_philo)) ==\
	SEM_FAILED)
		return (0);
	if ((vars->pickup = sem_open("/pickup", O_CREAT, 0660, 1)) == SEM_FAILED)
		return (0);
	if ((vars->putdown = sem_open("/putdown", O_CREAT, 0660, 1)) == SEM_FAILED)
		return (0);
	if ((vars->print = sem_open("/print", O_CREAT, 0660, 1)) == SEM_FAILED)
		return (0);
	if ((vars->alive = sem_open("/alive", O_CREAT, 0660, 1)) == SEM_FAILED)
		return (0);
	if ((vars->someone_died = sem_open("/someone_died", O_CREAT, 0660, 1)) ==\
	SEM_FAILED)
		return (0);
	if ((vars->print_error = sem_open("/print_error", O_CREAT, 0660, 1))\
	== SEM_FAILED)
		return (0);
	return (1);
}

int			init(int argc, char **argv)
{
	t_vars	*vars;

	if (argc != 5 && argc != 6)
		return (0);
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
	if (!(init_semaphore(vars)))
		return (0);
	return (1);
}
