# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/11 15:42:22 by sangyhan          #+#    #+#              #
#    Updated: 2025/03/15 18:44:53 by sangyhan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME     = webserv

CXX      = c++
CXXFLAGS = -O2 -Wall -Wextra -Werror -std=c++17

SRCS     = $(shell find src -type f -name '*.cpp')
INCS     = $(wildcard include/*.hpp)
OBJDIR   = obj
OBJS     = $(SRCS:src/%.cpp=$(OBJDIR)/%.o)

OBJDIRS  = $(sort $(dir $(OBJS)))

all: $(OBJDIRS) $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJDIRS):
	mkdir -p $@

$(OBJDIR)/%.o: src/%.cpp $(INCS)
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re