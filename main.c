/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-laf <aait-laf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:18:28 by aait-laf          #+#    #+#             */
/*   Updated: 2025/04/28 16:22:42 by aait-laf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void    initial_sct(t_philo *p)
{
    p->nbr_philo = 0;
    p->num_eat = -1;
    p->time_eat = 0;
    p->flag_current = 0;
    p->time_sleep = 0;
    p->time_to_die = 0;
    p->current = 0;
    // p->last_eat = 0;
    p->flag_eat = 0;
    if (pthread_mutex_init(&p->flag_mutex, NULL) != 0)
        return;
}

void    initai_sct2(t_elemt *philo)
{
    philo->idx_philo = 0;
    philo->tid_philo = 0;
    philo->last_eat = 0;
}

int check_args(t_philo *p)
{
    if (p->nbr_philo <= 0 || p->time_to_die <= 0 || p->time_eat <= 0 
        || p->time_sleep <= 0)
        return (1);
    if (p->num_eat != -1 && p->num_eat <= 0)
        return (1);
    return (0);
}

int main(int ac, char **av)
{
    t_philo     *p;
    t_elemt     *philo;
    
    if(ac != 5 && ac != 6)
        return(1);
    
    p = malloc(sizeof(t_philo));
    if (!p)
    {
        perror("Failed to allocate t_philo");
        return(1);
    }
    initial_sct(p);
    
    p->nbr_philo = ft_atoi(av[1]);
    p->time_to_die = ft_atoi(av[2]);
    p->time_eat = ft_atoi(av[3]);
    p->time_sleep = ft_atoi(av[4]);
    if (av[5])
        p->num_eat = ft_atoi(av[5]);
    if(check_args(p) == 1)
    {
        free(p);
        return(1);
    }
    
    philo =  malloc(sizeof(t_elemt) * p->nbr_philo);
    if (!philo)
    {
        perror("Failed to allocate philosophers");
        free(p);
        return(1);
    }
    initai_sct2(philo);
   
    philo->philo = p;
    
    // Créer les philosophes (même avec 1 seul)
    if (creat_philo(p, philo) == -1)
    {
        // En cas d'erreur, creat_philo a déjà nettoyé sa mémoire
        free(philo);
        free(p);
        return(1);
    }
    
    // Si succès, creat_philo() a tout nettoyé
    return (0);
}
