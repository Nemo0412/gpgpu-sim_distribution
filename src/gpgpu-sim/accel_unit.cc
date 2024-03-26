#include <iostream>

#include "accel_unit.h"
#include "icnt_wrapper.h"
#include "gpu-sim.h"

/* READ_PACKET_SIZE:
   bytes: 6 address (flit can specify chanel so this gives up to ~2GB/channel,
   so good for now), 2 bytes   [shaderid + mshrid](14 bits) + req_size(0-2 bits
   if req_size variable) - so up to 2^14 = 16384 mshr total
 */
#define READ_PACKET_SIZE 8
// WRITE_PACKET_SIZE: bytes: 6 address, 2 miscelaneous.
#define WRITE_PACKET_SIZE 8

void accel_unit::cycle(){
  // TODO: Figure out what to do in each cycle
  send_memory_request();
}

void accel_unit::send_memory_request(){
  std::cout << "Acceleration Unit: Sending memory request." << std::endl;

  auto access = mem_access_t(GLOBAL_ACC_W, 0x1000, 8, true, m_gpu->gpgpu_ctx);
  mem_fetch *mf = new mem_fetch(access, NULL, WRITE_PACKET_SIZE, warp_id,
    m_core_id, m_cluster_id, m_gpu->getMemoryConfig(), m_gpu->gpu_tot_sim_cycle + m_gpu->gpu_tot_sim_cycle);

  icnt_push(m_id, 80, (void *)mf, 8);
}