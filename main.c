/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 18:28:42 by vkuikka           #+#    #+#             */
/*   Updated: 2020/09/07 21:23:00 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asd.h"
#include <math.h>

# include "SDL2.framework/Headers/SDL.h"
# include "SDL2_image.framework/Headers/SDL_image.h"

SDL_Texture		*ft_empty_texture(SDL_Renderer *renderer)
{
	SDL_Texture *texture = NULL;
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, RES_X, RES_Y);
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, NULL);
	return (texture);
}

int		ft_buttons(int button, const int pressed)
{
	static int		*keys;
	int				i;

	if (!(keys) && (i = -1))
	{
		if (!(keys = (int *)malloc(sizeof(int) * 4)))
			ft_error("memory allocation failed\n");
		while (++i < 4)
			keys[i] = -1;
	}
	i = -1;
	while (++i < 4)
		if (pressed == -1 && keys[i] == button)
			return (1);
		else if (pressed == 0 && keys[i] == button)
		{
			keys[i] = -1;
			return (0);
		}
		else if (pressed == 1 && keys[i] == -1)
		{
			keys[i] = button;
			return (0);
		}
	return (0);
}

t_sphere	ft_loop(t_window *window, SDL_Texture *txt)
{
	static t_sphere sphere = {0, 0, 2, 1};
	static t_plane	plane = {0.5, 0.5, 0.4, 0.5};

	if (ft_buttons(SDL_SCANCODE_1, -1))
	{
		sphere.r += 0.01;
		printf("r: %f\n", sphere.r);
	}
	if (ft_buttons(SDL_SCANCODE_2, -1))
	{
		sphere.r -= 0.01;
		printf("r: %f\n", sphere.r);
	}

	if (ft_buttons(ARROW_L, -1))
		plane.vec[0] -= 0.1;
	if (ft_buttons(ARROW_R, -1))
		plane.vec[0] += 0.1;
	if (ft_buttons(ARROW_U, -1))
		plane.vec[1] -= 0.1;
	if (ft_buttons(ARROW_D, -1))
		plane.vec[1] += 0.1;
	if (ft_buttons(SDL_SCANCODE_3, -1))
		plane.pos -= 0.1;
	if (ft_buttons(SDL_SCANCODE_4, -1))
		plane.pos += 0.1;

	if (ft_buttons(A_KEY, -1))
	{
		sphere.vec[0] -= 0.1;
		printf("x: %f\n", sphere.vec[0]);
	}
	if (ft_buttons(D_KEY, -1))
	{
		sphere.vec[0] += 0.1;
		printf("x: %f\n", sphere.vec[0]);
	}
	if (ft_buttons(W_KEY, -1))
	{
		sphere.vec[1] -= 0.1;
		printf("y: %f\n", sphere.vec[1]);
	}
	if (ft_buttons(S_KEY, -1))
	{
		sphere.vec[1] += 0.1;
		printf("y: %f\n", sphere.vec[1]);
	}
	if (ft_buttons(Q_KEY, -1))
	{
		sphere.vec[2] -= 0.1;
		printf("z: %f\n", sphere.vec[2]);
	}
	if (ft_buttons(E_KEY, -1))
	{
		sphere.vec[2] += 0.1;
		printf("z: %f\n", sphere.vec[2]);
	}
	

	SDL_SetRenderDrawColor(window->SDLrenderer, 0, 0, 0, 255);
	SDL_SetRenderTarget(window->SDLrenderer, NULL);
	SDL_RenderClear(window->SDLrenderer);


	SDL_SetRenderTarget(window->SDLrenderer, txt);

	SDL_RenderClear(window->SDLrenderer);

	ft_render(*window, sphere, plane);

	// ft_smooth_step(mid * 10, window);
	SDL_SetRenderTarget(window->SDLrenderer, NULL);
	SDL_RenderCopy(window->SDLrenderer, txt, NULL, NULL);
	SDL_RenderPresent(window->SDLrenderer);
	// printf("%f\n", mid);
	return (sphere);
}

int			main(int argc, char **argv)
{
	SDL_Event	event;
	SDL_Texture *txt = NULL;
	t_window	*window;

	if (SDL_Init(SDL_INIT_EVERYTHING) || !IMG_Init(IMG_INIT_PNG))
		ft_error("could not initialize SDL\n");
	if (!(window = (t_window *)malloc(sizeof(t_window))))
		ft_error("memory allocation failed\n");
	if (!(window->SDLwindow = SDL_CreateWindow("MATH", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RES_X, RES_Y, 0)))
		ft_error("could not create window");
	if (!(window->SDLrenderer = SDL_CreateRenderer(window->SDLwindow, -1, 0)))
		ft_error("could not create renderer");
	txt = ft_empty_texture(window->SDLrenderer);
	t_sphere	sphere;
	t_plane	plane;
	plane.vec[0] = 0.1;
	plane.vec[1] = 0.1;
	plane.vec[2] = 0.8;
	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || event.key.keysym.scancode == ESC)
				return (0);
			else if (event.key.repeat == 0 && event.type == SDL_KEYDOWN)
				ft_buttons(event.key.keysym.scancode, 1);
			else if (event.key.repeat == 0 && event.type == SDL_KEYUP)
				ft_buttons(event.key.keysym.scancode, 0);
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_SetRenderTarget(window->SDLrenderer, txt);
				ft_2d_lines(1, window, event.motion.x, event.motion.y);
				SDL_SetRenderTarget(window->SDLrenderer, NULL);
				SDL_RenderCopy(window->SDLrenderer, txt, NULL, NULL);
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_SPACE)
			{
				SDL_SetRenderTarget(window->SDLrenderer, txt);
				SDL_SetRenderDrawColor(window->SDLrenderer, 0, 0, 0, 255);

				ft_render(*window, sphere, plane);

				SDL_SetRenderTarget(window->SDLrenderer, NULL);
				SDL_SetRenderDrawColor(window->SDLrenderer, 0, 0, 0, 255);
				SDL_RenderCopy(window->SDLrenderer, txt, NULL, NULL);
			}
		}
		sphere = ft_loop(window, txt);
	}
	argc = 0;
	argv = NULL;
}
