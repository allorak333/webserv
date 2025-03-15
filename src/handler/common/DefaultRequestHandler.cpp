/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultRequestHandler.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:52:38 by sangyhan          #+#    #+#             */
/*   Updated: 2025/03/13 21:56:47 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "DefaultReqeustHandler.hpp"

DefaultRequestHandler::DefaultRequestHandler(HttpRequest& request, std::string& targetPath, Buffer* client, int code)
    : request(&request), targetPath(targetPath), statusCode(code),  file(NULL) {
    (void)client;
    (void)file;
}

DefaultRequestHandler::DefaultRequestHandler(HttpRequest* request, Buffer* file, Buffer* client)
    : request(request), statusCode(200), file(file){
    (void)client;
    (void)file;
}

DefaultRequestHandler::~DefaultRequestHandler() {}

RequestResult DefaultRequestHandler::handleRequest()
{
	RequestResult result;
    
	result.code = statusCode;
    return result;
}
std::vector<char> DefaultRequestHandler::generateErrorResponse(int errorCode) {

    std::string message = returnErrorMsg(errorCode);
    std::string body;
    if (request->errorPages.find(errorCode) != request->errorPages.end()) {
        body = request->errorPages[errorCode];
     } else {
        body = HttpResponse::generateDefaultErrorPage(errorCode, message);
    }
    std::string contentType = "text/html"; // Default for error pages
	std::vector<char> vec(body.length());
  	std::copy(body.begin(), body.end(), vec.begin());
    std::string header = HttpResponse::generateHeader(*request, errorCode, message, vec, contentType);

    std::vector<char> response;
    response.reserve(header.size() + body.size());
    response.insert(response.end(), header.begin(), header.end());
    response.insert(response.end(), body.begin(), body.end());
    return response;
}

bool DefaultRequestHandler::findMatchingLocation(std::string& requestURL, const ServerConfig& config, Location& location)
{
	std::vector<Location> locationList = config.getLocationList();
	std::vector<std::string> splitUrl = ft_split(requestURL, '/');
	int urlNum = splitUrl.size();
	std::vector<std::string> splitPath;
	bool ans = false;
	int deepest = -1;

	for (std::vector<Location>::const_iterator it = locationList.begin(); it != locationList.end(); ++it)
	{
		std::string locPath = it->getPath();
		splitPath = ft_split(locPath, '/');
		int pathNum = splitPath.size();

		if (pathNum == 0 && urlNum == 0)
		{
			location = *it;
			return true;
		}
		if (pathNum > urlNum)
			continue;

		int i = 0;
		while (i < pathNum)
		{
			if (splitUrl[i] != splitPath[i])
				break;
			if (i + 1 == pathNum)
			{
				if (i > deepest)
				{
					deepest = i;
					location = *it;
					ans = true;
				}
			}
			i++;
		}
	}
    std::vector<Location>::const_iterator it = locationList.begin();
    for (; it != locationList.end(); ++it) {
        std::string locPath = it->getPath();
        if (ans == false && urlNum == 1 && ft_split(locPath, '/').size() == 0) {
            location = *it;
            return true;
        }
    }
	return ans;
}

bool DefaultRequestHandler::findMatchingExtension(std::string& target, const ServerConfig& config, Location& myExtension)
{
    std::vector<Location> extensionList = config.getExtensionList();
	for (std::vector<Location>::const_iterator it = extensionList.begin(); it != extensionList.end(); ++it)
	{
		std::string extension = it->getPath();
		if (target.length() >= extension.length())
		{
			if (target.compare(target.length() - extension.length(), extension.length(), extension) == 0)
			{
                myExtension = *it;
				return true;
			}
		}
	}
	return false;
}

int DefaultRequestHandler::validateRequest(HttpRequest& request, ServerConfig& config, std::string& target)
{
	if (request.method == NONE || request.url == "" || request.httpVersion == "")
		return 400; // Bad Request

	Location myExtension;
    if (DefaultRequestHandler::findMatchingExtension(request.url, config, myExtension))
    {
        target = myExtension.getRoot();
        target += '/';
        target += myExtension.getIndex();
        request.url = target;
        return 200;
    }
	Location myLocation;
	bool isLocation = DefaultRequestHandler::findMatchingLocation(request.url, config, myLocation);
	if (!isLocation)
	{
		// Use server-level configurations
		if (config.getErrorPages().size())
			request.errorPages = config.getErrorPages();
        if (config.getRedirection().first != 0)
            request.redirection = config.getRedirection();
		if (config.getClientMaxBodySize() < static_cast<unsigned long>(request.bodyEnd - request.bodyStart))
			return 413; // Payload Too Large
		if (config.getRoot() != "") {
            target += config.getRoot();
        }
        else {
            return 404; // Not Found
        }
		target += request.url;

		int isDir = isDirectory(target);
		if (isDir == 3)
		{
			if (request.url[request.url.size() - 1] == '/')
			{
				if (config.getAutoindex())
					return 800; // Autoindex
				else if (config.getIndex() != "")
					target += config.getIndex();
				else
					return 404; // Not Found
			}
            else
            {
                return 404;
            }
		}
        else if (isDir == 1)
            return 404;
		if (request.method != NONE)
		{
			std::set<METHOD> allowedMethods = config.getAllowedMethods();
			if (allowedMethods.find(request.method) == allowedMethods.end())
				return 405; // Method Not Allowed
		}
	}
	else
	{
		// Use location-specific configurations
        if (myLocation.getErrorPages().size()) {
            request.errorPages = myLocation.getErrorPages();
        } else if (config.getErrorPages().size()) {
            request.errorPages = config.getErrorPages();
        }
        if (myLocation.getRedirection().first != 0)
            request.redirection = myLocation.getRedirection();
        else if (config.getRedirection().first != 0)
            request.redirection = config.getRedirection();

		if (myLocation.getClientMaxBodySize() != INT_MAX && myLocation.getClientMaxBodySize() < static_cast<unsigned long>(request.bodyEnd - request.bodyStart))
			return 413;  // Payload Too Large
        else if (config.getClientMaxBodySize() < static_cast<unsigned long>(request.bodyEnd - request.bodyStart))
            return 413;

		if (myLocation.getRoot() != "")
			target += myLocation.getRoot();
		else if (config.getRoot() != "")
			target += config.getRoot();
        else
            return 404;

		target += '/';
		target += request.url.substr(myLocation.getPath().length(), request.url.length());
		int isDir = isDirectory(target);

		if (isDir == 3)
		{
			if (request.url[request.url.size() - 1] == '/')
			{
                if (myLocation.getAutoindex() || config.getAutoindex())
					return 800; // Autoindex
				if (myLocation.getIndex() != "")
					target += myLocation.getIndex();
				else if (config.getIndex() != "")
					target += config.getIndex();
				else
					return 404; // Not Found
			}
			else if (request.method == GET)
            {
                request.url += '/';
				return 301; // Moved Permanently
			}
            else if (request.method == POST)
            {
                request.url = "/";
                return 201; // 201 Created
            }
		}
        else if (isDir == 1)
        {
            return 404; // Not Found
        }
		if (request.method != NONE)
		{
            std::set<METHOD> allowedMethods;
            if (myLocation.getAllowedMethods().size()) {
                allowedMethods = myLocation.getAllowedMethods();
            } else if (config.getAllowedMethods().size()) {
                allowedMethods = config.getAllowedMethods();
            }
            if (allowedMethods.find(request.method) == allowedMethods.end()) {
                return 405;
            }
		}
	}
	request.url = target;
	return 200; // OK
}