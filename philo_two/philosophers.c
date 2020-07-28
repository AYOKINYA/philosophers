/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/28 15:41:20 by jkang             #+#    #+#             */
/*   Updated: 2020/07/28 15:46:51 by jkang            ###   ########.fr       */
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
		if ((sem_wait(vars->eats) == -1))
			ft_error("error: sem_wait\n", 1);
		if (get_time() - philo->last_eat_time > (unsigned)vars->t_die)
		{
			print_status(vars, philo, DIED);
			if ((sem_wait(vars->someone_died) == -1))
				ft_error("error: sem_wait\n", 1);
			vars->flag_died = 1;
			if ((sem_post(vars->someone_died) == -1))
				ft_error("error: sem_post\n", 1);
		}
		if ((sem_post(vars->eats) == -1))
			ft_error("error: sem_wait\n", 1);
		ft_usleep(5);
	}
	return (philo);
}

int		main(int argc, char **argv)
{
	t_vars	*vars;

	vars = get_vars();
	if (!init(argc, argv) || !create_philo(vars))
		return (-1);
	while (1)
	{
		if ((sem_wait(vars->alive) == -1))
			return (free_all("error: sem_wait\n", -1));
		if (vars->n_alive == 0)
			break ;
		if ((sem_post(vars->alive) == -1))
			return (free_all("error: sem_post\n", -1));
		if ((sem_wait(vars->someone_died) == -1))
			return (free_all("error: sem_wait\n", -1));
		if (vars->flag_died == 1)
			return (free_all(0, 0));
		if ((sem_post(vars->someone_died) == -1))
			return (free_all("error: sem_post\n", -1));
		ft_usleep(5);
	}
	if (vars->flag_died == 0)
		ft_putstr_fd("Every philosopher ate enough!\n", 1);
	return (free_all(0, 0));
}
