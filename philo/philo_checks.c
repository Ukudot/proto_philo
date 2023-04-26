/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_checks_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 11:15:51 by gpanico           #+#    #+#             */
/*   Updated: 2023/04/26 11:58:57 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_is_all_digit(int n, char **strs)
{
	int	i;
	int	j;

	i = 0;
	while (i < n)
	{
		j = 0;
		while (strs[i][j])
		{
			if (strs[i][j] < 48 || strs[i][j] > 57)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	ft_checks(int argc, char **argv, t_shared *shared)
{
	int	i;

	if (argc != 5 && argc != 6)
	{
		write(2, "Usage error: too many args.\n", 28);
		return (1);
	}
	if (ft_is_all_digit(argc - 1, &argv[1]))
	{
		write(2, "Syntax error: all args must be positive integers.\n", 50);
		return (1);
	}
	i = 0;
	while (++i < argc)
	{
		shared->args[i - 1] = ft_atoi(argv[i]);
		if (shared->args[i - 1] == -1)
		{
			write(2, "Syntax error: all args must be positive integers.\n", 50);
			return (1);
		}
	}
	if (argc == 5)
		shared->args[4] = -1;
	return (0);
}

