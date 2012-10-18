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
#include "tempi/midi_scheduler.h"
#include "tempi/log.h"
#include <cstdlib>
#include <iostream>

namespace tempi {
namespace midi {

MidiScheduler::MidiScheduler() :  
    process_midi_exit_flag_ (false),
    app_sysex_in_progress_ (false),
    thru_sysex_in_progress_ (false)
{
    portmidi_initialized_ = false;
    /* always start the timer before you start midi */
    Pt_Start(1, &process_midi, this); /* start a timer with millisecond accuracy */
    /* the timer will call our function, process_midi() every millisecond */
    Pm_Initialize();
    portmidi_initialized_ = true;
}
    
MidiScheduler::~MidiScheduler()
{
    /* the timer thread could be in the middle of accessing PortMidi stuff */
    /* to detect that it is done, we first clear process_midi_exit_flag and
     * then wait for the timer thread to set it
     */
    process_midi_exit_flag_ = false;
    portmidi_initialized_ = false;
    /* busy wait for flag from timer thread that it is done */
    while (! process_midi_exit_flag_)
    {
        Logger::log(DEBUG, "Waiting for portmidi to exit...");
        // TODO: sleep
    }
    /* at this point, midi thread is inactive and we need to shut down
     * the midi input and output
     */
    Pt_Stop(); /* stop the timer */
    Pm_Terminate();
}

PmStream * MidiScheduler::add_input_stream(int id)
{
    PmStream *midi_in;
    if (pmNoError != Pm_OpenInput(
        &midi_in, id, 
        NULL /* driver info */,
        0 /* use default input size */,
        NULL,
        NULL /* time info */))
    {
        return NULL;
    }
    /* Note: if you set a filter here, then this will filter what goes
     * to the MIDI THRU port. You may not want to do this.
     */
    Pm_SetFilter(midi_in, PM_FILT_ACTIVE | PM_FILT_CLOCK);
    input_queues_[midi_in] = new std::queue<PmEvent>();      
    
    return midi_in;
}

PmStream * MidiScheduler::add_output_stream(int id)
{
    PmStream *midi_out;
    if (pmNoError != Pm_OpenOutput(
        &midi_out, 
        id, 
        NULL /* driver info */,
        0 /* use default input size */,
        NULL,
        NULL, /* time info */
        0))
    {
        return NULL;
    }

    output_queues_[midi_out] = new std::queue<PmEvent>();      
    return midi_out;
}

PmEvent MidiScheduler::poll(PmStream *stream)
{
    PmEvent message = input_queues_[stream]->front();
    input_queues_[stream]->pop();
    return message;
}

bool MidiScheduler::is_queue_empty(PmStream *stream)
{
    return input_queues_[stream]->empty();
}

bool MidiScheduler::remove_input_stream(PmStream *stream)
{
    input_queues_.erase(stream);
    return true;
}

bool MidiScheduler::remove_output_stream(PmStream *stream)
{
    output_queues_.erase(stream);
    return true;
}

bool MidiScheduler::push_message(PmStream *stream, unsigned char status, unsigned char data1, unsigned char data2)
{
    PmEvent message_to_push;
    message_to_push.message = Pm_Message(status,data1,data2);
    message_to_push.timestamp=0; //use urrent time
  
    output_queues_[stream]->push (message_to_push);
    return true;
}

/* timer interrupt for processing midi data.
   Incoming data is delivered to main program via in_queue.
   Outgoing data from main program is delivered via out_queue.
   Incoming data from midi_in is copied with low latency to  midi_out.
   Sysex messages from either source block messages from the other.
*/
void MidiScheduler::process_midi(PtTimestamp timestamp, void *user_data)
{
    MidiScheduler *context = static_cast<MidiScheduler *>(user_data);
    PmError result;
    PmEvent buffer; /* just one message at a time */

    /* do nothing until initialization completes */
    if (!context->portmidi_initialized_)
    {
        /* this flag signals that no more midi processing will be done */
        context->process_midi_exit_flag_ = true;
        return;
    }
  
    std::map<PmStream *,std::queue<PmEvent> *>::iterator itr;
    for(itr = context->input_queues_.begin(); itr != context->input_queues_.end(); ++itr)
    {
        /* see if there is any midi input to process */
        if (! context->app_sysex_in_progress_)
        {
            do
            {
                result = Pm_Poll(itr->first);
                if (result)
                {
                    int status;
                    PmError rslt = (PmError)Pm_Read(itr->first, &buffer, 1);
                    if (rslt == pmBufferOverflow) 
                        continue;
                    //assert(rslt == 1);
                    
                    /* the data might be the end of a sysex message that
                       has timed out, in which case we must ignore it.
                       It's a continuation of a sysex message if status
                       is actually a data byte (high-order bit is zero). */
                    status = Pm_MessageStatus(buffer.message);
                    if (((status & 0x80) == 0) && !context->thru_sysex_in_progress_) {
                      continue; /* ignore this data */
                    }
          
                    // std::cout 
                    //   << "midi_scheduler msg: " 
                    //   << " status= "  
                    //   << Pm_MessageStatus(buffer.message) 
                    //   << ", data 1= "
                    //   << Pm_MessageData1(buffer.message)
                    //   << ", data 2= "
                    //   << Pm_MessageData2(buffer.message)
                    //   << std::endl;
            
                    itr->second->push(buffer);

                    /* sysex processing */
                    if (status == MIDI_SYSEX)
                        context->thru_sysex_in_progress_ = true;
                    else if ((status & 0xF8) != 0xF8)
                    {
                        /* not MIDI_SYSEX and not real-time, so */
                        context->thru_sysex_in_progress_ = false;
                    }
                    if (context->thru_sysex_in_progress_ && /* look for EOX */
                        (((buffer.message & 0xFF) == MIDI_EOX) ||
                        (((buffer.message >> 8) & 0xFF) == MIDI_EOX) ||
                        (((buffer.message >> 16) & 0xFF) == MIDI_EOX) ||
                        (((buffer.message >> 24) & 0xFF) == MIDI_EOX)))
                    {
                        context->thru_sysex_in_progress_ = false;
                    }
                }
            } while (result);
        }
    } //end of "for input_streams_"

    for(itr = context->output_queues_.begin(); itr != context->output_queues_.end(); ++itr)
    {
        /* see if there is application midi data to process */
        while (!itr->second->empty())
        {
            /* see if it is time to output the next message */
            PmEvent *next = &(itr->second->front());//(PmEvent *) Pm_QueuePeek(out_queue);
            //assert(next); /* must be non-null because queue is not empty */
            /* time to send a message, first make sure it's not blocked */
            int status = Pm_MessageStatus(next->message);
            if ((status & 0xF8) == 0xF8)
            {
                ; /* real-time messages are not blocked */
            }
            else if (context->thru_sysex_in_progress_)
            {
                /* maybe sysex has timed out (output becomes unblocked) */
                context->thru_sysex_in_progress_ = false;
            }
    
            // std::cout 
            //   << "midi_scheduler msg write: " 
            //   << " status= "  
            //   << Pm_MessageStatus(next->message) 
            //   << ", data 1= "
            //   << Pm_MessageData1(next->message)
            //   << ", data 2= "
            //   << Pm_MessageData2(next->message)
            //   << std::endl;
    
            Pm_Write(itr->first, next, 1);
            itr->second->pop();
    
            /* inspect message to update app_sysex_in_progress */
            if (status == MIDI_SYSEX)
                context->app_sysex_in_progress_ = true;
            else if ((status & 0xF8) != 0xF8)
            {
                /* not MIDI_SYSEX and not real-time, so */
                context->app_sysex_in_progress_ = false;
            }
            if (context->app_sysex_in_progress_ && /* look for EOX */
                (((buffer.message & 0xFF) == MIDI_EOX) ||
                (((buffer.message >> 8) & 0xFF) == MIDI_EOX) ||
                (((buffer.message >> 16) & 0xFF) == MIDI_EOX) ||
                (((buffer.message >> 24) & 0xFF) == MIDI_EOX)))
            {
                context->app_sysex_in_progress_ = false;
            }
        }
    }
}
    
} // end of namespace
} // end of namespace

