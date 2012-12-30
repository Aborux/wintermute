/***
 *  This file is part of the Wintermute project.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  Wintermute is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  Wintermute is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with Wintermute.
 *  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @author Jacky Alciné <jackyalcine@gmail.com>
 * @date 05/02/12 1:23:32 PM
 */
#include <zmq.hpp>
#include <iostream>
#include <string>

#include <unistd.h>

#include "global.hpp"
#include "core.hpp"

// #include "simplemessage.pb.h"

using namespace std;
using namespace Wintermute;

Core::Core (int argc, char **argv)
    : _context(1), _reply_socket(_context, ZMQ_REP)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // DEBUG Section
    _agents_names[1] = "Core";
    _agents_endpoints[1] = "tcp://*:8888";
    ////////

    cout << "[~~~~~~~~ Wintermute Core Launching ~~~~~~~~]\n\n";

    // Set sockets
    _reply_socket.bind("tcp://*:8888");

    // Read configuration
    // Configuration is read from file pointed by -c option. If no
    // configuration file is provided then last saved config is used.
    // Last saved config is read and saved using Protobuf.
    // For now, textual configuration file overrides all settings if it is
    // provided. In future we might implement merging.

    // Load modules
    // Modules specified by config files will be loaded.
    // vector<ModuleInfo> modules_to_load = config.getModules();
    // for (auto it = modules_to_load.begin(); it != modules_to_load.end(); ++it) {
    //  if (!this->loadModule(*it))
    //      throw runtime_exception;
    // }

    // Load agent's meta-data
    // Agent's info is stored in the binary configuration storage (Protobuf)
    // AgentInfoStorage agent_info;
    // agent_info.ParseFromFileDescriptor("bazz");
    // 

    // Start agents
    // for agent_i in agent_info:
    //      this->startAgent(agent_i);
}

int Core::exec()
{
    cout << "Entering request loop..." << endl;
    while (true) {
        // zmq::message_t request;

        // // Receive request
        // socket.recv(&request);
        // string req_str((char *) request.data());

        // SimpleMessage simpleMessage;
        // simpleMessage.ParseFromString(req_str);

        // cout << "Received request\n"
        //      << "ID: " << simpleMessage.id() << endl
        //      << "Content: " << simpleMessage.content() << endl
        //      << "Importance: " << (simpleMessage.importance() == SimpleMessage::CRITICAL ? "Critical!\n\n" : "Non-critical...\n\n");

        // sleep(1);

        // zmq::message_t reply(17);
        // memcpy((void *) reply.data(), "Request received.", 17);
        // socket.send(reply);

        zmq::message_t request_msg;
        _reply_socket.recv(&request_msg);
        string request_str(static_cast<char *>(request_msg.data()), request_msg.size());

        CoreRequest request;
        request.ParseFromString(request_str);

        reply(request);
    }

    return 0;
}

void Core::send(AgentInfo& response)
{
    string response_str;
    response.SerializeToString(&response_str);

    zmq::message_t reply(response_str.size());
    memcpy(reply.data(), response_str.data(), response_str.size());
    _reply_socket.send(reply);
}

void Core::reply(CoreRequest& request)
{
    AgentInfo sender_info = request.sender_info();
    int agent_id = request.id();
    AgentInfo response;

    switch (request.type()) {
        case CoreRequest::GET_ENDPOINT:
            if (agent_id != 0) {
                response.set_endpoint(_agents_endpoints[agent_id]);
                send(response);
            } else {
                // _logger.warning("Wrong request passed by " sender_info.name())
                cout << "Wrong request passed by " << sender_info.name() << endl;

                // Send empty response; I'll think about some kind of "exception" message
                send(response);
            }
            break;

        case CoreRequest::GET_NAME:
            if (agent_id != 0) {
                response.set_name(_agents_names[agent_id]);
                send(response);
            } else {
                // _logger.warning("Wrong request passed by " sender_info.name())
                cout << "Wrong request passed by " << sender_info.name() << endl;

                // Send empty response; I'll think about some kind of "exception" message
                send(response);
            }
            break;

        default:
                // _logger.warning("Wrong request passed by " sender_info.name())
            cout << "Wrong request passed by " << sender_info.name() << endl;
    }
            //////// To be continued!
}
