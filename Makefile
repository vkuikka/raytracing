# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/08 13:46:11 by vkuikka           #+#    #+#              #
#    Updated: 2020/09/04 20:03:09 by vkuikka          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = asd
FILES = main.c smoothstep.c weighted_avg.c draw.c raytracing.c
LIB = libft/libft.a
INCLUDE = libft/includes
#FLAGS = -Wall -Wextra -Werror
.PHONY: clean fclean re all

all: $(NAME)

$(NAME):
	#@make -C libft
	gcc $(FLAGS) -framework SDL2 -framework SDL2_image -F ./ $(FILES) $(LIB) -I $(INCLUDE) -o $(NAME) -rpath @executable_path

clean:
	#@make clean -C libft

fclean: clean
	rm -f $(NAME)
	#@make fclean -C libft

re: fclean
	make all
