/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 18:28:50 by vkuikka           #+#    #+#             */
/*   Updated: 2020/09/11 15:38:33 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# define RES_X 500.0
# define RES_Y 500.0
# define POINTLIGHT -1
# define CONE 1
# define PLANE 2
# define SPHERE 3
# define CYLINDER 4

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
	float				normal[3];	//normal of intersection point
	float				reflect[3];	//reflected ray direction from intersection point
	int					obj_index;	//prevent ray from intersecting the object it is cast from in bounces
}						t_ray;

typedef struct			s_objects
{
	int					type;		//cone, plane, sphere, cylinder or negative numbers for lights
	int					index;
	float				dir[3];		//not used with sphere
	float				pos[3];
	float				modifier;	//for example the size of sphere or cylinder
	struct s_objects	*next;		//linked list so new objects can easily be added
}						t_objects;

typedef struct			s_world
{
	struct s_objects	*obj;			//linked list of objects in world
	struct s_objects	*first_obj;		//used for reversing object linked list
	struct s_objects	*lights;		//linked list of lights in world
	struct s_objects	*first_light;	//used for reversing light linked list
	struct s_ray		*view;			//first ray that is cast from the "eye"
}						t_world;

void		ft_smooth_step(float mid, t_window *window);
void		ft_2d_weighted_avg(float mid, t_window *window, int line1[2], int line2[2]);
void		ft_2d_lines(float mid, t_window *window, int x, int y);
void		ft_draw_line(int line[4], t_window *window, float mid);

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
