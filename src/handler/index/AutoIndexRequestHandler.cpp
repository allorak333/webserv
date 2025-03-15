/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndexRequestHandler.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 20:10:18 by sangyhan          #+#    #+#             */
/*   Updated: 2025/02/12 23:44:20 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "AutoIndexRequestHandler.hpp"

AutoIndexRequestHandler::AutoIndexRequestHandler(HttpRequest& request, const ServerConfig& config, std::string& targetPath, Buffer* client)
: request(request), targetPath(targetPath), client(client) {
    (void)config;
}

AutoIndexRequestHandler::~AutoIndexRequestHandler() {}

RequestResult AutoIndexRequestHandler::handleRequest() {
    DIR* dp;
	RequestResult result;
    struct dirent* entry;
    std::string body;
    int code = 200;

    dp = opendir(targetPath.c_str());
    if (dp != NULL) {
		body = "<html>\r\n<head><title>Index of /</title></head>\r\n<body>\r\n<h1>Index of /</h1><hr><pre>";
		while ((entry = readdir(dp)) != 0)
		{
			struct stat stbuf;
			std::string fullpath;
			fullpath += targetPath;
			fullpath += entry->d_name;
			if (stat(fullpath.c_str(), &stbuf) == -1)
			{
				closedir(dp);
				result.code = 500;
				return result;
			}
			bool is_dir = S_ISDIR(stbuf.st_mode);
			std::string filename = entry->d_name;
			if (is_dir == true)
			{
				filename += "/";
			}
			body += "<a href=\"";
			body += filename;
			body += "\">";
			body += filename;
			body += "</a>";
			char time[100];
			strftime(time, 100, "%Y-%m-%d %H:%M:%S", localtime(&stbuf.st_mtime));
			body += "                                        ";
			body += time;
			body += "                   -\r\n";
		}
		body += "</pre><hr></body>\r\n</html>";
        closedir(dp);
		std::string message = "OK";
		std::vector<char> tempBody;
		tempBody.insert(tempBody.end(), body.begin(), body.end());
		std::string contentType = "text/html";
		std::string header = HttpResponse::generateHeader(request, code, message, tempBody, contentType); // Pass request

        std::vector<char> response;
        response.reserve(header.size() + tempBody.size());
		response.insert(response.end(), header.begin(), header.end());
		response.insert(response.end(), tempBody.begin(), tempBody.end());

		client->getWriteBuffer().reserve(response.size());
		client->getWriteBuffer().insert(client->getWriteBuffer().end(), response.begin(), response.end());
        pushEvent(client->getFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, result.changeList);
    } else {
		result.code = 500;
        return result; // Internal Server Error - Couldn't open directory
    }
	result.code = 0;
    return result;
}

std::vector<char> AutoIndexRequestHandler::generateErrorResponse(int errorCode) {
    DefaultRequestHandler defaultHandler(request, targetPath, client);
    return defaultHandler.generateErrorResponse(errorCode);
}