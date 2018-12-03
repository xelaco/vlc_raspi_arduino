/* xelaco 2018
 * Visible light communication between a Raspberry Pi and an Arduino
 * sender.c
 * 
 * ___Requirements for the sender___
 * > Raspberry Pi
 * > Same LED as receiver
 * > Wires
 * > This program must be compiled with the Raspberry Pi.
 * > "wiringpi" library to use gpio, if this program can be compiled, it is good! Else go to wiringpi.com to install it.
 * 
 * ___Circuit___
 * LED: (+) to BCM pin 17 (physical pin 11) /// (-) to ground (physical pin 9, for example)
 * 
 * ___Compilation command___
 * gcc -Wall sender.c -lwiringPi
 */

#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#define bitRead(value, bit) (((value) >> (bit)) & 0x01) /* From Arduino, gets binary from integer. */

#define MAX_CHAR 10 /* Number max of character */

int main(void)
{
    char msg[MAX_CHAR+1];
    char *return_position = NULL;
    int c = 0;
    int msg_bin[7];
    
    wiringPiSetup();
    pinMode (0, OUTPUT); /* Pin 0 is BCM 17 */
    
    while(1)
    {
        printf("Message (max %d characters) / \"stop\" to stop program: ", MAX_CHAR);
        if(fgets(msg, MAX_CHAR+1, stdin) != NULL)
        {
            return_position = strchr(msg, '\n');
            if(return_position != NULL)
            {
                *return_position = '\0';
            }
            else
            {
                while ((c = getchar()) != '\n' && c != EOF) { }
            }
        }
        
        if(strcmp(msg, "stop") == 0)
        {
            printf("Goodbye!\n");
            break;
        }
        
        digitalWrite(0, HIGH); /* Informs the Arduino the transmission starts. */
        delay(2000);
        
        printf("Message: %s\n", msg); /* Different from input if there was too many characters. */
        for (int k = 0; k < strlen(msg); k++)
        {
            printf("%c : ", msg[k]);
            for (int i = 0; i < 7; i++)
            {
                msg_bin[i] = bitRead(msg[k], i);
            }
            for (int i = 6; i >= 0; i--) /* Sends LSB first. */
            {
                digitalWrite(0, msg_bin[i]);
                printf("%d  ", msg_bin[i]);
                delay(500);
            }
            printf("\n");
        }
        digitalWrite(0, LOW);
        printf("Wait please.\n");
        delay(10000);
    }
    
    return 0;
}
