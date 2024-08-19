/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 17:18:51 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/19 17:34:03 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>    /* printf */
# include <stdlib.h>   /* malloc, free */
# include <string.h>   /* memset */
# include <unistd.h>   /* write, usleep */
# include <sys/time.h> /* gettimeofday */

# include <pthread.h>
/* pthread_{create,detach,join,mutex_{init,destroy,lock,unlock}} */

# define INT_MAX 2147483647

typedef struct s_state	t_state;
typedef struct s_philo	t_philo;
typedef pthread_mutex_t	t_mutex;
typedef pthread_t		t_hread;

struct s_philo
{
	t_hread	thread;
	t_mutex	mutex;

	size_t	last_meal;
	size_t	meals_eaten;

	size_t	id;

	size_t	time_eat;
	size_t	time_sleep;

	size_t	fork;
	size_t	in_hand;

	size_t	thinking;
	size_t	eating;
	size_t	sleeping;
	size_t	dead;

	t_philo	*next;
	t_state	*state;
};

struct s_state
{
	size_t	argc;

	size_t	num_philo;
	size_t	time_die;
	size_t	time_eat;
	size_t	time_sleep;
	size_t	eat_limit;

	size_t	err;

	t_philo	*philos;
};

/* get.c */
size_t	get_time_now(void);
size_t	get_time(void);
void	get_input(int ac, char **av, t_state *state);

/* philo.c */
t_philo	*philo_init(t_state *state);
void	*philo_routine(void *ptr);

/* main.c */
int		usage(char *name);
int		main(int ac, char **av);
void	check_watchphilos(t_state *state);

#endif
