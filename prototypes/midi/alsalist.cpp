#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <glib.h>
#include <alsa/asoundlib.h>
#include <boost/tuple/tuple.hpp>

class MidiProber
{
    public:
        typedef boost::tuple<int, int, std::string, std::string> PortInfo;
        typedef enum { SOURCE = 0, SINK = 1 } Direction;
        MidiProber();
        ~MidiProber();
        bool isOpen() const;
        void printPorts(Direction direction);
        std::vector<PortInfo> listPorts(Direction direction);
    private:
        bool isOpen_;
	    snd_seq_t *handle_;
};

MidiProber::MidiProber() :
    isOpen_(false)
{
	int err = snd_seq_open(&handle_, "hw", SND_SEQ_OPEN_DUPLEX, 0);
	if (err < 0)
    {
        std::cerr << "Could not open sequencer " << errno << std::endl;
    }
    else
    {
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
        snd_seq_drop_output(handle_);
#endif
        isOpen_ = true;
    }
}

bool MidiProber::isOpen() const
{
    return isOpen_;
}

MidiProber::~MidiProber()
{
    if (isOpen())
        snd_seq_close(handle_);
}

std::vector<MidiProber::PortInfo> MidiProber::listPorts(Direction direction)
{
    std::vector<PortInfo> ret;
    if (! isOpen())
    {
        std::cerr << "MidiProber::" << __FUNCTION__ << "(): Cannot list ports since our client is not open.\n";
        return ret;
    }
    else
    {
        snd_seq_client_info_t *cinfo;
        snd_seq_port_info_t *pinfo;
        int client;
        int port;
        snd_seq_client_info_alloca(&cinfo);
        snd_seq_client_info_set_client(cinfo, -1);
        while (snd_seq_query_next_client(handle_, cinfo) >= 0)
        {
            client = snd_seq_client_info_get_client(cinfo);
            snd_seq_port_info_alloca(&pinfo);
            snd_seq_port_info_set_client(pinfo, client);
            snd_seq_port_info_set_port(pinfo, -1);
            while (snd_seq_query_next_port(handle_, pinfo) >= 0)
            {
                int cap;
                if (direction == SINK)
                    cap = (SND_SEQ_PORT_CAP_SUBS_WRITE | SND_SEQ_PORT_CAP_WRITE);
                else
                    cap = (SND_SEQ_PORT_CAP_SUBS_READ | SND_SEQ_PORT_CAP_READ);
                if ((snd_seq_port_info_get_capability(pinfo) & cap) == cap)
                {
                    ret.push_back(
                        PortInfo(
                            snd_seq_port_info_get_client(pinfo),
                            snd_seq_port_info_get_port(pinfo),
                            std::string(snd_seq_client_info_get_name(cinfo)),
                            std::string(snd_seq_port_info_get_name(pinfo))
                            )
                        );
                    // std::cout <<  "hello: "
                    //         << " " << snd_seq_port_info_get_client(pinfo)
                    //         << " " << snd_seq_port_info_get_port(pinfo)
                    //         << " " << std::string(snd_seq_client_info_get_name(cinfo))
                    //         << " " << std::string(snd_seq_port_info_get_name(pinfo))
                    //         << " " << std:: endl;
                } // if
            } // while client
        } // while port
        return ret;
	} // if
}

void MidiProber::printPorts(Direction direction)
{
    if (isOpen())
    {
        printf(" Port     %-30.30s    %s\n", "Client name", "Port name");
        std::vector<PortInfo> ports = listPorts(direction);
        std::vector<PortInfo>::const_iterator iter;
        for (iter = ports.begin(); iter != ports.end(); ++iter)
        {
            printf("%3d:%-3d   %-30.30s    %s\n",
                (*iter).get<0>(),
                (*iter).get<1>(),
                (*iter).get<2>().c_str(),
                (*iter).get<3>().c_str()
                );
        }
    }
    else
        std::cout << "Cannot list MIDI ports.\n";
}

int main(int argc, char *argv[])
{
    MidiProber prober;
    std::cout << "Sinks: \n";
    prober.printPorts(MidiProber::SINK);
    std::cout << "Sources: \n";
    prober.printPorts(MidiProber::SOURCE);
    return 0;
}

