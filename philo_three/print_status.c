/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/28 14:41:37 by jkang             #+#    #+#             */
/*   Updated: 2020/07/28 16:53:38 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	print_status_body(t_vars *vars, t_philo *philo, char *phrase,\
															int exit_status)
{
	unsigned long	time;
	int				philo_no;

	philo_no = philo->p_idx + 1;
	time = get_time() - philo->start_time;
	if ((sem_wait(vars->print) == -1))
		ft_error("error: sem_wait\n", 1);
	ft_putnbr(time);
	ft_putstr_fd(" ", 1);
	ft_putnbr(philo_no);
	ft_putstr_fd(phrase, 1);
	free(phrase);
	if (exit_status == 0)
	{
		if ((sem_post(vars->print) == -1))
			ft_error("error: sem_post\n", 1);
	}
}

int			print_status(t_vars *vars, t_philo *philo, t_status status,\
														int exit_status)
{
	char	*phrase;

	phrase = 0;
	if (status == THINKING)
		phrase = ft_strdup(" is thinking\n");
	else if (status == EATING)
		phrase = ft_strdup(" is eating\n");
	else if (status == SLEEPING)
		phrase = ft_strdup(" is sleeping\n");
	else if (status == FORK_TAKEN)
		phrase = ft_strdup(" has taken a fork\n");
	else if (status == DIED)
		phrase = ft_strdup(" died\n");
	if (phrase == 0)
		return (0);
	print_status_body(vars, philo, phrase, exit_status);
	return (1);
}
