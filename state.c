/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-laf <aait-laf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:09:01 by aait-laf          #+#    #+#             */
/*   Updated: 2025/04/28 16:53:16 by aait-laf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


int    thinking_ph(t_elemt *philo, t_philo *p)
{
    philo->state = THINKING;

    if(p->flag_eat == 2)
        return(-1);
    printf("%lu\t%d\tis of Thinking\n", (get_time() - p->current) ,philo->idx_philo);
    usleep(1000);
    return(0);
}

int    eting_ph(t_elemt *philo, t_philo *p)
{
    philo->state = EATING;
    
    if(p->flag_current == 0)
    {
        p->current = get_time();
        p->flag_current = 1;
    }
    if(p->flag_eat  == 2)
        return(-1);
    pthread_mutex_lock(philo->left_fork);
    printf("%lu\t%d\thas tak left_fork\n", (get_time() - p->current), philo->idx_philo);
    
    pthread_mutex_lock(philo->right_fork);
    printf("%lu\t%d\thas tak right_fork\n", (get_time() - p->current), philo->idx_philo);
    printf("%lu\t%d\tPhilosofer is eating\n", (get_time() - p->current) ,philo->idx_philo);
    
    usleep(p->time_eat * 1000);
    
    // pthread_mutex_init(&philo->last_et, NULL);

    pthread_mutex_unlock(philo->left_fork);
    
    pthread_mutex_unlock(philo->right_fork);
    return(0);
    
}

int    sleeping_ph(t_elemt *philo, t_philo *p)
{
    philo->state = SLEEPING;

    if(p->flag_eat == 2)
        return(-1);
    printf("%lu\t%d\tis sleeping\n", (get_time() - p->current) ,philo->idx_philo);

    usleep(p->time_sleep * 1000);
    return(0);
}

