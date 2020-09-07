/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 20:02:39 by vkuikka           #+#    #+#             */
/*   Updated: 2020/09/04 23:48:43 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asd.h"

void		ft_3d_plane(t_window *window, float mid)
{
	float	rayline[6];
	float	point_mid;
	t_plane	plane;
	t_ray	ray;

	plane.x = 1;
	plane.y = mid;
	plane.z = 1;

	ray.x = 0;
	ray.y = 0;
	ray.z = 0;

	rayline[0] = 0.001;
	rayline[1] = 0.001;
	rayline[2] = 0.001;

	rayline[3] = 0;
	rayline[4] = 0;
	rayline[5] = 1;


	float	fov = 1;
	rayline[3] = 0 - fov / 2;
	rayline[4] = 0 - fov / 2;
	rayline[5] = 1;

	SDL_SetRenderDrawColor(window->SDLrenderer, 255, 255, 255, 255);
	float	ray_increment_x = fov / (float)RES_X;
	float	ray_increment_y = fov / (float)RES_Y;

	int x = -1;
	int y = -1;
	while (++x < RES_X)
	{
		rayline[3] += ray_increment_x;
		rayline[4] = 0 - fov / 2;
		y = 0;
		while (++y < RES_Y)
		{
			point_mid = 0.001;
			rayline[4] += ray_increment_y;
			point_mid = ((rayline[0] * plane.x) +
						(rayline[1] * plane.y) +
						(rayline[2] * plane.z)) /
					(rayline[0] * plane.x - rayline[3] * plane.x +
					rayline[1] * plane.y - rayline[4] * plane.y +
					rayline[2] * plane.z - rayline[5] * plane.z);

			if (point_mid < -rayline[2] && point_mid > -10)
			{
				point_mid *= -1;
				point_mid *= 10;
				point_mid += 0.1;
				if (point_mid > 1)
					point_mid = 1;

				SDL_SetRenderDrawColor(window->SDLrenderer, 255 * point_mid, 255 * point_mid, 255 * point_mid, 255);
			}
			else
				SDL_SetRenderDrawColor(window->SDLrenderer, 0, 0, 0, 255);
			SDL_RenderDrawPoint(window->SDLrenderer, x, y);
		}
	}
}

void		ft_3d_sphere(t_window *window, float mid)
{
	float		rayline[6];
	float		point_mid;
	t_sphere	sphere;
	t_ray		ray;

	sphere.x = 0.01;
	sphere.y = 0.01;
	sphere.z = mid;
	sphere.r = 1;
	// sphere.r = 4.5;

	ray.x = 0;
	ray.y = 0;
	ray.z = 0;

	rayline[0] = 0.001;
	rayline[1] = 0.001;
	rayline[2] = 0.001;

	float	fov = 1;
	rayline[3] = 0 - fov / 2;
	rayline[4] = 0 - fov / 2;
	rayline[5] = fov;

	SDL_SetRenderDrawColor(window->SDLrenderer, 255, 255, 255, 255);
	int x = -1;
	int y = -1;

	float	ray_increment_x = fov / (float)RES_X;
	float	ray_increment_y = fov / (float)RES_Y;

	while (++x < RES_X)
	{
		y = 0;
		rayline[3] += ray_increment_x;
		rayline[4] = 0 - fov / 2;
		while (++y < RES_Y)
		{
			point_mid = 0;
			rayline[4] += ray_increment_y;
			// while (point_mid < 5)
			// {
			// 	ray.x = (1 - point_mid) * rayline[0] + point_mid * rayline[3];
			// 	ray.y = (1 - point_mid) * rayline[1] + point_mid * rayline[4];
			// 	ray.z = (1 - point_mid) * rayline[2] + point_mid * rayline[5];
			// 	ray.x -= sphere.x;
			// 	ray.y -= sphere.y;
			// 	ray.z -= sphere.z;
			// 	float	res = ray.x * ray.x +
			// 					ray.y * ray.y +
			// 					ray.z * ray.z;
				float	spr = sphere.r * sphere.r;

				double sqrtable = -rayline[1] * -rayline[1] + 2 * rayline[1] * rayline[4] - rayline[4] * rayline[4] -
									rayline[2] * rayline[2] + 2 * rayline[2] * rayline[5] - rayline[5] * rayline[5];
				// printf("%f ", sqrtable);
				// int negat = 0;
				if (sqrtable < 0)
				{
					sqrtable *= -1;
				}
				float tmp = sqrt(sqrtable);
				// if (x > RES_X / 2)
				// 	tmp *= -1;

				point_mid = (rayline[3] * rayline[3] - 2 * rayline[3] * sphere.x + 2 * rayline[3] * tmp + sphere.x * sphere.x -
							2 * sphere.x * tmp + 2 * rayline[1] * rayline[4] - 2 * rayline[1] * rayline[4] - 2 * rayline[1] * sphere.y - rayline[4] * rayline[4] + sphere.y * sphere.y +
							2 * rayline[2] * rayline[5] - 2 * rayline[2] * sphere.z - rayline[5] * rayline[5] + sphere.z * sphere.z) /
							(2 * (rayline[3] * tmp - sphere.x * tmp +
rayline[1] * rayline[4] - rayline[1] * sphere.y - rayline[4] * rayline[4] + rayline[4] * sphere.y + rayline[2] * rayline[5] - rayline[2] * sphere.z - rayline[5] * rayline[5] + rayline[5] * sphere.z));

			// printf("%f\n", tmp);
				float res = point_mid;
				if (res >= spr && res < spr + 1)
				{
					// res /= 10;
					// SDL_SetRenderDrawColor(window->SDLrenderer, 255, 0, 0, 255);
					SDL_SetRenderDrawColor(window->SDLrenderer, 255 * res, 255 * res, 255 * res, 255);
					SDL_RenderDrawPoint(window->SDLrenderer, x, y);
					// SDL_SetRenderDrawColor(window->SDLrenderer, 0, 0, 0, 255);
				}
			// 	point_mid += 0.1;
			// }
		}
		// printf("\n");
	}
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
	