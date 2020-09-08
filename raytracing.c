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
			(ray.pos[2] * plane.vec[2])) - plane.pos /
			(ray.pos[0] * plane.vec[0] - ray.dir[0] * plane.vec[0] +
			ray.pos[1] * plane.vec[1] - ray.dir[1] * plane.vec[1] +
			ray.pos[2] * plane.vec[2] - ray.dir[2] * plane.vec[2]);

	if (res > 0.0)
		return (res);
	return (0);
}

float		ft_3d_sphere(t_sphere sphere, t_ray ray)
{
	float	disc;

	float	rsp[3];
	rsp[0] = sphere.vec[0] - ray.pos[0];
	rsp[1] = sphere.vec[1] - ray.pos[1];
	rsp[2] = sphere.vec[2] - ray.pos[2];

	float a = ft_vector_dot(ray.dir, ray.dir);
	float b = 2 * ft_vector_dot(ray.dir, rsp);
	float c = ft_vector_dot(rsp, rsp) - sphere.r * sphere.r;
	disc = b * b - 4 * a * c;

	float x0;
	float x1;

	if (disc < 0)
		return (0);

	if (disc == 0)
		x0 = x1 = - 0.5 * b / a; 
    else
	{ 
        float q = (b > 0) ? 
            -0.5 * (b + sqrt(disc)) : 
            -0.5 * (b - sqrt(disc)); 
        x0 = q / a; 
        x1 = c / q; 
    } 
	if (x1 > 0 || x1 > -0.0)
		x1 = 0;
	x1 *= -1;
	return (x1);
	// return ((-b - sqrt(disc)) / (2.0 * a));
}

void		ft_render(t_window window, t_sphere sphere, t_plane plane, t_light light)
{
	float		ray_increment_x;
	float		ray_increment_y;
	int			hit;
	t_ray		ray;
	int			x;
	int			y;

	float	fov = 1;
	ray_increment_x = fov / RES_X;
	ray_increment_y = fov / RES_Y;

	ray.pos[0] = 0;
	ray.pos[1] = 0;		//eye position
	ray.pos[2] = 0;

	ray.dir[0] = 0 - fov / 2;
	ray.dir[1] = 0 - fov / 2;
	ray.dir[2] = fov;

	x = -1;
	y = -1;
	float res;
	float tmp;
	while (++x < RES_X)
	{
		y = 0;
		ray.dir[1] = 0 - fov / 2;
		while (++y < RES_Y)
		{
			res = ft_3d_sphere(sphere, ray);
			tmp = ft_3d_plane(plane, ray);

			if (res)
				hit = 1;
			if ((tmp < res || !res) && tmp)
			{
				hit = 2;
				res = tmp;
			}
			else if (!res)
				hit = 0;

			if (hit == 1)
			{
				ray.normal[0] = res * ray.dir[0] - sphere.vec[0];
				ray.normal[1] = res * ray.dir[1] - sphere.vec[1];
				ray.normal[2] = res * ray.dir[2] - sphere.vec[2];

				ray.reflect[0] = 2 * ray.normal[0] * (ray.dir[0] * ray.normal[0]);
				ray.reflect[1] = 2 * ray.normal[1] * (ray.dir[1] * ray.normal[1]);
				ray.reflect[2] = 2 * ray.normal[2] * (ray.dir[2] * ray.normal[2]);

				res = ft_vector_dot(ray.normal, light.vec) /
					(ft_vector_length(ray.normal) * ft_vector_length(light.vec));
				res = acos(res);
			}
			if (hit == 2)
			{
				ray.normal[0] = res * ray.dir[0] - plane.vec[0];
				ray.normal[1] = res * ray.dir[1] - plane.vec[1];
				ray.normal[2] = res * ray.dir[2] - plane.vec[2];

				ray.reflect[0] = 2 * ray.normal[0] * (ray.dir[0] * ray.normal[0]);
				ray.reflect[1] = 2 * ray.normal[1] * (ray.dir[1] * ray.normal[1]);
				ray.reflect[2] = 2 * ray.normal[2] * (ray.dir[2] * ray.normal[2]);

				t_ray ray2;
				ray2.pos[0] = res * ray.dir[0];
				ray2.pos[1] = res * ray.dir[1];
				ray2.pos[2] = res * ray.dir[2];
				ray2.dir[0] = light.vec[0];
				ray2.dir[1] = light.vec[1];
				ray2.dir[2] = light.vec[2];
				res = ft_3d_sphere(sphere, ray2);
				if (!res)
				{
					res = ft_vector_dot(ray.normal, light.vec) /
						(ft_vector_length(ray.normal) * ft_vector_length(light.vec));
					res = acos(res);
				}
				else
					res = 0;
			}

			if (y == RES_Y / 2 && x == RES_X / 2)
				printf("%f\n", res);
				// printf("%f %f %f\n", res * ray.dir[0], res * ray.dir[1], res * ray.dir[2]);
				// printf("%f %f %f\n", reflection[0], reflection[1], reflection[2]);
				// printf("%f %f %f\n", res * ray_norm[0], res * ray_norm[1], res * ray_norm[2]);
			if (res)
			{
				int r = 15;
				int g = 15;
				int b = 15;
				// if (hit == 1)
				// if (ray_norm[0] > 0)
				// r = ray_norm[0] * 255;
				// if (ray_norm[1] > 0)
				// g = ray_norm[1] * 255;
				// if (ray_norm[2] > 0)
				// b = ray_norm[2] * 255;

				if (res < 2)
				{
					res /= 2;
					r += 255 - 255 * res;
					g += 255 - 255 * res;
					b += 255 - 255 * res;
				}

				SDL_SetRenderDrawColor(window.SDLrenderer, r, g, b, 255);
				// else
				// 	SDL_SetRenderDrawColor(window.SDLrenderer, 255 * res * hit, 255 * res, 255 * res, 255);
				// res *= hit;
				// SDL_SetRenderDrawColor(window.SDLrenderer, 255, 255, 255, 255);
			}
			else
				SDL_SetRenderDrawColor(window.SDLrenderer, 0, 0, 0, 255);
			SDL_RenderDrawPoint(window.SDLrenderer, x, y);
			ray.dir[1] += ray_increment_y;
		}
		ray.dir[0] += ray_increment_x;
	}

	SDL_SetRenderDrawColor(window.SDLrenderer, 255, 255, 255, 255);
	for (int x = RES_X / 2 - 10; x < RES_X / 2 + 10; x++)
		SDL_RenderDrawPoint(window.SDLrenderer, x, RES_Y / 2);
	for (int y = RES_Y / 2 - 10; y < RES_Y / 2 + 10; y++)
		SDL_RenderDrawPoint(window.SDLrenderer, RES_X / 2, y);
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
	