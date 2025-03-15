/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiAfterRequestHandler.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 20:09:41 by sangyhan          #+#    #+#             */
/*   Updated: 2025/03/13 21:25:50 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "CgiAfterRequestHandler.hpp"


CgiAfterRequestHandler::CgiAfterRequestHandler(HttpRequest* request, Buffer* file, Buffer* client)
    : request(request), file(file), client(client) {}

CgiAfterRequestHandler::~CgiAfterRequestHandler() {}

RequestResult CgiAfterRequestHandler::handleRequest() {
    // Process CGI output and generate HTTP response
    RequestResult result;

    Pipe* pipe = static_cast<Pipe*>(file);
    int code = 0;
    std::string message;
    std::string header;

    if (pipe->getError()) {
        file->getReadBuffer().clear();
        code = 500;
        std::cout << "cgi error!" << std::endl;
    }

    RequestParser parser;
    std::string target = "\r\n\r\n";
    size_t endHeader = parser.kmp(file->getReadBuffer(), target, 0);

    if (code == 500 || endHeader == RequestParser::npos)
    {
		code = 500;
		message = "Internal Server Error";
		std::string errorPage;
		if (request->errorPages.find(code) != request->errorPages.end()) {
			errorPage = request->errorPages[code];
		}
		else {
			errorPage = HttpResponse::generateDefaultErrorPage(code, message);
		}
		std::vector<char> body(errorPage.begin(), errorPage.end());
		std::string contentType = "text/html";
		header = HttpResponse::generateHeader(*request, code, message, body, contentType);
		client->getWriteBuffer().reserve(header.size() + body.size());
		client->getWriteBuffer().insert(client->getWriteBuffer().end(), header.begin(), header.end());
		client->getWriteBuffer().insert(client->getWriteBuffer().end(), body.begin(), body.end());
    }
    else
    {
        CgiParser cgiParser(file->getReadBuffer(), endHeader);

        try
        {
            HttpRequest cgiResponse = cgiParser.parse(code, message);
            for (std::map<std::string, std::string>::iterator tempIt = cgiResponse.headers.begin(); tempIt != cgiResponse.headers.end(); tempIt++)
            {
                request->headers[tempIt->first] = tempIt->second;
            }
            if (code / 100 == 4) { // 4xx error.
				std::string errorPage;
                if (request->errorPages.find(code) != request->errorPages.end()) {
                    errorPage = request->errorPages[code];
                }
                else {
                    errorPage = HttpResponse::generateDefaultErrorPage(code, message);
                }
				file->getReadBuffer().reserve(file->getReadBuffer().size() + errorPage.size());
				file->getReadBuffer().insert(file->getReadBuffer().end(), errorPage.begin(), errorPage.end());
            }
            std::ostringstream oss;
            oss << request->httpVersion << " " << code << " " << message << "\r\n";

			//Add content-type
            if (request->headers.find("Content-Type") != request->headers.end())
                oss << "Content-Type: " << request->headers["Content-Type"] << "\r\n";
            else
                oss << "Content-Type: " << request->contentType << "\r\n";

            oss << "Content-Length: " << file->getReadBuffer().size() - (endHeader + 4) << "\r\n"; // Length without header
			oss << "Connection: keep-alive\r\n";
            //Add Location
            if (code / 100 == 3)
            {
                 oss << "Location: " << request->url << "\r\n";
            }
            oss << "\r\n";
            header = oss.str();
            client->getWriteBuffer().reserve(header.size() + file->getReadBuffer().size() - endHeader - 4); // Reserve space
            client->getWriteBuffer().insert(client->getWriteBuffer().end(), header.begin(), header.end()); // Add header
            client->getWriteBuffer().insert(client->getWriteBuffer().end(), file->getReadBuffer().begin() + endHeader + 4, file->getReadBuffer().end());// Add body.
        }
        catch(const std::runtime_error& e)
        {
            message = "Internal Server Error";
            code = 500;
            std::cout << "cgi parse error!" << std::endl;
            std::string errorPage;
            if (request->errorPages.find(code) != request->errorPages.end()) {
                errorPage = request->errorPages[code];
            }
            else {
                errorPage = HttpResponse::generateDefaultErrorPage(code, message);
            }
            std::vector<char> body(errorPage.begin(), errorPage.end());
            std::string contentType = getContentType(request->url); // Use original request URL for content type.
            header = HttpResponse::generateHeader(*request, code, message, body, contentType); // Pass request
            client->getWriteBuffer().reserve(header.size() + body.size());
            client->getWriteBuffer().insert(client->getWriteBuffer().end(), header.begin(), header.end());
            client->getWriteBuffer().insert(client->getWriteBuffer().end(), body.begin(), body.end());
        }
    }

    pushEvent(client->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, result.changeList); // Add write event for client
    result.code = 0;
    return result; // Return 0 to indicate processing is complete
}

std::vector<char> CgiAfterRequestHandler::generateErrorResponse(int errorCode)
{
    //You should not use this function
    (void)errorCode;
    return std::vector<char>();
}