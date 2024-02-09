/*
 * Copyright (c) 2020 Inria
 * Copyright (c) 2016 Georgia Institute of Technology
 * Copyright (c) 2008 Princeton University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "mem/ruby/network/garnet/InputUnit.hh"

#include "debug/RubyNetwork.hh"
#include "mem/ruby/network/garnet/Credit.hh"
#include "mem/ruby/network/garnet/Router.hh"

namespace gem5
{

namespace ruby
{

namespace garnet
{

    std::vector<int> nack_packets;
    int no_nack = 0;

    int trojan_router = 20;
    int no_of_affected_packets = 0;
    int no_of_total_packets = 0;
    int no_of_affected_flits = 0;
    int no_of_total_flits = 0;
    std::vector<int> affected_packets;

InputUnit::InputUnit(int id, PortDirection direction, Router *router)
  : Consumer(router), m_router(router), m_id(id), m_direction(direction),
    m_vc_per_vnet(m_router->get_vc_per_vnet())
{
    const int m_num_vcs = m_router->get_num_vcs();
    m_num_buffer_reads.resize(m_num_vcs/m_vc_per_vnet);
    m_num_buffer_writes.resize(m_num_vcs/m_vc_per_vnet);
    for (int i = 0; i < m_num_buffer_reads.size(); i++) {
        m_num_buffer_reads[i] = 0;
        m_num_buffer_writes[i] = 0;
    }

    // Instantiating the virtual channels
    virtualChannels.reserve(m_num_vcs);
    for (int i=0; i < m_num_vcs; i++) {
        virtualChannels.emplace_back();
    }
}

/*
 * The InputUnit wakeup function reads the input flit from its input link.
 * Each flit arrives with an input VC.
 * For HEAD/HEAD_TAIL flits, performs route computation,
 * and updates route in the input VC.
 * The flit is buffered for (m_latency - 1) cycles in the input VC
 * and marked as valid for SwitchAllocation starting that cycle.
 *
 */

void
InputUnit::wakeup()
{
    flit *t_flit;
    if (m_in_link->isReady(curTick())) {

        t_flit = m_in_link->consumeLink();
        DPRINTF(RubyNetwork, "Router[%d] Consuming:%s Width: %d Flit:%s\n",
        m_router->get_id(), m_in_link->name(),
        m_router->getBitWidth(), *t_flit);
        assert(t_flit->m_width == m_router->getBitWidth());
        int vc = t_flit->get_vc();
        t_flit->increment_hops(); // for stats

        if ((t_flit->get_type() == HEAD_) ||
            (t_flit->get_type() == HEAD_TAIL_)) {

            assert(virtualChannels[vc].get_state() == IDLE_);
            set_vc_active(vc, curTick());

            // Route computation for this vc
            int outport = m_router->route_compute(t_flit->get_route(),
                m_id, m_direction);

            // Update output port in VC
            // All flits in this packet will use this output port
            // The output port field in the flit is updated after it wins SA
            grant_outport(vc, outport);

        } else {
            assert(virtualChannels[vc].get_state() == ACTIVE_);
        }

        //my changes for packet path printing using a vector here
        int t=m_router->get_id();
        //std::cout<<m_router->get_id()<<" ";
        t_flit->add_to_path(t);
        // for debugging: t_flit->print_path();

        //for random affecting flits for 40%
        /*if (t==trojan_router){
            if (t_flit->get_type()==HEAD_ || t_flit->get_type()==HEAD_TAIL_){
                if (t_flit->get_id()==0 && std::find(affected_packets.begin(), affected_packets.end(), t_flit->getPacketID())==affected_packets.end()){
                    int x;
                    x = std::rand()%1000;
                    if (x>200 && x<=600){
                        t_flit->modify = true;
                        no_of_affected_flits++;
                        no_of_total_flits++;
                        no_of_affected_packets++;
                        no_of_total_packets++;
                        affected_packets.push_back(t_flit->getPacketID());
                        std::cout<<"affected packets: "<<no_of_affected_packets<<" total packets: "<<no_of_total_packets<<std::endl;
                        std::cout<<"packet id: "<<t_flit->getPacketID()<<std::endl;
                    }
                    else{
                        no_of_total_flits++;
                        no_of_total_packets++;
                }
                }
            }
            else{
                if (std::find(affected_packets.begin(), affected_packets.end(), t_flit->getPacketID())!=affected_packets.end()){
                    t_flit->modify = true;
                    no_of_affected_flits++;
                    no_of_total_flits++;
                }
                else{
                    no_of_total_flits++;
                }
            }
            //std::cout<<"packet id: "<<t_flit->getPacketID()<<"\tflit id: "<<t_flit->get_id()<<"\taffected\n";no of nack packets: 1

            //std::cout<<"affected flits: "<<no_of_affected_flits<<" total flits: "<<no_of_total_flits<<std::endl;
        }*/

        //affecting all flits

        /*if (t==trojan_router){
            t_flit->modify = true;
            no_of_affected_flits++;
            no_of_total_flits++;
            if (t_flit->get_type()==HEAD_ || t_flit->get_type()==HEAD_TAIL_){
                no_of_affected_packets++;
                no_of_total_packets++;
                affected_packets.push_back(t_flit->getPacketID());

            }
            std::cout<<"affected packets: "<<no_of_affected_packets<<" total packets: "<<no_of_total_packets<<std::endl;
            std::cout<<"packet id: "<<t_flit->getPacketID()<<std::endl;
        }*/

        if (t_flit->is_nack == true){
            if (t==t_flit->get_route().dest_router && std::find(nack_packets.begin(), nack_packets.end(), t_flit->getPacketID())==nack_packets.end()){
                nack_packets.push_back(t_flit->getPacketID());
                no_nack++;
                std::cout<<"nack received at router "<<t<<std::endl;
                std::cout<<"path of nack: ";
                t_flit->print_path();
                std::cout<<"packet id: "<<t_flit->getPacketID();
                std::cout<<std::endl;
                std::cout<<"no nack packets: "<<no_nack<<std::endl;
                std::cout<<std::endl;
            }
            //td::cout<<"destination router: "<<t_flit->get_route().dest_router<<std::endl;
        }

        // Buffer the flit
        virtualChannels[vc].insertFlit(t_flit);


        int vnet = vc/m_vc_per_vnet;
        // number of writes same as reads
        // any flit that is written will be read only once
        m_num_buffer_writes[vnet]++;
        m_num_buffer_reads[vnet]++;

        Cycles pipe_stages = m_router->get_pipe_stages();
        if (pipe_stages == 1) {
            // 1-cycle router
            // Flit goes for SA directly
            t_flit->advance_stage(SA_, curTick());
        } else {
            assert(pipe_stages > 1);
            // Router delay is modeled by making flit wait in buffer for
            // (pipe_stages cycles - 1) cycles before going for SA

            Cycles wait_time = pipe_stages - Cycles(1);
            t_flit->advance_stage(SA_, m_router->clockEdge(wait_time));

            // Wakeup the router in that cycle to perform SA
            m_router->schedule_wakeup(Cycles(wait_time));
        }

        if (m_in_link->isReady(curTick())) {
            m_router->schedule_wakeup(Cycles(1));
        }
    }
}

// Send a credit back to upstream router for this VC.
// Called by SwitchAllocator when the flit in this VC wins the Switch.
void
InputUnit::increment_credit(int in_vc, bool free_signal, Tick curTime)
{
    DPRINTF(RubyNetwork, "Router[%d]: Sending a credit vc:%d free:%d to %s\n",
    m_router->get_id(), in_vc, free_signal, m_credit_link->name());
    Credit *t_credit = new Credit(in_vc, free_signal, curTime);
    creditQueue.insert(t_credit);
    m_credit_link->scheduleEventAbsolute(m_router->clockEdge(Cycles(1)));
}

bool
InputUnit::functionalRead(Packet *pkt, WriteMask &mask)
{
    bool read = false;
    for (auto& virtual_channel : virtualChannels) {
        if (virtual_channel.functionalRead(pkt, mask))
            read = true;
    }

    return read;
}

uint32_t
InputUnit::functionalWrite(Packet *pkt)
{
    uint32_t num_functional_writes = 0;
    for (auto& virtual_channel : virtualChannels) {
        num_functional_writes += virtual_channel.functionalWrite(pkt);
    }

    return num_functional_writes;
}

void
InputUnit::resetStats()
{
    for (int j = 0; j < m_num_buffer_reads.size(); j++) {
        m_num_buffer_reads[j] = 0;
        m_num_buffer_writes[j] = 0;
    }
}

} // namespace garnet
} // namespace ruby
} // namespace gem5
