#ifdef END_DEVICE_TEMPERATURE_SENSOR

#define B_MAJOR_VERSION (1U)
#define B_MINOR_VERSION (0U)
#define B_MICRO_VERSION (0U) // maintenance
#define B_TYPE_VERSION (release)

#define DEFAULT_H_MAJOR_VERSION (1U)
#define DEFAULT_H_MINOR_VERSION (1U)
#define DEFAULT_H_MICRO_VERSION (0U) // maintenance
#define DEFAULT_H_TYPE_VERSION (none)

#define MAJOR_VERSION (1U)
#define MINOR_VERSION (0U)
#define MICRO_VERSION (0U) //maintenance
#define TYPE_VERSION  (release)

#endif

#ifdef LITE_GATEWAY

#define B_MAJOR_VERSION (1U)
#define B_MINOR_VERSION (0U)
#define B_MICRO_VERSION (0U) // maintenance
#define B_TYPE_VERSION (beta)

#define DEFAULT_H_MAJOR_VERSION (1U)
#define DEFAULT_H_MINOR_VERSION (0U)
#define DEFAULT_H_MICRO_VERSION (0U) // maintenance
#define DEFAULT_H_TYPE_VERSION (none)

#define MAJOR_VERSION (1U)
#define MINOR_VERSION (0U)
#define MICRO_VERSION (0U) // maintenance
#define TYPE_VERSION (release)

#endif

#ifdef PIGSTORE_PBLOCK

#define B_MAJOR_VERSION (0U)
#define B_MINOR_VERSION (5U)
#define B_MICRO_VERSION (0U) // maintenance
#define B_TYPE_VERSION (alpha)

#define DEFAULT_H_MAJOR_VERSION (1U)
#define DEFAULT_H_MINOR_VERSION (0U)
#define DEFAULT_H_MICRO_VERSION (0U) // maintenance
#define DEFAULT_H_TYPE_VERSION (none)

#define MAJOR_VERSION (0U)
#define MINOR_VERSION (5U)
#define MICRO_VERSION (0U) // maintenance
#define TYPE_VERSION (alpha)

#endif



#ifdef END_DEVICE_TEMPERATURE_SENSOR
#define HW_TYPE (wireless_temperature_sensor)
#endif

#ifdef LITE_GATEWAY
#define HW_TYPE (lite_gateway_lbs)
#endif

#ifdef PIGSTORE_PBLOCK
#define HW_TYPE (pigstore_pblock)
#endif

#define BOOTLOADER_VERSION ((B_MAJOR_VERSION << 24) + (B_MINOR_VERSION << 16) + (B_MICRO_VERSION << 8) + B_TYPE_VERSION)

#define HARDWARE_VERSION ((DEFAULT_H_MAJOR_VERSION << 24) + (DEFAULT_H_MINOR_VERSION << 16) + (DEFAULT_H_MICRO_VERSION << 8) + DEFAULT_H_TYPE_VERSION)



#define MAIN_APP_VERSION ((MAJOR_VERSION << 24) + (MINOR_VERSION << 16) + (MICRO_VERSION << 8) + TYPE_VERSION)

#define WIRE_PROTOCOL_VERSION (1U)
