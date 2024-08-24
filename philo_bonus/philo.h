/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 17:18:51 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/24 04:14:58 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <fcntl.h>     /* macros */

# include <stdio.h>     /* printf */
# include <stdlib.h>    /* malloc, free, exit */
# include <string.h>    /* memset */
# include <unistd.h>    /* write, usleep */
# include <sys/time.h>  /* gettimeofday */
# include <sys/wait.h>  /* waitpid */
# include <semaphore.h> /* sem_{open,close,post,wait,unlink} */
# include <pthread.h>   /* pthread_{create,detach,join} */

# define INT_MAX 2147483647

typedef struct s_state	t_state;
typedef struct s_philo	t_philo;
typedef pthread_t		t_hread;

struct s_philo
{
	t_hread	thread;
	t_hread	end;

	char	*name;

	size_t	last_meal;
	size_t	meals_eaten;

	size_t	id;

	size_t	time_eat;
	size_t	time_sleep;
	size_t	alive;

	sem_t	*sem;
	sem_t	*forks;
};

struct s_state
{
	t_hread	thread;
	size_t	argc;

	size_t	num_philo;
	size_t	time_die;
	size_t	time_eat;
	size_t	time_sleep;
	size_t	eat_limit;

	size_t	err;

	sem_t	*print;
	sem_t	*end;
	sem_t	*main;

	t_philo	philo;
};

/* get.c */
size_t	get_time_now(void);
size_t	get_time(void);
void	get_input(int ac, char **av, t_state *state);
char	*get_semaphore_name(char *start, size_t id);

/* philo.c */
void	*patrol(void *ptr);
void	*end_patrol(void *ptr);
void	philo_routine(t_state *state);

/* main.c */
int		usage(char *name, int err);
int		main(int ac, char **av);

#endif
