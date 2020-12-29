/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 18:28:42 by vkuikka           #+#    #+#             */
/*   Updated: 2020/12/30 00:17:07 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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

void	ft_loop(t_window *window, t_world world, SDL_Texture *txt)
{
	if (ft_buttons(SDL_SCANCODE_1, -1))
		world.obj->modifier += 0.1;
	if (ft_buttons(SDL_SCANCODE_2, -1))
		world.obj->modifier -= 0.1;

	if (ft_buttons(SDL_SCANCODE_3, -1))
		world.obj->next->modifier -= 0.1;
	if (ft_buttons(SDL_SCANCODE_4, -1))
		world.obj->next->modifier += 0.1;

	if (ft_buttons(SDL_SCANCODE_9, -1))
		world.lights->pos[2] -= 0.3;
	if (ft_buttons(SDL_SCANCODE_0, -1))
		world.lights->pos[2] += 0.3;

	if (ft_buttons(SDL_SCANCODE_T, -1))
		world.view->dir[1] -= 0.05;
	if (ft_buttons(SDL_SCANCODE_G, -1))
		world.view->dir[1] += 0.05;
	if (ft_buttons(SDL_SCANCODE_F, -1))
		world.view->dir[0] -= 0.05;
	if (ft_buttons(SDL_SCANCODE_H, -1))
		world.view->dir[0] += 0.05;

	if (ft_buttons(SDL_SCANCODE_I, -1))
		world.lights->pos[1] -= 0.5;
	if (ft_buttons(SDL_SCANCODE_K, -1))
		world.lights->pos[1] += 0.5;
	if (ft_buttons(SDL_SCANCODE_J, -1))
		world.lights->pos[0] -= 0.5;
	if (ft_buttons(SDL_SCANCODE_L, -1))
		world.lights->pos[0] += 0.5;
		// world.view->pos[0] += 0.1;

	if (ft_buttons(SDL_SCANCODE_RIGHT, -1))
		world.obj->next->dir[0] -= 0.1;
	if (ft_buttons(SDL_SCANCODE_LEFT, -1))
		world.obj->next->dir[0] += 0.1;
	if (ft_buttons(SDL_SCANCODE_UP, -1))
		world.obj->next->dir[2] -= 0.1;
	if (ft_buttons(SDL_SCANCODE_DOWN, -1))
		world.obj->next->dir[2] += 0.1;

	if (ft_buttons(SDL_SCANCODE_COMMA, -1))
		world.obj->next->dir[1] -= 0.1;
	if (ft_buttons(SDL_SCANCODE_SLASH, -1))
		world.obj->next->dir[1] += 0.1;

	if (ft_buttons(SDL_SCANCODE_N, -1))
		world.obj->next->pos[1] -= 0.1;
	if (ft_buttons(SDL_SCANCODE_M, -1))
		world.obj->next->pos[1] += 0.1;
	if (ft_buttons(SDL_SCANCODE_V, -1))
		world.obj->next->pos[0] -= 0.1;
	if (ft_buttons(SDL_SCANCODE_B, -1))
		world.obj->next->pos[0] += 0.1;

	if (ft_buttons(SDL_SCANCODE_A, -1))
		world.obj->dir[0] -= 0.1;
	if (ft_buttons(SDL_SCANCODE_D, -1))
		world.obj->dir[0] += 0.1;
	if (ft_buttons(SDL_SCANCODE_W, -1))
		world.obj->dir[1] -= 0.1;
	if (ft_buttons(SDL_SCANCODE_S, -1))
		world.obj->dir[1] += 0.1;
	if (ft_buttons(SDL_SCANCODE_Q, -1))
		world.obj->dir[2] -= 0.1;
	if (ft_buttons(SDL_SCANCODE_E, -1))
		world.obj->dir[2] += 0.1;
	
	SDL_SetRenderDrawColor(window->SDLrenderer, 0, 0, 0, 255);
	SDL_SetRenderTarget(window->SDLrenderer, NULL);
	SDL_RenderClear(window->SDLrenderer);


	SDL_SetRenderTarget(window->SDLrenderer, txt);

	SDL_RenderClear(window->SDLrenderer);

	ft_render(*window, world);

	// ft_smooth_step(mid * 10, window);
	SDL_SetRenderTarget(window->SDLrenderer, NULL);
	SDL_RenderCopy(window->SDLrenderer, txt, NULL, NULL);
	SDL_RenderPresent(window->SDLrenderer);
	// printf("%f\n", mid);
	return ;
}

int			main(int argc, char **argv)
{
	SDL_Texture *txt;
	SDL_Event	event;
	t_window	*window;
	t_world		world;
	unsigned	frametime;

	ft_init_window(argc, argv, &window, &txt);
	ft_load_world(argv[1], &world);
	while (1)
	{
		frametime = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
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
				ft_load_world(argv[1], &world);
		}
		ft_loop(window, world, txt);
		frametime = SDL_GetTicks() - frametime;
		if (frametime < TICKTIME)
		{
			usleep((TICKTIME - frametime) * 1000);
			printf("overtick: %u ms\n", (TICKTIME - frametime));
		}
	}
}
