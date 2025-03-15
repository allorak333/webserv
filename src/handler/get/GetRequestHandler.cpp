/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequestHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:54:49 by sangyhan          #+#    #+#             */
/*   Updated: 2025/02/13 07:31:20 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetRequestHandler.hpp"


GetRequestHandler::GetRequestHandler(HttpRequest& request, const ServerConfig& config, std::string& targetPath, Buffer* client)
: request(request), targetPath(targetPath), client(client) {
    (void)config;
}

GetRequestHandler::~GetRequestHandler() {}

RequestResult GetRequestHandler::handleRequest() {
    RequestResult result;

    if (access(targetPath.c_str(), F_OK | R_OK) == 0)
    {
        int fileFd = open(targetPath.c_str(), O_RDONLY);
        if (fileFd > 0) {
            struct stat st;
            stat(targetPath.c_str(), &st);
            size_t fileSize = st.st_size;

            File* file = new File(fileFd);
            file->setServerFd(request.serverFd); // Assuming serverFd is accessible
            static_cast<Message*>(client)->addResource(file);

            if (fileSize == 0)
            {
                // Directly create the response if the file is empty
                struct kevent change;

                //Post Processing
                std::string contentType;
                int code;
                std::string message;

                code = 200;
                message = "OK";

                contentType = getContentType(request.url);
                std::string header = HttpResponse::generateHeader(request, code, message, file->getReadBuffer(), contentType);
                client->getWriteBuffer().reserve(header.size() + file->getReadBuffer().size());
                client->getWriteBuffer().insert(client->getWriteBuffer().end(), header.begin(), header.end());
                client->getWriteBuffer().insert(client->getWriteBuffer().end(), file->getReadBuffer().begin(), file->getReadBuffer().end());
                EV_SET(&change, client->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
                result.changeList.push_back(change);
                close(fileFd);
                delete file;
                result.code = code;
                return result; //Don't add to changeList because the response has already been added.
            }
            else
            {
                result.resBuffer.push_back(file);
                pushEvent(fileFd, EVFILT_READ, EV_ADD | EV_ENABLE, result.changeList);
            }
        }
        else
        {
            // Handle file open error (e.g., log it, return 500)
            result.code = 500;
            return result; // Internal Server Error
        }

    }
    else
    {
        if (access(targetPath.c_str(), F_OK) == 0) {
            result.code = 403;
            return result; // 403 Forbidden
        } else {
            result.code = 404;
            return result; // 404 Not Found
        }
    }
    result.code = 200;
    return result;
}


std::vector<char> GetRequestHandler::generateErrorResponse(int errorCode) {
    // Delegate error response generation to DefaultRequestHandler
    DefaultRequestHandler defaultHandler(request, targetPath, client); // Create a temporary instance
    return defaultHandler.generateErrorResponse(errorCode);
}