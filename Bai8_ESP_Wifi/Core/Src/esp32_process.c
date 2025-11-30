
#include "esp32_process.h"

uint8_t light_status = 0;

void lightProcess(){
	if(button_count[13] == 1){
		light_status = 1 - light_status;
		if(light_status == 1){
			uart_EspSendBytes((uint8_t*)"A", 1);
		} else {
			uart_EspSendBytes((uint8_t*)"a", 1);
		}
	}
	if(light_status == 1){
		HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 1);
	} else {
		HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
	}
}

//  Thu thập dữ liệu từ cảm biến nhiệt độ trên Kit thí nghiệm và gửi lên server Adafruit
//  mỗi 30s. Tạo dashboard trên Adafruit giúp hiển thị dữ liệu nhiệt độ theo dạng
//  biểu đồ(Gợi ý: truyền dữ liệu nhiệt độ giữa ST và ESP dưới dạng "!TEMP:<Nhiệt
//  độ>#")
void tempProcess(){
	// Placeholder for temperature processing logic
	static uint16_t counter_send = 0;
	// Giả sử hàm này được gọi mỗi 50ms
	// => 30s = 30000ms / 50ms = 600 lần gọi
	if (counter_send++ >= 100) { 
		counter_send = 0;
		float temperature = sensor_GetTemperature();
		uart_EspSendFloat(temperature);
		// Debug info
		char debug_msg[50];
		sprintf(debug_msg, "Temp sent: %.2f\n", temperature);
		lcd_ShowStr(10, 160, debug_msg, GREEN, BLACK, 24, 0);
	}
}



void test_Esp(){
	if(uart_EspCheck() == 0) uart_EspSendBytes("o", 1);
	else lcd_ShowStr(10, 50, "ESP Connect", GREEN, BLACK, 24, 0);
}
