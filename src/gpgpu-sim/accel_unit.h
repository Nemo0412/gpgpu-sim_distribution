#ifndef ACCEL_UNIT_H
#define ACCEL_UNIT_H

// #include "../abstract_hardware_model.h"
class gpgpu_sim;



class accel_unit {
  public:
    accel_unit(class gpgpu_sim *m_gpu): m_gpu(m_gpu) {}

    void cycle();
  
  private:
    void send_memory_request();

    class gpgpu_sim *m_gpu;
    
    unsigned m_id = 0;
    unsigned warp_id = 0;  // assume warp_id
    unsigned m_core_id = 0;
    unsigned m_cluster_id = 0;
};

#endif