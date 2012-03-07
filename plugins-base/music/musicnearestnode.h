/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
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

/**
 * @file
 * The NearestNoteNode class.
 */
#ifndef __TEMPI_MUSIC_NEAREST_NOTE_NODE_H__
#define __TEMPI_MUSIC_NEAREST_NOTE_NODE_H__

#include "tempi/node.h"
#include "tempi/utils.h"
#include <cmath>

namespace tempi {
namespace plugins_base {

namespace { // anonymous namespace

float getOctava(float incoming)
{
    return (incoming - fmod(incoming, 12.0f)) / 12.0f;
}

float getNote(float incoming)
{
    return fmod(incoming, 12.0f);
}

float getClosest(float incoming, std::vector<float> &notes)
{
    if (notes.size() == 0)
        return incoming;
    float octava = getOctava(incoming);
    float note = getNote(incoming);
    unsigned int closest = 0;
    float smallest = 12.0f;
    for (unsigned int i = 0; i < notes.size(); ++i)
    {
        float difference = fabs(notes[i] - note);
        if (difference < smallest)
        {
            smallest = difference;
            closest = i;
        }
    }
    return notes[closest] + octava * 12.0f;
}

} // end of anonymous namespace

/**
 * The NearestNodeNode rounds a note to the nearest within a given scale, in its own octava.
 */
class NearestNoteNode : public Node
{
    public:
        NearestNoteNode() :
            Node()
        {
            this->setShortDocumentation("Rounds a MIDI note to the nearest within a given scale, in its own octava.");
            this->addInlet("0", "Incoming note. (float)");
            this->addOutlet("0", "Resulting note. (float).");
            this->addAttribute(Attribute::ptr(new Attribute("scale", Message("fffff", 0.0f, 2.0f, 4.0f, 7.0f, 9.0f), "Musical scale to pick the note from, in any octava. Default is a pentatonic major scale in C.", false)));
        }
    protected:
        virtual void onNodeAttibuteChanged(const char *name, const Message &value)
        {
            {
                std::ostringstream os;
                os << "NearestNoteNode." << __FUNCTION__ << ": " << name << "=" << value;
                Logger::log(DEBUG, os.str().c_str());
            }
            if (utils::stringsMatch(name, "scale"))
            {
                Message corrected;
                bool to_be_corrected = false;
                for (unsigned int i = 0; i < value.getSize(); ++i)
                {
                    if (value.indexMatchesType(i, 'f'))
                    {
                        if (value.getFloat(i) > 12.0f)
                            to_be_corrected = true;
                        float val = fmod(value.getFloat(i), 12.0f);
                        corrected.appendFloat(val);
                    }
                    else
                        to_be_corrected = true;
                }
                if (to_be_corrected)
                {
                    Logger::log(DEBUG, "NearestNoteNode: calling setAttributeValue within onNodeAttibuteChanged might cause infinite recursion.");
                    setAttributeValue("scale", corrected);
                }
            }
        }
    private:
        virtual void processMessage(const char *inlet, const Message &message)
        {
            if (utils::stringsMatch(inlet, "0"))
            {
                if (! message.typesMatch("f"))
                {
                    std::ostringstream os;
                    os << "NearestNoteNode: need a float, not " << message.getTypes();
                    Logger::log(ERROR, os.str().c_str());
                    return;
                }
                float result = message.getFloat(0);
                std::vector<float> scale;
                Message scale_value = getAttributeValue("scale");
                for (unsigned int i = 0; i < scale_value.getSize(); ++i)
                    scale.push_back(scale_value.getFloat(i));
                result = getClosest(message.getFloat(0), scale);
                output("0", Message("f", result));
            }
        }
};

} // end of namespace
} // end of namespace

#endif // ifndef

