/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-laf <aait-laf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:16:14 by aait-laf          #+#    #+#             */
/*   Updated: 2025/04/28 16:51:32 by aait-laf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void    *routine(void *t)
{
    t_elemt *p = (t_elemt *)t;

    p->eat_mutex = malloc(sizeof(pthread_mutex_t) * p->philo->nbr_philo);

    int     i;
    int j ;

    
    if(p->philo->num_eat != -1)
    {
        j = 0 ;
        while (j < p->philo->nbr_philo)
        {
            pthread_mutex_init(&p->eat_mutex[j], NULL);
            j++;
        }
        
        i = 0;
        while (i < p->philo->num_eat)
        {
            pthread_mutex_lock(&p->eat_mutex[p->idx_philo - 1]);
            if(p->philo->flag_eat == 2)
            {
                pthread_mutex_unlock(&p->eat_mutex[p->idx_philo - 1]);
                printf("Philosopher %d is died.\n", p->idx_philo);
                break;
            }
            pthread_mutex_unlock(&p->eat_mutex[p->idx_philo - 1]);
            eting_ph(p, p->philo);
            sleeping_ph(p, p->philo);
            thinking_ph(p, p->philo);
            i++;
        }
    }
    else
    {
        j = 0 ;
        while (j < p->philo->nbr_philo)
        {
            pthread_mutex_init(&p->eat_mutex[j], NULL);
            j++;
        }
        while (1)
        {
            pthread_mutex_lock(&p->eat_mutex[p->idx_philo - 1]);
            if(p->philo->flag_eat == 2 )
            {
                pthread_mutex_unlock(&p->eat_mutex[p->idx_philo - 1]);
                printf("Philosopher %d is died.\n", p->idx_philo);
                break;
            }
            pthread_mutex_unlock(&p->eat_mutex[p->idx_philo - 1]);
            if(eting_ph(p, p->philo) == -1)
                break;
            if(sleeping_ph(p, p->philo) == -1)
                break;
            if(thinking_ph(p, p->philo) == -1)
                break;
        } 
    }
    return(NULL);
}

unsigned long   get_time(void)
{
    struct timeval t;
    
    gettimeofday(&t, NULL);
    return((t.tv_sec) * 1000 + (t.tv_usec) / 1000);
}

void    creat_philo(t_philo *p, t_elemt *philo)
{
    int i = 0;
    int k = 0; 
    
    philo->forks = malloc(sizeof(pthread_mutex_t) * p->nbr_philo);
    philo->eat_mutex = malloc(sizeof(pthread_mutex_t) * p->nbr_philo);
    
    int j = 0;
    while (j < p->nbr_philo)
    {
        pthread_mutex_init(&philo->forks[j], NULL);
        pthread_mutex_init(&philo[j].last_et, NULL);
        j++;
    }
    j = 0;
    while (j < p->nbr_philo)
    {
        philo[j].left_fork = &philo->forks[j];
        philo[j].right_fork = &(philo->forks[(j + 1) % p->nbr_philo]);
        j++;
    }

    i = 0  ;
    
    while (i < p->nbr_philo)
    {
        (philo[i]).idx_philo = i + 1;
        philo[i].philo = p;
        pthread_create(&philo[i].tid_philo, NULL, &routine, &philo[i]);
        i++;
    }
    
    while (k < p->nbr_philo)
    {
        pthread_mutex_init(&philo->eat_mutex[k], NULL);
        k++;
    }

    while (1)
    {
        j = 0;
        while (j < p->nbr_philo)
        {
            pthread_mutex_lock(&philo->eat_mutex[j]);
            if ((get_time() - philo[j].last_eat) > (unsigned long)p->time_to_die)
            {
                p->flag_eat = 2;
                
                pthread_mutex_unlock(&philo->eat_mutex[j]);
                k = -1;
                break; 
            }
            pthread_mutex_unlock(&philo->eat_mutex[j]);
            j++;
        }
        if(k == -1)
            break;
    }

    i = 0;
    while (i < p->nbr_philo)
    {
        pthread_join(philo[i].tid_philo, NULL);
        i++;
    }
}

    //  le problem de Deadlock 
// void    change_fork(t_elemt *philo, t_philo *p)
// {
//     if(philo->idx_philo % 2 == 0)
//     {
        // pthread_mutex_lock(philo->left_fork);
        // pthread_mutex_unlock(philo->right_fork);
//     }
//     else if (philo->idx_philo % 2 == 1)
//     {
            // pthread_mutex_unlock(philo->left_fork);
            // pthread_mutex_lock(philo->right_fork);
//     }
// }

// la gestion des treatheads le temps (eating ? sleeping)

// void    treadth_manager(t_elemt *philo, t_philo *p)
// {
//     int    i;
    
//     if(p->nbr_philo % 2 == 0)
//     {
//         usleep(1000);
//         if(p->num_eat != 0)
//         {
//             i = 0;
//             while (i < p->num_eat)
//             {
//                 sleeping_ph(philo, p);
//                 eting_ph(philo, p);
//                 i++;
//             }
            
//         }
//         else if(p->num_eat == 0)
//         {
//             while (1)
//             {
//                 sleeping_ph(philo, p);
//                 eting_ph(philo, p);
//             }
               
//         }
//     }
//     else if(p->nbr_philo % 2 != 1)
//     {
        
//     }
// }



// void    *monitor(void *x)
// {
//     t_elemt *p = (t_elemt *)x;

//     if(p->idx_philo % 2 == 0)
//         usleep(1000);
//     if(p->philo->num_eat == -1)
//     {
//         while (1)
//         {
            
//             eting_ph(p, p->philo);
            
//             sleeping_ph(p, p->philo);
            
//             thinking_ph(p, p->philo);
//         }
//     }
// }
