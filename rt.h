/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 18:28:50 by vkuikka           #+#    #+#             */
/*   Updated: 2020/09/08 22:26:32 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# define RES_X 500.0
# define RES_Y 500.0

#include <stdio.h>

# include <math.h>
# include <fcntl.h>
# include "get_next_line.h"
# include "SDL2/SDL.h"
# include "SDL2_image/SDL_image.h"

typedef struct			s_window
{
	SDL_Renderer		*SDLrenderer;
	SDL_Window			*SDLwindow;
}						t_window;

typedef struct			s_ray
{
	float				pos[3];
	float				dir[3];
	float				normal[3];
	float				reflect[3];
	int					obj_index;	//for bounces
}						t_ray;

typedef struct			s_objects
{
	int					type;	//index: plane, sphere, cylinder, cone
	int					index;
	float				vec[3];
	float				modifier;
	struct s_objects	*next;
}						t_objects;

typedef struct			s_world
{
	struct s_objects	*obj;
	struct s_objects	*first_obj;
	struct s_objects	*lights;
	struct s_objects	*first_light;
	struct s_ray		*view;
}						t_world;


void		ft_smooth_step(float mid, t_window *window);
void		ft_2d_weighted_avg(float mid, t_window *window, int line1[2], int line2[2]);
void		ft_2d_lines(float mid, t_window *window, int x, int y);
void		ft_draw_line(int line[4], t_window *window, float mid);//unsigned c2)

void		ft_normalize_two(float vec1[3], float vec2[3]);
void		ft_normalize(float vec1[3]);

double		ft_vector_length(float vec[3]);
double		ft_vector_dot(float ve1[3], float ve2[3]);

// void		ft_init_window(int argc, char **av, t_window *window, SDL_Texture *txt);
void		ft_init_window(int argc, char **av, t_window **window, SDL_Texture **txt);

void		ft_render(t_window window, t_world world);

// void		ft_add_object(t_objects *obj, int type, char *values);
t_objects	*ft_add_object(t_objects *obj, int type, char *values);

void		ft_load_world(char *file, t_world *world);

#endif
