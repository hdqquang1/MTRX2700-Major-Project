# MTRX2700 Mechatronics 2
# Major Project

## Role allocations:

|Module|Sub-module|Contributions|
|------------|------------------------|---------------|
|Heads-up|Gyroscope|Omar, Dinh|
||LEDs on board|Lucas|
||Timers|Dinh|
||USART1|Dinh, Omar|
|Wavelength|Potentiometer|Lovedale|
||Servo|Lovedale|
||PWM|Lovedale|
||UART4|Dinh, Omar|
|Roulette Prize Wheel|Prize Allocation|Lucas|
||Timers|Lucas|
||Roulette Wheel LED|Lucas|
|3D Print Design|Wavelength game|Lovedale|
||STM32 Heads-up case|Lucas|
|GUI|Heads up Display|Jayden|
||Data Representation |Jayden|
||Leaderboard|Jayden|




- Omar Abdulahlim [530281373]
- Dinh Quang Huynh [530128207]
- Lucas Ma [520469505]
- Jayden Wu [530518040]
- Lovedale Njaravaza [530360623]

## System Block Diagram:

![Alt text]([https://private-user-images.githubusercontent.com/160550731/332243981-142e6c0b-edb5-4cef-9219-1b1078347925.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MTYyNTYyMTIsIm5iZiI6MTcxNjI1NTkxMiwicGF0aCI6Ii8xNjA1NTA3MzEvMzMyMjQzOTgxLTE0MmU2YzBiLWVkYjUtNGNlZi05MjE5LTFiMTA3ODM0NzkyNS5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjQwNTIxJTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI0MDUyMVQwMTQ1MTJaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT0xNTM4M2M4YjE4NDU0OTA2OGE5Mjg2NmE0NmI2ZGE5NTc2NDliODM3ZGI5NTZhODkwNDI5YmI2MDQ1MGE5NDlhJlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCZhY3Rvcl9pZD0wJmtleV9pZD0wJnJlcG9faWQ9MCJ9.O8e61FnW9GUoFxkesprpA2jHbgdGurwdPObEJOozJLE)](https://github.com/hdqquang1/MTRX2700-Major-Project/issues/8#issue-2307261946))


## Modules:

<ins> Heads Up <ins>


Requirement Specification: 
Players each play three rounds, guessing words that they cannot see, based on hints provided by the other player. This is done by holding an STM32 board to a player’s forehead , and tilting it downwards for a correct guess, and upwards for an incorrect guess. This is measured by using the gyroscope on the STM32 board.

The requirement functionality is as follows:
- Player presses the user input button to signify beginning of the round
- The STM32 board reads the tilting gesture for “Correct” or ‘“Pass” from the user
- User interface displays the words for the player to guess
- Each round is timed precisely to ensure consistency in games 
- The results of each round are stored and used to calculate which player can progress to Wavelength.

System Design: 
The primary file is headsUp.c, which stores all the possible guessing words in the function randomWord(). The headsUp function orchestrates the game flow for two players, each taking turns. The game starts a timer for each player using the game_timer function and sets a flag when the time is up. Players' movements are monitored via the gyroscope, with different actions (correct, pass) triggering corresponding LED signals and messages via the serial interface. These scores are tracked and displayed after each turn, and user inputs are managed through GPIO.

Detailed Design: 
- The randomWord function generates unique random words using a static array. It will track used words to prevent repetition within a round.
- The headsUp function manages the game flow, which begins with function game_timer, called from timer.c, and sets a 30-second timer for each turn, with a flag indicating when time is up.
- The random words are sent to the player via SerialOutputString, with the code monitoring gyroscope values in get_gyro_values to determine player actions (correct, pass).
- The score is updated in outcome, which is then sent through to SerialOutputString to display, and the LEDs for the correct or incorrect guess is displayed in its respective pattern.
Each player’s score is then updated, in *P1score = Pscore and *P2score = Pscore.

|Function Call|Arguments|Returns|Function Description|
|-------------|---------|-------|--------------------|
|randomWord()|VOID|char *words| Return an array of random words from a bigger predefined pool|
|headsUp()|uint8_t *P1score, uint8_t *P2score|VOID|Heads Up game and updating the score of two players after the game|


_______________________


<ins> Wavelength <ins>


Requirement Specification: 
The objective of wavelength is for two players to align their thinking. There is a card with two opposite concepts(e.g. hot and cold). One player secretly dials in their rating on where they think the topic falls on the scale. The other player has to put their maker on top of the first player's maker. The closer the marker is, the more points the player gets.
- Read the user’s physical input between a range of values
- Physically display the user’s input
- Send the final score to the main game

System Design: 
The code uses functions to turn on the ADC converters. System also uses timers to aid in creating a PWM signal to control the servos. The system calculates the difference between given inputs, determines the score and sends this to another board.


Detailed Design:
- Stm32 starts the ADC converters using HAL_TIM_PWM_Start(), and the PWM signal is started using the function HAL_TIM_PWM_Start()
- It then enter a while loop and continuously read the value of the two ADCs and set them to given variable 
- From these given variables we set the PWM signal to those values by setting the values to TIM1 and TIM2, that in turns sets the positioning of the servos.
- A condition is used to check if the user has pressed a button if so it calculates how close the two ADC values are to each other and outputs an appropriate score.
- The score is then sent to another board using UART4 so that the score goes to the main game.


_______________________

<ins> Roulette Wheel <ins>


Requirement Specification: 
The objective of the roulette wheel is to allow the winning player to spin the wheel, and determine their prize. This is done by pressing the user button on the STM32, which will trigger the LEDs to spin in a clockwise motion.

The requirement functionality is as follows:
- Wait until the user button is pressed.
- Rotate the LEDs five times around the board, with a deceleration effect which slows the LEDs down gradually
- Using the gyroscope values, generate a random number, which will correspond to one of the LEDs from 1 to 8, and ultimately the prize.


System Design: 
The roulette wheel is handled completely in roulette.c. Within this file, the module uses functions from timer.c to delay the LED animation. To create the chasing effect of the LED, a function is used from digital_io.c. Ultimately, in order to generate the random number for the prize, the module uses serial.c to use the random gyroscope value. It will then send the output to main.c by using USART1.


Detailed Design: 
- The code loops and waits until the button connected to pin 0 of GPIOA is pressed. Then it will loop around the LED wheel five times, lighting all eight LEDs in each loop, one by one. A deceleration effect is triggered by the delay function which slows the following LED down by delay_period of 25 millisecond.
- A buffer called string_to_send is created, which will store the value of ‘r’. This is the variable that stores the random gyro seed value, which is initialised in main.c in srand(get_gyro_values()). A pointer called prize corresponds to the variable r, so that the function calls immediately to that section of the code for better optimisation. 
- The string_to_send is then passed through the function SerialOutputString, and displayed on the computer screen. Using a bitmask, the chosen random prize LED is displayed with the function set_leds(mask).

|Function Call|Arguments|Returns|Function Description|
|-------------|---------|-------|--------------------|
|roulette()|uint8_t *prize|VOID|Spins the LEDs in a decelerating motion and stores the random prize value in string_to_send, which is then displayed using a bitmask.|

_______________________

<ins>Computer Interface<ins>


Requirement Specification:
The objective of the computer interface is to allow for the user to see the front end of the task (heads up, leaderboard and timer) but also show some back end information/data that will be useful in understanding the movements of the system in a physical representation. 


System Design: 
As the game begins, the user will be greeted with a message and the game Heads Up will start, a word will need to be acted out by a user (player 2) and another player (player 1) will need to guess this word based on the act. Player 1 will flip the microcontroller down for correction and up to pass to the next word. This is all shown on the computer interface where the screen will light up green for correct and red for pass. Furthermore, the timer module used throughout the game will be shown where the amount of time left the player has is displayed. This has been done through python where the serial data from the microcontroller is being sent to the python program. 

This computer interface involves several components working together however the main components are:
- Serial Data Acquisition: This reads the incoming data from the serial port through the use of a separate thread to ensure the interface remains responsive.
- Data Processing: The received data is processed to determine the data type, if it is sensor data, game messages, leaderboard values etc. 
- GUI components: The interface is divided into four main sections: top left (status messages), top right (leaderboard), bottom left (timer), bottom right (microcontroller positioning)
- Data Display and Visualisation: This displays the processed data into their appropriate sections of the interface. 


Detailed Design: 

|Function Name|Returns|Function Description|
|-------------|-------|--------------------|
|read_serial|None |Reads data from the serial, this will use regex to determine if the data is sensor data (gyro and acc) and process it accordingly/ A callback is used to update display, plot, leaderboard and timer based on data received. |
|create_display|tinker root window, update_display_callback, update_plot_callback, update_leaderboard_callback|Creates a frame for the different sections of the computer interface.|
|draw_prism|Prism object, Vertices array|Defines vertices, faces and edge colours of a rectangular prism. It will return the prism and the vertices array to be used. |
|rotate_prism|None|Rotates the vertices of the prism based on the pitch and roll angles, it will update the position of the prism on the 3D axes. |
|update_display|None|This updates the display of the heads up game, it will also change the colour of the background for messages like “Correct!” and “Pass!” |
|update_plot|None|Updates the gyro and accelerometer plots based on data received. An exponential moving average is applied to the accelerometer data to smoothen the plot due to unwanted noise. |
|update_leaderboard|None|Based on the data received, the score is updated to the leaderboard. |
|periodic_update |None|This was used to keep the interface responsive, without this the 3D model will not run properly. |



## Specific Modules:

<ins>initialise.c<ins>

|Function Call|Arguments|Returns|Function Description|
|-------------|---------|-------|--------------------|
|initialise_board()|VOID|VOID|Initialise all the necessary modules|


<ins>digital_io.c<ins>
  
|Function Call|Arguments|Returns|Function Description|
|-------------|---------|-------|--------------------|
|set_leds()|uint8_t led_mask_pattern|VOID|Set LEDs pattern by binary number|
|get_leds()|VOID|uint8_t led_state|Get LEDs current state|
|correct_leds()|VOID|VOID|Turn on circle LEDs pattern for 1s|
|incorrect_leds()|VOID|VOID|Turn on cross LEDs pattern for 1s|
|end_leds()|VOID||VOID|Turn on circle LEDs pattern for 2s|
|chase_leds()|VOID|VOID|Shift LED 1 by 1|

<ins>gyroscope.c<ins>

|Function Call|Arguments|Returns|Function Description|
|-------------|---------|-------|--------------------|
|get_gyro_values|VOID|float pitch_gyro_value|Get pitch gyroscope value|

<ins>serial.c<ins>

|Function Call|Arguments|Returns|Function Description|
|-------------|---------|-------|--------------------|
|SerialInitialise()|BAUD_XXXX, SerialPort *serial_port|VOID|Initialise the serial port|
|SerialOutputString|uint8_t *string|VOID|Output a null terminated string to the serial port|
|SerialInputString|uint8_t *buffer, uint32_t buffer_size, SerialPort *serial_port, uint8_t terminatioin_char|uint8_t *string|Input a string from the serial port|

<ins>timer.c<ins>

|Function Call|Arguments|Returns|Function Description|
|-------------|---------|-------|--------------------|
|game_timer()|uint16_t time, void (*callback_function)|VOID|Timing each round of the game using interrupt, the input time is in millisecond|
|delay()|uint16_t delay|VOID|Delay for a period of time using polling method, the input time is in millisecond|





## Testing:


USART1: Testing that the serial stream to PuTTY works.

```C
initialise_board();

uint8_t string_to_send[64] = "This is a string!\r\n";
sprintf(string_to_send, "Testing USART1!\r\n");
SerialOutputString(string_to_send, &USART1_PORT);

```
- The string “Testing USART1!” should be outputted to PuTTY.
_______________

UART4: Testing that the serial stream between boards works.

Board Sending:
```C
initialise_board();


uint8_t string_to_send[64] = "This is a string!\r\n";
sprintf(string_to_send, "Testing UART4!\r\n");
SerialOutputString(string_to_send, &UART4_PORT);
```
Board Receiving: The string “Testing UART4!” should be in the buffer’s memory browser.
```C
initialise_board();


uint8_t buffer[8];
while (!buffer){
	SerialInputString(buffer, 8, &UART4_PORT, '\r');
}
```

_______________

Gyroscopic Sensors: Testing that board xyz sensors work.

```C
initialise_board();
uint8_t string_to_send[64] = "This is a string !\r\n";
float gyro_values[3];


/* Infinite loop */
while (1){
BSP_GYRO_GetXYZ(&gyro_values[0]);
sprintf(string_to_send, "%0.3f,%0.3f,%0.3f,%0.3f,%0.3f,%0.3f\r\n",
gyro_values[0]/20000, gyro_values[1]/20000, gyro_values[2]/20000);
SerialOutputString(string_to_send, &USART1_PORT);
}
```
_______________
Timers: Testing that the timer is accurate. 
```C

void set_timesUp(){
   if (timesUp == 0) {
       timesUp = 1;
   }
   else {
       timesUp = 0;
   }
}


uint8_t timesUp = 0;
game_timer(10000, &set_timesUp);
while (timesUp == 0){
	uint8_t string_to_send[64] = "This is a string!\r\n";
sprintf(string_to_send, "Testing Timer!\r\n");
SerialOutputString(string_to_send, &UART4_PORT);
delay(1000);
}
```
PuTTY should output "Testing Timer!" 10 times to screen.

_______________





## Limitations and Constraints:

- Instead of using static variables, consider the use of structs. This will allow for better modularity and inter-use between all codes.
- Although currently done in headsUp.c, make clearing memory more consistent throughout all files.
- The random seed value generated is limited to the value of the gyroscope, which may result in inconsistent results if the board is moved in the same motion.
- A few wires are soldered, which can cause for inconsistent performance whereby greater noise can exist. This may result in the serial communication not performing as precisely as it can.
- A constraint is that two laptops are used, however simplify the process with one laptop would be better.

## Acknowledgement:

[MTRX2700 Lecture examples, adapted from Stewart Worrall] (https://github.com/stewart-worrall/MTRX2700-2024)

[STM32F303-specific-datasheet.pdf] (https://canvas.sydney.edu.au/courses/55768/files/34751258?wrap=1)

[STM32F3-Reference-Manual-large.pdf] (https://canvas.sydney.edu.au/courses/55768/files/34751264?wrap=1)


