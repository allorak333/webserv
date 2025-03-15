/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetAfterRequestHandler.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 07:02:00 by sangyhan          #+#    #+#             */
/*   Updated: 2025/02/13 07:16:49 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "GetAfterRequestHandler.hpp"

GetAfterRequestHandler::GetAfterRequestHandler(HttpRequest* request, Buffer* file, Buffer* client)
    : request(request), file(file), client(client) {}

GetAfterRequestHandler::~GetAfterRequestHandler() {}

RequestResult GetAfterRequestHandler::handleRequest() {
    RequestResult result;
    std::string contentType;
    int code;
    std::string message;
    std::string header;

    if ((static_cast<File *>(file))->getError() == true || request->error == true)
    {
        code = 500;
        message = "Internal Server Error";
        file->getReadBuffer().clear();
    }  
    else 
    {
        code = 200;
        message = "OK";
    }
    contentType = getContentType(request->url);
    header = HttpResponse::generateHeader(*request, code, message, file->getReadBuffer(), contentType); // Pass request
    client->getWriteBuffer().reserve(header.size() + file->getReadBuffer().size());
    client->getWriteBuffer().insert(client->getWriteBuffer().end(), header.begin(), header.end());
    client->getWriteBuffer().insert(client->getWriteBuffer().end(), file->getReadBuffer().begin(), file->getReadBuffer().end());
    pushEvent(client->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, result.changeList);
    return result;
}

std::vector<char> GetAfterRequestHandler::generateErrorResponse(int errorCode)
{
    (void)errorCode;
    return std::vector<char>();
}