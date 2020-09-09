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
	float	res;
	// float	rv[3];

	// // plane->vec[2] = 0.01;
	// ft_normalize(ray.dir);
	// ft_normalize(ray.pos);

	// rv[0] = ray.dir[0] - ray.pos[0];
	// rv[1] = ray.dir[1] - ray.pos[1];
	// rv[2] = ray.dir[2] - ray.pos[2];

	// res = ft_vector_dot(plane->vec, rv);
	// // plane->modifier = 1.2;
	// // if (res > 0.01)
	// if (res > 1 * M_E - 6)
	// { 
	// 	float p[3];
	// 	p[0] = plane->vec[0] * plane->modifier;// - ray.pos[0];
	// 	p[1] = plane->vec[1] * plane->modifier;// - ray.pos[1];
	// 	p[2] = plane->vec[2] * plane->modifier;// - ray.pos[2];
	// 	float n[3];
	// 	n[0] = plane->vec[0];
	// 	n[1] = plane->vec[1];
	// 	n[2] = plane->vec[2];
	// 	float t = ft_vector_dot(p, n) / res; 
	// 	t*=10;
	// 	return (t >= 0); 
	// } 
	// return (0);

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

float		ft_choose_obj(t_ray ray, t_objects *obj)
{
	// if (type == 1)
	// 	hit = ft_3d_cone(world->obj, world->view);
	if (obj->type == 2)
		return(ft_3d_plane(obj, ray));
	else if (obj->type == 3)
		return(ft_3d_sphere(obj, ray));
	// else if (type == 4)
	// 	return(ft_3d_cylinder(world->obj, world->view));
	ft_error("object not valid yet pls\n");
	return (0);
}

float		ft_trace_ray(t_objects *obj, t_objects *lights, t_ray ray, int bounces)
{
	float		hit;
	float		tmp;
	t_objects	*first;
	t_objects	*hit_obj;

	tmp = 0;
	first = obj;
	hit_obj = NULL;
	while (obj)
	{
		hit = ft_choose_obj(ray, obj);
		// if ((hit && hit < tmp) || !tmp && (bounces == 0 || ray.obj_index != obj->index))
		if (!(bounces && obj->type == 2))
		if (((hit && hit < tmp) || !tmp) && ray.obj_index != obj->index)
		{
			tmp = hit;
			hit_obj = obj;
		}
		obj = obj->next;
	}
	if (!hit_obj)
		return (0);
	ray.obj_index = hit_obj->index;

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

	if (!bounces)
		tmp = ft_trace_ray(first, lights, ray, 1);
	else
		return (res);
	if (res && !tmp)
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

	ray.dir[0] = world.view->dir[0];
	ray.dir[1] = world.view->dir[1];
	ray.dir[2] = 1;

	ray.obj_index = -5;

	while (++x < RES_X)
	{
		y = 0;
		ray.dir[1] = world.view->dir[1];
		while (++y < RES_Y)
		{
			// cast.dir[0] = ray.dir[0];
			// cast.dir[1] = ray.dir[1];
			// cast.dir[2] = ray.dir[2];

			// cast.pos[0] = ray.pos[0];
			// cast.pos[1] = ray.pos[1];
			// cast.pos[2] = ray.pos[2];
			res = ft_trace_ray(world.obj, world.lights, ray, 0);
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

				// if (ray.normal[0] > 0)
				// 	r = ray.normal[0] * 255;
				// if (ray.normal[1] > 0)
				// 	g = ray.normal[1] * 255;
				// if (ray.normal[2] > 0)
				// 	b = ray.normal[2] * 255;

				if (res < 2)
				{
					res /= 2;
					r += 255 - 255 * res;
					g += 255 - 255 * res;
					b += 255 - 255 * res;
				}
				SDL_SetRenderDrawColor(window.SDLrenderer, r, g, b, 255);
			}
			else
				SDL_SetRenderDrawColor(window.SDLrenderer, 10, 10, 10, 255);
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
	
