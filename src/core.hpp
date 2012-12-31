/*
 * @section lcns Licensing
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
/**
 * @file core.hpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 *
 */

#ifndef WINTERMUTE_CORE_HPP
#define WINTERMUTE_CORE_HPP

#include <map>
#include <string>

#include <global.hpp>
#include <diagnoser.hpp>

#include <zmq.hpp>

#include "coremessages.pb.h"
#include "agentinfo.pb.h"

using namespace std;

WINTER_BEGIN_NAMESPACE

/**
 * @brief The central management class of Wintermute.
 *
 * The Core holds the vital activities of Wintermute's startup. It
 * handles the actions from the outside system environment and, with a little
 * Qt magic and hours of coding, transfers control to the parts of Wintermute
 * that would be appropriate to be under control.
 *
 * @nonreentrant
 * @class Core wintermute.hpp "core.hpp"
 */
class Core
{
public:

    Core (int argc, char **argv);

    int exec();

    void send_response(AgentInfo& response);
    void reply(CoreRequest& request);

private:
    zmq::context_t _context;
    zmq::socket_t _reply_socket;
    // Logger _logger;
    
    /// Endpoints of registered agents accessible by id.
    map<int, string> _agents_endpoints;
    map<int, string> _agents_names;
};

WINTER_END_NAMESPACE

#endif /* WINTERMUTE_CORE_HPP */
