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

all: install

install: $(LMXA) $(NAME)

$(NAME): $(OBJS)
	@clang $(FLAG) $(OBJS) $(LMXA) -o $@

$(OBJD)/%.o: $(SRCD)/%.c $(INCS)
	@clang $(FLAG) -c $< -o $@ -I$(INCD) -I$(LMXI)

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

reinstall: uninstall install
