/**
 * @author Wintermute Development <wntr-devel@thesii.org>
 * @file logging.hpp
 *
 * @section lcns Licensing
 * This library is free software; you can redistribute it and/or
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
 *
 */

#ifndef WNTR_LOGGING_HPP
#define WNTR_LOGGING_HPP

#include <QObject>
#include <QDebug>
#include <QTime>

namespace Wintermute {
class Logging;

/// @todo Allow triggering the saving of such logs to disc.
/// @todo Allow dynamic printing of certain flags (overriding build commands).
/// @todo Add a event filter for this into the application.
class Logging : public QObject {
    Q_OBJECT
    Q_CLASSINFO ( "objectName","Logging Singleton" )
    Q_DISABLE_COPY ( Logging )

signals:
    void linePrinted ( const QString& );
    void warningEncountered ( const QString& );
    void criticalErrorEncountered ( const QString& );
    void fatalErrorEncountered ( const QString& );

public:
    static void catchQDebugMessage ( QtMsgType, const char* );
    static Logging* instance();

private:
    Logging();
    static Logging* s_inst;
};
} // namespace

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
