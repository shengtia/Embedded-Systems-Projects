#include <stdio.h>

unsigned calculateCheckSum(char* record){
    unsigned int sum = 0x00;
    for (int i = 0; i < 40; i+=2){
    //    printf("In %dth iteration, the value is : %x \n", i, (record[i] << 4) + record[i+1]);
        sum += (record[i] << 4) + record[i+1];
    }
    sum = ~sum;
    sum += 0x01;
    return sum & 0XFF;
}


int main()
{
    char record[40];
    record[0] = 0x1;
    record[1] = 0x0;
    record[2] = 0x0;
    record[3] = 0x2;
    record[4] = 0xB;
    record[5] = 0x0;
    record[6] = 0x0;
    record[7] = 0x0;
    record[8] = 0x8;
    record[9] = 0x4;
    record[10] = 0xB;
    record[11] = 0xD;
    record[12] = 0x1;
    record[13] = 0x5;
    record[14] = 0xB;
    record[15] = 0xC;
    record[16] = 0x0;
    record[17] = 0x8;
    record[18] = 0x9;
    record[19] = 0x5;
    record[20] = 0x8;
    record[21] = 0xF;
    record[22] = 0xE;
    record[23] = 0xF;
    record[24] = 0x8;
    record[25] = 0x7;
    record[26] = 0xB;
    record[27] = 0x9;
    record[28] = 0x8;
    record[29] = 0xB;
    record[30] = 0xB;
    record[31] = 0x9;
    record[32] = 0x8;
    record[33] = 0x8;
    record[34] = 0xB;
    record[35] = 0x9;
    record[36] = 0x0;
    record[37] = 0xE;
    record[38] = 0x9;
    record[39] = 0x4;

    printf("The result is %x \n", calculateCheckSum(record));

    return 0;
}



