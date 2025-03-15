/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetAfterRequestHandler.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 06:57:52 by sangyhan          #+#    #+#             */
/*   Updated: 2025/02/13 07:00:14 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_AFTER_REQUEST_HANDLER_HPP
#define GET_AFTER_REQUEST_HANDLER_HPP

#include "../IRequestHandler.hpp"
#include "../../server/ServerConfig.hpp"
#include "../../server/RequestListManager.hpp"
#include "../../parser/CgiParser.hpp" //For parsing the CGI Response.
#include "../../parser/RequestParser.hpp"
#include "../../buffer/Pipe.hpp"
#include "../../buffer/File.hpp"
#include "../HttpResponse.hpp"
#include <sys/event.h>

class Buffer;

class GetAfterRequestHandler : public IRequestHandler {
public:
    GetAfterRequestHandler(HttpRequest* request, Buffer* file, Buffer* client);
    ~GetAfterRequestHandler() override;
    RequestResult handleRequest() override;
    std::vector<char> generateErrorResponse(int errorCode) override;

private:
    HttpRequest* request;
    Buffer*      file;
    Buffer*      client;
};

#endif