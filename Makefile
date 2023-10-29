CXX			:=	c++
CXXFLAGS	:= -std=c++98 -g
#-Wall -Wextra -Werror
LDFLAGS		:=

SRCDIR		:=	src
INCDIR		:=	inc

SRCS		:=	$(SRCDIR)/main.cpp\
				$(SRCDIR)/IrcServ.cpp\

INCS		:=	$(INCDIR)/IrcServ.cpp\

OBJS		:=	$(SRCS:.cpp=.o)

NAME		:= ircserv

.SUFFIXES : .cpp .o
.cpp.o :
	$(CXX) $(CXXFLAGS) -c $< -o $@

all	: $(NAME)

$(NAME)	:	$(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

clean :
	rm -f $(OBJS)

fclean :
	make clean
	rm -f $(NAME)

re :
	make fclean
	make $(NAME)

.PHONY : all clean fclean re
