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

#include "simplemessage.pb.h"

using namespace std;
using namespace Wintermute;

Core::Core (int argc, char **argv)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    cout << "[~~~~~~~~ Wintermute Core Launching ~~~~~~~~]\n\n";

    // Read configuration

    // Load modules

    // Load agent's meta-data

    // Start agents    
}

int Core::exec()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:8888");

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

        sleep(1);

        // zmq::message_t reply(17);
        // memcpy((void *) reply.data(), "Request received.", 17);
        // socket.send(reply);

    }

    return 0;
}
