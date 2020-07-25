#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

struct	s_vars;

typedef struct  s_philo
{
	int         	position;
	int         	eat;
	int				n_eat_so_far;
	int         	sleep;
	int         	think;
	int         	die;
	int         	l_fork;
	int         	r_fork;
	struct s_vars	*vars;
	int				last_eat;
	int				limit;
	pthread_mutex_t	mutex;
}               t_philo;

typedef struct  	s_vars
{
	int         	n_philos;
	int         	t_eat;
	int         	t_sleep;
	int         	t_die;
	int         	n_must_eat;
	int         	n_forks;
	t_philo			*philos;
	pthread_mutex_t	*forks_mutex;
	int				start;
	int				last_eat;
	int				limit;
}               	t_vars;

int     ft_strlen(char *s)
{
	int count;

	count = 0;
	while (s[count] != '\0')
		++count;
	return (count);
}

void    ft_putstr(char *s)
{
	while (*s != '\0')
	{
		write(1, s, 1);
		++s;
	}
}

static int	ft_is_space(const char c)
{
	if (c == ' ' || c == '\f' || c == '\n' ||\
		c == '\r' || c == '\t' || c == '\v')
		return (1);
	return (0);
}

static int	ft_is_numeric(const char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int			ft_atoi(const char *str)
{
	int res;
	int i;
	int sign;

	res = 0;
	i = 0;
	sign = 1;
	while (str[i] != '\0' && ft_is_space(str[i]))
		++i;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = sign * -1;
		++i;
	}
	while (str[i] != '\0' && ft_is_numeric(str[i]))
	{
		res = res * 10 + (str[i] - 48);
		++i;
	}
	return (sign * res);
}

void	init_philos(t_vars *vars)
{
	int i;

	i = 0;
	while (i < vars->n_philos)
	{
		vars->philos[i].position = i;
		vars->philos[i].eat = 0;
		vars->philos[i].n_eat_so_far = 0;
		vars->philos[i].sleep = 0;
		vars->philos[i].think = 0;
		vars->philos[i].die = 0;
		vars->philos[i].l_fork = i;
		vars->philos[i].r_fork = (i + 1) % vars->n_philos;
		++i;
	}
}

int		init_mutexes(t_vars *vars)
{
	int i;

	if (!(vars->forks_mutex = (pthread_mutex_t *)malloc(sizeof(vars->forks_mutex) * vars->n_philos)))
		return (0);
	i = 0;
	while (i < vars->n_forks)
	{
		if (pthread_mutex_init(&vars->forks_mutex[i++], 0) == -1)
			return (0);
	}
	return (1);
}

int		init(int argc, char **argv, t_vars *vars)
{
	vars->n_philos = ft_atoi(argv[1]);
	vars->t_die = ft_atoi(argv[2]);
	vars->t_eat = ft_atoi(argv[3]);
	vars->t_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		vars->n_must_eat = ft_atoi(argv[5]);
	else
		vars->n_must_eat = 0;
	if (vars->n_philos < 2 || vars->n_philos > 100)
		return (0);
	if (vars->t_die < 0 || vars->t_eat < 0 || vars->t_sleep < 0 || vars->n_must_eat < 0)
		return (0);
	vars->n_forks = vars->n_philos;
	if (!(vars->philos = (t_philo *)malloc(sizeof(vars->philos) * vars->n_philos)))
		return (0);
	vars->forks_mutex = 0;
	init_philos(vars);
	if (!(init_mutexes(vars)))
		return (0);
	return (1);
}

int		get_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	*monitor_count(void *v_vars)
{
	t_vars	*vars;
	int 	i;
	int		total;

	vars = (vars *)v_vars;
	total = 0;
	while (total < vars->n_must_eat)
	{
		i = 0;
		while (i < vars->n_philos)
			pthread_mutex_lock(&vars->philos[i++].eat_m?);
		++total;
	}
	print_msg(&vars->philos[0], 3);
	pthread_mutex_unnlock(&somebody_dead_m);
	return (0);
}

void	*monitor(void *v_philo)
{
	t_philo	*philo;

	philo = (t_philo *)v_philo;
	while (1)
	{
		pthread_mutex_lock(&philo->mutex);
		if (!philo->eat && get_time() > philo->limit)
		{
			print_msg(philo);
			pthread_mutex_unlock(&philo->mutex);
			pthread_mutex_unlock(&philo->state->somebody_dead_m);
			return (0);
		}
		pthread_mutex_unlock(&philo->mutex);
		usleep(100);
	}
}

void	*routine(void *v_philo)
{
	t_philo			*philo;
	pthread_t		t_id;

	philo = (t_philo *)v_philo;
	philo->last_eat = get_time();
	philo->limit = philo->last_eat + philo->vars->t_die;
	if (pthread_create(&t_id, 0, &monitor, v_philo) != 0)
		return (0);
	pthread_detach(t_id);
	while (1)
	{
		take_forks(philo);
		eat(philo);
		clean_forks(philo);
		print_msg(philo);
	}
	return (0);
}

int		start_threads(t_vars *vars)
{
	int			i;
	pthread_t	t_id;
	void		*philo;

	vars->start = get_time();
	if (vars->n_must_eat > 0)
	{
		if (pthread_create(&t_id, 0, monitor_count, (void *)vars) != 0)
			return (0);
		pthread_detach(t_id);
	}
	i = 0;
	while (i < vars->n_philos)
	{
		philo = (void *)(vars->philos[i]);
		if (pthread_create(&t_id, 0, &routine, philo) != 0)
			return (0);
		pthread_detach(t_id);
		usleep(100);
		++i;
	}
	return (1);
}

int     main(int argc, char **argv)
{
	/*
		- number_of_philosophers: is the number of philosophers and also the number of forks
		- time_to_die: is in milliseconds, if a philosopher doesn’t start eating ’time_to_die’
		milliseconds after starting his last meal or the beginning of the simulation, it
		dies
		- time_to_eat: is in milliseconds and is the time it takes for a philosopher to
		eat-> During that time he will need to keep the two forks->
		- time_to_sleep: is in milliseconds and is the time the philosopher will spend
		sleeping->
		- number_of_times_each_philosopher_must_eat: argument is optional, if all
		philosophers eat at least ’number_of_times_each_philosopher_must_eat’ the
		simulation will stop-> If not specified, the simulation will stop only at the death
		of a philosopher->
	*/
	t_vars  *vars;

	if (argc != 5 && argc != 6)
		return (-1);
	vars = 0;
	if (!init(argc, argv, vars))
		return (-1);
	if (!start_threads(vars))
		return (-1);
	return (0);
}