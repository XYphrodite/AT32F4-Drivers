#include "ClockCfg.h"

void ClockCfg::SetSpeed(MCU_speed s)
{
    /* reset crm */
    crm_reset();
    /* enable hext */
    crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);
    /* wait till hext is ready */
    while (crm_hext_stable_wait() == ERROR)
        ;

    switch (s)
    {
    case _240Mhz:
        /* config pll clock resource */
        crm_pll_config(CRM_PLL_SOURCE_HEXT, CRM_PLL_MULT_20, CRM_PLL_OUTPUT_RANGE_GT72MHZ);

        /* enable pll */
        crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

        /* wait till pll is ready */
        while (crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
            ;

        /* config ahbclk */
        crm_ahb_div_set(CRM_AHB_DIV_1);

        /* config apb2clk */
        crm_apb2_div_set(CRM_APB2_DIV_2);

        /* config apb1clk */
        crm_apb1_div_set(CRM_APB1_DIV_2);

        /* enable auto step mode */
        crm_auto_step_mode_enable(TRUE);
        /* select pll as system clock source */
        crm_sysclk_switch(CRM_SCLK_PLL);

        /* wait till pll is used as system clock source */
        while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
            ;
        /* disable auto step mode */
        crm_auto_step_mode_enable(FALSE);
        cpu_speed = 240000000;
        break;
    case _24Mhz:
        /* config pll clock resource */
        crm_pll_config(CRM_PLL_SOURCE_HEXT, CRM_PLL_MULT_2, CRM_PLL_OUTPUT_RANGE_LE72MHZ);

        /* enable pll */
        crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

        /* wait till pll is ready */
        while (crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
        {
        }

        /* config ahbclk */
        crm_ahb_div_set(CRM_AHB_DIV_1);

        /* config apb2clk */
        crm_apb2_div_set(CRM_APB2_DIV_2);

        /* config apb1clk */
        crm_apb1_div_set(CRM_APB1_DIV_2);

        /* select pll as system clock source */
        crm_sysclk_switch(CRM_SCLK_PLL);

        /* wait till pll is used as system clock source */
        while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
            ;
        cpu_speed = 24000000;
        break;
    case _8Mhz:
        /* config pll clock resource */
        crm_pll_config(CRM_PLL_SOURCE_HEXT_DIV, CRM_PLL_MULT_2, CRM_PLL_OUTPUT_RANGE_LE72MHZ);

        /* config hext division */
        crm_hext_clock_div_set(CRM_HEXT_DIV_3);

        /* enable pll */
        crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

        /* wait till pll is ready */
        while (crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
            ;

        /* config ahbclk */
        crm_ahb_div_set(CRM_AHB_DIV_1);

        /* config apb2clk */
        crm_apb2_div_set(CRM_APB2_DIV_2);

        /* config apb1clk */
        crm_apb1_div_set(CRM_APB1_DIV_1);

        /* select pll as system clock source */
        crm_sysclk_switch(CRM_SCLK_PLL);

        /* wait till pll is used as system clock source */
        while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
            ;
        cpu_speed = 8000000;
        break;
    case _4dot8Mhz:
        /* config pll clock resource */
        crm_pll_config(CRM_PLL_SOURCE_HEXT_DIV, CRM_PLL_MULT_2, CRM_PLL_OUTPUT_RANGE_LE72MHZ);

        /* config hext division */
        crm_hext_clock_div_set(CRM_HEXT_DIV_5);

        /* enable pll */
        crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

        /* wait till pll is ready */
        while (crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
            ;

        /* config ahbclk */
        crm_ahb_div_set(CRM_AHB_DIV_1);

        /* config apb2clk */
        crm_apb2_div_set(CRM_APB2_DIV_2);

        /* config apb1clk */
        crm_apb1_div_set(CRM_APB1_DIV_2);

        /* select pll as system clock source */
        crm_sysclk_switch(CRM_SCLK_PLL);

        /* wait till pll is used as system clock source */
        while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
            ;
        cpu_speed = 4800000;
        break;
    case _16Mhz:
        /* config pll clock resource */
        crm_pll_config(CRM_PLL_SOURCE_HEXT_DIV, CRM_PLL_MULT_4, CRM_PLL_OUTPUT_RANGE_LE72MHZ);

        /* config hext division */
        crm_hext_clock_div_set(CRM_HEXT_DIV_3);

        /* enable pll */
        crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

        /* wait till pll is ready */
        while (crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
            ;

        /* config ahbclk */
        crm_ahb_div_set(CRM_AHB_DIV_1);

        /* config apb2clk */
        crm_apb2_div_set(CRM_APB2_DIV_2);

        /* config apb1clk */
        crm_apb1_div_set(CRM_APB1_DIV_1);

        /* select pll as system clock source */
        crm_sysclk_switch(CRM_SCLK_PLL);

        /* wait till pll is used as system clock source */
        while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
            ;
        cpu_speed = 16000000;
        break;
    case _12Mhz:
        /* config pll clock resource */
        crm_pll_config(CRM_PLL_SOURCE_HEXT_DIV, CRM_PLL_MULT_3, CRM_PLL_OUTPUT_RANGE_LE72MHZ);

        /* config hext division */
        crm_hext_clock_div_set(CRM_HEXT_DIV_3);

        /* enable pll */
        crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

        /* wait till pll is ready */
        while (crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
            ;

        /* config ahbclk */
        crm_ahb_div_set(CRM_AHB_DIV_1);

        /* config apb2clk */
        crm_apb2_div_set(CRM_APB2_DIV_2);

        /* config apb1clk */
        crm_apb1_div_set(CRM_APB1_DIV_1);

        /* select pll as system clock source */
        crm_sysclk_switch(CRM_SCLK_PLL);

        /* wait till pll is used as system clock source */
        while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
            ;
        cpu_speed = 12000000;
        break;
    case _9dot6Mhz:
        /* config pll clock resource */
        crm_pll_config(CRM_PLL_SOURCE_HEXT_DIV, CRM_PLL_MULT_4, CRM_PLL_OUTPUT_RANGE_LE72MHZ);

        /* config hext division */
        crm_hext_clock_div_set(CRM_HEXT_DIV_5);

        /* enable pll */
        crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

        /* wait till pll is ready */
        while (crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
            ;

        /* config ahbclk */
        crm_ahb_div_set(CRM_AHB_DIV_1);

        /* config apb2clk */
        crm_apb2_div_set(CRM_APB2_DIV_2);

        /* config apb1clk */
        crm_apb1_div_set(CRM_APB1_DIV_1);

        /* select pll as system clock source */
        crm_sysclk_switch(CRM_SCLK_PLL);

        /* wait till pll is used as system clock source */
        while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
            ;
        cpu_speed = 9600000;
        break;
    default:
        break;
    }

    /* update system_core_clock global variable */
    system_core_clock_update();
}
