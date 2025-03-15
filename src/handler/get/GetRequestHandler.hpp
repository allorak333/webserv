/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequestHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:53:38 by sangyhan          #+#    #+#             */
/*   Updated: 2025/03/13 21:55:51 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef GETREQUESTHANDLER_HPP
#define GETREQUESTHANDLER_HPP

#include <sys/stat.h> // For stat
#include "../IRequestHandler.hpp"
#include "../../server/ServerConfig.hpp"
#include "../../server/RequestListManager.hpp"
#include "../../buffer/Message.hpp"
#include "../common/DefaultReqeustHandler.hpp"
#include "../../buffer/File.hpp"
#include "../HttpResponse.hpp"

class Buffer;
class GetRequestHandler : public IRequestHandler {
public:
    GetRequestHandler(HttpRequest& request, const ServerConfig& config,  std::string& targetPath, Buffer* client);
    ~GetRequestHandler() override;

    RequestResult handleRequest() override;
    std::vector<char> generateErrorResponse(int errorCode) override;

private:
    HttpRequest& request;
    std::string targetPath;
    Buffer* client;
};

#endif