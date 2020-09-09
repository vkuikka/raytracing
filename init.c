/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 14:38:45 by vkuikka           #+#    #+#             */
/*   Updated: 2020/09/09 11:54:18 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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

void		ft_load_world(char *file, t_world *world)
{
	char	*line;
	char	*str;
	int		type;
	int		err;
	int		fd;

	if (3 > (fd = open(file, O_RDONLY)))
		ft_error("could not open given file\n");
	if (!(world->view = (t_ray*)malloc(sizeof(t_ray))))
		ft_error("memory allocation failed\n");
	world->view->normal[0] = 1;
	world->view->pos[0] = 0;
	world->view->pos[1] = 0;
	world->view->pos[2] = 0;
	world->view->dir[0] = -0.5;
	world->view->dir[1] = -0.5;
	world->view->dir[2] = 1;
	world->obj = NULL;
	world->lights = NULL;
	world->first_obj = NULL;
	world->first_light = NULL;
	while (0 < (err = get_next_line(fd, &line)))
	{
		type = 0;
		if (!strstr(line, "#"))
		{
			if ((str = ft_strstr(line, "cone")) && (type = 1))
				str += ft_strlen("cone\0") + 1;
			else if ((str = ft_strstr(line, "plane")) && (type = 2))
				str += ft_strlen("plane") + 1;
			else if ((str = ft_strstr(line, "sphere")) && (type = 3))
				str += ft_strlen("sphere") + 1;
			else if ((str = ft_strstr(line, "cylinder")) && (type = 4))
				str += ft_strlen("cylinder") + 1;
			else if ((str = ft_strstr(line, "pointlight")) && (type = -1))
				str += ft_strlen("pointlight") + 1;
		}
		if (type && line[0] != '#')
		{
			if (type > 0)
			{
				world->obj = ft_add_object(world->obj, type, str);
				if (!world->first_obj)
					world->first_obj = world->obj;
			}
			else if (type < 0)
			{
				world->lights = ft_add_object(world->lights, type, str);
				if (!world->first_light)
					world->first_light = world->lights;
			}
		}
		world->obj = world->first_obj;
		world->lights = world->first_light;
		free(line);
	}
	// printf("mod 1: %f\n", world->obj->modifier);
	// printf("mod 2: %f\n", world->obj->next->modifier);
	// printf("mod l: %f\n", world->lights->modifier);
	err < 0 || close(fd) ? ft_error("failed to read given file\n") : free(line);
}

void		ft_init_window(int argc, char **av, t_window **window, SDL_Texture **txt)
{
	int		fd;
	int		i;

	i = 0;
	if (argc != 2)
		ft_error("give one map file to read\n");
	av[0] = NULL;
	if ((fd = open(av[1], O_RDONLY)) < 3 || 0 >= get_next_line(fd, &av[0]))
		ft_error("could not open given file\n");
	free(av[0]);
	close(fd);
	if (SDL_Init(SDL_INIT_EVERYTHING) || !IMG_Init(IMG_INIT_PNG))
		ft_error("could not initialize SDL\n");
	if (!(*window = (t_window *)malloc(sizeof(t_window))))
		ft_error("memory allocation failed\n");
	if (!(window[0]->SDLwindow = SDL_CreateWindow("RTv1", SDL_WINDOWPOS_CENTERED,
							SDL_WINDOWPOS_CENTERED, RES_X, RES_Y, 0)))
		ft_error("could not create window");
	if (!(window[0]->SDLrenderer = SDL_CreateRenderer(window[0]->SDLwindow, -1, 0)))
		ft_error("could not create renderer");
	*txt = ft_empty_texture(window[0]->SDLrenderer);
}