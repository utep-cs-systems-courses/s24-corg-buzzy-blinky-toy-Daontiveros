//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "buzzer.h"

// define the buttons
#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8
#define SWITCHES 15


int mainNum = 1;

static char switch_update_interrupt_sense(){
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);/* if switch down, sense up */
  return p2val;
}



void switch_init(){
  P2REN |= SWITCHES;/* enables resistors for switches */
  P2IE |= SWITCHES;/* enable interrupts from switches */
  P2OUT |= SWITCHES;/* pull-ups for switches */
  P2DIR &= ~SWITCHES;/* set switches' bits for input */
  switch_update_interrupt_sense();
}

// the main
void main(void) {
  configureClocks();
  switch_init();
  // the WDT
  enableWDTInterrupts();
  // leds off at first
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;
  // the buzzer
  buzzer_init();
  or_sr(0x18); 
} 

// define states with numbers so that state machine switches
#define STATE_ONE 0
#define STATE_TWO 1
#define STATE_THREE 2
#define STATE_FOUR 3
#define STATE_MAX 3

// the maximum case of different state machines
#define SONG_ONE_MAX 7
#define SONG_TWO_MAX 3
#define SONG_THREE_MAX 7
#define SONG_FOUR_MAX 16

// variables that change from 0 to maximum every second
// it's used to switch between the cases of many state machines

static char change_state = STATE_ONE;

static int change_state_machine = 0;

static int song_one_change_state = 0;
static int song_two_change_state = 0;
static int song_three_change_state = 0;
static int song_four_change_state = 0;

// turn on green light
// 1 for on, 0 for off
int green(int on){
 if(on){
   P1OUT |= LED_GREEN;
 }else{
   P1OUT &= ~LED_GREEN;
 }
}

// turn on red light
// 1 for on, 0 for off
int red(int on){
  if(on){
    P1OUT |= LED_RED;
  }else{
    P1OUT &= ~LED_RED;
  }
}

// turn red and green light
void both(){
  P1OUT |= LED_RED;
  P1OUT |= LED_GREEN;
  
}

// toggle the lights in one way
int toggle_led_one(){
  switch(change_state){
  case STATE_ONE:
    red(1);
    green(0);
    break;
  case STATE_TWO:
    red(0);
    green(1);
    break;
  case STATE_THREE:
    both();
    break;
  case STATE_FOUR:
    red(0);
    green(0);
    break;
  }
}

// toggle the lights in another way
int toggle_led_two(){
  switch(change_state){
  case STATE_ONE:
    both();
    break;
  case STATE_TWO:
    red(0);
    green(0);
    break;
  case STATE_THREE:
    red(1);
    green(0);
    break;
  case STATE_FOUR:
    red(0);
    green(1);
    break;
  }
}

// change the lights in another way
int toggle_led_three(){
  switch(change_state){
  case STATE_ONE:
    green(0);
    red(0);
    red(1);
    break;
  case STATE_TWO:
    both();
    break;
  case STATE_THREE:
    red(0);
    green(0);
    green(1);
    break;
  case STATE_FOUR:
    green(0);
    both();
    break;
 }
}

// toggle the lights in another way
 int toggle_led_four(){
   switch(change_state){
   case STATE_ONE:
     green(1);
     green(0);
     break;
   case STATE_TWO:
     green(1);
     green(0);
     break;
   case STATE_THREE:
     red(1);
     red(0);
     break;
   case STATE_FOUR:
     red(1);
     red(0);
     break;
   }
 }

// state machine changes every second
// it changes the frequencies
// from case 0 to case 7
int song_one(){
  switch(song_one_change_state){
  case 0:
    buzzer_set_period(164);
    break;   
  case 1:
    buzzer_set_period(329);
    break;
  case 2:
    buzzer_set_period(164);
    break;
  case 3:
    buzzer_set_period(329);
    break;
  case 4:
    buzzer_set_period(164);
    break;
  case 5:
    buzzer_set_period(329);
    break;
  case 6:
    buzzer_set_period(164);
    break;
  case 7:
    buzzer_set_period(329);
    break;
 }
}

// state machine changes from case 0 to case 3
// changes frequencies
int song_two(){
  switch(song_two_change_state){
  case 0:
    buzzer_set_period(659);
    break;
  case 1:
    buzzer_set_period(987);
    break;
  case 2:
    buzzer_set_period(440);
    break;
  case 3:
    buzzer_set_period(987);
    break;
  }
}

// state machine changes from case 0 to case 7
// changes frequencies
int song_three(){
  switch(song_three_change_state){
  case 0:
    buzzer_set_period(880);
    break;
  case 1:
    buzzer_set_period(880);
    break;
  case 2:
    buzzer_set_period(933);
    break;
  case 3:
    buzzer_set_period(880);
    break;
  case 4:
    buzzer_set_period(880);
    break;
  case 5:
    buzzer_set_period(1975);
    break;
  case 6:
    buzzer_set_period(880);
    break;
  case 7:
    buzzer_set_period(1975);
    break;
  }
}

//state machine changes from case 0 to case 4
// changes frequencies
int song_four(){
  switch(song_four_change_state){
  case 0:
    buzzer_set_period(494);
    break;
  case 1:
    buzzer_set_period(784);
    break;
  case 2:
    buzzer_set_period(659);
    break;
  case 3:
    buzzer_set_period(880);
    break;
  case 4:
    buzzer_set_period(987);
    break;
  case 5:
    buzzer_set_period(493);
    break;
  case 6:
    buzzer_set_period(880);
    break;
  case 7:
    buzzer_set_period(784);
    break;
  case 8:
    buzzer_set_period(659);
    break;
  case 9:
    buzzer_set_period(784);
    break;
  case 10:
    buzzer_set_period(880);
    break;
  case 11:
    buzzer_set_period(698);
    break;
  case 12:
    buzzer_set_period(784);
    break;
  case 13:
    buzzer_set_period(659);
    break;
  case 14:
    buzzer_set_period(494);
    break;
  case 15:
     buzzer_set_period(523);
     break;
  case 16:
     buzzer_set_period(987);
     break;
  }
}



// This state machine changes from case 0 to case 3
// change_state_machine is the variable used to switch
// between the cases. This variable is controlled by
// the buttons in switch_interrupt_handler
int state_machine(){
  switch(change_state_machine){
  case 0:
    toggle_led_one();
    song_one();
    break;
  case 1:
    toggle_led_two();
    song_two();
    break;
  case 2:
    toggle_led_three();    
    song_three();
    break;
  case 3:
    toggle_led_four();
    song_four();
    break;
  }
 }

// the variables that switch the cases
// of all the state machines are changed
// every second until they find their limit
// after they find their limit, the variables
// are reset
int sec(){
  change_state++;
  if(change_state >= STATE_MAX){
    change_state = 0;
  }
  song_one_change_state++;
  if(song_one_change_state >= SONG_ONE_MAX){
    song_one_change_state = 0;
  }
  song_two_change_state++;
  if(song_two_change_state >= SONG_TWO_MAX){
    song_two_change_state = 0;
  }
  song_three_change_state++;
  if(song_three_change_state >= SONG_THREE_MAX){
    song_three_change_state = 0;
  }
  song_four_change_state++;
  if(song_four_change_state >= SONG_FOUR_MAX){
    song_four_change_state = 0;
  }
  
 }

// every second call sec()
int update_250th(){
    static int sec_count = 0; //remember value last time it was called
    sec_count++;
    if(sec_count == 100){
      sec_count = 0;
      sec();
    }
}

int switches = 0;

// if S1, S2, S3, S4 are pressed they will change
// the variable from the state machine
// thus changes the cases of 'state machine'
void switch_interrupt_handler(){
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
  
  if(switches & SW1) {
    change_state_machine = 0;
  }

  if(switches & SW2){
    change_state_machine = 1;
  }

  if(switches & SW3){
    change_state_machine = 2;
  }

  if(switches & SW4){
    change_state_machine = 3;
  }
}

void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {     
    P2IFG &= ~SWITCHES;     
    switch_interrupt_handler();
  }
}

void __interrupt_vec(WDT_VECTOR) WDT(){
  update_250th(); // every second changes all the variables that switch the cases
  state_machine(); // state machine controls 'state machine' function
}


