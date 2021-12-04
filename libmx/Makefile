NAME = libmx.a

SRC_FILES = $(wildcard src/*.c)
OBJ_FILES = $(addprefix obj/, $(notdir $(SRC_FILES:%.c=%.o)))
INC_FILES = $(wildcard inc/*.h)

CC = clang
CFLAGS = -std=c11 $(addprefix -W, all extra error pedantic) -g \

MKDIR = mkdir -p
RM = rm -rf

all: clean $(NAME)

$(NAME): $(OBJ_FILES)
	@ar rcs $@ $^

$(OBJ_FILES): | obj

obj/%.o: src/%.c $(INC_FILES)
	@$(CC) $(CFLAGS) -c $< -o $@ -I inc
	@printf "\r\33[2K$(NAME)\033[33;1m\t compile \033[0m$(<:src/%.c=%)\r"

obj:
	@$(MKDIR) $@

clean:
	@$(RM) obj
	@printf "obj in $(NAME)\t \033[31;1mdeleted\033[0m\n"

uninstall:
	@$(RM) obj
	@$(RM) $(NAME)
	@@printf "$(NAME)\t \033[31;1muninstalled\033[0m\n"

reinstall: uninstall all

.PHONY: all uninstall clean reinstall

