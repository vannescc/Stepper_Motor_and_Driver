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

/* C Prototypes of functions */
void pwm_hw_setup();

void my_delay_ms( unsigned int delay);
void step(int stepsToMove);
void stepMotor(unsigned int stepNumber);

int abs(int num);

int main (void) 
{

	/* need outputs on PD3 (red = Pin2), PD4 (yellow = Pin4), PD5 (green = Pin5) */
	DDRB |= _BV(DDB0);
	DDRB |= _BV(DDB1);
	DDRB |= _BV(DDB2);
    DDRB |= _BV(DDB3);

    

	// DDRD |= _BV(DDD3);
	// DDRB |= _BV(DDB2);
	// DDRB |= _BV(DDB3);

	while(1) 
	{
		step(2048);
        step(-2048);
	}
}

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

/*
 * Sets up the three PWM timers (0, 1, 2) and the pins related to them with 0 duty cycle.
 *
 * Note: might want to disable pins you aren't using!!!
 *
 * I also set up the three pins to output here, but you could do in main
 */
void pwm_hw_setup()
{
	// TIMER 0 - 8 bit
	// EXAMPLE set PWM 50%
	// OCR0A = 128;
	// OCR0B = 128;
	
	OCR0A = 0;
	OCR0B = 0;

    	// set none-inverting mode
	TCCR0A |= (1 << COM0A1);
    	// set fast PWM Mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
    	// set prescaler to 8 and starts PWM
	TCCR0B |= (1 << CS01);
	
	// TIMER 1 - 16 bit
	// OC1A and OC1B synced
	// EXAMPLE set PWM for 25% duty cycle @ 16bit
	// OCR1A = 0x3FFF;
	// set PWM for 75% duty cycle @ 16bit
	// OCR1B = 0xBFFF;
	
    	// set TOP to 16bit
	ICR1 = 0xFFFF;

	OCR1A = 0x0000;
	OCR1B = 0x0000;

	// set none-inverting mode
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
	// set Fast PWM mode using ICR1 as TOP - MODE 14
	TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12);
    
	// START the timer with no prescaler
	TCCR1B |= (1 << CS10);

	// TIMER 2 - 8 bit
	// OC2A and OC2B synced
	// EXAMPLE - 50% DUTY
	// OCR2A = 128;

	// set PWM for 0% duty cycle
	OCR2A = 0;
	OCR2B = 0;

	// set none-inverting mode A
	TCCR2A |= (1 << COM2A1);
	// set none-inverting mode B
	TCCR2A |= (1 << COM2B1);

	// set fast PWM Mode
	TCCR2A |= (1 << WGM21) | (1 << WGM20);
	// START WITH NO PRESCALER
	TCCR2B |= (1 << CS20);

	// SELECT PINS we're going out on for our schematic
	/* set OC2B = Arduino_Pin3 pin as output - TIMER 2 */
	DDRD |= (1 << DDD3);
	/* set OC1B = Arduino_Pin10 pin as output - TIMER 1 */
	DDRB |= (1 << DDB2);
	/* set OC2A = Arduino_Pin11 pin as output - TIMER 2 */
	DDRB |= (1 << DDB3);
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
