/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiAfterRequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 20:09:04 by sangyhan          #+#    #+#             */
/*   Updated: 2025/02/12 23:42:48 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CGI_AFTER_REQUEST_HANDLER_HPP
#define CGI_AFTER_REQUEST_HANDLER_HPP

#include <sys/event.h>
#include "../IRequestHandler.hpp"
#include "../../server/ServerConfig.hpp"
#include "../../server/RequestListManager.hpp"
#include "../../parser/CgiParser.hpp" //For parsing the CGI Response.
#include "../../parser/RequestParser.hpp"
#include "../../buffer/Pipe.hpp"
#include "../HttpResponse.hpp"


class Buffer;

class CgiAfterRequestHandler : public IRequestHandler {
public:
    CgiAfterRequestHandler(HttpRequest* request, Buffer* file, Buffer* client);
    ~CgiAfterRequestHandler() override;
    RequestResult handleRequest() override;
    std::vector<char> generateErrorResponse(int errorCode) override;

private:
    HttpRequest* request;
    Buffer*      file;
    Buffer*      client;
};

#endif