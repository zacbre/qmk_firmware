#pragma once
#include "quantum.h"
#include <string.h>
#include <stdio.h>

enum packet_header {
    cpu_usage_header = 0x01,
    mem_usage_header = 0x02,
    proc_count_header = 0x03,
    set_volume_header = 0x04,
    get_volume_header = 0x05,
    force_set_volume_header = 0x06,
    gpu_utilization_header = 0x07,
    stats_header = 0x08
};

typedef struct program_volume {
    uint16_t key;
    char *value;
} program_volume;

extern char *current_application;
extern int current_volume;
extern uint16_t selected_app;

extern bool encoder_volume_mode;

void setup_program_volume_list(void);
bool check_volume_fn_key(uint16_t keycode, keyrecord_t *record);

void get_volume_packet(void);
void send_volume_packet(bool vol_up, bool force);