/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smoothstep.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 13:58:46 by vkuikka           #+#    #+#             */
/*   Updated: 2020/09/04 14:01:13 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		ft_smooth_step(float mid, t_window *window)
{
	int		screen_x;
	float	x;
	float	tmp;
	float	res;

	screen_x = 0;
	SDL_SetRenderDrawColor(window->SDLrenderer, 255, 255, 255, 255);
	while (screen_x < RES_X)
	{
		x = screen_x / RES_X;
		x *= mid;
		x -= mid / 2;
		tmp = pow(M_E, x);
		res = tmp / (tmp + 1.0);
		SDL_RenderDrawPoint(window->SDLrenderer, screen_x, res * RES_Y);
		screen_x++;
	}
}


