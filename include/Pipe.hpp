/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipe.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 17:58:34 by sangyhan          #+#    #+#             */
/*   Updated: 2024/08/09 19:59:53 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_HPP
# define PIPE_HPP

#include <string>
#include <unistd.h>
#include "Buffer.hpp"

class Pipe : public Buffer {
	private:
		int serverFd;
		bool error;
		bool procEnd;
		bool readEnd;
		pid_t pid;

	public:
		Pipe(int fd);
		~Pipe();

		int whoAmI();
		const bool &getError();
		void setError(bool code);
		const int &getServerFd();
		const pid_t &getPid();
		void setPid(pid_t pid);
		void setServerFd(int fd);
		bool End();
		void setProcEnd(bool code);
		void setReadEnd(bool code);
};

#endif
