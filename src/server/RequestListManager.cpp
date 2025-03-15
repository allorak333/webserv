/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestListManager.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 20:11:04 by sangyhan          #+#    #+#             */
/*   Updated: 2025/03/15 14:19:34 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "RequestListManager.hpp"

RequestListManager::RequestListManager() {}

RequestListManager::~RequestListManager() {
    for (std::map<Buffer*, std::pair<Buffer*, HttpRequest*> >::iterator it = requestList.begin(); it != requestList.end(); ++it) {
		if (it->second.second->fileCount == 1) //Check if you are the last one using it
        	delete it->second.second; // Delete the HttpRequest*
        else
            it->second.second->fileCount -= 1;
    }
    requestList.clear();
}

void RequestListManager::addRequest(Buffer* file, Buffer* client, HttpRequest* request) {
    requestList[file] = std::make_pair(client, request);
}

void RequestListManager::removeRequest(Buffer* file) {
	std::map<Buffer*, std::pair<Buffer*, HttpRequest*> >::iterator it;

    it = requestList.find(file);
    if (it != requestList.end())
    {
        if (it->second.second->fileCount == 1)
            delete it->second.second;
        else
            it->second.second->fileCount -= 1;
        requestList.erase(it);
    }
}

std::pair<Buffer*, HttpRequest*> RequestListManager::getRequest(Buffer* file) {
    std::map<Buffer*, std::pair<Buffer*, HttpRequest*> >::iterator it = requestList.find(file);
    if (it != requestList.end()) {
        return it->second;
    }
    return std::make_pair(nullptr, nullptr);
}

std::map<Buffer*, std::pair<Buffer*, HttpRequest*> >& RequestListManager::getRequestList(){
	return requestList;
}