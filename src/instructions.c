#include <stdint.h>
#include <stdio.h>

#include "constants.h"
#include "cpu.h"


void clearScreen(uint8_t* pixelBuff)
{
    printf("clearScreen\n");
    for (int i = 0; i < BUFF_WIDTH*BUFF_HEIGHT; i++) {
        pixelBuff[i] = 0;
    }
}

void jump(struct cpu* cpu, uint16_t nnn)
{
    printf("jump\n");
    cpu->pc = nnn;
}

void setVX(struct cpu* cpu, uint8_t x, uint8_t nn)
{
    printf("setVX\n");
    cpu->registers[x] = nn;
}

void addToVX(struct cpu* cpu, uint8_t x, uint8_t nn)
{
    printf("addToVX\n");
    cpu->registers[x] += nn; // TODO: What happens if overflow?
}

void setIdx(struct cpu* cpu, uint16_t nnn)
{
    printf("setIdx\n");
    cpu->idx = nnn;
}

void updateBuffer(uint8_t* pixelBuff, struct cpu* cpu, uint8_t x, uint8_t y, uint8_t n)
{
    printf("updateBuffer\n");
    for (int yOff = 0; yOff < n; yOff++) {
        int wrappedY = (y + yOff) % BUFF_HEIGHT;
        uint8_t spriteByte = cpu->heap[cpu->idx+yOff];
        uint8_t byteMask = 0x80;
        for (int xOff = 0; xOff < 8; xOff++) {
            int wrappedX = (x + xOff) % BUFF_WIDTH;
            if (spriteByte&byteMask) {
                if (pixelBuff[wrappedY*BUFF_WIDTH+wrappedX]) {
                    cpu->registers[VF] = 1;
                }

                pixelBuff[wrappedY*BUFF_WIDTH+wrappedX] ^= 1;
            }
            byteMask >>= 1;
        }
    }
}
