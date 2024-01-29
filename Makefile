NAME = ush
FLAG = -std=c99 -Wall -Wextra -Werror -Wpedantic
SRCD = src
INCD = inc
OBJD = obj

LMXA := libmx/libmx.a
LMXI := libmx/inc

INC  = ush.h
INCS = $(addprefix $(INCD)/, $(INC))

SRC = $(wildcard $(SRCD)/*.c)
OBJS = $(patsubst $(SRCD)/%.c, $(OBJD)/%.o, $(SRC))

CLEAR = \r\33[2K
GREENB = \033[32;1m
YELLOWB = \033[33;1m
REDB = \033[31;1m
NORMAL = \033[0m

all: install

install: $(LMXA) $(NAME)

$(NAME): $(OBJS)
	@clang $(FLAG) $(OBJS) $(LMXA) -o $@
	@printf "${CLEAR}$@\t   ${GREENB}created${NORMAL}\n"

$(OBJD)/%.o: $(SRCD)/%.c $(INCS)
	@clang $(FLAG) -c $< -o $@ -I$(INCD) -I$(LMXI)
	@printf "${CLEAR}$(NAME)\t   ${YELLOWB}compile ${NORMAL}$(<:$(SRCD)/%.c=%) "

$(OBJS): | $(OBJD)

$(OBJD):
	@mkdir -p $@

$(LMXA):
	@make -sC libmx

clean:
	@make -sC libmx $@
	@rm -rf $(OBJD)

uninstall: clean
	@make -sC libmx $@
	@rm -rf $(NAME)
	@printf "$(NAME)\t   ${REDB}uninstalled${NORMAL}\n"

reinstall: uninstall install
