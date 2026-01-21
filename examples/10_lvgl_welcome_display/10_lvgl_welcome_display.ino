#include <Arduino.h>
#include <esp_display_panel.hpp>

#include <lvgl.h>
#include "lvgl_v8_port.h"

#include "ui.h"
#include "actions.h"

using namespace esp_panel::drivers;
using namespace esp_panel::board;

#define PIN_BTN 6            // GPIO que usas para el contacto a GND
const unsigned long DEBOUNCE_MS = 50;
const unsigned long SHOW_MS = 3000; // 3 segundos

// Bandera marcada por la ISR
volatile bool buttonIrqFlag = false;
volatile uint32_t buttonIrqCount = 0;

// Estado de la aplicación
enum AppState { STATE_IDLE = 0, STATE_SHOW_ADELANTE = 1 };
AppState appState = STATE_IDLE;
unsigned long stateStartMs = 0;
unsigned long lastProcessedMs = 0;

// ISR muy corta: solo marca bandera (IRAM_ATTR para ESP32)
void IRAM_ATTR onButtonISR() {
  buttonIrqFlag = true;
  buttonIrqCount++;
}

void setup()
{
    String title = "LVGL porting example";

    Serial.begin(115200);

    Serial.println("Initializing board");
    Board *board = new Board();
    board->init();

    #if LVGL_PORT_AVOID_TEARING_MODE
    auto lcd = board->getLCD();
    // When avoid tearing function is enabled, the frame buffer number should be set in the board driver
    lcd->configFrameBufferNumber(LVGL_PORT_DISP_BUFFER_NUM);
#if ESP_PANEL_DRIVERS_BUS_ENABLE_RGB && CONFIG_IDF_TARGET_ESP32S3
    auto lcd_bus = lcd->getBus();
    /**
     * As the anti-tearing feature typically consumes more PSRAM bandwidth, for the ESP32-S3, we need to utilize the
     * "bounce buffer" functionality to enhance the RGB data bandwidth.
     * This feature will consume `bounce_buffer_size * bytes_per_pixel * 2` of SRAM memory.
     */
    if (lcd_bus->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_RGB) {
        static_cast<BusRGB *>(lcd_bus)->configRGB_BounceBufferSize(lcd->getFrameWidth() * 10);
    }
#endif
#endif
    assert(board->begin());

    Serial.println("Initializing LVGL");
    lvgl_port_init(board->getLCD(), board->getTouch());

    Serial.println("Creating UI");
    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    lvgl_port_lock(-1);

    ui_init();

    /* Release the mutex */
    lvgl_port_unlock();

    // Configuración del botón
    pinMode(PIN_BTN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_BTN), onButtonISR, FALLING);
    Serial.println("Button configured on pin " + String(PIN_BTN));
}

void loop()
{
    unsigned long now = millis();

    // **ELIMINADO**: lv_timer_handler() ya se ejecuta en lvgl_port_task()
    // La biblioteca lvgl_v8_port maneja esto internamente

    // Procesar evento marcado por ISR (debounce y cambio de pantalla)
    if (buttonIrqFlag) {
        // Capturamos y limpiamos bandera de forma atómica
        noInterrupts();
        buttonIrqFlag = false;
        uint32_t irqSnap = buttonIrqCount;
        interrupts();

        // Anti-rebote: aceptar solo si ha pasado el tiempo
        if (now - lastProcessedMs >= DEBOUNCE_MS) {
            lastProcessedMs = now;

            // Solo reaccionar si estamos en IDLE
            if (appState == STATE_IDLE) {
                Serial.printf("IRQ #%u -> cargar pantalla ADELANTE\n", irqSnap);
                
                if (lvgl_port_lock(-1)) {
                    if (objects.adelante) {
                        lv_scr_load(objects.adelante);
                    } else {
                        Serial.println("Warning: objects.adelante == NULL");
                    }
                    lvgl_port_unlock();
                }
                appState = STATE_SHOW_ADELANTE;
                stateStartMs = now;
            } else {
                // Si ya está visible la pantalla 'adelante', ignoramos
                Serial.println("Evento recibido pero ya en estado ADELANTE -> ignorado");
            }
        }
    }

    // Si estamos mostrando 'adelante' y pasó SHOW_MS -> volver a main
    if (appState == STATE_SHOW_ADELANTE) {
        if (now - stateStartMs >= SHOW_MS) {
            Serial.println("3s transcurridos -> volver a MAIN");
            
            if (lvgl_port_lock(-1)) {
                if (objects.main) {
                    lv_scr_load(objects.main);
                } else {
                    Serial.println("Warning: objects.main == NULL");
                }
                lvgl_port_unlock();
            }
            appState = STATE_IDLE;
        }
    }

    // Pequeño delay para no saturar el CPU
    delay(10);
}