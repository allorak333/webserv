/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequestHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 20:03:55 by sangyhan          #+#    #+#             */
/*   Updated: 2025/03/13 21:50:47 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "CgiRequestHandler.hpp"
#include <cstring> //For strcpy

CgiRequestHandler::CgiRequestHandler(HttpRequest& request, const ServerConfig& config, std::string& targetPath, Buffer* client)
: request(request), config(config), targetPath(targetPath), client(client) {}

CgiRequestHandler::~CgiRequestHandler() {}

RequestResult CgiRequestHandler::handleRequest() {
    RequestResult result;

    if (access(targetPath.c_str(), F_OK | X_OK) == 0) {
        int inputPipe[2];
        int outputPipe[2];

        if (pipe(inputPipe) == -1 || pipe(outputPipe) == -1) {
            result.code = 500;
            return result; // Internal Server Error
        }

        Pipe* pipeResource = new Pipe(inputPipe[1], outputPipe[0]);
        pid_t pid = handleCGI(inputPipe, outputPipe, targetPath, request);
		pipeResource->setPid(pid);
        close(inputPipe[0]);  // Close unused read end in parent
        close(outputPipe[1]); // Close unused write end in parent
        setNonblock(inputPipe[1]);
        setNonblock(outputPipe[0]);

        //Register with kqueue.
        struct kevent procEvent;
        EV_SET(&procEvent, pid, EVFILT_PROC, EV_ADD | EV_ENABLE, NOTE_EXIT, 0, (void *)pipeResource);
        result.changeList.push_back(procEvent);

        static_cast<Message*>(client)->addResource(pipeResource);  // Add to client resources.
        pushEvent(inputPipe[1], EVFILT_WRITE, EV_ADD | EV_ENABLE, result.changeList);
        pushEvent(outputPipe[0], EVFILT_READ, EV_ADD | EV_ENABLE, result.changeList);

        pipeResource->getReadBuffer().reserve(request.bodyEnd - request.bodyStart);
        pipeResource->getWriteBuffer().insert(pipeResource->getWriteBuffer().end(), client->getReadBuffer().begin() + request.bodyStart, client->getReadBuffer().begin() + request.bodyEnd);
		pipeResource->setServerFd(request.serverFd); // Set Server fd to the pipe
        
        result.resBuffer.push_back(static_cast<Buffer*>(pipeResource));
        result.code = 200;
        return result; // Or a specific CGI status code.
    } else {

         if (access(targetPath.c_str(), F_OK) != 0) {
            result.code = 404;
            return result;
         } else {
            result.code = 403;
            return result;
         }
    }
}

char** CgiRequestHandler::makeEnvp(std::string & target, HttpRequest &request) {
     std::list<char *> envpList;
     std::string envp;
     char *tmp;

     envp = "SERVER_PROTOCOL=";
     envp += request.httpVersion;
     tmp = new char[envp.length() + 1];
     std::strcpy(tmp, envp.c_str());
     envpList.push_back(tmp);

     envp = "GATEWAY_INTERFACE=CGI/1.1";
     tmp = new char[envp.length() + 1];
     std::strcpy(tmp, envp.c_str());
     envpList.push_back(tmp);

     envp = "QUERY_STRING=";
     envp += request.query;
     tmp = new char[envp.length() + 1];
     std::strcpy(tmp, envp.c_str());
     envpList.push_back(tmp);

     envp = "REQUEST_METHOD=";
     if (request.method == GET)
         envp += "GET";
     else if (request.method == POST)
         envp += "POST";
     tmp = new char[envp.length() + 1];
     std::strcpy(tmp, envp.c_str());
     envpList.push_back(tmp);

     envp = "SERVER_PORT=";
     envp += ft_toString(config.getListen());
     tmp = new char[envp.length() + 1];
     std::strcpy(tmp, envp.c_str());
     envpList.push_back(tmp);

     envp = "CONTENT_LENGTH=";
     envp += request.contentLength;
     tmp = new char[envp.length() + 1];
     std::strcpy(tmp, envp.c_str());
     envpList.push_back(tmp);

     envp = "CONTENT_TYPE=";
     envp += request.contentType;
     tmp = new char[envp.length() + 1];
     std::strcpy(tmp, envp.c_str());
     envpList.push_back(tmp);

     envp = "SERVER_NAME=";
     envp += request.host;
     tmp = new char[envp.length() + 1];
     std::strcpy(tmp, envp.c_str());
     envpList.push_back(tmp);

     envp = "PATH_INFO=";
     envp += target;
     tmp = new char[envp.length() + 1];
     std::strcpy(tmp, envp.c_str());
     envpList.push_back(tmp);

     for (std::map<std::string, std::string>::iterator it = request.headers.begin(); it != request.headers.end(); ++it) {
         std::string key = (*it).first;
         if (key != "Host" && key != "User-Agent" && key != "Accept"
          && key != "Content-Length" && key != "Content-Type") {
             for (size_t i=0; i < key.length(); i++) {
                 if (key[i] == '-') {
                     key[i] = '_';
                 } else if ('a' <= key[i] && key[i] <= 'z') {
                     key[i] -= 32;
                 }
             }
             envp = "HTTP_" + key + "=" + (*it).second;
             tmp = new char[envp.length() + 1];
             std::strcpy(tmp, envp.c_str());
             envpList.push_back(tmp);
         }
     }
     char **envpArgs = new char *[envpList.size() + 1];
     int i = 0;
     for (std::list<char *>::iterator it = envpList.begin(); it != envpList.end(); ++it) {
         envpArgs[i++] = *it;
     }
     envpArgs[i] = NULL;
     return envpArgs;
}

pid_t CgiRequestHandler::handleCGI(int inputFileFd[2], int outputFileFd[2], std::string & target, HttpRequest &request) {
    pid_t pid = fork();

    if (pid == -1) {
        throw RuntimeException("fork");
    }
    if (pid == 0) {
        if (dup2(inputFileFd[0], STDIN_FILENO) == -1) {
            throw RuntimeException("dup2");
        }
        if (dup2(outputFileFd[1], STDOUT_FILENO) == -1) {
            throw RuntimeException("dup2");
        }
        close(inputFileFd[1]);
        close(inputFileFd[0]);
        close(outputFileFd[1]);
        close(outputFileFd[0]);
        char *execArgs[2];
        execArgs[0] = const_cast<char *>(target.c_str());
        execArgs[1] = NULL;
        char** envp = makeEnvp(target, request);
        execve(target.c_str(), execArgs, envp);
        arrFree(envp);
        throw RuntimeException("execve");
    } else {
        return (pid);
    }
}

std::vector<char> CgiRequestHandler::generateErrorResponse(int errorCode) {
    DefaultRequestHandler defaultHandler(request, targetPath, client);
    return defaultHandler.generateErrorResponse(errorCode);
}

void CgiRequestHandler::arrFree(char **arr)
{
	if (arr)
	{
		for (int i = 0; arr[i] != NULL; i++)
		{
			delete[] arr[i];
			arr[i] = NULL;
		}
		delete[] arr;
		arr = NULL;
	}
}