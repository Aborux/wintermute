/**
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 *
 * @legalese
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
 * @endlegalese
 */

#include "config.hpp"
#include "core.hpp"
#include "ipc.hpp"
#include "plugins.hpp"
#include "ncurses.hpp"
#include <iostream>
#include <QtDebug>
#include <QProcess>
#include <QtDebug>
#include <QLibraryInfo>
#include <QVariantMap>
#include <boost/program_options.hpp>
#include <wntrdata.hpp>
#include <wntrntwk.hpp>
#include <wntrling.hpp>

using namespace std;
using namespace Wintermute;
namespace po = boost::program_options;

using boost::program_options::variables_map;
using boost::program_options::variable_value;
using boost::program_options::options_description;
using Wintermute::Linguistics::Parser;
using std::cout;
using std::endl;

namespace Wintermute {
    QApplication* Core::s_app = NULL;
    QVariantMap* Core::s_args = NULL;
    Core* Core::s_core = NULL;

    Core::Core ( int &p_argc, char **p_argv ) {
        Core::s_core = this;
        Core::Configure ( p_argc,p_argv );
    }

    void Core::Configure ( int& p_argc, char** p_argv ) {
        QString l_ipcMod = "master";
        s_app = new QApplication ( p_argc,p_argv );
        s_app->setApplicationName ( "Wintermute" );
        s_app->setApplicationVersion ( QString::number ( WINTERMUTE_VERSION ) );
        s_app->setOrganizationDomain ( "thesii.org" );
        s_app->setOrganizationName ( "Synthetic Intellect Institute" );
        connect ( s_app , SIGNAL ( aboutToQuit() ) , s_core , SLOT ( doDeinit() ) );
        s_core->setParent (s_app);

        configureCommandLine();

        if ( s_args->count ( "ipc" ) != 0 )
            l_ipcMod = s_args->value ( "ipc" ).toString ();

        Core::start ();
    }

    const QVariantMap* Core::arguments () { return s_args; }

    /// @todo Allow arbitrary arguments to be added into the system
    void Core::configureCommandLine () {
        variables_map l_vm;
        s_args = new QVariantMap;
        int l_argc = s_app->argc ();
        char** l_argv = s_app->argv ();
        options_description l_publicOptions ( "General Options" ),
        l_configOptions ( "Configuration" ),
        l_hiddenOptions ( "Hidden Options" ),
        l_allOptions ( "All Options" );

        l_publicOptions.add_options()
        ( "help,h"       , "show help screen" )
        ( "verbose-help" , "show all options" )
        ( "copyright,C"  , "show copyright information" )
        ( "version,V"    , "output version number." )
        ;

        l_configOptions.add_options ()
        ( "locale,l" , po::value<string>()->default_value ( WNTRDATA_DEFAULT_LOCALE ) ,
          "Defines the locale used by the system for parsing." )
        ( "data-dir,datadir,l" , po::value<string>()->default_value ( WNTRDATA_DATA_DIR ) ,
          "Defines the directory where Wintermute's data is stored." )
        ;

        l_hiddenOptions.add_options ()
        ( "plugin,p"  , po::value<string>()->default_value("root") ,
          "Loads a plug-in; used for module 'Plugin'. (default: 'root' [the manager])" )
        ( "ipc,i"     , po::value<string>()->default_value ( "master" ) ,
          "Defines the IPC module to run this process as." )
        ( "daemon"    , po::value<string>()->default_value( "false" ),
           "Determines whether or not this process runs as a daemon.")
        ( "ncurses,n" , po::value<string>()->default_value ( "false" ),
          "Determines whether or not the nCurses interface is being used. This automatically disables the GUI.")
        ( "gui,g"     , po::value<string>()->default_value ( "false" ),
          "Determines whether or not the graphical user interface is loaded. This automatically disables nCurses." )
        ;

        l_allOptions.add ( l_publicOptions ).add ( l_configOptions );
        l_allOptions.add ( l_hiddenOptions );

        try {
            po::store ( po::parse_command_line ( l_argc, l_argv , l_allOptions ), l_vm );
            po::notify ( l_vm );
        } catch ( exception &e ) {
            qDebug() << "Command-line parsing error: " << e.what();
        }

        if ( !l_vm.empty () ) {
            if ( l_vm.count ( "help" ) || l_vm.count ( "verbose-help" ) ) {
                cout << "\"There's no help for those who lack the valor of mighty men!\"" << endl;

                if ( l_vm.count ( "help" ) )
                    cout << l_publicOptions;
                else if ( l_vm.count ( "verbose-help" ) )
                    cout << l_allOptions;

                cout << endl << endl
                     << "If you want more help and/or information, visit <http://www.thesii.org> to" << endl
                     << "learn more about Wintermute or visit us on IRC (freenode) in ##sii-general." << endl;

                endProgram ();
            } else if ( l_vm.count ( "version" ) ) {
                cout << endl << "Wintermute " << QApplication::applicationVersion ().toStdString () << " "
                     << "using Qt v" << QT_VERSION_STR << ", build " << QLibraryInfo::buildKey ().toStdString ()
                     << ", on " << QLibraryInfo::buildDate ().toString ().toStdString () << "." << endl
                     << "Boost v" << BOOST_VERSION << endl << endl;

                endProgram ( );
            } else if ( l_vm.count ( "copyright" ) ) {
                cout << "Copyright (C) 2010 Synthetic Intellect Institute <contact@thesii.org> <sii@lists.launchpad.net>" << endl
                     << "Copyright (C) 2010 Wintermute Developers <wintermute-devel@lists.launchpad.net> " << endl
                     << "Copyright (C) 2010 Wintermute Robo-Psychologists <wintermute-psych@lists.launchpad.net> " << endl << endl
                     << "\tThis program is free software; you can redistribute it and/or modify "  << endl
                     << "\tit under the terms of the GNU General Public License as published by " << endl
                     << "\tthe Free Software Foundation; either version 3 of the License, or" << endl
                     << "\t(at your option) any later version." << endl << endl;
                endProgram ( );
            }

            for (variables_map::const_iterator l_itr = l_vm.begin (); l_itr != l_vm.end (); l_itr++){
                const QString l_key = QString::fromStdString (l_itr->first);
                const variable_value l_val = l_itr->second;
                s_args->insert (l_key,QString::fromStdString(l_val.as<string>()));
            }

        } else
            cout << "(core) [Core] Run this application with '--help' to get more information." << endl;
    }

    Core* Core::instance () {
        return s_core;
    }

    void Core::unixSignal (int p_sig) const {
        qDebug() << "(core) Caught signal " << p_sig;
        switch (p_sig){

        }
    }

    void Core::start () {
        if (Core::arguments ()->value("ipc").toString () == "master"){
            cout << qPrintable ( s_app->applicationName () ) << " "
                 << qPrintable ( s_app->applicationVersion () )
                 << " (pid " << s_app->applicationPid () << ") :: "
                 << "Artificial intelligence for common Man. (Licensed under the GPL3+)" << endl;
        }

        IPC::System::start ();
        emit s_core->started();

        if (IPC::System::module () == "master"){
            const QStringList l_ntwkArgs = QString("--ipc ntwk").split (" ");
            const QStringList l_lingArgs = QString("--ipc ling").split (" ");
            QStringList l_dataArgs = QString("--ipc data").split (" ");
            l_dataArgs << "--locale" << s_args->value ("locale").toString ()
                       << "--data-dir" << s_args->value ("data-dir").toString ();

            if (Core::arguments ()->value ("daemon").toBool ()){
                qDebug() << QProcess::startDetached (QApplication::applicationFilePath (),l_ntwkArgs)
                         << QProcess::startDetached (QApplication::applicationFilePath (),l_dataArgs)
                         << QProcess::startDetached (QApplication::applicationFilePath (),l_lingArgs);
            } else {
                QProcess l_ntwk(Core::instance ());
                QProcess l_data(Core::instance ());
                QProcess l_ling(Core::instance ());

                l_data.setProcessChannelMode (QProcess::ForwardedChannels);
                l_data.start(QApplication::applicationFilePath (),l_dataArgs);

                l_ntwk.setProcessChannelMode (QProcess::ForwardedChannels);
                l_ntwk.start(QApplication::applicationFilePath (),l_ntwkArgs);

                l_ling.setProcessChannelMode (QProcess::ForwardedChannels);
                l_ling.start(QApplication::applicationFilePath (),l_lingArgs);
            }

            if ( Core::arguments ()->value ( "ncurses" ).toBool () )
                Core::startCurses();
            else
                (new Thread)->run ();
        }
    }

    void Core::endProgram (){
        qDebug() << "(core) Shutting down Wintermute...";
        if (IPC::System::module () != "master"){
            QDBusMessage l_msg = QDBusMessage::createMethodCall ("org.thesii.Wintermute","/Master", "org.thesii.Wintermute.Master","quit");
            QDBusMessage l_reply = IPC::System::bus ()->call (l_msg,QDBus::Block);
            if (l_reply.type () == QDBusMessage::ErrorMessage){
                qDebug() << "(core) Can't shutdown Wintermute's core :"
                         << l_reply.errorMessage ();
            }
        }

        QApplication::quit ();
    }

    void Core::stop () {
        if (IPC::System::module () == "master"){
            if (!s_args->value ("daemon").toBool ())
                Core::stopCurses();
        }

        IPC::System::stop ();
        emit s_core->stopped ();
    }

    void Core::doDeinit () const {
        qDebug() << "(core [module=" << IPC::System::module () << "]) Cleaning up..";
        Core::stop ();
        qDebug() << "(core [module=" << IPC::System::module () << "]) All clean!";
    }

    void Core::startCurses() {
        if (s_args->value ("ncurses").toBool ())
            Curses::start();
        else
            qDebug() << "(core [module=" << IPC::System::module () << "]) nCurses is disabled, not starting.";
    }

    void Core::stopCurses() {
        if (s_args->value ("ncurses").toBool ())
            Curses::stop();
        else
            qDebug() << "(core [module=" << IPC::System::module () << "]) nCurses is disabled, not stopping.";
    }

    void Thread::run() {
        if ( !Core::arguments ()->value ( "gui" ).toBool () ) {
            resetty ();
            Wintermute::Linguistics::Parser l_prsr;
            QTextStream l_strm ( stdin );

            while ( !l_strm.atEnd () ) {
                cout << "(main) Statement: ] ";
                QString l_ln = l_strm.readLine ();
                if ( l_ln == "--" ) {
                    cout << "(main) Statement parsing stopped." << endl;
                    break;
                } else {
                    l_prsr.parse ( l_ln );
                    l_strm << endl;
                }
            }
        }

        QApplication::quit();
    }

}
// kate: indent-mode cstyle; space-indent on; indent-width 4;