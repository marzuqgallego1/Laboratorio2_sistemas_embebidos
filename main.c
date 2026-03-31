#include <stdio.h>
#include "esp_random.h"
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/timer.h"
#include "esp_timer.h"

//filas de leds
#define Led3r 32
#define Led3v 14
#define Led4r 15
#define Led4v 27
#define Led5r 5
#define Led5v 4
#define Led6r 18
#define Led6v 16

// columnas de leds
#define col3 22
#define col4 23
#define col5 26
#define col6 25

//botones
#define v 35
#define r 34
#define set 39

// generacion de aleatorios
int indice = 5; 
uint8_t patron_objetivo[10];
uint8_t patron_usuario[10];
int count = 0;
int flag = 0;

//funcion que genera aleatorios e inicializa el array patron usuario en un valor imposible
void generar_random(){
    for (int i =0; i < indice; i++){
        patron_objetivo[i] = esp_random() % 2;
        patron_usuario[i] = 9;
    }
}

// banderas
static volatile bool flag_timer = false;
static volatile bool flag_v = false;
static volatile bool flag_r = false;
static volatile bool flag_set = false;

//interrupciones
static bool IRAM_ATTR timer_isr(void *arg){
    flag_timer = true; //cambia el valor del pin
    return false;
}
static void IRAM_ATTR button_v(void *arg){
    flag_v = true; //cambia el valor del pin 
}
static void IRAM_ATTR button_r(void *arg){
    flag_r = true; //cambia el valor del pin 
}
static void IRAM_ATTR button_set(void *arg){
    flag_set = true; //cambia el valor del pin 
}
void app_main() {
    //pines de salidas
    gpio_config_t matriz_cfg = {
        .pin_bit_mask = (1ULL << Led3r)|(1ULL << Led3v)|(1ULL << Led4r)|(1ULL << Led4v)|(1ULL << Led5r)|(1ULL << Led5v)|(1ULL << Led6r)|(1ULL << Led6v)|(1ULL << col3)|(1ULL << col4)|(1ULL << col5)|(1ULL << col6),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    //patron de incio del juego
    gpio_config(&matriz_cfg);
    gpio_set_level(col3, 1);
    gpio_set_level(col4, 0);
    gpio_set_level(col5, 0);
    gpio_set_level(col6, 1);
    //(#._.)//
    gpio_set_level(Led3v, 0);
    gpio_set_level(Led3r, 0);
    gpio_set_level(Led4v, 0);
    gpio_set_level(Led4r, 0);
    gpio_set_level(Led5v, 0);
    gpio_set_level(Led5r, 0);
    gpio_set_level(Led6v, 0);
    gpio_set_level(Led6r, 0);

    //pines de entrada
     gpio_config_t bts_cfg = {
        .pin_bit_mask = (1ULL << v)|(1ULL << r)|(1ULL << set),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&bts_cfg);

    //configuracion del timer y alarmas
     timer_config_t timer_cfg ={
        .divider = 80, //frecuencia de 1MHz
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = true
    };
    timer_init(TIMER_GROUP_0, TIMER_0, &timer_cfg);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
    timer_set_alarm_value(
        TIMER_GROUP_0,
        TIMER_0,
        10000000
    );
    timer_isr_callback_add(
        TIMER_GROUP_0,
        TIMER_0,
        timer_isr,
        NULL,
        0
    );
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    
    //interrupciones
    gpio_install_isr_service(0); //habilita interrupciones
    gpio_isr_handler_add(v, button_v, NULL);
    gpio_isr_handler_add(r, button_r, NULL);
    gpio_isr_handler_add(set, button_set, NULL);

    while(1){
        

        if(flag_set){
           
            generar_random();
            //encendemos todas las columnas y apagamos las filas
            gpio_set_level(col3, 1);
            gpio_set_level(col4, 1);
            gpio_set_level(col5, 1);
            gpio_set_level(col6, 1);

            gpio_set_level(Led3v, 1);
            gpio_set_level(Led4v, 1);
            gpio_set_level(Led5v, 1);
            gpio_set_level(Led6v, 1);
            gpio_set_level(Led3r, 1);
            gpio_set_level(Led4r, 1);
            gpio_set_level(Led5r, 1);
            gpio_set_level(Led6r, 1);
            vTaskDelay(pdMS_TO_TICKS(1000)); // 1s de espera

            //muestra de patron
            for (int i =0; i < indice; i++){
                if(patron_objetivo[i] == 1){
                    gpio_set_level(Led3v, 0);
                    gpio_set_level(Led4v, 0);
                    gpio_set_level(Led5v, 0);
                    gpio_set_level(Led6v, 0);
                    vTaskDelay(pdMS_TO_TICKS(1000));
                    gpio_set_level(Led3v, 1);
                    gpio_set_level(Led4v, 1);
                    gpio_set_level(Led5v, 1);
                    gpio_set_level(Led6v, 1);
                    gpio_set_level(Led3r, 1);
                    gpio_set_level(Led4r, 1);
                    gpio_set_level(Led5r, 1);
                    gpio_set_level(Led6r, 1);
                }
                else {
                    gpio_set_level(Led3r, 0);
                    gpio_set_level(Led4r, 0);
                    gpio_set_level(Led5r, 0);
                    gpio_set_level(Led6r, 0);
                    vTaskDelay(pdMS_TO_TICKS(1000));
                    gpio_set_level(Led3v, 1);
                    gpio_set_level(Led4v, 1);
                    gpio_set_level(Led5v, 1);
                    gpio_set_level(Led6v, 1);
                    gpio_set_level(Led3r, 1);
                    gpio_set_level(Led4r, 1);
                    gpio_set_level(Led5r, 1);
                    gpio_set_level(Led6r, 1);
                }
                vTaskDelay(pdMS_TO_TICKS(1000)); // 1s de espera

            }
            //empieza a contar el reloj
            timer_start(TIMER_GROUP_0, TIMER_0);

            // se fuerza a que las entradas de los botones sean false por si las oprimieron antes
            flag_r = false;
            flag_v = false;
            //se lee el patron ingresado por el usuario
            while (count < indice && !flag_timer){
                if (flag_r){
                    
                    patron_usuario[count] = 1;
                    count++;
                    while(gpio_get_level(r) == 0){vTaskDelay(pdMS_TO_TICKS(10));} //antirrebote
                    flag_r = false;
                }
                else if (flag_v){
                    
                    patron_usuario[count] = 0;
                    count++;
                    while(gpio_get_level(v) == 0){vTaskDelay(pdMS_TO_TICKS(10));} //antirebote
                    flag_v = false;
                }
            }

            //reinicio del timer
            timer_pause(TIMER_GROUP_0, TIMER_0);
            timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
            flag_timer = false;

            //se hace la comprobacion del patron
            for(int i =0; i<indice;i++){
                if (patron_objetivo[i] != patron_usuario[i]){
                    

                    gpio_set_level(col3, 1);
                    gpio_set_level(col4, 0);
                    gpio_set_level(col5, 0);
                    gpio_set_level(col6, 1);

                    gpio_set_level(Led3v,0);
                    gpio_set_level(Led4v,0);
                    gpio_set_level(Led5v,0);
                    gpio_set_level(Led6v,0);
                    gpio_set_level(Led3r,0);
                    gpio_set_level(Led4r,0);
                    gpio_set_level(Led5r,0);
                    gpio_set_level(Led6r,0);
                    
                    indice = 5;
                    flag_set = false;
                    vTaskDelay(pdMS_TO_TICKS(2000)); //espera de 2s
                    break;
                };
                flag++; 
            
            };
            if(flag == indice){
                //patron correcto
                gpio_set_level(col3, 1);
                gpio_set_level(col4, 0);
                gpio_set_level(col5, 0);
                gpio_set_level(col6, 1);

                gpio_set_level(Led3v,0);
                gpio_set_level(Led4v,0);
                gpio_set_level(Led5v,0);
                gpio_set_level(Led6v,0);
                
                vTaskDelay(pdMS_TO_TICKS(2000)); //espera de 2s

                gpio_set_level(Led3r,0);
                gpio_set_level(Led4r,0);
                gpio_set_level(Led5r,0);
                gpio_set_level(Led6r,0);
                if(indice < 10){
                    indice++;
                flag_set = true;
                };

            };
            count = 0;
            flag = 0;
        }
    }

}