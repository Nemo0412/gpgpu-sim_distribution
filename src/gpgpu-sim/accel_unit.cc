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

accel_unit::accel_unit(gpgpu_sim *m_gpu) : m_gpu(m_gpu) {
  m_cluster_id = m_gpu->get_config().num_cluster();
  state = 1;
}

void accel_unit::cycle(){
  // TODO:  A state machine (state.1 request memory (transit to stage.2); stage.2 wait for memory response (check icnt_pop mf); stage.3 compute (count 1000 cycles); stage.4 write memory (transit to stage.1))

  // check if there is a memory response



  // state transition
  switch (state) {
    case 1:
      // request memory
      send_memory_request(false);
      state = 2;
      break;
    case 2:
      // wait for memory response
      break;
    case 3:
      // compute
      break;
    case 4:
      // write memory
      send_memory_request(true);
      state = 1;
      break;
    default:
      break;
  }

}

void accel_unit::send_memory_request(bool is_write){
  std::cout << "Acceleration Unit: Sending memory request." << std::endl;

  mem_access_type access_type = is_write ? GLOBAL_ACC_W : GLOBAL_ACC_R;

  auto access = mem_access_t(access_type, 0x1000, 64, is_write, m_gpu->gpgpu_ctx);
  mem_fetch *mf = new mem_fetch(access, NULL, is_write ? WRITE_PACKET_SIZE : READ_PACKET_SIZE, warp_id,
    m_core_id, m_cluster_id, m_gpu->getMemoryConfig(), m_gpu->gpu_tot_sim_cycle + m_gpu->gpu_tot_sim_cycle);

  unsigned destination = mf->get_sub_partition_id();
  unsigned request_size = mf->get_is_write()? mf->size(): mf->get_ctrl_size();
  icnt_push(m_cluster_id, m_gpu->getShaderCoreConfig()->mem2device(destination), (void *)mf, request_size);
}