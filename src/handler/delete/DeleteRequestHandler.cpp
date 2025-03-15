/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteRequestHandler.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:58:26 by sangyhan          #+#    #+#             */
/*   Updated: 2025/02/12 23:26:15 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DeleteRequestHandler.hpp"

DeleteRequestHandler::DeleteRequestHandler(HttpRequest& request, const ServerConfig& config, std::string& targetPath, Buffer* client)
    : request(request), targetPath(targetPath), client(client) {
    (void)config;
}

DeleteRequestHandler::~DeleteRequestHandler() {}

RequestResult DeleteRequestHandler::handleRequest() {
    RequestResult result;

    if (access(targetPath.c_str(), F_OK) == 0) {
        if (std::remove(targetPath.c_str()) < 0) {
            result.code = 500;
            return result; // Internal Server Error
        } else {
            // Successful deletion, create a 200 OK response with no body
            std::vector<char> header = HttpResponse::generateResponseWithNoBody(request, 200); //Use the HttpResponse static method.
            client->getWriteBuffer().reserve(header.size());
            client->getWriteBuffer().insert(client->getWriteBuffer().end(), header.begin(), header.end());
            pushEvent(client->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, result.changeList);
            result.code = 200;
            return result;
        }
    } else {
        result.code = 404;
        return result; // Not Found
    }
}

std::vector<char> DeleteRequestHandler::generateErrorResponse(int errorCode) {
    DefaultRequestHandler defaultHandler(request, targetPath, client);
    return defaultHandler.generateErrorResponse(errorCode);
}