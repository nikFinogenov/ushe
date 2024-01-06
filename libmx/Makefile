NAME	=	libmx.a

CFLG	=	-std=c11 -Wall -Wextra -Werror -Wpedantic
COMP 	= 	clang

SRCD	=	src
INCD	=	inc
OBJD	=	obj

INC		=	libmx.h
INCS	=	$(addprefix $(INCD)/, $(INC))

SRC = $(wildcard $(SRCD)/*.c)

SRCS	=	$(addprefix $(SRCD)/, $(SRC))
OBJS = $(addprefix $(OBJD)/, $(notdir $(SRC:%.c=%.o)))

CLEAR = \r\33[2K
GREENB = \033[32;1m
YELLOWB = \033[33;1m
REDB = \033[31;1m
NORMAL = \033[0m

all: install
	
install: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $@ $^
	@printf "${CLEAR}$@\t   ${GREENB}created${NORMAL}\n"

$(OBJD)/%.o: $(SRCD)/%.c $(INCS) | $(OBJD)
	@$(COMP) $(CFLG) -c $< -o $@ -I$(INCD)
	@printf "${CLEAR}$(NAME)\t   ${YELLOWB}compile ${NORMAL}$(<:$(SRCD)/%.c=%) "

$(OBJS): | $(OBJD)

$(OBJD):
	@mkdir -p $@
	
clean:
	@rm -rf $(OBJD)
	@printf "$(OBJD)\t   ${REDB}deleted${NORMAL}\n"

uninstall: clean
	@rm -rf $(NAME)
	@printf "$(NAME)\t   ${REDB}uninstalled${NORMAL}\n"

reinstall: uninstall install

start: 
	$(COMP) $(CFLG) main.c libmx.a && ./a.out

start_i: install
	$(COMP) $(CFLG) main.c libmx.a && ./a.out
	