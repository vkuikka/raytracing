/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 18:28:50 by vkuikka           #+#    #+#             */
/*   Updated: 2020/09/07 21:19:39 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASD_H
# define ASD_H

# define RES_X 500.0
# define RES_Y 500.0

#define APL_SDL2
#ifdef APL_SDL2
# define ESC 41
# define W_KEY 26
# define A_KEY 4
# define S_KEY 22
# define D_KEY 7
# define Q_KEY 20
# define E_KEY 8
# define ARROW_U 82
# define ARROW_L 80
# define ARROW_D 81
# define ARROW_R 79
# define NUM1 30
# define NUM2 31
# define NUM3 32
# define NUM4 33
# define NUM5 34
# define NUM6 35
# define NUM7 36
# define NUM8 37
# define NUM9 38
# define NUM0 39
#endif

# include <math.h>
# include <fcntl.h>
# include "get_next_line.h"
# include "SDL2/SDL.h"
# include "SDL2_image/SDL_image.h"

typedef struct		s_window
{
	SDL_Renderer	*SDLrenderer;
	SDL_Window		*SDLwindow;
}					t_window;

typedef struct		s_vector
{
	float			vector[3];
}					t_vector;

typedef struct		s_ray
{
	float			pos[3];
	float			dir[3];
}					t_ray;

typedef struct		s_plane
{
	float			vec[3];
	float			pos;
}					t_plane;

typedef struct		s_sphere
{
	float			vec[3];
	float			r;
}					t_sphere;

void		ft_smooth_step(float mid, t_window *window);
void		ft_2d_weighted_avg(float mid, t_window *window, int line1[2], int line2[2]);
void		ft_2d_lines(float mid, t_window *window, int x, int y);
void		ft_draw_line(int line[4], t_window *window, float mid);//unsigned c2)

void		ft_normalize_two(float vec1[3], float vec2[3]);
void		ft_normalize(float vec1[3]);

double		ft_vector_length(float vec[3]);
double		ft_vector_dot(float ve1[3], float ve2[3]);

void		ft_render(t_window window, t_sphere sphere, t_plane plane);

#endif
