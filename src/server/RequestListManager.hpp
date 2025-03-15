/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestListManager.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sangyhan <sangyhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 20:10:32 by sangyhan          #+#    #+#             */
/*   Updated: 2025/02/12 19:16:21 by sangyhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef REQUESTLISTMANAGER_HPP
#define REQUESTLISTMANAGER_HPP

#include <map>
#include "../buffer/Buffer.hpp"

class Buffer;

class RequestListManager {
public:
    RequestListManager();
    ~RequestListManager();

    void addRequest(Buffer* file, Buffer* client, HttpRequest* request);
    void removeRequest(Buffer* file);
    std::pair<Buffer*, HttpRequest*> getRequest(Buffer* file);
	std::map<Buffer*, std::pair<Buffer*, HttpRequest*> >& getRequestList();

private:
    std::map<Buffer*, std::pair<Buffer*, HttpRequest*> > requestList;
};

#endif