NAME := ft_ping

CC := gcc
CFLAGS := -Wall -Wextra -Werror -MMD

SRCS_PATH := src
OBJS_PATH := .obj

SRCS_FILES := main.c parse_args.c socket.c ping.c recv_ping.c

SRCS := ${addprefix ${SRCS_PATH}/, ${SRCS_FILES}}
OBJS := ${addprefix ${OBJS_PATH}/, ${SRCS_FILES:.c=.o}}
MMD_FILES := ${OBJS:.o=.d}

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} -o $@ $^

${OBJS_PATH}/%.o: ${SRCS_PATH}/%.c | ${OBJS_PATH}
	${CC} ${CFLAGS} -o $@ -c $<

${OBJS_PATH}:
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf ${OBJS_PATH}

.PHONY: fclean
fclean: clean
	rm -rf ${NAME}

.PHONY: re
re: fclean all

-include ${MMD_FILES}

### Vagrant

.PHONY: vm.up
vm.up:
	vagrant up

.PHONY: vm.stop
vm.stop:
	vagrant halt

.PHONY: vm.reboot
vm.reboot: vm.stop vm.up

.PHONY: vm.reload
vm.reload:
	vagrant reload

.PHONY: vm.ssh
vm.ssh: vm.up
	vagrant ssh

.PHONY: vm.clean
vm.clean:
	vagrant destroy
