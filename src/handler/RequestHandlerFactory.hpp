/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerFactory.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:50:04 by sangyhan          #+#    #+#             */
/*   Updated: 2025/02/13 07:17:34 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef REQUEST_HANDLER_FACTORY_HPP
#define REQUEST_HANDLER_FACTORY_HPP

#include <memory> //For smart Pointers
#include "../server/ServerConfig.hpp"
#include "../server/RequestListManager.hpp"
#include "./common/DefaultReqeustHandler.hpp"
#include "./get/GetRequestHandler.hpp"
#include "./post/PostRequestHandler.hpp"
#include "./delete/DeleteRequestHandler.hpp"
#include "./cgi/CgiRequestHandler.hpp"
#include "./index/AutoIndexRequestHandler.hpp"
#include "./cgi/CgiAfterRequestHandler.hpp"
#include "./get/GetAfterRequestHandler.hpp"
#include "./post/PostAfterRequestHandler.hpp"
#include "../buffer/Library.hpp"

class Buffer;

class RequestHandlerFactory {
public:
    static std::unique_ptr<IRequestHandler> createRequestHandler(
        HttpRequest& request,
        ServerConfig& config,
        std::string& targetPath,
        Buffer* client
    );

	static std::unique_ptr<IRequestHandler> createAfterRequestHandler(
        HttpRequest* request,
		Buffer* file,
		Buffer* client
    );
};

#endif