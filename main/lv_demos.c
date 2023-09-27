/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "bsp/esp-bsp.h"

#include "demos/lv_demos.h"

void slider_create(void);

void app_main(void)
{
    /* Initialize I2C (for touch and audio) */
    bsp_i2c_init();

    /* Initialize display and LVGL */
    bsp_display_start();

    /* Set display brightness to 100% */
    bsp_display_backlight_on();

    /**
     * @brief Demos provided by LVGL.
     *
     * @note Only enable one demo every time.
     *
     */
#if LV_USE_DEMO_WIDGETS
    //lv_demo_widgets();      /* A widgets example. This is what you get out of the box */
#endif

#if LV_USE_DEMO_KEYPAD_AND_ENCODER
    lv_demo_keypad_encoder();   /* Demonstrate the usage of encoder and keyboard */
#endif

#if LV_USE_DEMO_BENCHMARK
    lv_demo_benchmark();    /* A demo to measure the performance of LVGL or to compare different settings. */
#endif

#if LV_USE_DEMO_STRESS
    lv_demo_stress();       /* A stress test for LVGL. */
#endif

#if LV_USE_DEMO_MUSIC
    lv_demo_music();        /* A modern, smartphone-like music player demo. */
#endif

    // slider_create();

    #include "ui_sample/ui.h"
    ui_init();

}


/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define HEADING_Y_POS                       20
#define SLIDE_START_Y_POS                   50

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
// Sliders
lv_obj_t* p_slider_r = NULL;
lv_obj_t* p_slider_g = NULL;
lv_obj_t* p_slider_b = NULL;
lv_obj_t* p_color_mixer = NULL;

// Labels
lv_obj_t* p_rgb_r_label = NULL;
lv_obj_t* p_rgb_g_label = NULL;
lv_obj_t* p_rgb_b_label = NULL;
/******************************************************************************
* Function Prototypes
*******************************************************************************/
void slider_evt_cb(struct _lv_event_t* e);
/******************************************************************************
* Function Definitions
*******************************************************************************/
void slider_create(void)
{
    lv_obj_t* p_act_screen = lv_scr_act();

    // Creating sliders
    p_slider_r = lv_slider_create(lv_scr_act());
    lv_obj_align_to(p_slider_r, lv_scr_act(), LV_ALIGN_TOP_MID, 0, SLIDE_START_Y_POS);
    lv_obj_set_style_bg_color(p_slider_r, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(p_slider_r, lv_palette_main(LV_PALETTE_RED), LV_PART_KNOB);
    lv_slider_set_range(p_slider_r, 0, 255);

    p_slider_g = lv_slider_create(lv_scr_act());
    lv_obj_align_to(p_slider_g, p_slider_r, LV_ALIGN_TOP_MID, 0, 50);
    lv_obj_set_style_bg_color(p_slider_g, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(p_slider_g, lv_palette_main(LV_PALETTE_GREEN), LV_PART_KNOB);
    lv_slider_set_range(p_slider_g, 0, 255);

    p_slider_b = lv_slider_create(lv_scr_act());
    lv_obj_align_to(p_slider_b, p_slider_g, LV_ALIGN_TOP_MID, 0, 50);
    lv_obj_set_style_bg_color(p_slider_b, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(p_slider_b, lv_palette_main(LV_PALETTE_BLUE), LV_PART_KNOB);
    lv_slider_set_range(p_slider_b, 0, 255);

    // Create RGB rectangle mixer object
    p_color_mixer = lv_obj_create(lv_scr_act());
    lv_obj_set_size(p_color_mixer, lv_obj_get_width(lv_scr_act()) - 20, 50);
    lv_obj_align(p_color_mixer, LV_ALIGN_BOTTOM_MID, 0, -15);
    lv_obj_set_style_border_width(p_color_mixer, 5, LV_PART_MAIN);

    // Creating labels

    lv_obj_t* p_heading = lv_label_create(lv_scr_act());
    lv_label_set_text(p_heading, "RGB MIXER");
    lv_obj_align(p_heading, LV_ALIGN_TOP_MID, 0, HEADING_Y_POS);

    p_rgb_r_label = lv_label_create(lv_scr_act());
    lv_label_set_text(p_rgb_r_label, "0");
    lv_obj_align_to(p_rgb_r_label, p_slider_r, LV_ALIGN_OUT_TOP_MID, 0, 0);

    p_rgb_g_label = lv_label_create(lv_scr_act());
    lv_label_set_text(p_rgb_g_label, "0");
    lv_obj_align_to(p_rgb_g_label, p_slider_g, LV_ALIGN_OUT_TOP_MID, 0, 0);

    p_rgb_b_label = lv_label_create(lv_scr_act());
    lv_label_set_text(p_rgb_b_label, "0");
    lv_obj_align_to(p_rgb_b_label, p_slider_b, LV_ALIGN_OUT_TOP_MID, 0, 0);

    // Set callbacks
    lv_obj_add_event_cb(p_slider_r, &slider_evt_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(p_slider_g, &slider_evt_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(p_slider_b, &slider_evt_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

void slider_evt_cb(struct _lv_event_t* e)
{
    lv_obj_t* event_source = lv_event_get_target(e);
    static uint32_t r = 0, g = 0, b = 0;
    if (event_source == p_slider_r)
    {
        r = lv_slider_get_value(event_source);
        lv_label_set_text_fmt(p_rgb_r_label, "%ld", r);
    }
    else if (event_source == p_slider_g)
    {
        g = lv_slider_get_value(event_source);
        lv_label_set_text_fmt(p_rgb_g_label, "%ld", g);
    }
    else if (event_source == p_slider_b)
    {
        b = lv_slider_get_value(event_source);
        lv_label_set_text_fmt(p_rgb_b_label, "%ld", b);
    }
    else
    {

    }
    lv_obj_set_style_bg_color(p_color_mixer, lv_color_make(r, g, b), LV_PART_MAIN);
}

//C:\Users\VHT-Student\Itachi\projects\sample\lv_sim_visual_studio\LvglWindowsSimulator\rgb_mixer\rgb_mixer.c