/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:22:49 by haejeong          #+#    #+#             */
/*   Updated: 2024/07/29 14:03:33 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>
#include <vector>
#include <memory> // For smart pointers
#include "ServerConfig.hpp"
#include "RequestListManager.hpp"
#include "../buffer/Message.hpp"
#include "../buffer/Pipe.hpp"
#include "../buffer/File.hpp"
#include "../handler/RequestHandlerFactory.hpp"

class Server {
private:
    ServerConfig                        config;
    int									serverFd;
    struct sockaddr_in 					serverAddr;
    RequestListManager                  requestListManager;
    std::unique_ptr<IRequestHandler>    requestHandler; // Use strategy pattern
    std::map<pid_t, clock_t>			childTime;

public:
    Server();
    ~Server();
    std::map<pid_t, clock_t>&   getChildTime();
    void                        initServer(const ServerConfig& config);
    ServerConfig                getConfig() const;
    int                         getServerFd() const;
    std::vector<Buffer*>        processRequest(Buffer* client, HttpRequest& request, std::vector<struct kevent>& changeList);
    void                        afterProcessRequest(Buffer *file, std::vector<struct kevent>& changeList);
    void                        deleteRequestByFile(Buffer *buf);

};

#endif