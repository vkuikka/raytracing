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

#include "rt.h"

float		ft_3d_plane(t_objects *plane, t_ray ray)
{
	float res;

	res = ((ray.pos[0] * plane->vec[0]) +
			(ray.pos[1] * plane->vec[1]) +
			(ray.pos[2] * plane->vec[2])) - plane->modifier /
			(ray.pos[0] * plane->vec[0] - ray.dir[0] * plane->vec[0] +
			ray.pos[1] * plane->vec[1] - ray.dir[1] * plane->vec[1] +
			ray.pos[2] * plane->vec[2] - ray.dir[2] * plane->vec[2]);
	if (res > 0.0)
		return (res);
	return (0);
}

float		ft_3d_sphere(t_objects *sphere, t_ray ray)
{
	float	disc;

	float	rsp[3];
	rsp[0] = sphere->vec[0] - ray.pos[0];
	rsp[1] = sphere->vec[1] - ray.pos[1];
	rsp[2] = sphere->vec[2] - ray.pos[2];

	float a = ft_vector_dot(ray.dir, ray.dir);
	float b = 2 * ft_vector_dot(ray.dir, rsp);
	float c = ft_vector_dot(rsp, rsp) - sphere->modifier * sphere->modifier;
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

float		ft_choose_obj(t_ray ray, t_objects *obj, t_ray view)
{
	// if (type == 1)
	// 	hit = ft_3d_cone(world->obj, world->view);
	if (obj->type == 2)
		return(ft_3d_plane(obj, view));
	else if (obj->type == 3)
		return(ft_3d_sphere(obj, view));
	// else if (type == 4)
	// 	return(ft_3d_cylinder(world->obj, world->view));
	ft_error("wtf\n");
	return (0);
}

float		ft_trace_ray(t_objects *obj, t_objects *lights, t_ray ray, int bounces)
{
	float		hit;
	float		tmp;
	t_objects	*hit_obj;

	tmp = 0;
	while (obj)
	{
		hit = ft_choose_obj(ray, obj, ray);
		if ((hit && hit < tmp) || !tmp)
		{
			tmp = hit;
			hit_obj = obj;
		}
		obj = obj->next;
	}
	float res = tmp;
	ray.normal[0] = res * ray.dir[0] - hit_obj->vec[0];
	ray.normal[1] = res * ray.dir[1] - hit_obj->vec[1];
	ray.normal[2] = res * ray.dir[2] - hit_obj->vec[2];
	ray.reflect[0] = 2 * ray.normal[0] * (ray.dir[0] * ray.normal[0]);
	ray.reflect[1] = 2 * ray.normal[1] * (ray.dir[1] * ray.normal[1]);
	ray.reflect[2] = 2 * ray.normal[2] * (ray.dir[2] * ray.normal[2]);

	ray.pos[0] = res * ray.dir[0];
	ray.pos[1] = res * ray.dir[1];
	ray.pos[2] = res * ray.dir[2];

	ray.dir[0] = lights->vec[0];
	ray.dir[1] = lights->vec[1];
	ray.dir[2] = lights->vec[2];

	// if (bounces < 1)
	// 	hit = ft_trace_ray(hit_obj, lights, ray, bounces + 1);
	// else
	// 	return (tmp);
	if (res)
	{
		res = ft_vector_dot(ray.normal, lights->vec) /
			(ft_vector_length(ray.normal) * ft_vector_length(lights->vec));
		res = acos(res);
		return (res);
	}
	return (0);
}

void		ft_render(t_window window, t_world world)
{
	float		ray_increment_x;
	float		ray_increment_y;
	t_ray		ray;
	t_ray		cast;
	float		col;
	int			x;
	int			y;

	ray_increment_x = world.view->normal[0] / RES_X;
	ray_increment_y = world.view->normal[0] / RES_Y;
	x = -1;
	y = -1;
	float res;
	float tmp;

	ray.pos[0] = world.view->pos[0];
	ray.pos[1] = world.view->pos[1];
	ray.pos[2] = world.view->pos[2];

	ray.pos[0] = 0.01;
	ray.pos[1] = 0.01;
	ray.pos[2] = 0.01;

	ray.dir[0] = world.view->dir[0];
	ray.dir[1] = world.view->dir[1];
	ray.dir[0] = -0.5;
	ray.dir[1] = -0.5;
	ray.dir[2] = 1;
	printf("%f\n", world.first_light->vec[0]);
	printf("%f\n", world.first_light->vec[1]);
	printf("%f\n", world.first_light->vec[2]);

	while (++x < RES_X)
	{
		y = 0;
		// ray.dir[1] = 0 - world.view->normal[0] / 2;
		ray.dir[1] = world.view->dir[1];
		while (++y < RES_Y)
		{
			cast.dir[0] = ray.dir[0];
			cast.dir[1] = ray.dir[1];
			cast.dir[2] = ray.dir[2];

			cast.pos[0] = ray.pos[0];
			cast.pos[1] = ray.pos[1];
			cast.pos[2] = ray.pos[2];
			res = ft_trace_ray(world.obj, world.lights, cast, 0);
			world.obj = world.first_obj;
			world.lights = world.first_light;

			// if (y == RES_Y / 2 && x == RES_X / 2)
			// {
			// 	printf("%f\n", world.obj->vec[0]);
			// 	printf("%f\n", world.view->dir[0]);
			// 	printf("%f\n", res);
			// }
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
	
