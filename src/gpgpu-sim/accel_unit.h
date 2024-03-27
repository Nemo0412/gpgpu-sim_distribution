#ifndef ACCEL_UNIT_H
#define ACCEL_UNIT_H

// #include "../abstract_hardware_model.h"
class gpgpu_sim;



class accel_unit {
  public:
    accel_unit(class gpgpu_sim *m_gpu);

    void cycle();
  
  private:
    void send_memory_request(bool is_write);

    class gpgpu_sim *m_gpu;
    int state;
    unsigned compute_cycles;
    
    unsigned warp_id = 0;
    unsigned m_core_id = 0;
    unsigned m_cluster_id;
};

#endif