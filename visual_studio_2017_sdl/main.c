/**
* @file main
*
*/

/*********************
*      INCLUDES
*********************/
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <SDL.h>
#include "lvgl/lvgl.h"
#include "lv_drivers/display/monitor.h"
#include "lv_drivers/indev/mouse.h"
#include "lv_drivers/indev/keyboard.h"
#include "lv_examples/lv_examples.h"

#include "Res/ResDeclare.h"

/*********************
*      DEFINES
*********************/

/**********************
*      TYPEDEFS
**********************/

/**********************
*  STATIC PROTOTYPES
**********************/
static void hal_init(void);
static int tick_thread(void *data);

/**********************
*  STATIC VARIABLES
**********************/
static lv_indev_t * kb_indev;

/**********************
*      MACROS
**********************/

/**********************
*   GLOBAL FUNCTIONS
**********************/
void WifiPassword();
void ShowSoftwarePswd();

void SystemMngEventHandler(lv_obj_t * obj, lv_event_t event);
void SoftwarePswdEventHandler(lv_obj_t * obj, lv_event_t event);


lv_obj_t* img_Set_SysMng;
lv_obj_t* btn_Cancel_SoftwarePwd;
lv_obj_t* area_SoftPswd;


int main(int argc, char** argv)
{
	/*Initialize LittlevGL*/
	lv_init();

	/*Initialize the HAL for LittlevGL*/
	hal_init();

	/*
	 * Demos, benchmarks, and tests.
	 *
	 * Uncomment any one (and only one) of the functions below to run that
	 * item.
	 */

	WifiPassword();

	 //lv_demo_widgets();
	 //lv_demo_benchmark();
	 //lv_demo_keypad_encoder();
	 //lv_demo_printer(); 
	 //lv_demo_stress();
	 //lv_ex_get_started_1();
	 //lv_ex_get_started_2();
	 //lv_ex_get_started_3();    //  volume bar.
	 //lv_ex_style_1();
	 //lv_ex_style_2();
	 //lv_ex_style_3();
	 //lv_ex_style_4();
	 //lv_ex_style_5();
	 //lv_ex_style_6();
	 //lv_ex_style_7();
	 //lv_ex_style_8();
	 //lv_ex_style_9();
	 //lv_ex_style_10();
	 //lv_ex_style_11();

	 /*
	  * There are many examples of individual widgets found under the
	  * lv_examples/src/lv_ex_widgets directory.  Here are a few sample test
	  * functions.  Look in that directory to find all the rest.
	  */
	  //lv_ex_arc_1();
	  //lv_ex_cpicker_1();
	  //lv_ex_gauge_1();
	  //lv_ex_img_1();
	  //lv_ex_tileview_1();

	while (1) {
		/* Periodically call the lv_task handler.
		* It could be done in a timer interrupt or an OS task too.*/
		lv_task_handler();
		Sleep(10);       /*Just to let the system breathe */
	}

	return 0;
}

/**********************
*   STATIC FUNCTIONS
**********************/


/**
* Initialize the Hardware Abstraction Layer (HAL) for the Littlev graphics library
*/
static void hal_init(void)
{
	/* Add a display
	* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
	monitor_init();

	static lv_disp_buf_t disp_buf1;
	static lv_color_t buf1_1[LV_HOR_RES_MAX * 120];
	lv_disp_buf_init(&disp_buf1, buf1_1, NULL, LV_HOR_RES_MAX * 120);

	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
	disp_drv.buffer = &disp_buf1;
	disp_drv.flush_cb = monitor_flush;
	lv_disp_drv_register(&disp_drv);

	/* Add the mouse (or touchpad) as input device
	* Use the 'mouse' driver which reads the PC's mouse*/
	mouse_init();
	lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = mouse_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
	lv_indev_drv_register(&indev_drv);

	/* If the PC keyboard driver is enabled in`lv_drv_conf.h`
	* add this as an input device. It might be used in some examples. */
#if USE_KEYBOARD
	lv_indev_drv_t kb_drv;
	lv_indev_drv_init(&kb_drv);
	kb_drv.type = LV_INDEV_TYPE_KEYPAD;
	kb_drv.read_cb = keyboard_read;
	kb_indev = lv_indev_drv_register(&kb_drv);
#endif

	/* Tick init.
	* You have to call 'lv_tick_inc()' in every milliseconds
	* Create an SDL thread to do this*/
	SDL_CreateThread(tick_thread, "tick", NULL);
}

/**
* A task to measure the elapsed time for LittlevGL
* @param data unused
* @return never return
*/
static int tick_thread(void *data)
{
	while (1) {
		lv_tick_inc(5);
		SDL_Delay(5);   /*Sleep for 1 millisecond*/
	}

	return 0;
}


void WifiPassword()
{
	const char* szItemsStr[] = { "Line1","Line2", "Line3", "Line4", "Line5" };

	//Show background
	img_Set_SysMng = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img_Set_SysMng, &UI_Background);

	/*System Manager list*/
	lv_obj_t* pSysMngList = lv_list_create(img_Set_SysMng, NULL);
	lv_obj_set_size(pSysMngList, 320, 380);
	lv_obj_align(pSysMngList, img_Set_SysMng, LV_ALIGN_IN_TOP_LEFT, 0, 100);

	lv_obj_t* btn;
	for (int n = 0; n < (sizeof(szItemsStr) / sizeof(const char*)); n++)
	{
		btn = lv_list_add_btn(pSysMngList, NULL, szItemsStr[n]);
		lv_obj_set_event_cb(btn, SystemMngEventHandler);
	}
}



void ShowSoftwarePswd()
{
	static lv_style_t MsgboxStyle;
	lv_style_init(&MsgboxStyle);
	lv_style_set_bg_color(&MsgboxStyle, LV_STATE_DEFAULT, lv_color_hex(0xefefef));
	lv_style_set_border_width(&MsgboxStyle, LV_STATE_DEFAULT, 2);
	lv_style_set_border_color(&MsgboxStyle, LV_STATE_DEFAULT, lv_color_hex(0x959595));
	lv_style_set_border_side(&MsgboxStyle, LV_STATE_DEFAULT, LV_BORDER_SIDE_FULL);

	// static lv_style_t LineStyle;
	// lv_style_init(&LineStyle);
	// lv_style_set_line_color(&LineStyle, LV_STATE_DEFAULT, lv_color_hex(0x959595));
	// lv_style_set_line_width(&LineStyle, LV_STATE_DEFAULT, 1);
	// static lv_point_t p[] = {
	//     {20, 50},
	//     {280, 50}
	// };

	//Container
	lv_obj_t* cont_SoftPswd = lv_cont_create(lv_layer_top(), NULL);
	lv_obj_set_click(cont_SoftPswd, FALSE);
	lv_obj_set_size(cont_SoftPswd, 300, 200);
	lv_obj_add_style(cont_SoftPswd, LV_OBJ_PART_MAIN, &MsgboxStyle);
	lv_obj_align(cont_SoftPswd, NULL, LV_ALIGN_CENTER, -80, 10);

	// //Title
	// lv_obj_t* title = lv_label_create(cont_SoftPswd, NULL);
	// lv_label_set_text(title, g_stLang.stTitle.szSoftMng);
	// lv_obj_align(title, cont_SoftPswd, LV_ALIGN_IN_TOP_MID, 0, 20);
	// lv_obj_t* pLine = lv_line_create(cont_SoftPswd, NULL);
	// lv_obj_add_style(pLine, LV_LINE_PART_MAIN, &LineStyle);
	// lv_line_set_points(pLine, p, 2);

	// lv_obj_t* PWD = lv_label_create(cont_SoftPswd, NULL);
	// lv_label_set_text(PWD, "Password");
	// lv_obj_align(PWD, cont_SoftPswd, LV_ALIGN_IN_TOP_LEFT, 20, 65);

	 area_SoftPswd = lv_textarea_create(cont_SoftPswd, NULL);
	 lv_textarea_set_pwd_mode(area_SoftPswd, TRUE);
	 lv_textarea_set_pwd_show_time(area_SoftPswd, 400);
	 lv_textarea_set_text(area_SoftPswd, "1234567890");
	 lv_textarea_set_one_line(area_SoftPswd, true);
	 lv_textarea_set_cursor_hidden(area_SoftPswd, false);
	 lv_page_focus(area_SoftPswd, lv_textarea_get_label(area_SoftPswd), LV_ANIM_OFF);
	 lv_textarea_set_cursor_blink_time(area_SoftPswd, 250);
	 //lv_textarea_set_sscrollbar_mode(area_SoftPswd, LV_SCROLLBAR_MODE_OFF);
	 lv_obj_align(area_SoftPswd, cont_SoftPswd, LV_ALIGN_IN_TOP_LEFT, 20, 95);
	 lv_obj_set_event_cb(area_SoftPswd, SoftwarePswdEventHandler);

	// btn_Confirm_SoftwarePwd = lv_btn_create(cont_SoftPswd, NULL);
	// lv_btn_set_layout(btn_Confirm_SoftwarePwd, LV_LAYOUT_OFF);
	// lv_obj_set_size(btn_Confirm_SoftwarePwd, 100, 35);
	// lv_obj_align(btn_Confirm_SoftwarePwd, cont_SoftPswd, LV_ALIGN_IN_BOTTOM_MID, -70, -20);
	// lv_obj_set_event_cb(btn_Confirm_SoftwarePwd, SoftwarePswdEventHandler);
	// SetNormalBtnStyle(btn_Confirm_SoftwarePwd, GREEN);

	btn_Cancel_SoftwarePwd = lv_btn_create(cont_SoftPswd, NULL);
	lv_btn_set_layout(btn_Cancel_SoftwarePwd, LV_LAYOUT_OFF);
	lv_obj_set_size(btn_Cancel_SoftwarePwd, 100, 35);
	lv_obj_align(btn_Cancel_SoftwarePwd, cont_SoftPswd, LV_ALIGN_IN_BOTTOM_MID, 70, -20);
	lv_obj_set_event_cb(btn_Cancel_SoftwarePwd, SoftwarePswdEventHandler);

	// lv_obj_t* LTemp = lv_label_create(btn_Confirm_SoftwarePwd, NULL);
	// lv_label_set_text(LTemp, g_stLang.stBtn.szValidate);
	// lv_obj_align(LTemp, btn_Confirm_SoftwarePwd, LV_ALIGN_CENTER, 0, 0);

	// LTemp = lv_label_create(btn_Cancel_SoftwarePwd, NULL);
	// lv_label_set_text(LTemp, g_stLang.stBtn.szCancel);
	// lv_obj_align(LTemp, btn_Cancel_SoftwarePwd, LV_ALIGN_CENTER, 0, 0);

	//Focus the cusor to password area.
	// g_CurArea = area_SoftPswd;
}


void SystemMngEventHandler(lv_obj_t * obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED)
	{
		printf("SysMng item:%s  \n", lv_list_get_btn_text(obj));

		if (!strcmp(lv_list_get_btn_text(obj), "Line1"))
		{
		}
		else if (!strcmp(lv_list_get_btn_text(obj), "Line2"))
		{
		}
		else if (!strcmp(lv_list_get_btn_text(obj), "Line3"))
		{
		}
		else if (!strcmp(lv_list_get_btn_text(obj), "Line4"))
		{
			ShowSoftwarePswd();
		}
	}
}

void SoftwarePswdEventHandler(lv_obj_t * obj, lv_event_t event)
{
	if (obj == btn_Cancel_SoftwarePwd && event == LV_EVENT_PRESSED)
	{
		lv_obj_del(lv_obj_get_parent(obj));

		if (lv_debug_check_obj_valid(img_Set_SysMng))
		{
			//DEBUG_INFO(DEBUGINFO_NORMAL, "Delete Set SysMng.");
			lv_obj_del(img_Set_SysMng);
		}
		WifiPassword();
	}
}