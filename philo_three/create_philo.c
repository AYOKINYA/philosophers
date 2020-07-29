/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/28 14:41:06 by jkang             #+#    #+#             */
/*   Updated: 2020/07/28 20:46:05 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	taken_fork_and_eat(t_vars *vars, t_philo *philo)
{
	if ((sem_wait(vars->pickup) == -1))
		ft_error("error: sem_wait\n", 1);
	if ((sem_wait(vars->forks) == -1))
		ft_error("error: sem_wait\n", 1);
	print_status(vars, philo, FORK_TAKEN, 0);
	if ((sem_wait(vars->forks) == -1))
		ft_error("error: sem_wait\n", 1);
	print_status(vars, philo, FORK_TAKEN, 0);
	if ((sem_post(vars->pickup) == -1))
		ft_error("error: sem_post\n", 1);
	if ((sem_wait(vars->eats) == -1))
		ft_error("error: sem_wait\n", 1);
	philo->last_eat_time = get_time();
	if ((sem_post(vars->eats) == -1))
		ft_error("error: sem_post\n", 1);
	print_status(vars, philo, EATING, 0);
	ft_usleep(vars->t_eat);
	if ((sem_wait(vars->putdown) == -1))
		ft_error("error: sem_wait\n", 1);
	if ((sem_post(vars->forks) == -1) || (sem_post(vars->forks) == -1))
		ft_error("error: sem_wait\n", 1);
	if ((sem_post(vars->putdown) == -1))
		ft_error("error: sem_post\n", 1);
	return ((++(philo->n_eat) == vars->n_must_eat) ? 0 : 1);
}

static void	*monitoring(void *v_philo)
{
	t_vars	*vars;
	t_philo	*philo;

	philo = (t_philo *)v_philo;
	vars = get_vars();
	while (1)
	{
		if ((sem_wait(vars->eats) == -1))
			ft_error("error: sem_wait\n", 1);
		if (get_time() - philo->last_eat_time > (unsigned)vars->t_die)
		{
			print_status(vars, philo, DIED, 1);
			exit(1);
		}
		if ((sem_post(vars->eats) == -1))
			ft_error("error: sem_wait\n", 1);
		ft_usleep(100);
	}
	return (philo);
}

static void	*philosophing(void *v_philo)
{
	t_vars	*vars;
	t_philo	*philo;

	vars = get_vars();
	philo = (t_philo *)v_philo;
	if (pthread_create(&philo->m_thread, NULL, &monitoring, philo) != 0)
		exit(ft_error("Error: can not create pthread", 1));
	if (pthread_detach(philo->m_thread) != 0)
		exit(ft_error("Error: can not deatch pthread", 1));
	while (1)
	{
		print_status(vars, philo, THINKING, 0);
		if (!taken_fork_and_eat(vars, philo))
			exit(0);
		print_status(vars, philo, SLEEPING, 0);
		ft_usleep(vars->t_sleep);
	}
	exit(0);
}

int			create_philo(t_vars *vars)
{
	unsigned long	start_time;
	int				i;

	if (!(vars->philo = (t_philo *)malloc(sizeof(t_philo) * vars->n_philo)))
		return (0);
	start_time = get_time();
	i = 0;
	while (i < vars->n_philo)
	{
		vars->philo[i].p_idx = i;
		vars->philo[i].n_eat = 0;
		vars->philo[i].start_time = start_time;
		vars->philo[i].last_eat_time = vars->philo[i].start_time;
		if ((vars->philo[i].pid = fork()) == 0)
			philosophing(&vars->philo[i]);
		++i;
	}
	return (1);
}
