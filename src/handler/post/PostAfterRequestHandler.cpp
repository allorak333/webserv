/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostAfterRequestHandler.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 07:02:57 by sangyhan          #+#    #+#             */
/*   Updated: 2025/02/13 07:15:40 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostAfterRequestHandler.hpp"



PostAfterRequestHandler::PostAfterRequestHandler(HttpRequest* request, Buffer* file,Buffer* client)
    : request(request), file(file), client(client) {}

PostAfterRequestHandler::~PostAfterRequestHandler() {}

RequestResult PostAfterRequestHandler::handleRequest() {
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
        code = 201;
        message = "Created";
    }
    contentType = getContentType(request->url);
    header = HttpResponse::generateHeader(*request, code, message, file->getReadBuffer(), contentType); // Pass request
    client->getWriteBuffer().reserve(header.size() + file->getReadBuffer().size());
    client->getWriteBuffer().insert(client->getWriteBuffer().end(), header.begin(), header.end());
    client->getWriteBuffer().insert(client->getWriteBuffer().end(), file->getReadBuffer().begin(), file->getReadBuffer().end());
    pushEvent(client->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, result.changeList);
    return result;
}

std::vector<char> PostAfterRequestHandler::generateErrorResponse(int errorCode)
{
    //You should not use this function
    (void)errorCode;
    return std::vector<char>();
}