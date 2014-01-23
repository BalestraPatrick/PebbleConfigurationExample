/*
 * Patrick Balestra
 */

#include "pebble.h"

static Window *window;
static TextLayer *text_layer;

static char settingText[2];
static int settingValue;
static bool firstRequest;

static AppSync sync;
static uint8_t sync_buffer[128];

enum SettingsKeys {
    TEST_KEY = 0x0
};

static void sync_tuple_changed_callback(const uint32_t key, const Tuple *new_tuple, const Tuple *old_tuple, void *context) {
    switch (key) {
        case TEST_KEY:
        
        if (!firstRequest) {
            firstRequest = true;
        } else {
            settingValue = new_tuple->value->int8;
            snprintf(settingText, sizeof(settingText), "%d", settingValue);
            text_layer_set_text(text_layer, settingText);
            persist_write_int(TEST_KEY, settingValue);
            APP_LOG(APP_LOG_LEVEL_DEBUG, "Saved new value to watch = %s", settingText);
        }
        break;
    }
}

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void send_cmd(void) {
    Tuplet value = TupletInteger(1, 1);
    
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    
    if (iter == NULL) {
        return;
    }
    
    dict_write_tuplet(iter, &value);
    dict_write_end(iter);
    
    app_message_outbox_send();
}

static void init(void) {
    
    window = window_create();
    window_stack_push(window, true);
    window_set_background_color(window, GColorWhite);
    
    settingValue = persist_exists(TEST_KEY) ? persist_read_int(TEST_KEY) : 0;
    
    snprintf(settingText, 3, "%d", settingValue);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Loaded value = %s", settingText);
    
    text_layer = text_layer_create(GRect(0, 59, 144, 74));
    text_layer_set_text_color(text_layer, GColorBlack);
    text_layer_set_background_color(text_layer, GColorClear);
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
    text_layer_set_text(text_layer, settingText);
    
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
    
    Tuplet initial_values[] = {
        TupletInteger(TEST_KEY, 0)
    };
    
    app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
                  sync_tuple_changed_callback, sync_error_callback, NULL);
    
    send_cmd();
    
    app_message_open(128, 128);
    
}

static void deinit(void) {
    text_layer_destroy(text_layer);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}