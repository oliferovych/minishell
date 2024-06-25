/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:22:31 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/25 14:22:31 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_swap(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void bubble_sort(char **array, int size)
{
	int		i;
	int		j;
	int		swapped;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		swapped = 0;
		while (j < size - 1 - i)
		{
			if (ft_strcmp(array[j], array[j + 1]) > 0)
			{
				ft_swap(&array[j], &array[j + 1]);
				swapped = 1;
			}
			j++;
		}
		if (!swapped)
			break;
		i++;
	}
}
