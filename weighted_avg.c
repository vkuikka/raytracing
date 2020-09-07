/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weighted_avg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 13:59:26 by vkuikka           #+#    #+#             */
/*   Updated: 2020/09/04 20:02:22 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asd.h"

void		ft_2d_weighted_avg(float mid, t_window *window, int line1[4], int line2[4])
{
	int		line3[4];
	float	point_mid;

	line3[0] = 0;
	line3[1] = 0;
	line3[2] = 0;
	line3[3] = 0;
	point_mid = 0;

	// ft_draw_line(line1, window, mid);
	// ft_draw_line(line2, window, mid);

	float x = 0;
	float y = 0;
	while (point_mid < 1)
	{
		line3[0] = (1 - point_mid) * line1[0] + point_mid * line1[2];
		line3[1] = (1 - point_mid) * line1[1] + point_mid * line1[3];
		line3[2] = (1 - point_mid) * line2[0] + point_mid * line2[2];
		line3[3] = (1 - point_mid) * line2[1] + point_mid * line2[3];
		x = (1 - point_mid) * line3[0] + point_mid * line3[2];
		y = (1 - point_mid) * line3[1] + point_mid * line3[3];
		SDL_RenderDrawPoint(window->SDLrenderer, x, y);
		point_mid += 0.001;
	}
	mid = 3;
}

void		ft_2d_lines(float mid, t_window *window, int x, int y)
{
	static int		coord1[2] = {0, 0};
	static int		coord2[2] = {0, 0};
	int				line1[4];
	int				line2[4];

	SDL_SetRenderDrawColor(window->SDLrenderer, 255, 255, 255, 255);
	SDL_RenderDrawPoint(window->SDLrenderer, x, y);
	if (!coord1[0] && !coord1[1])
	{
		coord1[0] = x;
		coord1[1] = y;
		return ;
	}
	else if (!coord2[0] && !coord2[1])
	{
		coord2[0] = x;
		coord2[1] = y;
		return ;
	}
	line1[0] = coord1[0];
	line1[1] = coord1[1];
	line1[2] = coord2[0];
	line1[3] = coord2[1];

	line2[0] = line1[2];
	line2[1] = line1[3];
	line2[2] = x;
	line2[3] = y;

	ft_2d_weighted_avg(1, window, line1, line2);

	coord2[0] = x - (coord2[0] - x);
	coord2[1] = y - (coord2[1] - y);

	coord1[0] = x;
	coord1[1] = y;
}
