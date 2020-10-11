#include "stm32f2xx_gpio.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "semphr.h"
#include "event_groups.h"
#include "stm32_uart.h"
#include "stm32_spi.h"
#include "report.h"
#include "ad770x.h"
#include "touch.h"
#include "GUI.h"
#include "WM.h"
#include "stepmotor.h"
#include "delay.h"
#include "w25xxx.h"
#include "ff.h"
#include "diskio.h"
#include "main.h"
#include "sysconf.h"
#include "beep.h"
#include "ds18b20.h"
#include "data.h"
#include "test.h"
#include "rtc.h"

struct ui_msg g_ui_msg;

static struct tprinter g_printer;
static TaskHandle_t handle_touch, handle_gui;

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

extern int main_menu_creat(void);
extern int ui_check_creat(void);
extern int ui_test_creat(void);
extern int ui_setting_creat(void);
extern int ui_data_creat(void);
extern int ui_detail_creat(void);

extern void lcd_io_init(void);

extern const GUI_BITMAP bmlogo;
extern const GUI_FONT GUI_FontHZ_yahei_20;
extern const GUI_FONT GUI_FontHZ_yahei_16;

static void task_touch(void *args)
{
	while (1) {
		touch_update();
		vTaskDelay(10);
	}
}

static void task_usb(void *args)
{
	while (1) {
		USBH_Process(&USB_OTG_Core, &USB_Host);
		vTaskDelay(10);
	}
}

static void task_ui(void *args)
{
	g_ui_msg.msg = UI_MSG_LOAD_MENU;
	while (1) {
		switch (g_ui_msg.msg) {
		case UI_MSG_LOAD_MENU:
			main_menu_creat();
			break;
		case UI_MSG_LOAD_CHECK:
			ui_check_creat();
			break;
		case UI_MSG_LOAD_TEST:
			ui_test_creat();
			break;
		case UI_MSG_LOAD_SETTING:
			ui_setting_creat();
			break;
		case UI_MSG_LOAD_DATA:
			ui_data_creat();
			break;
		case UI_MSG_LOAD_DETAIL:
			ui_detail_creat();
			break;
		case UI_MSG_LOAD_TOUCH_CALC:
			vTaskSuspend(handle_touch);
			touch_calibrate(1);
			vTaskResume(handle_touch);
			ui_setting_creat();
			break;
		default:
			break;
		}
	}
}

static void task_init(void)
{
	xTaskCreate(task_touch, "touch", 128, NULL, 2, &handle_touch);
	xTaskCreate(task_ui, "ui", 512, NULL, 1, &handle_gui);
	xTaskCreate(task_usb, "usb", 256, NULL, 1, NULL);
}

static int g_printer_send(uint8_t *buf, int len)
{
	return uart_send_buf(PRINTER_PORT, buf, len);
}

int main(void)
{
    //int status = 0;
	/* disable global interrupt, it will be opened by prvStartFirstTask int port.c */
	//__set_PRIMASK(1);
	/* enable CRC, for stemwin */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	/* rtos will not run if I set group_2, group_4 is work */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	/* console and uart init, depent on config.h */
	uart1_init();
	uart4_init();
	printf("System Init!\r\n");
	printf("CoreClock = %dMHz\r\n", SystemCoreClock / 1000000);
	rtc_init();
	lcd_io_init();
	GUI_Init();

	/* show logo */
	GUI_SetBkColor(GUI_WHITE);
	GUI_Clear();
	GUI_DrawBitmap(&bmlogo, 315, 150);
	GUI_SetFont(&GUI_FontHZ_yahei_20);
	GUI_SetColor(GUI_BLACK_33);
	GUI_DispStringHCenterAt("全自动氨离子分析仪", 400, 60);
	GUI_SetFont(&GUI_FontHZ_yahei_20);
	GUI_DispStringHCenterAt("北京同创中仪科技为您服务", 400, 350);
	delay_ms(1500);
	
	spi1_init();
	spi2_init();
	spi3_init();
	beep_init();
	w25xxx_init();
	//w25xxx_erase_chip();
	
	/* load from flash */
	sysconf_load();
	data_init();
	test_init();

	touch_init();
	touch_calibrate(0);

	ad770x_init();

	g_printer.name = "simple printer";
    g_printer.send = g_printer_send;
	report_init(&g_printer);

	/* creat freertos task */
	task_init();

	/* run rtos */
	vTaskStartScheduler();

	/* if run here, system error */
	while (1) {
		printf("system error!\r\n");
        delay_ms(2000);
	}
	return 0;
}


