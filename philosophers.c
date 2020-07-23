#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct  s_timeval {
    time_t      tv_sec; // 초 저장
    suseconds_t tv_usec; // 마이크로 초 저장
}               t_timeval;

typedef struct  s_args
{
    int         n_philosophers;
    int         t_eat;
    int         t_sleep;
    int         t_die;
    int         n_times_eat;
    int         n_forks;
}               t_args;

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

int     feed_argv(int argc, char **argv, t_args *info)
{
    info.n_philosophers = ft_atoi(argv[1]));
    info.t_die = ft_atoi(argv[2]);
    info.t_eat = ft_atoi(argv[3]);
    info.t_sleep = ft_atoi(argv[4]);
    if (argc == 6)
        info.n_times_eat = ft_atoi(argv[5]));
    else
        info.n_times_eat = 0;
    if (info.n_philosophers < 2 || info.n_philosophers > 100)
        return (0);
    if (info.t_die < 0 || info.t_eat < 0 || info.t_sleep < 0 || info.n_times_eat < 0)
        return (0);
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
        eat. During that time he will need to keep the two forks.
        - time_to_sleep: is in milliseconds and is the time the philosopher will spend
        sleeping.
        - number_of_times_each_philosopher_must_eat: argument is optional, if all
        philosophers eat at least ’number_of_times_each_philosopher_must_eat’ the
        simulation will stop. If not specified, the simulation will stop only at the death
        of a philosopher.
    */
    t_args  *info;

    if (argc != 5 && argc != 6)
        return (-1);
    if (!feed_argv(argc, argv, info))
        return (-1);
    return (0);
}