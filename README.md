# MTRX2700 Mechatronics 2
# Major Project

## Role allocations:
- Omar Abdulahlim [530281373]
- Dinh Quang Huynh [530128207]
- Lucas Ma [520469505]
- Jayden Wu [530518040]
- Lovedale Njaravaza [530360623]

## Modules:

<ins> Heads Up <ins>


Requirement Specification: Players each play three rounds, guessing words that they cannot see, based on hints provided by the other player. This is done by holding an STM32 board to a player’s forehead , and tilting it downwards for a correct guess, and upwards for an incorrect guess. This is measured by using the gyroscope on the STM32 board.

The requirement functionality is as follows:
- Player presses the user input button to signify beginning of the round
- The STM32 board reads the tilting gesture for “Correct” or ‘“Pass” from the user
- User interface displays the words for the player to guess
- Each round is timed precisely to ensure consistency in games 
- The results of each round are stored and used to calculate which player can progress to Wavelength.

System Design: The primary file is headsUp.c, which stores all the possible guessing words in the function randomWord(). The headsUp function orchestrates the game flow for two players, each taking turns. The game starts a timer for each player using the game_timer function and sets a flag when the time is up. Players' movements are monitored via the gyroscope, with different actions (correct, pass) triggering corresponding LED signals and messages via the serial interface. These scores are tracked and displayed after each turn, and user inputs are managed through GPIO.

Detailed Design: 
- The randomWord function generates unique random words using a static array. It will track used words to prevent repetition within a round.
- The headsUp function manages the game flow, which begins with function game_timer, called from timer.c, and sets a 30-second timer for each turn, with a flag indicating when time is up.
- The random words are sent to the player via SerialOutputString, with the code monitoring gyroscope values in get_gyro_values to determine player actions (correct, pass).
- The score is updated in outcome, which is then sent through to SerialOutputString to display, and the LEDs for the correct or incorrect guess is displayed in its respective pattern.
Each player’s score is then updated, in *P1score = Pscore and *P2score = Pscore.


<ins> Wavelength <ins>


Requirement Specification: The objective of wavelength is for two players to align their thinking. There is a card with two opposite concepts(e.g. hot and cold). One player secretly dials in their rating on where they think the topic falls on the scale. The other player has to put their maker on top of the first player's maker. The closer the marker is, the more points the player gets.

- Read the user’s physical input between a range of values
- Physically display the user’s input
- Send the final score to the main game

System Design: The code uses functions to turn on the ADC converters. System also uses timers to aid in creating a PWM signal to control the servos. The system calculates the difference between given inputs, determines the score and sends this to another board.


Detailed Design:
- Stm32 starts the ADC converters using HAL_TIM_PWM_Start(), and the PWM signal is started using the function HAL_TIM_PWM_Start()
- It then enter a while loop and continuously read the value of the two ADCs and set them to given variable 
- From these given variables we set the PWM signal to those values by setting the values to TIM1 and TIM2, that in turns sets the positioning of the servos.
- A condition is used to check if the user has pressed a button if so it calculates how close the two ADC values are to each other and outputs an appropriate score.
- The score is then sent to another board using UART4 so that the score goes to the main game.


<ins> Roulette Wheel <ins>


Requirement Specification: The objective of the roulette wheel is to allow the winning player to spin the wheel, and determine their prize. This is done by pressing the user button on the STM32, which will trigger the LEDs to spin in a clockwise motion.

The requirement functionality is as follows:

- Wait until the user button is pressed.
- Rotate the LEDs five times around the board, with a deceleration effect which slows the LEDs down gradually
- Using the gyroscope values, generate a random number, which will correspond to one of the LEDs from 1 to 8, and ultimately the prize.


System Design: The roulette wheel is handled completely in roulette.c. Within this file, the module uses functions from timer.c to delay the LED animation. To create the chasing effect of the LED, a function is used from digital_io.c. Ultimately, in order to generate the random number for the prize, the module uses serial.c to use the random gyroscope value. It will then send the output to main.c by using USART1.


Detailed Design: 
- The code loops and waits until the button connected to pin 0 of GPIOA is pressed. Then it will loop around the LED wheel five times, lighting all eight LEDs in each loop, one by one. A deceleration effect is triggered by the delay function which slows the following LED down by delay_period of 25 millisecond.
- A buffer called string_to_send is created, which will store the value of ‘r’. This is the variable that stores the random gyro seed value, which is initialised in main.c in srand(get_gyro_values()). A pointer called prize corresponds to the variable r, so that the function calls immediately to that section of the code for better optimisation. 
- The string_to_send is then passed through the function SerialOutputString, and displayed on the computer screen. Using a bitmask, the chosen random prize LED is displayed with the function set_leds(mask).

## Instructions for the user:

## Testing:

## Limitations and Constraints:
