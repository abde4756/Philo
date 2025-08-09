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
    pthread_mutex_lock(&philo->last_et);
    philo->state = THINKING;
    pthread_mutex_unlock(&philo->last_et);

    if(check_death(p))
        return(-1);
    printf("%lu %d is thinking\n", (get_time() - p->current), philo->idx_philo);
    return(0);
}

int    eting_ph(t_elemt *philo, t_philo *p)
{
    pthread_mutex_lock(&philo->last_et);
    philo->state = HUNGRY;
    pthread_mutex_unlock(&philo->last_et);
    
    // Vérifier mort avant toute action
    if(check_death(p))
        return(-1);

    // CAS SPÉCIAL : 1 seul philosophe
    if(p->nbr_philo == 1)
    {
        printf("%lu %d has taken a fork\n", (get_time() - p->current), philo->idx_philo);
        usleep(p->time_to_die * 1000);
        // printf("%d %d died\n", p->time_to_die, philo->idx_philo);
        return(-1);
    }

    if(philo->idx_philo % 2 == 0)
    {
        pthread_mutex_lock(philo->left_fork);
        if(check_death(p)) {
            pthread_mutex_unlock(philo->left_fork);
            return(-1);
        }
        printf("%lu %d has taken a fork\n", (get_time() - p->current), philo->idx_philo);
        
        pthread_mutex_lock(philo->right_fork);
        if(check_death(p)) {
            pthread_mutex_unlock(philo->left_fork);
            pthread_mutex_unlock(philo->right_fork);
            return(-1);
        }
        printf("%lu %d has taken a fork\n", (get_time() - p->current), philo->idx_philo);
    }
    else
    {
        pthread_mutex_lock(philo->right_fork);
        if(check_death(p)) {
            pthread_mutex_unlock(philo->right_fork);
            return(-1);
        }
        printf("%lu %d has taken a fork\n", (get_time() - p->current), philo->idx_philo);
        
        pthread_mutex_lock(philo->left_fork);
        if(check_death(p)) {
            pthread_mutex_unlock(philo->left_fork);
            pthread_mutex_unlock(philo->right_fork);
            return(-1);
        }
        printf("%lu %d has taken a fork\n", (get_time() - p->current), philo->idx_philo);
    }
    
    // Vérifier une dernière fois avant de manger
    if(check_death(p)) {
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
        return(-1);
    }
    
    // Maintenant on peut manger
    pthread_mutex_lock(&philo->last_et);
    philo->state = EATING;
    pthread_mutex_unlock(&philo->last_et);
    pthread_mutex_lock(&philo->eat_mutex[philo->idx_philo - 1]);
    philo->last_eat = get_time();
    pthread_mutex_unlock(&philo->eat_mutex[philo->idx_philo - 1]);
    printf("%lu %d is eating\n", (get_time() - p->current), philo->idx_philo);
    
    usleep(p->time_eat * 1000);

    // NOUVEAU: Incrémenter le compteur de repas après avoir mangé
    pthread_mutex_lock(&philo->eat_mutex[philo->idx_philo - 1]);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->eat_mutex[philo->idx_philo - 1]);

    // Vérifier mort après avoir mangé
    if(check_death(p)) {
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
        return(-1);
    }

    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
    return(0);
    
}

int    sleeping_ph(t_elemt *philo, t_philo *p)
{
    pthread_mutex_lock(&philo->last_et);
    philo->state = SLEEPING;
    pthread_mutex_unlock(&philo->last_et);

    if(check_death(p))
        return(-1);
    printf("%lu %d is sleeping\n", (get_time() - p->current), philo->idx_philo);

    usleep(p->time_sleep * 1000);
    return(0);
}

