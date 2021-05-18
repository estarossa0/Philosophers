NAME = philo_three
FLAGS = # -Wall -Wextra -Werror
SRC_FOLDER = ./srcs
HEADER_FOLDER = ./includes
OBJECT_FOLDER = ./objects
LIBS =
RED = \033[0;31m
GREEN = \033[0;32m
GOLD = \033[0;33m
RESET = \033[0m

SRC_FILES = main.c \
			need.c \
			libft.c \
			philo.c \

OBJECT_FILES = $(SRC_FILES:.c=.o)
OBJECT_FILES := $(addprefix $(OBJECT_FOLDER)/, $(OBJECT_FILES))

.PHONY = all clean fclean re credit

all: credit $(NAME)

$(NAME): $(OBJECT_FILES)
	@clang -g -I $(HEADER_FOLDER)  $(OBJECT_FILES) $(LIBS) -o $@
	@echo
	@echo $(NAME)" created $(GREEN)successfully$(RESET)"

$(OBJECT_FOLDER)/%.o: $(SRC_FOLDER)/%.c
	@(mkdir $(OBJECT_FOLDER) 2> /dev/null && echo "creating "$(OBJECT_FOLDER)" folder $(GREEN){OK}$(RESET)") || true
	@clang $(FLAGS) -g -I $(HEADER_FOLDER) -o $@ -c $< && echo "creating" $< "object $(GREEN){OK}$(RESET)"

clean:
	@(rm $(OBJECT_FILES) 2> /dev/null && echo "$(RED)deleting$(RESET): " $(OBJECT_FILES)) || true
	@(rm -r $(OBJECT_FOLDER) 2> /dev/null && echo "$(RED)deleting$(RESET): " $(OBJECT_FOLDER)) || true

fclean: clean
	@(rm $(NAME) 2> /dev/null && echo "$(RED)deleting$(RESET): " $(NAME)) || true

re: fclean $(NAME)

credit:
	@printf "					$(RED)_$(NAME) made by:_$(RESET)\n"
	@echo "███████╗ ██████╗████████╗ █████╗ ██████╗  █████╗  ██████╗ ██████╗ █████╗"
	@echo "██╔════╝██╔════╝╚══██╔══╝██╔══██╗██╔══██╗██╔══██╗██╔════╝██╔════╝██╔══██╗"
	@echo "█████╗  ╚█████╗    ██║   ███████║██████╔╝██║  ██║╚█████╗ ╚█████╗ ███████║"
	@echo "██╔══╝   ╚═══██╗   ██║   ██╔══██║██╔══██╗██║  ██║ ╚═══██╗ ╚═══██╗██╔══██║"
	@echo "███████╗██████╔╝   ██║   ██║  ██║██║  ██║╚█████╔╝██████╔╝██████╔╝██║  ██║"
	@echo "╚══════╝╚═════╝    ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝"
