#include "board.h"

static UART_Regs* current_uart = UART_PC_INST;
void board_set_uart(UART_Regs* uart) {
    current_uart = uart;
}

int fputc(int c, FILE* stream) {
    DL_UART_Main_transmitDataBlocking(current_uart, c);
    return c;
}

int fputs(const char* restrict s, FILE* restrict stream) {
    uint16_t i, len;
    len = strlen(s);
    for (i = 0; i < len; i ++) {
        DL_UART_Main_transmitDataBlocking(current_uart, s[i]);
    }
    return len;
}

int puts(const char* _ptr) {
    int count = fputs(_ptr, stdout);
    count += fputs("\n", stdout);
    return count;
}