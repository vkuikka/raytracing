/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 18:03:41 by vkuikka           #+#    #+#             */
/*   Updated: 2019/12/19 15:57:30 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	digit_count(long nb, int base)
{
	size_t		i;

	i = 1;
	while (nb /= base)
		i++;
	return (i);
}

char			*ft_itoa_base(long num, int base, int lowercase)
{
	int		i;
	int		count;
	char	*b;
	char	*res;

	b = lowercase ? "0123456789abcdef" : "0123456789ABCDEF";
	count = digit_count(num, base);
	i = num < 0 ? 0 : 1;
	if (!(res = (char *)malloc(sizeof(char) * count + i)))
		return (NULL);
	res[count + i] = '\0';
	res[0] = '-';
	num = num < 0 ? num * -1 : num;
	while (num)
	{
		res[count - i++] = b[num % base];
		num /= base;
	}
	return (res);
}
