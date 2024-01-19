#define DIRB  *((volatile unsigned char*)0x37)

#define PORTB *((volatile unsigned char*)0x38)

int main() {
    DIRB  |= (0 << 0);


    while(1) {}
}