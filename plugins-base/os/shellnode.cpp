/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 * Copyright (C) 2012 Emmanuel Durand
 *
 * This file is part of Tempi-plugins-base.
 *
 * This program is free software; you can redistither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Tempi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tempi/config.h" // include before check for HAVE_GLIB

#ifdef HAVE_GLIB
#include "plugins-base/os/shellnode.h"
#include "tempi/log.h"

#include <iostream>
#include <cstdlib>
#include <glib.h>
#include <string>

namespace tempi {
namespace plugins_base {

const char * const ShellNode::COMMAND_INLET = "command";
const char * const ShellNode::OUTPUT_OUTLET = "output";
const char * const ShellNode::RETVAL_OUTLET = "retval";
const char * const ShellNode::SUCCESS_OUTLET = "success";

/**
 * Runs a command synchronously
 */
static bool run_command(const char * command, std::string &output, int &retval)
{
    GError *error = NULL;
    gint status = 0;
    gchar *stdout_message = NULL;
    gchar *stderr_message = NULL;
    gboolean ret = FALSE;
    
    ret = g_spawn_command_line_sync(command, &stdout_message, &stderr_message, &status, &error);
    if ((error and (error->code != 0)) or ! WIFEXITED(status) or WEXITSTATUS(status))
    {
        std::ostringstream os;
        os << "ShellNode::" << __FUNCTION__ << ": Failed to execute command.";
        os << " command: " << command;
        os << " exited: " << WIFEXITED(status);
        os << " status: " << WEXITSTATUS(status);
        os << " stderr: " << (stderr_message != NULL ? stderr_message : "");
        os << " stdout: " << (stdout_message != NULL ? stdout_message : "");
        Logger::log(ERROR, os);
    }
    else
    {
        if (Logger::isEnabledFor(INFO))
        {
            std::ostringstream os;
            os << "ShellNode::" << __FUNCTION__ << ": Suscessfully ran command.";
            os << " command: " << command;
            os << " exited: " << WIFEXITED(status);
            os << " status: " << WEXITSTATUS(status);
            os << " stderr: " << stderr_message != NULL ? stderr_message : "";
            os << " stdout: " << stdout_message != NULL ? stdout_message : "";
            Logger::log(INFO, os);
        }
        ret = TRUE;
    }

    // set values resulting
    retval = status;
    output = stdout_message;

    // cleanup
    if (error)
        g_error_free(error);
    g_free(stdout_message);
    g_free(stderr_message);

    // return success
    return ret == TRUE;
}

ShellNode::ShellNode() :
    Node()
{
    this->setShortDocumentation("Runs commands synchronously.");
    this->setLongDocumentation("Warning: the program will hang until the command is done running!"); // FIXME
    this->addInlet(COMMAND_INLET, "Command to execute.");
    this->addOutlet(SUCCESS_OUTLET, "Success or not of the last command. Outputs a boolean.");
    this->addOutlet(OUTPUT_OUTLET, "standard output of the child process.");
    this->addOutlet(RETVAL_OUTLET, "Return value of the child process.");
}

void ShellNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, COMMAND_INLET))
        return;
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "ShellNode::" << __FUNCTION__ << ": " << message;
        Logger::log(DEBUG, os.str().c_str());
    }
    bool success = false;

    // build command string
    std::ostringstream command;
    for (unsigned int i = 0; i < message.getSize(); i++)
    {
        command << utils::argumentToString(message, i) << " ";
    }

    std::string output;
    int retval;
    bool ok = run_command(command.str().c_str(), output, retval);

    this->output(SUCCESS_OUTLET, Message("b", success));
    this->output(RETVAL_OUTLET, Message("i", retval));
    this->output(OUTPUT_OUTLET, Message("s", output.c_str()));
}

} // end of namespace
} // end of namespace
#endif // HAVE_GLIB

