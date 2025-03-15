/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndexRequestHandler.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 20:09:57 by sangyhan          #+#    #+#             */
/*   Updated: 2025/02/12 19:18:08 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef AUTOINDEXREQUESTHANDLER_HPP
#define AUTOINDEXREQUESTHANDLER_HPP

#include "../IRequestHandler.hpp"
#include "../common/DefaultReqeustHandler.hpp"
#include "../../server/ServerConfig.hpp"
#include "../../server/RequestListManager.hpp"
#include "../HttpResponse.hpp"
#include <dirent.h> // For directory operations

class Buffer;

class AutoIndexRequestHandler : public IRequestHandler {
public:
    AutoIndexRequestHandler(HttpRequest& request, const ServerConfig& config, std::string& targetPath, Buffer* client);
    ~AutoIndexRequestHandler() override;

    RequestResult handleRequest() override;
    std::vector<char> generateErrorResponse(int errorCode) override;

private:
    HttpRequest& request;
    std::string targetPath;
    Buffer* client;
};

#endif