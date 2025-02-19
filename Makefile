# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/11 15:42:22 by sangyhan          #+#    #+#              #
#    Updated: 2025/01/18 17:58:02 by sangyhan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME     = webserv

CXX      = c++
CXXFLAGS = -O2 -Wall -Wextra -Werror -std=c++98

SRCS     = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
INCS     = $(wildcard include/*.hpp)
OBJDIR   = obj
OBJS     = $(SRCS:src/%.cpp=$(OBJDIR)/%.o)
# 필요한 모든 obj 하위 디렉토리 목록 생성
OBJDIRS  = $(sort $(dir $(OBJS)))

all: $(OBJDIRS) $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# 미리 모든 필요한 디렉토리를 생성
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