#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int APJ6_Car_get_id();

void APJ6_Car_initialize(const char *ssid, const char *pwd);

void APJ6_Car_terminate();

void APJ6_Car_set_pixel(unsigned int c);

void APJ6_Car_set_power(int p);
int APJ6_Car_set_power_perc(int p);

int APJ6_Car_get_distance();

unsigned int APJ6_Car_get_distance_signal_rate();

float APJ6_Car_get_speed();

unsigned int APJ6_Car_get_odometer();

int APJ6_Car_get_horizontal_acc();

int APJ6_Car_get_vertical_gyro();

float APJ6_Car_get_power_voltage();

float APJ6_Car_get_batt_voltage();

int APJ6_Car_get_wire_timeouts();

unsigned long APJ6_Car_get_time();

void APJ6_Car_configure(unsigned long i2c_clock, int speed_mode);

int APJ6_Car_configure_vl53l1(unsigned short center, unsigned short x, unsigned short y);

// data communication
void APJ6_Car_send_signals(float a, float b, float c, float d, float e);

void APJ6_Car_receive_signals(float *set_speed, float *set_distance, float *a, float *b, float *c, float *d, float *e, int *reset);

void APJ6_Car_times(unsigned long *times);

#ifdef __cplusplus
}
#endif
