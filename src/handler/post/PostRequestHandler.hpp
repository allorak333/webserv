/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostRequestHandler.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:55:25 by sangyhan          #+#    #+#             */
/*   Updated: 2025/03/15 16:32:37 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef POSTREQUESTHANDLER_HPP
#define POSTREQUESTHANDLER_HPP
#include "../IRequestHandler.hpp"
#include "../common/DefaultReqeustHandler.hpp"
#include "../../buffer/File.hpp"
#include "../HttpResponse.hpp"
#include "../../parser/MimeParser.hpp"

class Buffer;

class PostRequestHandler : public IRequestHandler {
public:
    PostRequestHandler(HttpRequest& request, const ServerConfig& config, std::string& targetPath, Buffer* client);
    ~PostRequestHandler() override;

    RequestResult handleRequest() override;
    std::vector<char> generateErrorResponse(int errorCode) override;

private:
     HttpRequest& request;
    const ServerConfig& config;
    std::string targetPath;
    Buffer* client;
};
#endif