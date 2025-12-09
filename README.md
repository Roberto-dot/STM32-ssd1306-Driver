# SSD1306 Driver README

## Introduction

This is a bare‑metal STM32 library for controlling OLED displays based on the SSD1306 driver. It provides a simple and efficient interface over I2C communication, allowing complete control of text, graphics, and screen operations without relying on HAL or external frameworks. The library is designed for lightweight, fast, and direct register‑level use on STM32 microcontrollers.

---

## 1. Initialization

Before calling any drawing or text functions, the display must be initialized.

### **Function:** `ssd1306_Init()`

**Description:** Initializes the SSD1306 display over I2C.

**Parameters:** None

**Return Value:** `uint8_t`

* `0` → Initialization successful
* `1` → Initialization failed (e.g., I2C communication error)

**Possible Errors:**

* Display not connected
* Incorrect I2C address
* I2C peripheral not configured

---

## 2. Screen Control Functions

### **`ssd1306_Fill(SSD1306_COLOR color)`**

Fills the internal display buffer with either black or white.

**Parameters:**

* `color` → `Black` or `White`

**Return Value:** None

---

### **`ssd1306_UpdateScreen()`**

Sends the internal buffer to the OLED display.

**Parameters:** None

**Return Value:** None

**Notes:** Must be called after drawing operations to see changes.

---

## 3. Cursor and Text Functions

### **`ssd1306_SetCursor(uint8_t x, uint8_t y)`**

Moves the cursor to a specific pixel position.

**Parameters:**

* `x` → Horizontal position (0–127)
* `y` → Vertical position (0–63)

**Return:** None

**Errors:** If values exceed display limits, text may not appear.

---

### **`ssd1306_WriteString(const char* str, FontDef Font, SSD1306_COLOR color)`**

Writes a string to the screen buffer.

**Parameters:**

* `str` → Null‑terminated string
* `Font` → Available fonts: `Font_6x8`, `Font_7x10`, `Font_11x18`, `Font_16x26`, etc.
* `color` → `White` or `Black`

**Return Value:** `uint8_t`

* `0` → Success
* `1` → Text goes out of display bounds

---

## 4. Drawing Functions

### **`ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color)`**

Sets a single pixel in the buffer.

**Errors:** Pixel ignored if out of bounds.

---

### **`ssd1306_DrawRectangle(uint8_t Base, uint8_t High, SSD1306_COLOR color)`**

Draws a rectangle starting from the current cursor.

**Parameters:**

* `Base` → Width
* `High` → Height
* `color` → Fill color

**Return:** None

---

## 5. Example of Usage (Main Test)

```c
// Rectangle test function is now located in the test folder.
// Please see: `tests/ssd1306_rect_test.c`

int main(void){(void){
    setConf();

    ssd1306_Init();          // Initialize display
    ssd1306_Fill(Black);     // Clear buffer

    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Hello!", Font_11x18, White);

    ssd1306_SetCursor(0, 30);
    ssd1306_WriteString("STM32", Font_11x18, White);

    ssd1306_UpdateScreen();  // Send buffer to display
    delay_ms(500);

    while(1){
        ssd1306_Fill(Black);

        ssd1306_SetCursor(0, 0);
        ssd1306_WriteString("Test Font 6x8", Font_6x8, White);

        ssd1306_SetCursor(0, 30);
        ssd1306_WriteString("Test Font 7x10", Font_7x10, White);

        ssd1306_SetCursor(0, 30);
        ssd1306_WriteString("Test Font 11x18", Font_11x18, White);

        ssd1306_UpdateScreen();
        delay_ms(700);

        ssd1306_Fill(Black);

        ssd1306_SetCursor(0, 0);
        ssd1306_WriteString("Test Font 16x26", Font_16x26, White);

        ssd1306_SetCursor(0, 30);
        ssd1306_WriteString("Test Font 16x24", Font_16x24, White);

        ssd1306_UpdateScreen();
    }
}
```

---

## 6. Notes

* Always call `ssd1306_UpdateScreen()` after writing or drawing.
* Buffers are not automatically refreshed.
* Large fonts consume more vertical space.
