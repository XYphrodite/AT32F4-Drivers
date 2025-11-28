#ifndef _BPR_SERVICE_H_
#define _BPR_SERVICE_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "at32f403a_407.h"
#include <stdbool.h>
#include "Macros.h"
#ifndef PREBOOT
#include "MyMath.h"
#endif


#define BPR_CONFIG_VERSION (1U)

#pragma pack(push, 1)
    struct bpr_state_machine_t
    {
        bool run_rx : 1;
        bool was_blink : 1;
        bool stop_usb : 1;
        bool work_with_uart : 1;
        bool is_rtc_running : 1;
        bool run_main_app : 1;
        uint16_t reserved : 10;
    };

    struct bpr_service_error_f
    {
        uint16_t reserved : 16;
    };

    struct bpr_service_data_t
    {
        uint8_t failed_booting : 4;
        uint8_t accel_lie : 4;
        uint8_t reserved : 8;
    };

#pragma pack(pop)

    extern struct bpr_state_machine_t state_machine;
    extern struct bpr_service_data_t service_data;
    extern uint16_t time_since_last_uplink_s;

    extern uint32_t voltages_raw_div_2_sum;
    extern uint32_t voltages_raw_all_sum;

    extern int64_t top_temperatures_sum;
    extern int64_t main_temperatures_sum;

    extern uint16_t measurements_amount;

    void init_bpr_logick(void);

    void save_state_machine(void);
    void save_service_data(void);
    void reset_failed_booting(void);
    void set_failed_booting(void);

    bool is_rtc_init(void);
    void set_rtc_was_init(void);

    void set_runRX(void);
    bool get_runRX(void);

    void set_bpr_version(uint16_t v);
    uint16_t get_bpr_version(void);

    void add_new_main_temperature(float new_main_temp);
    void add_new_top_temperature(float new_top_temp);
    void add_new_raw_voltages(uint16_t bat, uint16_t bat_div_2);
    void increase_measurementc_cnt(void);

    void set_time_since_last_sending(uint16_t seconds);
    float get_average_temperature(void);
    void clear_all_sum_and_measurements_cnt(void);

    int64_t read_int64_from_bpr(bpr_data_type start);

    void read_several_from_bpr(bpr_data_type start, uint8_t size);
    void write_several_to_bpr(bpr_data_type start, uint8_t size, void *value);

    int64_t read_int64_from_bpr(bpr_data_type start);
    void write_int64_to_bpr(bpr_data_type start, uint64_t value);
    uint32_t read_uint32_from_bpr(bpr_data_type start);
    void write_uint32_to_bpr(bpr_data_type start, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif