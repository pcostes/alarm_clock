// display.h
// Seven Segment Display class, Peter Costescu

#include <TimerOne.h>

#define NUM_PINS 8
#define POINT (NUM_PINS - 1) // the last segment
#define NUM_SEGMENTS 7 // point does not count as a segment
#define NUM_SECTIONS 4
#define NUM_NUMERALS 10 
#define NO_DIGIT 255 // -1 doesn't work

class Display
{
  private:
    // which segments to make numerals 0-9?
    String numbers[NUM_NUMERALS] = {"ABCDEF", "BC", "ABGED", "ABGCD", "FGBC", "AFGCD", "AFGECD", "ABC", "ABCDEFG", "ABGFC"}; //0=ABCDEF, 1 = BC, etc.
    char letters[NUM_SEGMENTS] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    bool number_patterns[10][7]; // ten numerals, seven segments
    // number patterns[0] is {1, 1, 1, 1, 1, 0} (1 for A-F, 0 for G)
    
    byte segment_pins[NUM_PINS] = {4, 5, 6, 7, 8, 9, 10, 11}; // A, B, C, D, E, F, G, P    
    byte section_pins[NUM_SECTIONS] = {12, 32, 34, 3};
    
    // data arrays
    byte data[NUM_SECTIONS] = {NO_DIGIT, NO_DIGIT, NO_DIGIT, NO_DIGIT};
    bool dps[NUM_SECTIONS] = {false, false, false, false}; // points have their own array
    bool blinking_state = false;
    int section = 0;
    
    // pwm brightnes
    byte pwm_counter = 0;
    byte pwm_max = 20;
    byte pwm_thresh = 5; // integer arithmetic for threshold, not floating duty cycle

    void refresh_display();
    void write_digit(byte section, byte digit, bool dps_on);
    static void interrupt_callback();
    static Display *current_disp;

    // blinking
    int m_digit_to_blink = -1;
    unsigned long last_blinked = 0;
  public:
    void init();
    void set_data_ind(byte ind, byte val);
    void set_data_time(byte hrs, byte mns);
    void set_data(byte in_data[NUM_SECTIONS]);
    void set_dp(byte ind, bool val);
    void set_dps(bool vals[NUM_SECTIONS]);
    void set_brightness(int pot);
    int set_digit_to_blink(int d);
    int blink_right();
};
