/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostAfterRequestHandler.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 06:58:01 by sangyhan          #+#    #+#             */
/*   Updated: 2025/02/13 06:59:41 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_AFTER_REQUEST_HANDLER_HPP
#define POST_AFTER_REQUEST_HANDLER_HPP

#include <sys/event.h>
#include "../IRequestHandler.hpp"
#include "../../server/ServerConfig.hpp"
#include "../../server/RequestListManager.hpp"
#include "../../parser/CgiParser.hpp" //For parsing the CGI Response.
#include "../../parser/RequestParser.hpp"
#include "../../buffer/Pipe.hpp"
#include "../../buffer/File.hpp"
#include "../HttpResponse.hpp"


class Buffer;

class PostAfterRequestHandler : public IRequestHandler {
public:
    PostAfterRequestHandler(HttpRequest* request, Buffer* file, Buffer* client);
    ~PostAfterRequestHandler() override;
    RequestResult handleRequest() override;
    std::vector<char> generateErrorResponse(int errorCode) override;

private:
    HttpRequest* request;
    Buffer*      file;
    Buffer*      client;
};

#endif