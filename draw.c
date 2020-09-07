/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 14:03:28 by vkuikka           #+#    #+#             */
/*   Updated: 2020/09/04 14:03:37 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asd.h"

void	ft_draw_line(int line[4], t_window *window, float mid)//unsigned c2)
{
	float	x_increment;
	float	y_increment;
	float	xy[2];
	int		steps;
	int		i;

	i = 0;
	xy[0] = line[0];
	xy[1] = line[1];
	steps = ft_abs(line[2] - line[0]) > ft_abs(line[3] - line[1]) ?
			ft_abs(line[2] - line[0]) : ft_abs(line[3] - line[1]);
	if (!steps)
		return ;
	x_increment = (line[2] - line[0]) / (float)steps;
	y_increment = (line[3] - line[1]) / (float)steps;
	while (i <= (steps))
	{
		if (i > (mid) * steps)
			SDL_SetRenderDrawColor(window->SDLrenderer, 255, 0, 0, 255);
		SDL_RenderDrawPoint(window->SDLrenderer, xy[0], xy[1]);
		xy[0] += x_increment;
		xy[1] += y_increment;
		i++;
	}
}


