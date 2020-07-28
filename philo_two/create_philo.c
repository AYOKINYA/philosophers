/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/28 15:41:10 by jkang             #+#    #+#             */
/*   Updated: 2020/07/28 16:27:11 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	taken_fork_and_eat(t_vars *vars, t_philo *philo)
{
	if ((sem_wait(vars->pickup) == -1))
		ft_error("error: sem_wait\n", 1);
	if ((sem_wait(vars->forks) == -1))
		ft_error("error: sem_wait\n", 1);
	print_status(vars, philo, FORK_TAKEN);
	if ((sem_wait(vars->forks) == -1))
		ft_error("error: sem_wait\n", 1);
	print_status(vars, philo, FORK_TAKEN);
	if ((sem_post(vars->pickup) == -1))
		ft_error("error: sem_post\n", 1);
	if ((sem_wait(vars->eats) == -1))
		ft_error("error: sem_wait\n", 1);
	philo->last_eat_time = get_time();
	if ((sem_post(vars->eats) == -1))
		ft_error("error: sem_post\n", 1);
	print_status(vars, philo, EATING);
	ft_usleep(vars->t_eat);
	if ((sem_wait(vars->putdown) == -1))
		ft_error("error: sem_wait\n", 1);
	if ((sem_post(vars->forks) == -1) || (sem_post(vars->forks) == -1))
		ft_error("error: sem_wait\n", 1);
	if ((sem_post(vars->putdown) == -1))
		ft_error("error: sem_post\n", 1);
	return ((++(philo->n_eat) == vars->n_must_eat) ? 0 : 1);
}

static void	*philosophing(void *v_philo)
{
	t_vars	*vars;
	t_philo	*philo;

	vars = get_vars();
	philo = (t_philo *)v_philo;
	while (1)
	{
		print_status(vars, philo, THINKING);
		if (!taken_fork_and_eat(vars, philo))
			break ;
		print_status(vars, philo, SLEEPING);
		ft_usleep(vars->t_sleep);
	}
	if ((sem_wait(vars->alive) == -1))
		ft_error("error: sem_wait\n", 1);
	vars->n_alive--;
	if ((sem_post(vars->alive) == -1))
		ft_error("error: sem_post\n", 1);
	return (philo);
}

static int	create_philo_even(t_vars *vars, unsigned long start_time)
{
	int	i;
	int	j;

	i = 0;
	while ((j = 2 * i) < vars->n_philo)
	{
		vars->philo[j].p_idx = j;
		vars->philo[j].n_eat = 0;
		vars->philo[j].start_time = start_time;
		vars->philo[j].last_eat_time = vars->philo[j].start_time;
		if (pthread_create(&vars->philo[j].thread, NULL, &philosophing,\
														&vars->philo[j]))
			return (ft_error("Error: can not create pthread", 0));
		if (pthread_detach((vars->philo[j].thread)))
			return (ft_error("Error: can not deatch pthread", 0));
		if (pthread_create(&vars->philo[j].m_thread, NULL, &monitoring,\
														&vars->philo[j]))
			return (ft_error("Error: can not create pthread", 0));
		if (pthread_detach((vars->philo[j].m_thread)))
			return (ft_error("Error: can not deatch pthread", 0));
		++i;
	}
	return (1);
}

static int	create_philo_odd(t_vars *vars, unsigned long start_time)
{
	int	i;
	int	j;

	i = 0;
	while ((j = 2 * i + 1) < vars->n_philo)
	{
		vars->philo[j].p_idx = j;
		vars->philo[j].n_eat = 0;
		vars->philo[j].start_time = start_time;
		vars->philo[j].last_eat_time = vars->philo[j].start_time;
		if (pthread_create(&vars->philo[j].thread, NULL, &philosophing,\
														&vars->philo[j]))
			return (ft_error("Error: can not create pthread", 0));
		if (pthread_detach((vars->philo[j].thread)))
			return (ft_error("Error: can not deatch pthread", 0));
		if (pthread_create(&vars->philo[j].m_thread, NULL, &monitoring,\
														&vars->philo[j]))
			return (ft_error("Error: can not create pthread", 0));
		if (pthread_detach((vars->philo[j].m_thread)))
			return (ft_error("Error: can not deatch pthread", 0));
		++i;
	}
	return (1);
}

int			create_philo(t_vars *vars)
{
	unsigned long	start_time;

	if (!(vars->philo = (t_philo *)malloc(sizeof(t_philo) * vars->n_philo)))
		return (0);
	start_time = get_time();
	if (!create_philo_even(vars, start_time))
		return (0);
	if (!create_philo_odd(vars, start_time))
		return (0);
	return (1);
}
