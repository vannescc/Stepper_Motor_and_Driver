#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>



#define SECOND_IN_ms 1000
#define STEPS 2048
#define RPM 10
#define FORWARDS 0
#define BACKWARDS 1

unsigned long stepDelay = 60L * 1000L / STEPS / RPM;
unsigned long stepNumber;


void my_delay_ms( unsigned int delay);
void step(int stepsToMove);
void stepMotor(unsigned int stepNumber);

int abs(int num);

int main (void) 
{

	/* need outputs on PD0, 1, 2, and 3. These are the 4 wires that your stepper motor is connected to.*/
	DDRB |= _BV(DDB0); //IN1
	DDRB |= _BV(DDB1); //IN3
	DDRB |= _BV(DDB2); //IN2
    DDRB |= _BV(DDB3); //IN4


	while(1) 
	{
		step(2048); //clockwise
        step(-2048); //counterclockwise
	}
}

/*
moves the stepper motor the defined number of steps
moves forwards is stepsToMove is positive, and backwards
if stepsToMove is negative
*/
void step(int stepsToMove) {
    int direction;
    int stepsLeft = abs(stepsToMove);

    if(stepsToMove > 0) {
        direction = FORWARDS;
    } else if(stepsToMove < 0) {
        direction = BACKWARDS;
    }

    while(stepsLeft > 0) {
        if(direction == FORWARDS) {
            stepNumber++;
            if(stepNumber == STEPS) {
                stepNumber = 0;
            }
        } else {
            if(stepNumber == 0) {
                stepNumber = STEPS;
            }
            stepNumber--;
        }
        stepsLeft--;

        stepMotor(stepNumber % 4);

        my_delay_ms(stepDelay);
    }
}

/*
moves the stepper motor one step forwards or backwards
*/
void stepMotor(unsigned int stepNumber) {
    switch(stepNumber) {
        case 0:
            PORTB |= _BV(PORTB0);
            PORTB &= ~_BV(PORTB1);
            PORTB |= _BV(PORTB2);
            PORTB &= ~_BV(PORTB3);
            break;
        case 1:
            PORTB &= ~_BV(PORTB0);
            PORTB |= _BV(PORTB1);
            PORTB |= _BV(PORTB2);
            PORTB &= ~_BV(PORTB3);
            break;
        case 2:
            PORTB &= ~_BV(PORTB0);
            PORTB |= _BV(PORTB1);
            PORTB &= ~_BV(PORTB2);
            PORTB |= _BV(PORTB3);
            break;
        case 3:
            PORTB |= _BV(PORTB0);
            PORTB &= ~_BV(PORTB1);
            PORTB &= ~_BV(PORTB2);
            PORTB |= _BV(PORTB3);
            break;
            
    }
}

int abs(int num) {
    if(num < 0) {
        return -1*num;
    }
    return num;
}

void my_delay_ms(unsigned int delay) 
{
	unsigned int i;

	for (i=0; i<(delay/10); i++) 
	{
		_delay_ms(10);
	}
	if (delay % 10) {
		_delay_ms(delay % 10);
	}
}
