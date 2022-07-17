/* USER CODE BEGIN Header */
/** FILE          : main.c
 PROJECT       : PROG8125
 PROGRAMMER    : R. Hofer
 FIRST VERSION : 2016-02-0
 REVISED:	   : 2017-01-23 - Changed to run on the Nucleo-32 and in TrueSTUDIO
 REVISED:	   : 2019-03-27 Allan Smith - Changed to work with v 9.20 of TrueSTUDIO and cubemx32 5.1
 and fix a number of minor issues
 2020-11-18 Allan Smith - fixed to work with CubeMx ide 1.4.2
 DESCRIPTION   : Demonstrates a debit machine banking transaction that implements a state machine.
 State machine diagram is found on slide 14 of Week 9 Switch Statement and State Machine.pptx

 Switches are assigned as follows
 note: these pins are set in the debounceInit functions and do not need to be configured in cube
 PA0			PA1			PA4			PA3
 chequing		savings		ok			cancel

 Note: Don't use PA2 as it is connected to VCP TX and you'll
 lose printf output ability.


 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/*******************************************************************************
 * File Name          : <assignment4.c>
 * Description        : <DEBIT MACHINE>
 *
 * Author:            : <Jeevan Henry Dsouza>
 * Student number:    : <8716171>
 * Date:              : <15-08-2021>
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <unistd.h>
#include <stdio.h>
#include "debounce.h"
#include "HD44780.h"
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
static const int16_t chequingPbPin = 0; //setting the pin assigned to each pb
static const int16_t savingsPbPin = 1;		//don't use pin 2 as it's connected
static const int16_t okPbPin = 4;		//to VCP TX
static const int16_t cancelPbPin = 3;

enum pushButton {
	none, chequing, savings, ok, cancel
};
//enumerated values for use with if
//(pbPressed == value) type conditional
//statements

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// FUNCTION      : setTone
// DESCRIPTION   : Calculates the PWM Period needed to obtain the freq
//				 : passed and the duty cycle of the PAM to
//				 : 50% (1/2 of the period)
// PARAMETERS    : int32 freq - frequency of the output
// RETURNS       : nothing
void setTone(int32_t freq) {
	int32_t pwmPeriod = 1000000000 / (freq * 250); //value can vary between 2 and 65535
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = pwmPeriod;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = pwmPeriod / 2;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	/* adding this as ST Tech Support said PWM should be stopped before
	 * calling HAL_TIM_PWM_ConfigChannel and I've been getting flakey start-up
	 * i.e.: sometime PWM starts up, other times the line remains stuck high.
	 **************************************/
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	/*************************************/
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}

	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	HAL_TIM_MspPostInit(&htim1);
}

// FUNCTION      : waitForPBRelease
// DESCRIPTION   : Loops until the PB that is currently
//				 : pressed and at a logic low
//				 : is released. Release is debounced
// PARAMETERS    : pin - pin number
//                 port- port letter ie 'A'
// RETURNS       : nothing
void waitForPBRelease(const int16_t pin, const char port) {
	while (deBounceReadPin(pin, port, 10) == 0) {
		//do nothing wait for key press to be released
	}
}

// FUNCTION      : startUpLCDSplashScreen()
// DESCRIPTION   : displays Debit Demo for 2s
//                 on line 1 of the display and
//				 : Disappears
// PARAMETERS    : None
// RETURNS       : nothing
void startUpLCDSplashScreen(void) {
	char stringBuffer[16] = { 0 };
	HD44780_GotoXY(0, 0);
	snprintf(stringBuffer, 16, "   Debit Demo");
	HD44780_PutStr(stringBuffer);
	HAL_Delay(2000);
	HD44780_ClrScr();
}

// FUNCTION      : pulsePWM
// DESCRIPTION   : Turns on the PWM for the pulseTime in ms
//                 provided and then turns off PWM
// PARAMETERS    : address of Timer Handle var (e.g.: &htim1)
//                 pulseTime in ms
// RETURNS       : nothing
void pulsePWM(TIM_HandleTypeDef *htim1, int32_t pulseTime) {
	HAL_TIMEx_PWMN_Start(htim1, TIM_CHANNEL_1);
	HAL_Delay(pulseTime);
	HAL_TIMEx_PWMN_Stop(htim1, TIM_CHANNEL_1);
}

//  FUNCTION      : pushButtonInit
//   DESCRIPTION   : Calls deBounceInit to initialize ports that
//                   will have pushbutton on them to be inputs.
//			         Initializing PA0,PA1,PA4 and PA3
//                   Switches are assigned as follows
//                   PA0			PA1			PA4			PA3
//                   chequing		savings		ok			cancel
//
//                   Note: Don't use PA2 as it is connected to VCP TX and you'll
//                   lose printf output ability.
//   PARAMETERS    : None
//   RETURNS       : nothing
void pushButtonInit(void) {
	deBounceInit(chequingPbPin, 'A', 1); 		//1 = pullup resistor enabled
	deBounceInit(savingsPbPin, 'A', 1); 		//1 = pullup resistor enabled
	deBounceInit(okPbPin, 'A', 1); 			//1 = pullup resistor enabled
	deBounceInit(cancelPbPin, 'A', 1); 		//1 = pullup resistor enabled
}

// FUNCTION      : displayWelcome()
// DESCRIPTION   : clears the LCD display and displays
//                 Welcome on line 1 of the display
// PARAMETERS    : None
// RETURNS       : nothing
void displayWelcome(void) {
	char stringBuffer[16] = { 0 };
	HD44780_ClrScr();
	snprintf(stringBuffer, 16, "Welcome ");
	HD44780_PutStr(stringBuffer);
}

// FUNCTION      : displayAmount()
// DESCRIPTION   : clears the LCD display and displays
//                 the $amount received on line 1 of the display
// PARAMETERS    : float - amount to display
// RETURNS       : nothing
void displayAmount(float amount) {
	char stringBuffer[16] = { 0 };
	HD44780_ClrScr();
	snprintf(stringBuffer, 16, "$%.2f", amount);
	HD44780_PutStr(stringBuffer);
}

// FUNCTION      : checkIfAmountRecd()
// DESCRIPTION   :
// PARAMETERS    : none
// RETURNS       : float, the amount in $ to be debited
float checkIfAmountRecd() {
	float debitAmount = 0;
	printf("waiting for debitAmount to be received on serial port\r\n");
	int16_t result = 0;
	result = scanf("%f", &debitAmount);
	if (result == 0)		//then somehow non-float chars were entered
			{						//and nothing was assigned to %f
		fpurge(STDIN_FILENO); //clear the last erroneous char(s) from the input stream
	}
	return debitAmount;
}

// FUNCTION      : checkOkOrCancel()
// DESCRIPTION   : Checks whether the OK or Cancel
//                 button has been pressed.
// PARAMETERS    : none
// RETURNS       : int8_t, 4 if cancel pressed, 3 if
//                 ok pressed. 0 returned if neither
//                 has pressed.
enum pushButton checkOkOrCancel(void) {
	if (deBounceReadPin(cancelPbPin, 'A', 10) == 0) {
		//then the cancel pushbutton has been pressed
		return cancel;
	} else if (deBounceReadPin(okPbPin, 'A', 10) == 0) {
		//then ok pressed
		return ok;
	}
	return none; //as ok or cancel was not pressed.
}
// FUNCTION      : checkSavingsOrChequing()
// DESCRIPTION   : Checks whether the chequing or savings
//                 button has been pressed.
// PARAMETERS    : none
// RETURNS       : int8_t, 1 if chequing pressed, 2 if
//                 savings pressed. 0 returned if neither
//                 has pressed.
enum pushButton checkSavingsOrChequing(void) {
	if (deBounceReadPin(chequingPbPin, 'A', 10) == 0) {
		//then the cancel pushbutton has been pressed
		return chequing;
	} else if (deBounceReadPin(savingsPbPin, 'A', 10) == 0) {
		//then ok pressed
		return savings;
	}
	return none; //as chequing or savings was not pressed.
}

// FUNCTION      : displayOkOrCancel()
// DESCRIPTION   : displays "OK or Cancel?" on line 2 of LCD
// PARAMETERS    : none
// RETURNS       : nothing.
void displayOkCancel(void) {
	char stringBuffer[16] = { 0 };
	HD44780_GotoXY(0, 1); //move to second line first position
	snprintf(stringBuffer, 16, "OK or Cancel?");
	HD44780_PutStr(stringBuffer);
}
// FUNCTION      : displayChequingOrSavings()
// DESCRIPTION   : displays "Chequing or" on line 1 and "Savings on line 2 of lcd"
// PARAMETERS    : none
// RETURNS       : nothing.
void displayChequingOrSavings() {
	HD44780_ClrScr();
	HD44780_GotoXY(0, 0);
	HD44780_PutStr("Chequing or");
	HD44780_GotoXY(0, 1);
	HD44780_PutStr("Savings?");
}

// FUNCTION      : pincode
// DESCRIPTION   : checks wether  the entered pincode is correct or not
// PARAMETERS    : none
// RETURNS       : 1 if pincode matched else when pincode is not matched returns 0
int pincode() {
	char password[10] = "1234";
	char string[10];
	int count = 5; //giving the user maximum 5 attempts to enter the correct password
	int i = 1; //loop variable
	int comparisonValue;
	while (count >= 0) {
		count = count - i;
		printf("Please enter the password\n");
		scanf("%s", string);
		comparisonValue = strcmp(password, string); //refernce taken from https://en.wikipedia.org/wiki/C_string_handling#Function_overview
		/* strcmp compares password and string lexographically and retrns 0 when both strings are same */
		if (comparisonValue == 0) {
			printf("Access Granted\n");
			return 1;
		} else {
			printf("You have entered a wrong password please try again\n");
			printf("The number of attempts left is %d\n", count);
		}

	}
	return 0;
}
// FUNCTION      : cancel()
// DESCRIPTION   : displays "transaction is cancelled" on the lcd
// PARAMETERS    : none
// RETURNS       : nothing
void cancelled(void) {
	HD44780_ClrScr();
	HD44780_GotoXY(0, 0); //move to first line first position
	HD44780_PutStr("Transaction is");
	HD44780_GotoXY(0, 1);
	HD44780_PutStr("cancelled..");
}
// FUNCTION      : authorization()
// DESCRIPTION   : checks wether the user has enough balance or not
// PARAMETERS    : amount and accountbalance
// RETURNS       : 1 if account balance is sufficient to debit the amount else returns 0
int authorization(int temp1, float temp2) {
	if ((temp1 - temp2) >= 0) {
		return 1;
	}
	return 0;
}
// FUNCTION      : printingTransaction()
// DESCRIPTION   : prints the transaction on the vcp
// PARAMETERS    : accounBalance and amount
// RETURNS       : nothing
void printingTransaction(float temp3, float temp4) {
	printf("The amount debited is $ %.2f\n", temp4);
	printf("Your Current Balance is $ %.2f\n", temp3);
	printf("Please visit us again\n");
	return;
}
// FUNCTION      : displayPincode()
// DESCRIPTION   : prints "enter pin " on the lcd
// PARAMETERS    : nothing
// RETURNS       : nothing
void displayPincode() {
	HD44780_ClrScr();
	HD44780_PutStr("enter pin");

}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	MX_TIM1_Init();
	/* USER CODE BEGIN 2 */

	printf("Debit Card State Machine\r\n");
	HD44780_Init();
	/* setup Port A bits 0,1,2 and 3, i.e.: PA0-PA3 for input */
	pushButtonInit();

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		static float accountBalance = 0; //holds the balance of the account
		static float amount = 0; //used to hold the transaction amount
		static int8_t transactionState = 1;
		enum pushButton pbPressed = none; //will hold pushbutton defined above depending on
		enum pushButton pbPressed1 = none;				//the pushbutton pressed
		int returnValue = 0;    //holds the value returned from pincode function
		int returnValue1 = 0;//holds the value returned from authorization function
		float addMoney = 0; // used as an intermediate variable to addMoney to main balance when savings pressed
		float addMoney1 = 0; //used as an intermediate variable to addMoney to main balance when chequing pressed
		/*states:   1   display Welcome Screen, wait for $ amount input from Serial port
		 2   @ amount Received, waiting for Ok or Cancel button
		 3   OK received, waiting for chequing or Savings button
		 4   C or S received, waiting for PIN to be entered from Serial Port
		 5   Pin Correct, send transaction data to bank. Waiting
		 for OK back from Bank If OK from Bank received. Print
		 Receipt, Record transaction. Move back to State 1.
		 6   Cancel Pressed. Display "Transaction Cancelled" back to state 1
		 */
		switch (transactionState) {
		case 1:
			displayWelcome();
			amount = checkIfAmountRecd(); //checking if an amount has been received
			if (amount != 0)          //returns a 0 if an transaction amount has
					{ 					//NOT been received on the serial port.
				displayAmount(amount); //but if we're we've received a debitAmount
				displayOkCancel();	 //so display it and the prompt ok or cancel
				HAL_Delay(1000);
				transactionState = 2; //and do that before we move on to state 2
			}
			break;
		case 2:
			HAL_Delay(1000);
			pbPressed = checkOkOrCancel();

			if (pbPressed != none) {
				if (pbPressed == ok) {
					printf("ok was Pressed\r\n"); 		//then ok was pressed
					displayChequingOrSavings(); //displayingChequingorSaving
					transactionState = 3;
				} else if (pbPressed == cancel) {
					printf("Cancel was Pressed\r\n"); //then cancel was pressed
					transactionState = 6;
					//more code needed to get to state 3 goes here
				}
			}

			break;
		case 3:
			HAL_Delay(1000);
			pbPressed1 = checkSavingsOrChequing();
			if (pbPressed1 != none) {
				if (pbPressed1 == savings) {
					printf("Savings was pressed\n");  //savings pressed
					printf("The amount that you want to store in the bank\n");
					scanf("%f", &addMoney); //adding money when saving is pressed
					accountBalance = accountBalance + addMoney; //adding it to the main balance
					transactionState = 4;
				} else if (pbPressed1 == chequing) {   //chequing pressed
					printf("Chequing was pressed\r\n");
					printf("The amount that you want to store in the bank\n");
					scanf("%f", &addMoney1); //adding money when chequing is pressed
					accountBalance = accountBalance + addMoney1; //adding it to the main balance
					transactionState = 4;

				}
			}

			printf("If you want to cancel the transaction press Cancel\n");
			HAL_Delay(10000);
			pbPressed = checkOkOrCancel();
			if (pbPressed == cancel) {
				printf("Cancel was pressed\r\n"); //then cancel was pressed
				transactionState = 6;
			}
			break;
		case 4:
			displayPincode();
			returnValue = pincode();
			if (returnValue == 1) {
				transactionState = 5;
			} else {
				printf("Access is denied\n");
				transactionState = 6;

			}
			printf("If you want to cancel the transaction press cancel\n");
			HAL_Delay(10000);
			pbPressed = checkOkOrCancel();
			if (pbPressed == cancel) {
				printf("Cancel was pressed\r\n"); 	//then cancel was pressed.
				transactionState = 6;
			}
			break;
		case 5:
			returnValue1 = authorization(accountBalance, amount);
			if (returnValue1 == 1) {
				printf("okay\n");
				accountBalance = accountBalance - amount;
				printingTransaction(accountBalance, amount);
				transactionState = 1;
			} else {
				printf("You have insufficient funds in your account\n");
				printf("Not Okay\n");
				transactionState = 6;
			}
			break;
		case 6:
			HAL_Delay(1000);
			cancelled();
			HAL_Delay(10000);
			transactionState = 1;
			break;
		default:
			break;
		} //closing brace for switch

	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Configure LSE Drive Capability
	 */
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE
			| RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 16;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
		Error_Handler();
	}
	/** Enable MSI Auto calibration
	 */
	HAL_RCCEx_EnableMSIPLLMode();
}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void) {

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 100;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */
	HAL_TIM_MspPostInit(&htim1);

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : LD3_Pin */
	GPIO_InitStruct.Pin = LD3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
