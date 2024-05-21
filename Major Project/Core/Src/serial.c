#include "serial.h"

#include "stm32f303xc.h"

// NOTE: these are stored as pointers because they
//       are const values so we can't store them directly
//       in the struct
struct _SerialPort {
	volatile uint32_t *BaudRate;
	volatile uint32_t *ControlRegister1;
	volatile uint32_t *FlagClearRegister;
	volatile uint32_t *StatusRegister;
	volatile uint16_t *DataOutputRegister;
	volatile uint16_t *DataInputRegister;
	volatile uint32_t *TimerEnableRegister;
	volatile uint32_t TimerEnableMask;
	volatile uint32_t SerialPortGPIO;
	volatile uint32_t *SerialPinModeRegister;
	volatile uint32_t SerialPinModeValue;
	volatile uint32_t *SerialPinSpeedRegister;
	volatile uint32_t SerialPinSpeedValue;
	volatile uint8_t *SerialPinAlternatePinRegister;
	volatile uint8_t SerialPinAlternatePinValue;
	void (*completion_function)(uint32_t);
};


// The different serial ports require different GPIO ports
enum {
	SERIAL_GPIO_A,
	SERIAL_GPIO_B,
	SERIAL_GPIO_C,
	SERIAL_GPIO_D,
	SERIAL_GPIO_E
};



// instantiate the serial port parameters
//   note: the complexity is hidden in the c file
SerialPort USART1_PORT = {&(USART1->BRR),
		&(USART1->CR1),
		&(USART1->ICR),
		&(USART1->ISR),
		&(USART1->TDR),
		&(USART1->RDR),
		&(RCC->APB2ENR),
		RCC_APB2ENR_USART1EN,
		SERIAL_GPIO_C,
		&(GPIOC->MODER),
		0xA00,
		&(GPIOC->OSPEEDR),
		0xF00,
		((uint8_t*)&(GPIOC->AFR[0])) + 2,
		0x77};

SerialPort UART4_PORT = {&(UART4->BRR),
		&(UART4->CR1),
		&(UART4->ICR),
		&(UART4->ISR),
		&(UART4->TDR),
		&(UART4->RDR),
		&(RCC->APB1ENR),
		RCC_APB1ENR_UART4EN,
		SERIAL_GPIO_C,
		&(GPIOC->MODER),
		0xA00000,
		&(GPIOC->OSPEEDR),
		0xF00000,
		((uint8_t*)&(GPIOC->AFR[1])) + 1,
		0x55};

// InitialiseSerial - Initialise the serial port
// Input: baudRate is from an enumerated set
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint32_t)) {

	serial_port->completion_function = completion_function;

	// enable clock power, system configuration clock and GPIOC
	// common to all UARTs
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	switch(serial_port->SerialPortGPIO) {
	case SERIAL_GPIO_C:
		RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
		break;
	default:
		break;
	}

	// set pin mode
	*(serial_port->SerialPinModeRegister) |= serial_port->SerialPinModeValue;

	// enable high speed clock for GPIOC
	*(serial_port->SerialPinSpeedRegister) |= serial_port->SerialPinSpeedValue;

	// set alternate function to enable USART to an external pin
	*(serial_port->SerialPinAlternatePinRegister) |= serial_port->SerialPinAlternatePinValue;

	*(serial_port->TimerEnableRegister) |= serial_port->TimerEnableMask;

	uint16_t *baud_rate_config = ((uint16_t*)serial_port->BaudRate); // only 16 bits used!

	// Baud rate calculation from datasheet
	switch(baudRate){
	case BAUD_9600:
		*baud_rate_config = 0x341 * 0x06; // 9600 at 8MHz
		break;
	case BAUD_19200:
		*baud_rate_config = 0x1A1 * 0x06; // 19200 at 8MHz
		break;
	case BAUD_38400:
		*baud_rate_config = 0xD0 * 0x06;  // 38400 at 8MHz
		break;
	case BAUD_57600:
		*baud_rate_config = 0x8B * 0x06;  // 57600 at 8MHz
		break;
	case BAUD_115200:
		*baud_rate_config = 0x46 * 0x06;  // 115200 at 8MHz
		break;
	}


	// enable serial port for tx and rx
	*(serial_port->ControlRegister1) |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}


void SerialOutputChar(uint8_t data, SerialPort *serial_port) {
	while((*(serial_port->StatusRegister) & USART_ISR_TXE) == 0){
	}

	*(serial_port->DataOutputRegister) = data;
}



void SerialOutputString(uint8_t *pt, SerialPort *serial_port) {

	uint32_t counter = 0;
	while(*pt) {
		SerialOutputChar(*pt, serial_port);
		counter++;
		pt++;
	}

	if (serial_port->completion_function != 0x00)
		serial_port->completion_function(counter);
}


uint8_t SerialInputChar(SerialPort *serial_port)
{
	while (*(serial_port->StatusRegister) & USART_ISR_ORE || *(serial_port->StatusRegister) & USART_ISR_FE)
	{
		*(serial_port->FlagClearRegister) |= USART_ICR_ORECF | USART_ICR_FECF;
	}

	while((*(serial_port->StatusRegister) & USART_ISR_RXNE) == 0)	{};

	//read in a character at a time and return it
	uint8_t character = *(serial_port->DataInputRegister);
	return character;
}

uint8_t* SerialInputString(uint8_t* buffer, uint32_t buffer_size, SerialPort *serial_port, uint8_t termination_char)
{
	//characters are only read until the buffer is full or a termination char is sent
	uint32_t counter = 0;
	for (uint32_t i = 0; i < buffer_size/sizeof(uint8_t); i++)
	{
		uint8_t character = SerialInputChar(serial_port);
		buffer[i] = character;
		counter++;
		if (character == termination_char)
		{
			break;
		}
	}
	//callback is called if not equal to NULL
	if (serial_port->completion_function != 0x00)
	{
		serial_port->completion_function(counter);
	}
	return buffer;
}

