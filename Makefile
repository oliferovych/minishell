# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/01 17:09:19 by dolifero          #+#    #+#              #
#    Updated: 2024/06/27 16:33:50 by dolifero         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= minishell

CC				= cc
CFLAGS			= -Wall -Wextra -Werror
LDFLAGS			= -lreadline

LIBFT_DIR 		= Libft
LIBFT			= $(LIBFT_DIR)/libft.a

SRC_DIR			= ./src
OBJ_DIR			= ./obj

SUBDIRS			=	parsing\
					builtin_commands\
					environment\
					execution\
					parsing\
					prompts\
					signals\
					utilities

SRC_FILES		= $(SRC_DIR)/main.c $(foreach dir, $(SUBDIRS), $(wildcard $(SRC_DIR)/$(dir)/*.c))
OBJ_FILES		= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

all: $(NAME)

$(NAME): $(OBJ_FILES) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR) $(foreach dir, $(SUBDIRS), $(OBJ_DIR)/$(dir))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


BOLD_PURPLE	=	\033[1;35m
BOLD_CYAN	=	\033[1;36m
BOLD_YELLOW	=	\033[1;33m
BOLD_WHITE	=	\033[1;97m
BOLD_GREEN	=	\033[1;92m
NO_COLOR	=	\033[0m
DEF_COLOR	=	\033[0;39m
GRAY		=	\033[0;90m
RED			=	\033[0;91m
GREEN		=	\033[0;92m
YELLOW		=	\033[0;93m
BLUE		=	\033[0;94m
MAGENTA		=	\033[0;95m
CYAN		=	\033[2;96m
BR_CYAN		=	\033[0;96m
WHITE		=	\033[0;97m

all:			$(NAME)
				clear;
				@$(MAKE) SHELL_CYAN
				@$(MAKE) loading
				clear;
				@$(MAKE) SHELL_GREEN
				@echo "             $(BOLD_GREEN)${NAME} DONE!\n$(DEF_COLOR)"

clean:
				@echo "$(CYAN)"
				rm -rf $(OBJ_DIR)
				make clean -C $(LIBFT_DIR)
				clear;
				@echo "$(DEF_COLOR)"

debug:			CFLAGS += -g
debug:			CFLAGS += -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment
debug:			CFLAGS += -DDEBUG=1
debug:			clean all

fclean:	clean
				@echo "$(CYAN)"
				rm -f $(NAME)
				make fclean -C $(LIBFT_DIR)
				clear;
				@echo "$(DEF_COLOR)"

re:				fclean all

SHELL_CYAN:
				@echo "\n$(BR_CYAN)              _       _      __         ____"
				@echo "   ____ ___  (_)___  (_)____/ /_  ___  / / /"
				@echo "  / __  __ \/ / __ \/ / ___/ __ \/ _ \/ / / "
				@echo " / / / / / / / / / / (__  ) / / /  __/ / /  "
				@echo "/_/ /_/ /_/_/_/ /_/_/____/_/ /_/\___/_/_/   "
				@echo "                                            $(BOLD_CYAN)\n"

SHELL_GREEN:
				@echo "\n$(GREEN)              _       _      __         ____"
				@echo "   ____ ___  (_)___  (_)____/ /_  ___  / / /"
				@echo "  / __  __ \/ / __ \/ / ___/ __ \/ _ \/ / / "
				@echo " / / / / / / / / / / (__  ) / / /  __/ / /        $(BOLD_GREEN)-tecker$(GREEN)"
				@echo "/_/ /_/ /_/_/_/ /_/_/____/_/ /_/\___/_/_/        $(BOLD_GREEN)-dolifero$(GREEN)"
				@echo "                                            $(DEF_COLOR)\n"

loading:
				@for i in {1..42}; do \
					printf '%s' "█"; \
					sleep 0.01; \
				done

.PHONY:			all clean fclean re
