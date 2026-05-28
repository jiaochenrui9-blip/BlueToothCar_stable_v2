#include "OLED.h"

#include <stdio.h>
#include <string.h>

#include "car.h"

extern I2C_HandleTypeDef hi2c1;

#define OLED_ADDR_3C     (0x3C << 1)
#define OLED_ADDR_3D     (0x3D << 1)
#define OLED_WIDTH       128
#define OLED_HEIGHT      64
#define OLED_PAGE_COUNT  (OLED_HEIGHT / 8)
#define OLED_BUF_SIZE    (OLED_WIDTH * OLED_PAGE_COUNT)
#define OLED_TIMEOUT_MS  10
#define OLED_DATA_CHUNK  16

static uint8_t OLED_Buffer[OLED_BUF_SIZE];
static uint8_t OLED_LastBuffer[OLED_BUF_SIZE];
static uint16_t OLED_Address = OLED_ADDR_3C;

static const uint8_t FontDigits[10][5] = {
    {0x3E, 0x51, 0x49, 0x45, 0x3E},
    {0x00, 0x42, 0x7F, 0x40, 0x00},
    {0x42, 0x61, 0x51, 0x49, 0x46},
    {0x21, 0x41, 0x45, 0x4B, 0x31},
    {0x18, 0x14, 0x12, 0x7F, 0x10},
    {0x27, 0x45, 0x45, 0x45, 0x39},
    {0x3C, 0x4A, 0x49, 0x49, 0x30},
    {0x01, 0x71, 0x09, 0x05, 0x03},
    {0x36, 0x49, 0x49, 0x49, 0x36},
    {0x06, 0x49, 0x49, 0x29, 0x1E},
};

static const uint8_t FontUpper[26][5] = {
    {0x7E, 0x11, 0x11, 0x11, 0x7E},
    {0x7F, 0x49, 0x49, 0x49, 0x36},
    {0x3E, 0x41, 0x41, 0x41, 0x22},
    {0x7F, 0x41, 0x41, 0x22, 0x1C},
    {0x7F, 0x49, 0x49, 0x49, 0x41},
    {0x7F, 0x09, 0x09, 0x09, 0x01},
    {0x3E, 0x41, 0x49, 0x49, 0x7A},
    {0x7F, 0x08, 0x08, 0x08, 0x7F},
    {0x00, 0x41, 0x7F, 0x41, 0x00},
    {0x20, 0x40, 0x41, 0x3F, 0x01},
    {0x7F, 0x08, 0x14, 0x22, 0x41},
    {0x7F, 0x40, 0x40, 0x40, 0x40},
    {0x7F, 0x02, 0x0C, 0x02, 0x7F},
    {0x7F, 0x04, 0x08, 0x10, 0x7F},
    {0x3E, 0x41, 0x41, 0x41, 0x3E},
    {0x7F, 0x09, 0x09, 0x09, 0x06},
    {0x3E, 0x41, 0x51, 0x21, 0x5E},
    {0x7F, 0x09, 0x19, 0x29, 0x46},
    {0x46, 0x49, 0x49, 0x49, 0x31},
    {0x01, 0x01, 0x7F, 0x01, 0x01},
    {0x3F, 0x40, 0x40, 0x40, 0x3F},
    {0x1F, 0x20, 0x40, 0x20, 0x1F},
    {0x3F, 0x40, 0x38, 0x40, 0x3F},
    {0x63, 0x14, 0x08, 0x14, 0x63},
    {0x07, 0x08, 0x70, 0x08, 0x07},
    {0x61, 0x51, 0x49, 0x45, 0x43},
};

static HAL_StatusTypeDef OLED_WriteCommand(uint8_t command)
{
    uint8_t data[2] = {0x00, command};
    return HAL_I2C_Master_Transmit(&hi2c1, OLED_Address, data, sizeof(data), OLED_TIMEOUT_MS);
}

static HAL_StatusTypeDef OLED_WriteData(const uint8_t *data, uint16_t size)
{
    uint8_t buffer[OLED_DATA_CHUNK + 1];

    buffer[0] = 0x40;
    while (size > 0)
    {
        uint16_t chunk = (size > OLED_DATA_CHUNK) ? OLED_DATA_CHUNK : size;
        memcpy(&buffer[1], data, chunk);

        if (HAL_I2C_Master_Transmit(&hi2c1, OLED_Address, buffer, chunk + 1, OLED_TIMEOUT_MS) != HAL_OK)
        {
            return HAL_ERROR;
        }

        data += chunk;
        size -= chunk;
    }

    return HAL_OK;
}

static HAL_StatusTypeDef OLED_SetPage(uint8_t page)
{
    if (OLED_WriteCommand(0xB0 + page) != HAL_OK) return HAL_ERROR;
    if (OLED_WriteCommand(0x00) != HAL_OK) return HAL_ERROR;
    if (OLED_WriteCommand(0x10) != HAL_OK) return HAL_ERROR;
    return HAL_OK;
}

static void OLED_UpdatePage(uint8_t page)
{
    uint16_t offset;

    if (page >= OLED_PAGE_COUNT)
    {
        return;
    }

    offset = (uint16_t)page * OLED_WIDTH;
    if (memcmp(&OLED_Buffer[offset], &OLED_LastBuffer[offset], OLED_WIDTH) == 0)
    {
        return;
    }

    if (OLED_SetPage(page) == HAL_OK &&
        OLED_WriteData(&OLED_Buffer[offset], OLED_WIDTH) == HAL_OK)
    {
        memcpy(&OLED_LastBuffer[offset], &OLED_Buffer[offset], OLED_WIDTH);
    }
}

static void OLED_CommandList(const uint8_t *commands, uint8_t count)
{
    while (count > 0)
    {
        OLED_WriteCommand(*commands);
        commands++;
        count--;
    }
}

static void OLED_DetectAddress(void)
{
    if (HAL_I2C_IsDeviceReady(&hi2c1, OLED_ADDR_3C, 2, 20) == HAL_OK)
    {
        OLED_Address = OLED_ADDR_3C;
        return;
    }

    if (HAL_I2C_IsDeviceReady(&hi2c1, OLED_ADDR_3D, 2, 20) == HAL_OK)
    {
        OLED_Address = OLED_ADDR_3D;
        return;
    }

    OLED_Address = OLED_ADDR_3C;
}

static const uint8_t *OLED_GetGlyph(char c)
{
    static const uint8_t GlyphSpace[5]   = {0x00, 0x00, 0x00, 0x00, 0x00};
    static const uint8_t GlyphColon[5]   = {0x00, 0x36, 0x36, 0x00, 0x00};
    static const uint8_t GlyphPercent[5] = {0x63, 0x13, 0x08, 0x64, 0x63};
    static const uint8_t GlyphMinus[5]   = {0x08, 0x08, 0x08, 0x08, 0x08};
    static const uint8_t GlyphDot[5]     = {0x00, 0x60, 0x60, 0x00, 0x00};
    static const uint8_t GlyphSlash[5]   = {0x20, 0x10, 0x08, 0x04, 0x02};
    static const uint8_t GlyphQuestion[5]= {0x02, 0x01, 0x51, 0x09, 0x06};

    if (c >= '0' && c <= '9')
    {
        return FontDigits[c - '0'];
    }

    if (c >= 'a' && c <= 'z')
    {
        c = (char)(c - 'a' + 'A');
    }

    if (c >= 'A' && c <= 'Z')
    {
        return FontUpper[c - 'A'];
    }

    switch (c)
    {
        case ':': return GlyphColon;
        case '%': return GlyphPercent;
        case '-': return GlyphMinus;
        case '.': return GlyphDot;
        case '/': return GlyphSlash;
        case ' ': return GlyphSpace;
        default:  return GlyphQuestion;
    }
}

static void OLED_SetPixel(uint8_t x, uint8_t y, uint8_t on)
{
    uint16_t index;
    uint8_t mask;

    if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
    {
        return;
    }

    index = x + (uint16_t)(y / 8) * OLED_WIDTH;
    mask = (uint8_t)(1U << (y % 8));

    if (on)
    {
        OLED_Buffer[index] |= mask;
    }
    else
    {
        OLED_Buffer[index] &= (uint8_t)~mask;
    }
}

static void OLED_ShowChar(uint8_t x, uint8_t y, char c)
{
    const uint8_t *glyph = OLED_GetGlyph(c);

    for (uint8_t col = 0; col < 6; col++)
    {
        uint8_t line = (col < 5) ? glyph[col] : 0x00;

        for (uint8_t row = 0; row < 8; row++)
        {
            OLED_SetPixel((uint8_t)(x + col), (uint8_t)(y + row), (line & (1U << row)) != 0);
        }
    }
}

void OLED_Init(void)
{
    static const uint8_t initCommands[] = {
        0xAE,
        0x20, 0x00,
        0xB0,
        0xC8,
        0x00,
        0x10,
        0x40,
        0x81, 0x7F,
        0xA1,
        0xA6,
        0xA8, 0x3F,
        0xA4,
        0xD3, 0x00,
        0xD5, 0x80,
        0xD9, 0xF1,
        0xDA, 0x12,
        0xDB, 0x40,
        0x8D, 0x14,
        0xAF
    };

    HAL_Delay(100);
    OLED_DetectAddress();
    OLED_CommandList(initCommands, (uint8_t)sizeof(initCommands));

    memset(OLED_LastBuffer, 0xFF, sizeof(OLED_LastBuffer));
    OLED_Clear();
    OLED_Update();
}

void OLED_Clear(void)
{
    memset(OLED_Buffer, 0x00, sizeof(OLED_Buffer));
}

void OLED_Update(void)
{
    for (uint8_t page = 0; page < OLED_PAGE_COUNT; page++)
    {
        OLED_UpdatePage(page);
    }
}

void OLED_ShowString(uint8_t x, uint8_t y, const char *str)
{
    while (str != NULL && *str != '\0')
    {
        if (*str == '\n')
        {
            x = 0;
            y = (uint8_t)(y + 8);
            str++;
            continue;
        }

        if (x > OLED_WIDTH - 6)
        {
            x = 0;
            y = (uint8_t)(y + 8);
        }

        if (y > OLED_HEIGHT - 8)
        {
            break;
        }

        OLED_ShowChar(x, y, *str);
        x = (uint8_t)(x + 6);
        str++;
    }
}

void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num)
{
    char text[11];
    sprintf(text, "%lu", (unsigned long)num);
    OLED_ShowString(x, y, text);
}

void OLED_ShowCarInfo(uint16_t distance)
{
    OLED_Clear();

    OLED_ShowString(0, 0, "MODE:");
    OLED_ShowString(36, 0, mode ? "AUTO" : "MANUAL");

    OLED_ShowString(0, 16, "DIST:");
    if (distance == 0)
    {
        OLED_ShowString(36, 16, "TIMEOUT");
    }
    else
    {
        OLED_ShowNum(36, 16, distance);
        OLED_ShowString(66, 16, "CM");
    }

    OLED_ShowString(0, 32, "SPEED:");
    OLED_ShowNum(42, 32, Speed);
    OLED_ShowString(60, 32, "%");

    OLED_Update();
}

static const char *OLED_GetManualStateName(void)
{
    switch (ManualState)
    {
        case MANUAL_STATE_FORWARD:  return "FWD";
        case MANUAL_STATE_BACKWARD: return "BWD";
        case MANUAL_STATE_LEFT:     return "LEFT";
        case MANUAL_STATE_RIGHT:    return "RIGHT";
        case MANUAL_STATE_STOP:     return "STOP";
        default:                    return "UNK";
    }
}

static const char *OLED_GetAutoStateName(void)
{
    switch (AutoState)
    {
        case AUTO_STATE_FORWARD:  return "FWD";
        case AUTO_STATE_BACKWARD: return "BWD";
        case AUTO_STATE_TURN:     return "TURN";
        case AUTO_STATE_STOP:     return "STOP";
        default:                  return "UNK";
    }
}

static char OLED_GetVisibleCmd(uint8_t cmd)
{
    if (cmd >= 32 && cmd <= 126)
    {
        return (char)cmd;
    }

    return '?';
}

void OLED_ShowCarState(uint16_t distance, uint8_t cmd)
{
    char text[22];

    OLED_Clear();

    OLED_ShowString(0, 0, "MODE:");
    OLED_ShowString(30, 0, (CarState == CAR_STATE_AUTO) ? "AUTO" : "MANUAL");

    OLED_ShowString(0, 16, "MAN:");
    OLED_ShowString(30, 16, OLED_GetManualStateName());

    OLED_ShowString(0, 32, "AUTO:");
    OLED_ShowString(36, 32, OLED_GetAutoStateName());

    if (distance == 0)
    {
        sprintf(text, "CMD:%c D:TIMEOUT", OLED_GetVisibleCmd(cmd));
    }
    else
    {
        sprintf(text, "CMD:%c D:%luCM", OLED_GetVisibleCmd(cmd), (unsigned long)distance);
    }
    OLED_ShowString(0, 48, text);

    OLED_Update();
}
