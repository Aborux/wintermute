#include <zmq.hpp>
#include <string>
#include <iostream>

#include "simplemessage.pb.h"

using namespace std;

int main ()
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    std::cout << "Connecting to the Wintermute server..." << std::endl;
    socket.connect ("tcp://localhost:8888");

    //  Do 10 requests, waiting each time for a response
    for (int request_nbr = 0; request_nbr != 10; ++request_nbr) {
        SimpleMessage sm;
        sm.set_id(35465);
        sm.set_content("Take over the world!");
        sm.set_importance(SimpleMessage::CRITICAL);

        string mdata;
        zmq::message_t request(200);
        sm.SerializeToString(&mdata);

        memcpy ((void *) request.data(), mdata.c_str(), mdata.size());
        std::cout << "Sending message " << request_nbr << " with size " << mdata.size() << "..." << std::endl;
        socket.send (request);

        //  Get the reply.
        zmq::message_t reply;
        socket.recv (&reply);
        std::cout << "Received something... " << request_nbr << std::endl;
    }
    return 0;
}
