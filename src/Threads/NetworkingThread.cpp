#include "NetworkingThread.h"

namespace zge
{
NetworkingThread::NetworkingThread()
    : AffinityThread(NetworkingThread::CORE_NUMBER)
{
}

NetworkingThread::~NetworkingThread() NOEXCEPT { this->join(); }

} // namespace zge

/*
udp thread, initial broadcast
listener thread
sender thread
*/
