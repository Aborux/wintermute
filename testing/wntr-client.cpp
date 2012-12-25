#include <zmq.hpp>
#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>

#include "simplemessage.pb.h"

using namespace std;

int main ()
{
    srand(time(NULL));

    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    cout << "Connecting to the Wintermute server..." << endl;
    socket.connect ("tcp://localhost:8888");

    //  Do 10 requests, waiting each time for a response
    for (int request_nbr = 0; request_nbr != 10; ++request_nbr) {
        SimpleMessage sm;
        sm.set_id(rand());
        sm.set_content("Take over the world!");
        sm.set_importance((rand() % 2 == 0 ? SimpleMessage::CRITICAL : SimpleMessage::LOW));

        string msg_data;
        sm.SerializeToString(&msg_data);

        zmq::message_t request(msg_data.size());
        memcpy ((void *) request.data(), msg_data.c_str(), msg_data.size());
        cout << "Sending message " << request_nbr << " with size " << msg_data.size() << "..." << endl;
        socket.send (request);

        //  Get the reply.
        zmq::message_t reply;
        socket.recv (&reply);
        cout << "Received something... " << request_nbr << endl;
    }
    return 0;
}
