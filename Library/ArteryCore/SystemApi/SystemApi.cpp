#include "SystemApi.h"
#include "at32f403a_407.h"

void SystemApi::Init(nvic_priority_group_type priority_group, MCU_speed clock,
                     bool swd_over_jtag, uint32_t vector_table_offset) {
  nvic_priority_group_config(priority_group);
  ClockCfg::SetSpeed(clock);
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

  if (swd_over_jtag) {
    /* jtag-dp disabled and sw-dp enabled */
    gpio_pin_remap_config(SWJTAG_GMUX_010, TRUE); // enable PB3
  }

  // set nvic
  SCB->VTOR = reinterpret_cast<uint32_t>(&vector_table_offset);

  __enable_irq();
}

error_status SystemApi::InitServices(const SystemServices_t &services) {
  if (services.UpTimeTmr) {
    init_UpTime_Tmr();
  }
  if (services.FlashService) {
    FlashService::Init();
  }
  if (services.UnitReset) {
    init_rst_units();
  }
  if (services.UartService) {
    if (services.init_config != nullptr) {
      UartService::Init(services.init_config);
    } else {
      return ERROR;
    }
  }
  if (services.WatchdogTimer) {
    watchdog_init();
  }
  if (services.BootloaderConfig) {
    BootloadConfig::Init();
  }
  return SUCCESS;
}