/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anon <anon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 20:59:58 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/07/23 11:54:35 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>    /* printf */
# include <stdlib.h>   /* malloc, free */
# include <string.h>   /* memset */
# include <unistd.h>   /* write, usleep */
# include <sys/time.h> /* gettimeofday */
# include <pthread.h>  /* pthread_{ create, detach, join,
						 mutex_{ init, destroy, lock, unlock } } */

# define INT_MAX 2147483647
# define INT_MIN -2147483648

enum e_input
{
	IN_ARGC,
	IN_NUM_PHILO,
	IN_TIME_DIE,
	IN_TIME_EAT,
	IN_TIME_SLEEP,
	IN_NUM_EAT,
	IN_ERROR,
	IN_TIME,
	IN_LEN
};

typedef struct s_philo	t_philo;
struct s_philo
{
	pthread_t		thread;
	pthread_mutex_t	fork;
	size_t			time_last_meal;
	size_t			*input;
	int				meals_eaten;
	int				status; // -1: dead, 0: thinking, 1: eating, 2: sleeping
	int				id;
	t_philo			*next;
};

#endif
