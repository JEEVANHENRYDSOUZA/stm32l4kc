/* USER CODE BEGIN Header */
/*******************************************************************************
  * File Name          : <assignment.c>
  * Description        : <Morse Code>
  *
  * Author:              <Jeevan Henry Dsouza>
  * Student number:      <8716171>
  * Date:                <12-06-2021>
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include<stdio.h>
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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void dot(int l);
void dash(int x);
char string[100];
int  j = 0; //decalring j for using at as while loop counter//
int time=0;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */
		HAL_Delay(500);//just to hold the screen for some time //
		printf("ENTER A STRING\n");
		scanf("%[^\n]", string); //here i am changing the default end of the string that is from space to new line//
		printf("%s\n", string); //reference for scanf---https://www.includehelp.com/c/c-program-to-read-string-with-spaces-using-scanf-function.aspx//
		printf("ENTER 1 UNIT VALUE OF TIME\n");
		scanf("%d", &time);
		printf("%d\n", time);


		while (string[j] != '\0') {   //all strings are terminated by '/0'//
			switch (string[j]) {             //string character changes to ascii code//
			case 'A':                         //here A will be replaced by its ascii value  //
				dot(time);
				HAL_Delay(time);       //delay between dot and dash is of one unit//
				dash(time);
				HAL_Delay(time);
				break;
	      	 case 'B':
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case 'C':
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case 'D':
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case 'E':
				dot(time);
				HAL_Delay(time);
				break;
			case 'F':
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case 'G':
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case 'H':
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case 'I':
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case 'J':
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				break;
			case 'K':
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				break;
			case 'L':
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case 'M':
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				break;
			case 'N':
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case 'O':
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				break;
			case 'P':
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case 'Q':
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				break;
			case 'R':
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case 'S':
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case 'T':
				dash(time);
				HAL_Delay(time);
				break;
			case 'U':
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				break;
			case 'V':
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				break;
			case 'W':
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				break;
			case 'X':
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				break;
			case 'Y':
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				break;
			case 'Z':
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case ' ':
				HAL_Delay(time*7);//SPACE between two words//
				break;
			case '1':
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				break;
			case '2':
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				break;
			case '3':
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				break;
			case '4':
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				break;
			case '5':
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case '6':
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				break;
			case '7':
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case '8':
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;
			case '9':
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dot(time);
				HAL_Delay(time);
				break;

			case '0':
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				dash(time);
				HAL_Delay(time);
				break;

	     default:
				printf("The string entered is not correct\n");
				break;

			}

			j++;
		}
		j = 0;       //re-initalizing because j is declared globally hence for the next string input we need to re-initalize it to zero//
		fflush(stdin);//if we dont do this ,null characters are placed automatically, for the next string to be input clearing the string//
                      //refernce taken from pervious lectures and https://en.wikipedia.org/wiki/C_file_input/output//
		/* USER CODE BEGIN 3 */
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

// FUNCTION      : <dot()>
// DESCRIPTION   : blinks led for dot in the morse code
//   <describe what the function does>
// PARAMETERS    : time-value of 1 unit of time
//   <list the parameters passed to function and what they do>
//
// RETURNS       :
//   <no value is returned >



void dot(int l) {

	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
	HAL_Delay(l);
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
	HAL_Delay(l*3); //SPACE between every letter is of 3 unit//
	return;

}
// FUNCTION      : <dash()>
// DESCRIPTION   : blinks led for dash in the morse code
//   <describe what the function does>
// PARAMETERS    : time-value of 1 unit of time
//   <list the parameters passed to function and what they do>
//
// RETURNS       :
//   <no value is returned >

void dash(int x) {

	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
	HAL_Delay(x * 3);
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
	HAL_Delay(x*3);
	return;

}






/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
