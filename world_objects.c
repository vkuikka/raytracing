/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 17:04:02 by vkuikka           #+#    #+#             */
/*   Updated: 2020/09/08 21:58:15 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "rt.h"

static char *ft_next_num(char *values)
{
	while (*values && *values != ' ')
		values++;
	while (*values && !((*values >= '0' && *values <= '9') || *values == '-'))
		values++;
	return (values);
}

t_objects	*ft_add_object(t_objects *obj, int type, char *values)
{
	int			i;
	t_objects	*new;

	if (!(new = (t_objects *)malloc(sizeof(t_objects))))
		ft_error("Failed to allocate memory\n");
	if (!obj)
		obj = new;
	else
	{
		while (obj->next)
		{
			obj = obj->next;
		}
		obj->next = new;
		obj = obj->next;
	}
	obj->type = type;
	i = 0;
	while (i < 3)
	{
		obj->vec[i] = ft_atoi(values);	//use float atoi from printf
		values = ft_next_num(values);
		if (!values)
			ft_error("given file is not valid\n");
		i++;
	}
	obj->modifier = ft_atoi(values);	//use float atoi from printf
	obj->next = NULL;
	return (obj);
}
