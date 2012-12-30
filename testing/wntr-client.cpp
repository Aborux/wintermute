#include <zmq.hpp>
#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>

// #include "simplemessage.pb.h"
#include "coremessages.pb.h"
#include "agentinfo.pb.h"

using namespace std;

int main (int argc, char **argv)
{
    srand(time(NULL));

    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    cout << "Connecting to the Wintermute server..." << endl;
    socket.connect ("tcp://localhost:8888");

    //  Do 10 requests, waiting each time for a response
    for (int request_nbr = 0; request_nbr != 10; ++request_nbr) {
        // SimpleMessage sm;
        // sm.set_id(rand());
        // sm.set_content("Take over the world!");
        // sm.set_importance((rand() % 2 == 0 ? SimpleMessage::CRITICAL : SimpleMessage::LOW));

        // string msg_data;
        // sm.SerializeToString(&msg_data);

        CoreRequest req;
        req.set_type(CoreRequest::GET_ENDPOINT);
        req.set_id(1);

        AgentInfo* sender_info = req.mutable_sender_info();

        sender_info->set_id(111111);
        sender_info->set_name("Wintermute Core Testing Client");

        string req_str;
        req.SerializeToString(&req_str);

        zmq::message_t request(req_str.size());
        memcpy (request.data(), req_str.data(), req_str.size());
        cout << "Sending message " << request_nbr << " with size " << req.ByteSize() << "..." << endl;
        socket.send (request);

        //  Get the reply.
        zmq::message_t reply;
        socket.recv (&reply);
        cout << "Received something... " << request_nbr << endl << endl;
    }
    return 0;
}
