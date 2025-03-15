/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequestHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:58:54 by sangyhan          #+#    #+#             */
/*   Updated: 2025/02/12 20:54:46 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CGIREQUESTHANDLER_HPP
#define CGIREQUESTHANDLER_HPP

#include <sys/wait.h>  // For waitpid
#include "../IRequestHandler.hpp"
#include "../../buffer/Message.hpp"
#include "../../buffer/Pipe.hpp"
#include "../../server/ServerConfig.hpp"
#include "../../server/RequestListManager.hpp"
#include "../common/DefaultReqeustHandler.hpp"

class Buffer;
class CgiRequestHandler : public IRequestHandler {
public:
    CgiRequestHandler(HttpRequest& request, const ServerConfig& config, std::string& targetPath, Buffer* client);
    ~CgiRequestHandler() override;

    RequestResult handleRequest() override;
    std::vector<char> generateErrorResponse(int errorCode) override;

private:
    HttpRequest& request;
    const ServerConfig& config;
    std::string targetPath;
    Buffer* client;
    std::map<pid_t, clock_t> childTime;
    char** makeEnvp(std::string & target, HttpRequest &request);
    pid_t handleCGI(int inputFileFd[2], int outputFileFd[2], std::string & target, HttpRequest &request);
    void arrFree(char **arr);
};

#endif