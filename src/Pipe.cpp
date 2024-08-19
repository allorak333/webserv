/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipe.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 17:58:05 by sangyhan          #+#    #+#             */
/*   Updated: 2024/08/09 20:00:22 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Pipe.hpp"


Pipe::Pipe(int fd) : Buffer(fd) {
    this->pid = -1;
    error = false;
    procEnd = false;
    readEnd = false;
}

Pipe::~Pipe() {
}

int Pipe::whoAmI() {
    return 3;
}

const int &Pipe::getServerFd()
{
    return serverFd;
}

void Pipe::setServerFd(int fd)
{
    serverFd = fd;    
}

const bool &Pipe::getError()
{
    return error;
}

void Pipe::setError(bool code)
{
    this->error = code;
}

bool Pipe::End()
{
    if (procEnd == true && readEnd == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void Pipe::setProcEnd(bool code)
{
    procEnd = code;
}

void Pipe::setReadEnd(bool code)
{
    readEnd = code;
}

const pid_t &Pipe::getPid()
{
    return pid;
}

void Pipe::setPid(pid_t pid)
{
    this->pid = pid;
}