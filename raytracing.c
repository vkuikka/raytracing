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

int g_print = 0;

float		ft_3d_plane(t_objects plane, t_ray r)
{
	float denom = ft_vector_dot(plane.dir, r.dir); 
    // if (fabs(denom) < M_PI)
    if (fabs(denom) > 1e-6)
	{
		float rsp[3];
		rsp[0] = plane.pos[0] - r.pos[0];
		rsp[1] = plane.pos[1] - r.pos[1];
		rsp[2] = plane.pos[2] - r.pos[2];

		float t = ft_vector_dot(rsp, plane.dir) / denom; 
		if (t > 0.00001)
			return (t);
	} 
	return (0); 
}

float		ft_3d_sphere(t_objects sphere, t_ray ray)
{
	float	disc;

	float	rsp[3];
	rsp[0] = sphere.pos[0] - ray.pos[0];
	rsp[1] = sphere.pos[1] - ray.pos[1];
	rsp[2] = sphere.pos[2] - ray.pos[2];

	float a = ft_vector_dot(ray.dir, ray.dir);
	float b = 2 * ft_vector_dot(ray.dir, rsp);
	float c = ft_vector_dot(rsp, rsp) - sphere.modifier * sphere.modifier;

	disc = b * b - 4 * a * c;

	float x0;
	float x1;

	if (disc < 0)
		return (0);
	else if (disc == 0)
		x0 = x1 = - 0.5 * b / a; 
    else
	{ 
        float q = (b > 0) ? 
            -0.5 * (b + sqrt(disc)) : 
            -0.5 * (b - sqrt(disc)); 
        x0 = q / a; 
        x1 = c / q; 
    } 
	if (x0 > x1)
		x1 = x0;

	if (x1 > 0)
		x1 = 0;
	x1 *= -1;
	return (x1);
	// return ((-b - sqrt(disc)) / (2.0 * a));
}

float		ft_3d_cylinder(t_objects cylinder, t_ray ray)
{

	// (p2- p1)/| p2- p1|
	// AxisPoint = cylinder.pos + DotProduct(ray.pos - cylinder.pos) * cylinder.dir;
	float	crossp[3];

	crossp[0] = ray.dir[1] * cylinder.dir[2] - ray.dir[2] * cylinder.dir[1]; 
	crossp[1] = ray.dir[2] * cylinder.dir[0] - ray.dir[0] * cylinder.dir[2]; 
	crossp[2] = ray.dir[0] * cylinder.dir[1] - ray.dir[1] * cylinder.dir[0]; 

	float	perp[3];
	float	perp2[3];

	perp[0] = cylinder.dir[1] * crossp[2] - cylinder.dir[2] * crossp[1]; 
	perp[1] = cylinder.dir[2] * crossp[0] - cylinder.dir[0] * crossp[2]; 
	perp[2] = cylinder.dir[1] * crossp[1] - cylinder.dir[1] * crossp[0]; 

	perp2[0] = ray.dir[1] * crossp[2] - ray.dir[2] * crossp[1]; 
	perp2[1] = ray.dir[2] * crossp[0] - ray.dir[0] * crossp[2]; 
	perp2[2] = ray.dir[1] * crossp[1] - ray.dir[1] * crossp[0]; 

	float	cdp[3];
	float	cdp2[3];

	cdp[0] = cylinder.pos[0] - ray.pos[0];
	cdp[1] = cylinder.pos[1] - ray.pos[1];
	cdp[2] = cylinder.pos[2] - ray.pos[2];

	cdp2[0] = ray.pos[0] - cylinder.pos[0];
	cdp2[1] = ray.pos[1] - cylinder.pos[1];
	cdp2[2] = ray.pos[2] - cylinder.pos[2];


	float d1 = ft_vector_dot(cdp, perp) / ft_vector_dot(ray.dir, perp);
	float d2 = ft_vector_dot(cdp2, perp2) / ft_vector_dot(cylinder.dir, perp2);


	float	res1[3];
	res1[0] = d1 * ray.dir[0];
	res1[1] = d1 * ray.dir[1];
	res1[2] = d1 * ray.dir[2];
	res1[0] += ray.pos[0];
	res1[1] += ray.pos[1];
	res1[2] += ray.pos[2];

	float	res2[3];
	res2[0] = d2 * ray.dir[0];
	res2[1] = d2 * ray.dir[1];
	res2[2] = d2 * ray.dir[2];
	res2[0] += ray.pos[0];
	res2[1] += ray.pos[1];
	res2[2] += ray.pos[2];

	res2[0] -= res1[0];
	res2[1] -= res1[1];
	res2[2] -= res1[2];

	if (g_print)
	{
		printf("%f ", res1[0]);
		printf("%f ", res1[1]);
		printf("%f\n\n", res1[2]);
	}

	float	clen = ft_vector_length(res2);
	if (clen > 0 && clen < cylinder.modifier)
		return (clen);
	return (0);
}

float		ft_choose_obj(t_ray ray, t_objects obj)
{
	// if (type == CONE)
		// return(ft_3d_cylinder(obj, ray));
	if (obj.type == PLANE)
		return(ft_3d_plane(obj, ray));
	else if (obj.type == SPHERE)
		return(ft_3d_sphere(obj, ray));
	else if (obj.type == CYLINDER)
		return(ft_3d_cylinder(obj, ray));
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
		if (obj->type == CYLINDER && bounces)
			obj = obj->next;
		hit = ft_choose_obj(ray, *obj);
		// if ((hit && hit < tmp) || !tmp && (bounces == 0 || ray.obj_index != obj->index))
		// if (!bounces && obj->type == PLANE && hit)
		// 	return (hit);
		// if (!(bounces && obj->type == PLANE))
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

	if (hit_obj->type == CYLINDER)
		return (tmp);

	float res = tmp;
	if (hit_obj->type == SPHERE)
	{
		ray.normal[0] = res * ray.dir[0] - hit_obj->pos[0];
		ray.normal[1] = res * ray.dir[1] - hit_obj->pos[1];
		ray.normal[2] = res * ray.dir[2] - hit_obj->pos[2];
		// ray.reflect[0] = 2 * ray.normal[0] * (ray.dir[0] * ray.normal[0]);
		// ray.reflect[1] = 2 * ray.normal[1] * (ray.dir[1] * ray.normal[1]);
		// ray.reflect[2] = 2 * ray.normal[2] * (ray.dir[2] * ray.normal[2]);
	}
	else if (hit_obj->type == PLANE)
	{
		ray.normal[0] = res * ray.dir[0] + hit_obj->dir[0];
		ray.normal[1] = res * ray.dir[1] + hit_obj->dir[1];
		ray.normal[2] = res * ray.dir[2] + hit_obj->dir[2];
	}


	ray.pos[0] = res * ray.dir[0];
	ray.pos[1] = res * ray.dir[1];
	ray.pos[2] = res * ray.dir[2];

	ray.dir[0] = lights->pos[0];
	ray.dir[1] = lights->pos[1];
	ray.dir[2] = lights->pos[2];

	if (!bounces)
		tmp = ft_trace_ray(first, lights, ray, 1);
	else
		return (res);

	if (res && !tmp)
	{
		res = ft_vector_dot(ray.normal, lights->pos) /
			(ft_vector_length(ray.normal) * ft_vector_length(lights->pos));
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

	ray_increment_x = 1 / RES_X;
	ray_increment_y = 1 / RES_Y;

	float res;
	float tmp;

	ray.pos[0] = world.view->pos[0];
	ray.pos[1] = world.view->pos[1];
	ray.pos[2] = world.view->pos[2];

	ray.obj_index = -5;

	ray.dir[0] = world.view->dir[0] - ray_increment_x * RES_X / 2;
	ray.dir[2] = 0.5;

	x = -1;
	while (++x < RES_X)
	{
		y = -1;
		ray.dir[1] = world.view->dir[1] - ray_increment_y * RES_Y / 2;
		while (++y < RES_Y)
		{
			world.obj = world.first_obj;
			world.lights = world.first_light;
			if (x == RES_X / 2 && y == RES_Y / 2)
				g_print = 1;
			else
				g_print = 0;
			res = ft_trace_ray(world.obj, world.lights, ray, 0);
			// if (x == RES_X / 2 && y == RES_Y / 2)
				// printf("%f\n", res);
			if (res != 0.0)
			{
				int r = 0;
				int g = 0;
				int b = 0;
				// if (ray.normal[0] > 0)
				// 	r = ray.normal[0] * 255;
				// if (ray.normal[1] > 0)
				// 	g = ray.normal[1] * 255;
				// if (ray.normal[2] > 0)
				// 	b = ray.normal[2] * 255;
				
				int shadow = 0;
				if (res < 0)
				{
					shadow = 1;
					res *= -1;
				}

				if (res <= M_PI / 2)
				{
					res /= M_PI / 2;
					if (!shadow)
					{
						r = 255 - 255 * res;
						g = 255 - 255 * res;
						b = 255 - 255 * res;
					}
					else
					{
						r = 255 * res;
						g = 255 * res;
						b = 255 * res;
					}
				}
				SDL_SetRenderDrawColor(window.SDLrenderer, r, g, b, 255);
			}
			else
				SDL_SetRenderDrawColor(window.SDLrenderer, 0, 0, 50, 255);
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
	
