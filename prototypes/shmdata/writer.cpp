/*
 * Copyright (C) 2012 Nicolas Bouillot (http://www.nicolasbouillot.net)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 */

#include <stdio.h>
#include <glib.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>		//sleep
#include <shmdata/any-data-writer.h>
#include <tempi/message.h>
#include <tr1/memory>
#include <string.h>

typedef std::tr1::shared_ptr<tempi::Message> MessagePtr;

shmdata_any_writer_t *writer;

static void on_done_with_data (void *priv);

//clean up pipeline when ctrl-c
void
leave (int sig)
{
  shmdata_any_writer_close (writer);
  exit (sig);
}

static std::vector<MessagePtr> messages;

static tempi::Message* addMessage()
{
  static int i = 0;
  static const char *text = "hello";
  static const float pi = 3.14159f;
  MessagePtr m(new tempi::Message("sif", text, i, pi));
  messages.push_back(m);
  i++;
  return m.get();
}

int
main (int argc, char *argv[])
{
  (void) signal (SIGINT, leave);

  /* Check input arguments */
  if (argc != 2)
    {
      printf ("Usage: %s <socket-path>\n", argv[0]);
      return -1;
    }

  writer = shmdata_any_writer_init ();
  if (! shmdata_any_writer_set_path (writer, argv[1]))
    {
      g_printerr ("**** The file %s exists, therefore a shmdata cannot be operated with this path.\n",argv[1]);
      shmdata_any_writer_close (writer);
      exit(0);
    }
  shmdata_any_writer_set_debug (writer, SHMDATA_ENABLE_DEBUG);
  shmdata_any_writer_set_data_type (writer, "application/x-tempi");
  shmdata_any_writer_start (writer);

  unsigned long long myclock = 0;
  unsigned long long nsecPeriod = 30000000;

  while (1)
    {
      tempi::Message *message = addMessage();
      std::cout << "write " << (*message) << std::endl;
      //data should be serialized if network is involved
      shmdata_any_writer_push_data (writer,
				    static_cast<void *>(message),
				    sizeof (message),
				    myclock,
				    &on_done_with_data, NULL);
      usleep (nsecPeriod / 1000);
      myclock += nsecPeriod;
    }

  return 0;
}

static void freeMessage(tempi::Message *message)
{
    std::vector<MessagePtr>::iterator iter;
    for (iter = messages.begin(); iter != messages.end(); iter++)
    {
        if ((*iter).get() == message)
        {
            std::cout << "free some data\n";
            messages.erase(iter);
            return;
        }
    }
}

static void
on_done_with_data (void *priv)
{
  //printf ("freeing buffer for pointer %p\n", priv);
  //free (priv);
  tempi::Message *message = static_cast<tempi::Message*>(priv);
  freeMessage(message);
}

