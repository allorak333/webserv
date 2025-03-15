/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 21:23:21 by sangyhan          #+#    #+#             */
/*   Updated: 2025/03/13 21:23:40 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"


Server::Server()
        : requestListManager()
{
}

void Server::initServer(const ServerConfig & config) {
    this->config = config;
    serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFd < 0) {
        throw RuntimeException("socket");
    }
    setNonblock(serverFd);
    int opt = 1;
    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        throw RuntimeException("setsockopt");
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(config.getListen());
    if (bind(serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        throw RuntimeException("bind");
    }
    if (listen(serverFd, 10000) < 0) {
        throw RuntimeException("listen");
    }
}

Server::~Server() {}

ServerConfig Server::getConfig() const {
    return config;
}

int Server::getServerFd() const {
    return serverFd;
}

std::map<pid_t, clock_t>& Server::getChildTime(){
    return childTime;
}

std::vector<Buffer*> Server::processRequest(Buffer* client, HttpRequest& request, std::vector<struct kevent>& changeList) {
    std::string targetPath;

    // Create a RequestHandler based on the request
    requestHandler = RequestHandlerFactory::createRequestHandler(request, config, targetPath, client);

    // Handle the request using the created handler
    RequestResult requestRes = requestHandler->handleRequest();

    if (requestRes.code != 200) {
        std::vector<char> response = requestHandler->generateErrorResponse(requestRes.code);
        client->getWriteBuffer().insert(client->getWriteBuffer().end(), response.begin(), response.end());
        pushEvent(client->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, changeList);
    }

    Message* message = static_cast<Message*>(client);
    if (message->getStatus() == WILL_BE_CLOSE) {
        pushEvent(client->getFd(), EVFILT_READ, EV_DELETE, changeList);
        pushEvent(client->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, changeList);
        message->setStatus(CLOSE);
    }
    if (requestRes.resBuffer.size() > 0) {
        HttpRequest *request_share = new HttpRequest;
        *request_share = request;
        request_share->fileCount = requestRes.resBuffer.size();
        for (Buffer *buf: requestRes.resBuffer) {
            requestListManager.addRequest(buf, client, request_share);
        }
    }
    for (struct kevent event: requestRes.changeList) {
        if (event.filter == EVFILT_PROC) {
            clock_t start;
            start = clock();
            childTime[static_cast<pid_t>(event.ident)] = start;
        }
        changeList.push_back(event);
    }
    return requestRes.resBuffer;
}

void Server::afterProcessRequest(Buffer* file, std::vector<struct kevent>& changeList) {

    std::pair<Buffer*, HttpRequest*> requestPair = requestListManager.getRequest(file);
    if (requestPair.first == NULL)
    {
        return ;
    }
    HttpRequest* request = requestPair.second;
    Buffer *client = requestPair.first;
    if (request->fileCount == 1)
    {
        requestHandler = RequestHandlerFactory::createAfterRequestHandler(request, file, client);
        RequestResult requestRes = requestHandler->handleRequest();
        Message *client_message = static_cast<Message*>(client);
        if (client_message->getStatus() == WILL_BE_CLOSE)
        {
            client_message->setStatus(CLOSE);
        }
        requestListManager.removeRequest(file);
        for (struct kevent event : requestRes.changeList)
        {
            changeList.push_back(event);
        }
        return;
    }
    else
    {
        if ((static_cast<File *>(file))->getError() == true)
        {
            request->error = true;
        }
        static_cast<Message*>(requestPair.first)->deleteResource(file);
        requestListManager.removeRequest(file);
    }
    return;
}

void Server::deleteRequestByFile(Buffer *buf)
{
    requestListManager.removeRequest(buf);
}
