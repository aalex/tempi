#include <stdio.h>
#include <iostream>
#include <glib.h>
#include <alsa/asoundlib.h>

/*
 * Show a list of possible output ports that midi could be sent
 * to.
 */
static void listMidiSinks()
{
	snd_seq_client_info_t *cinfo;
	snd_seq_port_info_t *pinfo;
	int  client;
	int  port;
	int  err;
	snd_seq_t *handle;

	err = snd_seq_open(&handle, "hw", SND_SEQ_OPEN_DUPLEX, 0);
	if (err < 0)
        std::cerr << "Could not open sequencer " << errno << std::endl;
		//except(ioError, _("Could not open sequencer %s"), snd_strerror(errno));

#ifdef USE_DRAIN
	/*
	 * NOTE: This is here so that it will give an error if the wrong
	 * version of alsa is used with USE_DRAIN set.  An incompatible change
	 * was made to ALSA on 29 Sep 2000 so that after that date you should
	 * define USE_DRAIN.
	 *
	 * Unfortunately with USE_DRAIN defined with an earlier version of alsa
	 * it will appear to compile fine but simply not work properly.  This
	 * unneeded call will give an error in this case.
	 *
	 * IF you see an error about this line, comment out the line beginning
	 * with USE_DRAIN in the make.conf file or compile with:
	 *    make USE_DRAIN=''
	 */
	snd_seq_drop_output(handle);
#endif

	snd_seq_client_info_alloca(&cinfo);
	snd_seq_client_info_set_client(cinfo, -1);
	printf(" Port     %-30.30s    %s\n", "Client name", "Port name");

	while (snd_seq_query_next_client(handle, cinfo) >= 0)
    {
		client = snd_seq_client_info_get_client(cinfo);
		snd_seq_port_info_alloca(&pinfo);
		snd_seq_port_info_set_client(pinfo, client);

		snd_seq_port_info_set_port(pinfo, -1);
		while (snd_seq_query_next_port(handle, pinfo) >= 0)
        {
			int  cap;
			cap = (SND_SEQ_PORT_CAP_SUBS_WRITE|SND_SEQ_PORT_CAP_WRITE);
			if ((snd_seq_port_info_get_capability(pinfo) & cap) == cap)
            {
				printf("%3d:%-3d   %-30.30s    %s\n",
					snd_seq_port_info_get_client(pinfo),
					snd_seq_port_info_get_port(pinfo),
					snd_seq_client_info_get_name(cinfo),
					snd_seq_port_info_get_name(pinfo)
					);
			}
		}
	}
}

/*
 * Show a list of possible input ports that midi could be received from
 */
static void listMidiSources()
{
	snd_seq_client_info_t *cinfo;
	snd_seq_port_info_t *pinfo;
	int  client;
	int  port;
	int  err;
	snd_seq_t *handle;

	err = snd_seq_open(&handle, "hw", SND_SEQ_OPEN_DUPLEX, 0);
	if (err < 0)
        std::cerr << "Could not open sequencer " << errno << std::endl;
		//except(ioError, _("Could not open sequencer %s"), snd_strerror(errno));

#ifdef USE_DRAIN
	/*
	 * NOTE: This is here so that it will give an error if the wrong
	 * version of alsa is used with USE_DRAIN set.  An incompatible change
	 * was made to ALSA on 29 Sep 2000 so that after that date you should
	 * define USE_DRAIN.
	 *
	 * Unfortunately with USE_DRAIN defined with an earlier version of alsa
	 * it will appear to compile fine but simply not work properly.  This
	 * unneeded call will give an error in this case.
	 *
	 * IF you see an error about this line, comment out the line beginning
	 * with USE_DRAIN in the make.conf file or compile with:
	 *    make USE_DRAIN=''
	 */
	snd_seq_drop_output(handle);
#endif

	snd_seq_client_info_alloca(&cinfo);
	snd_seq_client_info_set_client(cinfo, -1);
	printf(" Port     %-30.30s    %s\n", "Client name", "Port name");

	while (snd_seq_query_next_client(handle, cinfo) >= 0)
    {
		client = snd_seq_client_info_get_client(cinfo);
		snd_seq_port_info_alloca(&pinfo);
		snd_seq_port_info_set_client(pinfo, client);

		snd_seq_port_info_set_port(pinfo, -1);
		while (snd_seq_query_next_port(handle, pinfo) >= 0)
        {
			int  cap;
			cap = (SND_SEQ_PORT_CAP_SUBS_READ|SND_SEQ_PORT_CAP_READ);
			if ((snd_seq_port_info_get_capability(pinfo) & cap) == cap)
            {
				printf("%3d:%-3d   %-30.30s    %s\n",
					snd_seq_port_info_get_client(pinfo),
					snd_seq_port_info_get_port(pinfo),
					snd_seq_client_info_get_name(cinfo),
					snd_seq_port_info_get_name(pinfo)
					);
			}
		}
	}
}

int main(int argc, char *argv[])
{
    std::cout << "Sinks: \n";
    listMidiSinks();
    std::cout << "Sources: \n";
    listMidiSources();
    return 0;
}
