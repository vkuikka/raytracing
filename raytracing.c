/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 20:02:39 by vkuikka           #+#    #+#             */
/*   Updated: 2020/09/07 15:43:18 vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asd.h"

float		ft_3d_plane(t_plane plane, t_ray ray)
{
	float res;

	res = ((ray.pos[0] * plane.vec[0]) +
			(ray.pos[1] * plane.vec[1]) +
			(ray.pos[2] * plane.vec[2])) /
			(ray.pos[0] * plane.vec[0] - ray.dir[0] * plane.vec[0] +
			ray.pos[1] * plane.vec[1] - ray.dir[1] * plane.vec[1] +
			ray.pos[2] * plane.vec[2] - ray.dir[2] * plane.vec[2]);

	if (res < -ray.pos[2] && res > -10)
	{
		res *= -1;
		res *= 10;
		res += 0.1;
		if (res > 1)
			res = 1;
		return (res);
	}
	return (0);
}

float		ft_3d_sphere(t_sphere sphere, t_ray ray)
{
	float	dot;

	float	rsp[3];
	rsp[0] = sphere.vec[0] - ray.pos[0];
	rsp[1] = sphere.vec[1] - ray.pos[1];
	rsp[2] = sphere.vec[2] - ray.pos[2];

	float a = ft_vector_dot(ray.dir, ray.dir);
	float b = 2 * ft_vector_dot(ray.dir, rsp);
	float c = ft_vector_dot(rsp, rsp) - sphere.r * sphere.r;
	dot = b * b - 4 * a * c;

	if (dot > 0)
		return (dot);
	return (0);
}

void		ft_render(t_window window, t_sphere sphere, t_plane plane)
{
	float		ray_increment_x;
	float		ray_increment_y;
	t_ray		ray;
	int			x;
	int			y;

	ray.pos[0] = 0.001;
	ray.pos[1] = 0.001;
	ray.pos[2] = 0.001;

	float	fov = 1;
	ray.dir[0] = 0 - fov / 2;
	ray.dir[1] = 0 - fov / 2;
	ray.dir[2] = fov;
	ray_increment_x = fov / RES_X;
	ray_increment_y = fov / RES_Y;


	x = -1;
	y = -1;
	float res;
	float tmp;
	while (++x < RES_X)
	{
		y = 0;
		ray.dir[0] += ray_increment_x;
		ray.dir[1] = 0 - fov / 2;
		while (++y < RES_Y)
		{
			ray.dir[1] += ray_increment_y;

			res = ft_3d_plane(plane, ray);

			tmp = ft_3d_sphere(sphere, ray);
			if (tmp > res)
				res = tmp;

			SDL_SetRenderDrawColor(window.SDLrenderer, 255 * res, 255 * res, 255 * res, 255);
			SDL_RenderDrawPoint(window.SDLrenderer, x, y);

		}
		// printf("\n");
	}
	// printf("\n");



}

/*
	a = rayline[0]
	b = rayline[3]
	c = sphere.x

	d = rayline[1]
	f = rayline[4]
	g = sphere.y

	h = rayline[2]
	j = sphere.z
	i = rayline[5]

*/
	