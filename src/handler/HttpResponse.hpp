/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 20:11:42 by sangyhan          #+#    #+#             */
/*   Updated: 2025/02/12 19:14:04 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include <vector>
#include <sstream>
#include "../buffer/Library.hpp"

class HttpResponse {
public:
    static std::string generateHeader(HttpRequest& request, int code, const std::string& message, const std::vector<char>& body, const std::string& contentType);
    static std::vector<char> generateResponseWithNoBody(HttpRequest& request, int code);
    static std::string generateDefaultErrorPage(int code, const std::string& message);
};

#endif