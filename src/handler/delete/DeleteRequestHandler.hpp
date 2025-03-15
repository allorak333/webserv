/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteRequestHandler.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:56:25 by sangyhan          #+#    #+#             */
/*   Updated: 2025/03/13 21:55:44 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef DELETEREQUESTHANDLER_HPP
#define DELETEREQUESTHANDLER_HPP

#include "../IRequestHandler.hpp"
#include "../../server/ServerConfig.hpp"
#include "../../server/RequestListManager.hpp"
#include "../common/DefaultReqeustHandler.hpp"
#include "../HttpResponse.hpp"

class Buffer;
class DeleteRequestHandler : public IRequestHandler {
public:
    DeleteRequestHandler(HttpRequest& request, const ServerConfig& config, std::string& targetPath, Buffer* client);
    ~DeleteRequestHandler() override;

    RequestResult handleRequest() override;
    std::vector<char> generateErrorResponse(int errorCode) override;

private:
    HttpRequest& request;
    std::string targetPath;
    Buffer* client;
};

#endif