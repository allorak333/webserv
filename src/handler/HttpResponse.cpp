/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 20:12:08 by sangyhan          #+#    #+#             */
/*   Updated: 2025/02/11 20:12:09 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

std::string HttpResponse::generateHeader(HttpRequest& request, int code, const std::string& message, const std::vector<char>& body, const std::string& contentType) {
    std::ostringstream oss;

    if (request.redirection.first != 0) {
        code = request.redirection.first;
        request.url = request.redirection.second;
    }
    oss << request.httpVersion << " " << code << " " << message << "\r\n";
    oss << "Content-Type: " << contentType << "\r\n";
    oss << "Content-Length: " << body.size() << "\r\n";
    oss << "Connection: keep-alive\r\n";

    if (code / 100 == 3) {
        oss << "Location: " << request.url << "\r\n";
    }
    oss << "\r\n";
    return oss.str();
}

std::vector<char> HttpResponse::generateResponseWithNoBody(HttpRequest& request, int code)
{
	std::ostringstream response;
    std::vector<char> body;
    std::string header;
    std::string message;
    std::vector<char> res;

    std::string contentType = getContentType(request.url);
    if (code / 100 == 3) {
        if (code == 301) {
            message = "Moved Permanently";
        } else if (code == 303) {
            message = "See Other";
        }
    } else if (code / 100 == 4) {
        if (code == 400) {
            message = ERR400;
        } else if (code == 403) {
            message = ERR403;
        } else if (code == 404) {
            message = ERR404;
        } else if (code == 405) {
            message = ERR405;
        } else if (code == 408) {
            message = ERR408;
        } else if (code == 411) {
            message = ERR411;
        } else if (code == 413) {
            message = ERR413;
        } else if (code == 415) {
            message = ERR415;
        } else if (code == 500) {
            message = ERR500;
        } else if (code == 501) {
            message = ERR501;
        }
        std::string temp;
        if (request.errorPages.size() && request.errorPages.find(code) != request.errorPages.end()) {
            temp = request.errorPages[code];
        } else {
            temp = HttpResponse::generateDefaultErrorPage(code, message);
        }
        body.reserve(body.size() + temp.size());
        body.insert(body.end(), temp.begin(), temp.end());
        contentType = "text/html"; //HTML for error pages.
    }
    header = HttpResponse::generateHeader(request, code, message, body, contentType);
    res.reserve(header.size() + body.size());
    res.insert(res.end(), header.begin(), header.end());
    res.insert(res.end(), body.begin(), body.end());
    return res;
}

std::string HttpResponse::generateDefaultErrorPage(int code, const std::string& message) {
    std::ostringstream oss;

    oss << "<!DOCTYPE html>\n";
    oss << "<html lang=\"en\">\n";
    oss << "<head>\n";
    oss << "<meta charset=\"UTF-8\">\n";
    oss << "<title>Error " << code << "</title>\n";
    oss << "<style>\n";
    oss << "body { font-family: Arial, sans-serif; background-color: #f8f9fa; color: #343a40; text-align: center; margin-top: 50px; }\n";
    oss << "h1 { font-size: 3em; color: #d9534f; }\n";
    oss << "p { font-size: 1.2em; color: #6c757d; }\n";
    oss << "a { color: #007bff; text-decoration: none; }\n";
    oss << "a:hover { text-decoration: underline; }\n";
    oss << ".container { max-width: 600px; margin: 0 auto; }\n";
    oss << "</style>\n";
    oss << "</head>\n";
    oss << "<body>\n";
    oss << "<div class=\"container\">\n";
    oss << "<h1>Error " << code << "</h1>\n";
    oss << "<p>" << message << "</p>\n";
    oss << "</div>\n";
    oss << "</body>\n";
    oss << "</html>";

    return oss.str();
}