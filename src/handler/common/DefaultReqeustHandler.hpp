/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultReqeustHandler.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:51:12 by sangyhan          #+#    #+#             */
/*   Updated: 2025/03/13 21:56:05 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef DEFAULTREQUESTHANDLER_HPP
#define DEFAULTREQUESTHANDLER_HPP
#include "../../buffer/Library.hpp"
#include "../IRequestHandler.hpp"
#include "../../server/ServerConfig.hpp"
#include "../../server/RequestListManager.hpp"
#include "../HttpResponse.hpp"
#include "../../buffer/File.hpp"
#include "../../buffer/Library.hpp"

class Buffer;
class DefaultRequestHandler : public IRequestHandler {
public:
    DefaultRequestHandler(HttpRequest& request, std::string& targetPath, Buffer* client, int code = 200);
    DefaultRequestHandler(HttpRequest* request, Buffer* file, Buffer* client);
    ~DefaultRequestHandler() override;
    RequestResult handleRequest() override;
    std::vector<char> generateErrorResponse(int errorCode) override;

    static int validateRequest(HttpRequest& request, ServerConfig& config, std::string& target);

private:
    HttpRequest* request;
    std::string targetPath;
    int         statusCode;
    Buffer*     file;

    static bool findMatchingLocation(std::string& requestURL, const ServerConfig& config, Location& location);
    static bool findMatchingExtension(std::string& target, const ServerConfig& config, Location& myExtension);

};

#endif