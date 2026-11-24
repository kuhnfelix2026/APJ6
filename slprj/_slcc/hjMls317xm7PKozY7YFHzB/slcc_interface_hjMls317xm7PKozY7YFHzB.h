#include "customcode_hjMls317xm7PKozY7YFHzB.h"
#ifdef __cplusplus
extern "C" {
#endif


/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
DLL_EXPORT_CC extern const char_T *get_dll_checksum_hjMls317xm7PKozY7YFHzB(void);
DLL_EXPORT_CC extern int32_T APJ6_Car_get_id_hjMls317xm7PKozY7YFHzB(void);
DLL_EXPORT_CC extern void APJ6_Car_initialize_hjMls317xm7PKozY7YFHzB(const char_T *ssid, const char_T *pwd);
DLL_EXPORT_CC extern void APJ6_Car_terminate_hjMls317xm7PKozY7YFHzB(void);
DLL_EXPORT_CC extern void APJ6_Car_set_pixel_hjMls317xm7PKozY7YFHzB(uint32_T c);
DLL_EXPORT_CC extern void APJ6_Car_set_power_hjMls317xm7PKozY7YFHzB(int32_T p);
DLL_EXPORT_CC extern int32_T APJ6_Car_set_power_perc_hjMls317xm7PKozY7YFHzB(int32_T p);
DLL_EXPORT_CC extern int32_T APJ6_Car_get_distance_hjMls317xm7PKozY7YFHzB(void);
DLL_EXPORT_CC extern uint32_T APJ6_Car_get_distance_signal_rate_hjMls317xm7PKozY7YFHzB(void);
DLL_EXPORT_CC extern real32_T APJ6_Car_get_speed_hjMls317xm7PKozY7YFHzB(void);
DLL_EXPORT_CC extern uint32_T APJ6_Car_get_odometer_hjMls317xm7PKozY7YFHzB(void);
DLL_EXPORT_CC extern int32_T APJ6_Car_get_horizontal_acc_hjMls317xm7PKozY7YFHzB(void);
DLL_EXPORT_CC extern int32_T APJ6_Car_get_vertical_gyro_hjMls317xm7PKozY7YFHzB(void);
DLL_EXPORT_CC extern real32_T APJ6_Car_get_power_voltage_hjMls317xm7PKozY7YFHzB(void);
DLL_EXPORT_CC extern real32_T APJ6_Car_get_batt_voltage_hjMls317xm7PKozY7YFHzB(void);
DLL_EXPORT_CC extern int32_T APJ6_Car_get_wire_timeouts_hjMls317xm7PKozY7YFHzB(void);
DLL_EXPORT_CC extern uint32_T APJ6_Car_get_time_hjMls317xm7PKozY7YFHzB(void);
DLL_EXPORT_CC extern void APJ6_Car_configure_hjMls317xm7PKozY7YFHzB(uint32_T i2c_clock, int32_T speed_mode);
DLL_EXPORT_CC extern int32_T APJ6_Car_configure_vl53l1_hjMls317xm7PKozY7YFHzB(uint16_T center, uint16_T x, uint16_T y);
DLL_EXPORT_CC extern void APJ6_Car_send_signals_hjMls317xm7PKozY7YFHzB(real32_T a, real32_T b, real32_T c, real32_T d, real32_T e);
DLL_EXPORT_CC extern void APJ6_Car_receive_signals_hjMls317xm7PKozY7YFHzB(real32_T *set_speed, real32_T *set_distance, real32_T *a, real32_T *b, real32_T *c, real32_T *d, real32_T *e, int32_T *reset);
DLL_EXPORT_CC extern void APJ6_Car_times_hjMls317xm7PKozY7YFHzB(uint32_T *times);

/* Function Definitions */
DLL_EXPORT_CC const uint8_T *get_checksum_source_info(int32_T *size);
#ifdef __cplusplus
}
#endif

