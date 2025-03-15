/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRequestHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:49:17 by sangyhan          #+#    #+#             */
/*   Updated: 2025/03/15 16:32:55 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IREQUESTHANDLER_HPP
#define IREQUESTHANDLER_HPP

#include <vector>
#include <string>
#include <sys/event.h>
#include <memory>
#include "../buffer/Library.hpp"
#include "../server/ServerConfig.hpp"
#include "../buffer/Buffer.hpp"

struct RequestResult{
    int code;
    std::vector<Buffer *> resBuffer;
    std::vector<struct kevent> changeList;
};

class IRequestHandler {
public:
    virtual ~IRequestHandler() {}
    virtual RequestResult handleRequest() = 0;
    virtual std::vector<char> generateErrorResponse(int errorCode) = 0;
};

#endif