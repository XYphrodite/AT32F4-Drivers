#include "BprDriver.h"

static uint16_t bpr_buffer[8];

const bpr_data_type Version_bpr = BPR_DATA1;

const bpr_data_type State_machine_bpr = BPR_DATA2;
struct bpr_state_machine_t state_machine;

const bpr_data_type time_since_last_uplink_s_bpr = BPR_DATA3;
uint16_t time_since_last_uplink_s = 0;

const bpr_data_type service_data_bpr = BPR_DATA4;
bpr_service_data_t service_data{0, 0, 0};

/*free--------------*/

const bpr_data_type service_errors_bpr = BPR_DATA29;
bpr_service_error_f service_errors;

const bpr_data_type voltages_raw_div_2_sum_1_halfword_bpr = BPR_DATA30;
const bpr_data_type voltages_raw_div_2_sum_2_halfword_bpr = BPR_DATA31;
uint32_t voltages_raw_div_2_sum = 0;

const bpr_data_type voltages_raw_all_sum_1_halfword_bpr = BPR_DATA32;
const bpr_data_type voltages_raw_all_sum_2_halfword_bpr = BPR_DATA33;
uint32_t voltages_raw_all_sum = 0;

const bpr_data_type measurements_amount_bpr = BPR_DATA34;
uint16_t measurements_amount = 0;

const bpr_data_type top_temperatures_sum_1_halfword_bpr = BPR_DATA35;
const bpr_data_type top_temperatures_sum_2_halfword_bpr = BPR_DATA36;
const bpr_data_type top_temperatures_sum_3_halfword_bpr = BPR_DATA37;
const bpr_data_type top_temperatures_sum_4_halfword_bpr = BPR_DATA38;
int64_t top_temperatures_sum = 0;

const bpr_data_type main_temperatures_sum_1_halfword_bpr = BPR_DATA39;
const bpr_data_type main_temperatures_sum_2_halfword_bpr = BPR_DATA40;
const bpr_data_type main_temperatures_sum_3_halfword_bpr = BPR_DATA41;
const bpr_data_type main_temperatures_sum_4_halfword_bpr = BPR_DATA42;
int64_t main_temperatures_sum = 0;

void init_bpr_logick(void)
{
    // Init bpr
    /* enable pwc and bpr clocks */
    crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_BPR_PERIPH_CLOCK, TRUE);

    /* enable the battery-powered domain write operations */
    pwc_battery_powered_domain_access(TRUE);

    // check version
    uint16_t version = get_bpr_version();
    if (version == BPR_CONFIG_VERSION)
    {
        // read state machine struct
        uint16_t state_machine_16 = bpr_data_read(State_machine_bpr);
        state_machine = *reinterpret_cast<bpr_state_machine_t *>(&state_machine_16);
        // read time since last uplink
        time_since_last_uplink_s = bpr_data_read(time_since_last_uplink_s_bpr);
        // read service data
        uint16_t service_data_16 = bpr_data_read(service_data_bpr);
        service_data = *reinterpret_cast<bpr_service_data_t *>(&service_data_16);

        // read service errors
        uint16_t service_errors_16 = bpr_data_read(service_errors_bpr);
        service_errors = *reinterpret_cast<bpr_service_error_f *>(&service_errors_16);
        // read voltages div 2 sum
        voltages_raw_div_2_sum = read_uint32_from_bpr(voltages_raw_div_2_sum_1_halfword_bpr);
        // read commom voltage
        voltages_raw_all_sum = read_uint32_from_bpr(voltages_raw_all_sum_1_halfword_bpr);
        // read mesurements amount
        measurements_amount = bpr_data_read(measurements_amount_bpr);
        // read top temperatures sum
        top_temperatures_sum = read_int64_from_bpr(top_temperatures_sum_1_halfword_bpr);
        // read main temperatures sum
        main_temperatures_sum = read_int64_from_bpr(main_temperatures_sum_1_halfword_bpr);
    }
    else
    {
        bpr_reset();
        set_bpr_version(BPR_CONFIG_VERSION);
    }
}

void save_state_machine(void)
{
    uint16_t state_machine_16 = *reinterpret_cast<uint16_t *>(&state_machine);
    bpr_data_write(State_machine_bpr, state_machine_16);
}

void save_service_data(void)
{
    uint16_t service_data_16 = *reinterpret_cast<uint16_t *>(&service_data);
    bpr_data_write(service_data_bpr, service_data_16);
}

void reset_failed_booting(void)
{
    service_data.failed_booting = 0;
    save_service_data();
}

void set_failed_booting(void)
{
    service_data.failed_booting += 1;
    save_service_data();
}

void add_new_main_temperature(float new_main_temp)
{
    main_temperatures_sum += (new_main_temp * 10000);
    write_int64_to_bpr(main_temperatures_sum_1_halfword_bpr, main_temperatures_sum);
}

void add_new_top_temperature(float new_top_temp)
{
    top_temperatures_sum += (new_top_temp * 10000);
    write_int64_to_bpr(top_temperatures_sum_1_halfword_bpr, main_temperatures_sum);
}

void add_new_raw_voltages(uint16_t bat, uint16_t bat_div_2)
{
    voltages_raw_all_sum += bat;
    voltages_raw_div_2_sum += bat_div_2;

    write_uint32_to_bpr(voltages_raw_all_sum_1_halfword_bpr, voltages_raw_all_sum);
    write_uint32_to_bpr(voltages_raw_div_2_sum_1_halfword_bpr, voltages_raw_div_2_sum);
}

void increase_measurementc_cnt(void)
{
    ++measurements_amount;
    bpr_data_write(measurements_amount_bpr, measurements_amount);
}

void clear_all_sum_and_measurements_cnt(void)
{
    write_uint32_to_bpr(voltages_raw_div_2_sum_1_halfword_bpr, 0);
    write_uint32_to_bpr(voltages_raw_all_sum_1_halfword_bpr, 0);

    bpr_data_write(measurements_amount_bpr, 0);

    write_int64_to_bpr(top_temperatures_sum_1_halfword_bpr, 0);
    write_int64_to_bpr(main_temperatures_sum_1_halfword_bpr, 0);
}

void set_time_since_last_sending(uint16_t seconds)
{
    time_since_last_uplink_s = seconds;
    bpr_data_write(time_since_last_uplink_s_bpr, seconds);
}

void set_bpr_version(uint16_t v)
{
    bpr_data_write(Version_bpr, v);
}

uint16_t get_bpr_version(void)
{
    return (bpr_data_read(Version_bpr));
}

void set_runRX(void)
{
    state_machine.run_rx = true;
    bpr_data_write(State_machine_bpr, *reinterpret_cast<uint16_t *>(&state_machine));
}

bool get_runRX(void)
{
    uint16_t state_machine_16 = bpr_data_read(State_machine_bpr);
    state_machine = *reinterpret_cast<bpr_state_machine_t *>(&state_machine_16);
    return state_machine.run_rx;
}

void read_several_from_bpr(bpr_data_type start, uint8_t size)
{
    for (size_t i = 0; i < size; i++)
        bpr_buffer[i] = bpr_data_read(static_cast<bpr_data_type>(start + 0x4 * i));
}

void write_several_to_bpr(bpr_data_type start, uint8_t size, void *value)
{
    uint16_t data[size];
    memcpy(data, value, size * 2);
    for (size_t i = 0; i < size; i++)
    {
        bpr_data_write(static_cast<bpr_data_type>(start + 4 * i), data[i]);
    }
}

int64_t read_int64_from_bpr(bpr_data_type start)
{
    int64_t data = 0;
    read_several_from_bpr(start, 4);
    memcpy(&data, bpr_buffer, sizeof(int64_t));
    return data;
}

void write_int64_to_bpr(bpr_data_type start, uint64_t value)
{
    write_several_to_bpr(start, 4, &value);
}

uint32_t read_uint32_from_bpr(bpr_data_type start)
{
    uint32_t data;
    read_several_from_bpr(start, 2);
    memcpy(&data, bpr_buffer, sizeof(uint32_t));
    return data;
}

void write_uint32_to_bpr(bpr_data_type start, uint32_t value)
{
    write_several_to_bpr(start, 2, &value);
}