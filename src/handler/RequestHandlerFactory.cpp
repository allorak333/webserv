/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerFactory.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:50:38 by sangyhan          #+#    #+#             */
/*   Updated: 2025/03/13 21:46:30 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "RequestHandlerFactory.hpp"

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createRequestHandler(
    HttpRequest& request,
    ServerConfig& config,
    std::string& targetPath,
    Buffer* client
) {
    int responseCode = DefaultRequestHandler::validateRequest(request, config, targetPath);

    if (responseCode == 200)
    {
        if (isCGI(targetPath)) {
            return std::make_unique<CgiRequestHandler>(request, config, targetPath, client);
        } else if (request.method == GET) {
            return std::make_unique<GetRequestHandler>(request, config, targetPath, client);
        } else if (request.method == POST) {
            return std::make_unique<PostRequestHandler>(request, config, targetPath, client);
        } else if (request.method == DELETE) {
            return std::make_unique<DeleteRequestHandler>(request, config, targetPath, client);
        }
    }
    else if (responseCode == 800)
    {
        return std::make_unique<AutoIndexRequestHandler>(request, config, targetPath, client);
    }
    return std::make_unique<DefaultRequestHandler>(request, targetPath, client, responseCode); //For errors
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createAfterRequestHandler(
        HttpRequest* request,
		Buffer* file,
		Buffer* client
    ){
    if (file->whoAmI() == 3) {
        return std::make_unique<CgiAfterRequestHandler>(request, file, client);
    }
    else if (request->method == GET) {
        return std::make_unique<GetAfterRequestHandler>(request, file, client);
    } else {
        return std::make_unique<PostAfterRequestHandler>(request, file, client);
    }
}