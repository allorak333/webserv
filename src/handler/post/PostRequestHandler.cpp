/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostRequestHandler.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:55:56 by sangyhan          #+#    #+#             */
/*   Updated: 2025/03/15 16:32:38 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "PostRequestHandler.hpp"

PostRequestHandler::PostRequestHandler(HttpRequest& request, const ServerConfig& config, std::string& targetPath, Buffer* client)
: request(request), config(config), targetPath(targetPath), client(client) {}

PostRequestHandler::~PostRequestHandler() {}

RequestResult PostRequestHandler::handleRequest() {
    RequestResult result;

    if (request.contentType.compare(0, 20, "multipart/form-data;") == 0)
    {
        std::string boundary = request.contentType.substr(30, std::string::npos); // Extract boundary.
        if (!boundary.empty())
        {
            MimeParser parser(boundary, client->getReadBuffer(), request.bodyStart, request.bodyEnd);
            std::vector<Buffer*> files = parser.parse(config.getRoot()); // Get parsed files.

			if (files.size() > 0)
			{
				for (size_t i = 0; i < files.size(); ++i)
                {
                    static_cast<Message*>(client)->addResource(files[i]);  // Add to client's resources.
                    static_cast<File*>(files[i])->setServerFd(request.serverFd);
                    pushEvent(files[i]->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, result.changeList); // Add write event.
                    result.resBuffer.push_back(files[i]);
                }
			}
            else
            {
                result.code = 400;
                return result;
            }
        }
        else {
            result.code = 400;
            return result; // Bad Request
        }
    }
    else if (request.contentType.compare(0, 33, "application/x-www-form-urlencoded") == 0)
    {
        result.code = 501;
        return result; //Not implemented
    }
    else //Normal File Upload
    {
        int fileFd = open(targetPath.c_str(), O_RDWR, 0644); // Open with read/write permissions.
        if (fileFd > 0)
        {
            File* file = new File(fileFd);
            file->setServerFd(request.serverFd);
            static_cast<Message*>(client)->addResource(file);
            file->getWriteBuffer().reserve(request.bodyEnd - request.bodyStart);
            file->getWriteBuffer().insert(file->getWriteBuffer().end(), client->getReadBuffer().begin() + request.bodyStart,
                client->getReadBuffer().begin() + request.bodyEnd); // Write request body to file.
            pushEvent(fileFd, EVFILT_WRITE, EV_ADD | EV_ENABLE, result.changeList); // Add write event.
            result.resBuffer.push_back(file);
        }
        else
        {
            result.code = 500;
            return result;
        }
    }
    result.code = 200;
    return result; // Or 201 Created, depending on whether a new resource was created.
}


std::vector<char> PostRequestHandler::generateErrorResponse(int errorCode) {
    DefaultRequestHandler defaultHandler(request, targetPath, client);
    return defaultHandler.generateErrorResponse(errorCode);;
}