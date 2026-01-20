# 7inch-esp32-welcome-display

Sistema embebido basado en **ESP32** que controla una **pantalla TFT de 7 pulgadas** para mostrar un mensaje de bienvenida y cambiar dinámicamente a una imagen/mensaje de **“Código válido, Adelante”** cuando un sensor externo se activa.

El proyecto está pensado para desarrollarse con **Arduino IDE** y para el diseño de la interfaz gráfica usando **EEZ Studio**.

---

## 📌 Descripción general

El sistema funciona en dos estados principales:

1. **Estado inicial (Idle / Bienvenida)**
   La pantalla muestra un mensaje de bienvenida estático.

2. **Estado activado (Acceso válido)**
   Al activarse un sensor digital (botón, lector RFID, sensor magnético, etc.), la pantalla cambia y muestra una imagen o texto indicando:

   > **“Código válido, Adelante”**

Cuando el sensor se desactiva, el sistema vuelve al estado de bienvenida.

---

## 🧰 Tecnologías utilizadas

* **Microcontrolador:** ESP32S3
* **IDE:** Arduino IDE
* **Diseño de UI:** EEZ Studio
* **Pantalla:** TFT 7 pulgadas
* **Lenguaje:** C/C++ (Arduino framework)

---

## 📂 Estructura del repositorio

```
7inch-esp32-welcome-display/
├─ README.md
├─ LICENSE
├─ src/
│  └─ main.ino          # Código principal ESP32 (Arduino)
├─ data/
│  └─ codigo_valido.bmp # Imagen mostrada cuando el sensor se activa
├─ ui/
│  └─ eez_project       # Proyecto de interfaz creado en EEZ Studio
└─ wiring.md            # Conexiones eléctricas (opcional)
```

---

## 🖥️ Interfaz gráfica (EEZ Studio)

La interfaz se diseña en **EEZ Studio** y define:

* Pantalla de bienvenida
* Pantalla de acceso válido
* Elementos gráficos (textos, imágenes, fondos)

El flujo típico es:

```
Sensor → ESP32 → Lógica de control → UI (EEZ Studio) → Pantalla TFT
```

La lógica del cambio de pantallas se implementa en el código Arduino usando las señales del sensor.

---

## 🔌 Sensor

El sistema es compatible con cualquier sensor digital:

* Botón pulsador
* Sensor magnético (reed switch)
* Sensor infrarrojo
* Lector RFID

El sensor se conecta a un **GPIO del ESP32** y se configura como entrada digital (`INPUT` o `INPUT_PULLUP`).

---

## 🧪 Funcionamiento del sistema

1. El ESP32 inicializa la pantalla TFT.
2. Se carga la interfaz o se dibuja el mensaje de bienvenida.
3. El sistema monitorea el estado del sensor.
4. Cuando el sensor se activa:

   * Se muestra la imagen o mensaje **“Código válido, Adelante”**.
5. Cuando el sensor se desactiva:

   * El sistema vuelve al mensaje de bienvenida.

---

## 🗃️ Manejo de imágenes

* Las imágenes se almacenan en **SPIFFS / LittleFS**.
* Se recomienda usar imágenes **BMP sin compresión**.
* La resolución debe coincidir con la de la pantalla TFT.

---

## 🚀 Cómo usar el proyecto

1. Clonar el repositorio:

   ```bash
   git clone https://github.com/tu-usuario/7inch-esp32-welcome-display.git
   ```

2. Abrir `main.ino` en **Arduino IDE**.

3. Configurar la librería de la pantalla (por ejemplo `TFT_eSPI`).

4. Subir la carpeta `data/` al ESP32 (SPIFFS/LittleFS).

5. Cargar el programa en el ESP32.

---

## 📈 Posibles mejoras

* Integración con lector RFID o teclado matricial.
* Animaciones de transición entre pantallas.
* Registro de accesos válidos.
* Comunicación con PC o servidor (WiFi / MQTT).
* Sonido de confirmación mediante buzzer.

---

## 🧾 Licencia

Este proyecto se distribuye bajo la licencia **MIT**.
Eres libre de usarlo, modificarlo y adaptarlo para fines académicos o personales.

---

## 👨‍🎓 Contexto académico

Proyecto orientado a prácticas de:

* Sistemas embebidos
* Interfaces gráficas en microcontroladores
* Integración sensor–pantalla
* Desarrollo con ESP32

---

**Autor:** Anthony Alexander Porras Uchofen
**Plataforma:** ESP32 · Arduino IDE · EEZ Studio
