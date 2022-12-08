/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
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

ETH_TxPacketConfig TxConfig;
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

ETH_HandleTypeDef heth;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define DS3231_ADDRESS 0XD0

uint8_t decToBcd(int val)
{
	return(uint8_t)((val/10*16)+(val%10));
}
int bcdToDec(uint8_t val)
{
	return (int)((val/16*10)+(val%16));
}

typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
	uint8_t dayofweek;
	uint8_t dayofmonth;
	uint8_t month;
	uint8_t year;
} TIME;

TIME time;

void Set_Time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
{
	uint8_t set_time[7];
	set_time[0] = decToBcd(sec);
	set_time[1] = decToBcd(min);
	set_time[2] = decToBcd(hour);
	set_time[3] = decToBcd(dow);
	set_time[4] = decToBcd(dom);
	set_time[5] = decToBcd(month);
	set_time[6] = decToBcd(year);

	HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x00, 1, set_time, 7, 1000);
}

void Get_Time(void)
{
	uint8_t get_time[7];
	HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x00, 1, get_time, 7, 1000);
	time.seconds = bcdToDec(get_time[0]);
	time.minutes = bcdToDec(get_time[1]);
	time.hour = bcdToDec(get_time[2]);
	time.dayofweek = bcdToDec(get_time[3]);
	time.dayofmonth = bcdToDec(get_time[4]);
	time.month = bcdToDec(get_time[5]);
	time.year = bcdToDec(get_time[6]);
}

char date[30];
char hora[30];
uint8_t flagEsc;

typedef enum EStatesMain{
	MAIN_INIT
}EStatesMain;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	char semilla[16];
	char tags[10];
	char seleccion[10];
	char next[1];
	char opcion[10];
	char impresion[10];
	char frecuencia[10];
	char fre[10];
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
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  //Set_Time(10,26,2,4,8,12,22);
  RetargetInit(&huart3);
  enum EStatesMain statesMain;
  flagEsc =1;
  printf("\x1b[2J"); //Clear the screen
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  if(flagEsc)
	  {
		  flagEsc=0;
		  statesMain = MAIN_INIT;
	  }else switch(statesMain)
	  {
	  	  case MAIN_INIT:
	  	  Get_Time();
	  	  int band_menu=0;
	  	  int band_impresion;
	  	  	  	  printf("\r\nMenu principal, por favor seleccione la opcion de que desea realizar");
	  	  	  	  printf("\r\nAyuda General sistema: 1 ");
	  	  	  	  printf("\r\nAyuda detallada: 2 ");
	  	  	  	  printf("\r\nSistema control de led a frecuencia: 3 ");
	  	  	  	  printf("\r\nSecuencia pseudoaleatorias: 4 ");
	  	  	  	  printf("\r\nConfiguracion hora y fecha: 5 ");
	  	  	  	  printf("\r\nSensor de temperatura: 6 ");
	  	  	  	  printf("\r\nRegistros de ejecución: 7 ");
	  	  	  	  printf("\r\nHabilitacion/Deshabilitación de impresion interfaz serial: 8 ");
	  	  	  	  scanf("%s", seleccion);
	  	  	  	  int menu = atoi(seleccion);

	  	  	  	  if(menu==1)
	  	  	  	  {
	  	  	  		  printf("\x1b[2J");
	  	  	  		  printf("\r\nOpcion 2 Ayuda detallada. \r\n ");
	  	  	  		  printf("\r\nEn esta opcion se selecciona la opcion de la que desea tener mayor informacion para poder uso de esta.\r\n ");
	  	  	  		  printf("\r\nOpcion 3 Sistema control de led a frecuencia.\r\n ");
	  	  	  		  printf("\r\nEste sistema lo que hace es realizar el encendido y apagado de un led respecto a la frecuencia que desee escoger el usuario"
	  	  	  				  " siempre y cuando esta se encuentre en el intervalo entre 0.1 y 10 Hz.\r\n ");
	  	  	  		  printf("\r\nOpcion 4 Secuencia pseudaleatoria.\r\n ");
	  	  	  		  printf("\r\nEsta opcion es para inciar una secuencia pseudoaleatoria con dos terminos de realimentacion, la cual se podra realizar para "
	  	  	  				  "tamaños de semilla  entre 4 y 32 bits.\r\n ");
	  	  	  		  printf("\r\nOpcion 5 Configuracion hora y fecha.\r\n ");
	  	  	  		  printf("\r\nEsta opcion permite al usuario modificar fecha y hora, para poder realizar el registro correcto de las ejecuciones realizadas"
	  	  	  				"en el sistema.\r\n ");
	  	  	  		  printf("\r\nOpcion 6 Sensor de temperatura.\r\n ");
	  	  	  		  printf("\r\nEn esta opcion se puede observar la temperatura ambiente en donde se encuentra el sistema.\r\n ");

	  	  	  		  printf("\r\nOpcion 7 Registro de ejecucion.\r\n ");
	  	  	  		  printf("\r\nEn esta opcion encuentra el registro de los ultimos procesos de ejecucion realizados.\r\n ");

	  	  	  		  printf("\r\nOpcion 8 Habilitacion de impresion interfaz serial.\r\n ");
	  	  	  		  printf("\r\nEn esta opcion se puede escoger si se desea observar los valores de la secuencia pseudoaleatoria o no.\r\n ");
	  	  	  		  scanf("%s",next);

	  	  	  		  band_menu=1;

	  	  	  	  }
	  	  	  	  if(menu==2)
	  	  	  	  {
	  	  	  		  printf("\x1b[2J");
	  	  	  		  printf("\r\nSeleccione de cual opcion quiere tener mas información ");
	  	  	  		  printf("\r\nSistema control de led a frecuencia: 1 ");
	  	  	  		  printf("\r\nSecuencia pseudoaleatorias: 2 ");
	  	  	  		  printf("\r\nConfiguracion hora y fecha: 3 ");
	  	  	  		  printf("\r\nSensor de temperatura: 4 ");
	  	  	  		  printf("\r\nRegistros de ejecución: 5 ");
	  	  	  		  printf("\r\nHabilitacion/Deshabilitación de impresion interfaz serial: 6 ");
	  	  	  		  scanf("%s",opcion);
	  	  	  		  int help = atoi(opcion);
	  	  	  		  if(help==1)
	  	  	  		  {
	  	  	  			  printf("\x1b[2J");
	  	  	  			  printf("\r\nSistema control de led a frecuencia\r\n ");
	  	  	  			  printf("\r\nEn esta opcion tenemos unas frecuencias predeterminadas para escoger, aunque tambien se encuentra una opcion en la que podemos"
	  	  	  					  "escribir un valor de frecuencia diferente a las que nos da el sistema, la unica condicion es que se encuentre en el rango de 0.1 a 10 Hz"
	  	  	  					  " de los contrario el sistema arrojara un error y nos dirigira al menu principal.\r\n ");
	  	  	  			  scanf("%s",next);
	  	  	  			  band_menu=1;
	  	  	  		  }
	  	  	  		  if(help==2)
	  	  	  		  {
	  	  	  			  printf("\x1b[2J");
	  	  	  			  printf("\r\nSecuencia pseudaleatoria\r\n ");
	  	  	  			  printf("\r\nEn esta opcion usted puede ingresar un valor semilla que se encuentre entre 4 y 32 bits, el cual sera el inicio de la secuencia, ademas "
	  	  	  					  "tiene la opcion de escoger las posiciones que se utilizaran para la implementacion de la xor y de estas obtener el bit de realimentacion "
	  	  	  					  "en la secuencia.\r\n ");
	  	  	  			  scanf("%s",next);
	  	  	  			  band_menu=1;
	  	  	  		  }
	  	  	  		  if(help==3)
	  	  	  		  {
	  	  	  			  printf("\x1b[2J");
	  	  	  			  printf("\r\nConfiguracion hora y fecha.\r\n ");
	  	  	  			  printf("\r\nEn esta opcion usted puede ingresar a configurar la fecha y hora, ya sea porque se ha  desconfigurado el reloj o haya cambiado de locacion geografica.\r\n ");
	  	  	  			  scanf("%s",next);
	  	  	  			  band_menu=1;
	  	  	  		  }
	  	  	  		  if(help==4)
	  	  	  		  {
	  	  	  			  printf("\x1b[2J");
	  	  	  			  printf("\r\nSensor de temperatura.\r\n ");
	  	  	  			  printf("\r\nEn esta opcion se puede observar en tiempo real la temperatura ambiente.\r\n ");
	  	  	  			  scanf("%s",next);
	  	  	  			  band_menu=1;
	  	  	  		  }
	  	  	  		  if(help==5)
	  	  	  		  {
	  	  	  			  printf("\x1b[2J");
	  	  	  			  printf("\r\nRegistro de ejecucion.\r\n ");
	  	  	  			  printf("\r\nEn esta opcion se pueden observar los parametros claves e inciales de las ultimas ejecuciones realizadas en el sistema.\r\n ");
	  	  	  			  scanf("%s",next);
	  	  	  			  band_menu=1;
	  	  	  		  }
	  	  	  		  if(help==6)
	  	  	  		  {
	  	  	  			  printf("\x1b[2J");
	  	  	  			  printf("\r\nHabilitacion de impresion interfaz serial.\r\n ");
	  	  	  			  printf("\r\nEn esta opcion se presenta la posibilidad de desactivar o activar la impresion de los datos de la opcion 4 de secuencia pseudaleatoria.\r\n ");
	  	  	  			  scanf("%s",next);
	  	  	  			  band_menu=1;
	  	  	  		  }
	  	  	  		  band_menu=1;

	  	  	  	  }
	  	  	  	  if(menu==3)
	  	  	  	  {
	  	  	  		  int cont=0;
	  	  	  		  int var;
	  	  	  		  printf("\x1b[2J");
	  	  	  		  printf("\r\nSeleccione la frecuencia deseada");
	  	  	  		  printf("\r\n0.1 Hz: 1");
	  	  	  		  printf("\r\n0.5 Hz: 2");
	  	  	  		  printf("\r\n1 Hz: 3");
	  	  	  		  printf("\r\n3 Hz: 4");
	  	  	  		  printf("\r\n5 Hz: 5");
	  	  	  		  printf("\r\n7 Hz: 6");
	  	  	  		  printf("\r\n9 Hz: 7");
	  	  	  		  printf("\r\n10 Hz: 8");
	  	  	  		  printf("\r\nOtra: 9");
	  	  	  		  scanf("%s",frecuencia);

	  	  	  		  int dig = atoi(frecuencia);

	  	  	  		  if(dig==1)
	  	  	  		  {
	  	  	  			  while(cont<101)
	  	  	  			  {
	  	  	  				  HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	  	  	  				  HAL_Delay(10000);
	  	  	  				  cont++;
	  	  	  			  }
	  	  	  			  band_menu=1;
	  	  	  		  }
	  	  	  		  if(dig==2)
	  	  	  		  {
	  	  	  			  while(cont<101)
	  	  	  			  {
	  	  	  				  HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	  	  	  				  HAL_Delay(2000);
	  	  	  				  cont++;
	  	  	  			  }
	  	  	  			  band_menu=1;
	  	  	  		  }
	  	  	  		  if(dig==3)
	  	  	  		  {
	  	  	  			  while(cont<101)
	  	  	  			  {
	  	  	  				  HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	  	  	  				  HAL_Delay(1000);
	  	  	  				  cont++;
	  	  	  			  }
	  	  	  		  }
	  	  	  		  if(dig==4)
	  	  	  		  {
	  	  	  			  while(cont<101)
	  	  	  			  {
	  	  	  				  HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	  	  	  				  HAL_Delay(333);
	  	  	  				  cont++;
	  	  	  			  }
	  	  	  		  }
	  	  	  		  if(dig==5)
	  	  	  		  {
	  	  	  			  while(cont<101)
	  	  	  			  {
	  	  	  				  HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	  	  	  				  HAL_Delay(200);
	  	  	  				  cont++;
	  	  	  			  }
	  	  	  		  }
	  	  	  		  if(dig==6)
	  	  	  		  {
	  	  	  			  while(cont<101)
	  	  	  			  {
	  	  	  				  HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	  	  	  				  HAL_Delay(143);
	  	  	  				  cont++;
	  	  	  			  }
	  	  	  		  }
	  	  	  		  if(dig==7)
	  	  	  		  {
	  	  	  			  while(cont<101)
	  	  	  			  {
	  	  	  				  HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	  	  	  				  HAL_Delay(111);
	  	  	  				  cont++;
	  	  	  			  }
	  	  	  		  }
	  	  	  		  if(dig==8)
	  	  	  		  {
	  	  	  			  while(cont<101)
	  	  	  			  {
	  	  	  				  HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	  	  	  				  HAL_Delay(100);
	  	  	  				  cont++;
	  	  	  			  }
	  	  	  		  }
	  	  	  		  if(dig==9)
	  	  	  		  {
	  	  	  			  printf("\r\nValor frecuencia entre 0.1 a 10 Hz");
	  	  	  			  scanf("%s",fre);
	  	  	  			  int f = atoi (fre);
	  	  	  			  var = (1/f)*1000;
	  	  	  			  while(cont<101)
	  	  	  			  {
	  	  	  				  HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	  	  	  				  HAL_Delay(var);
	  	  	  				  cont++;
	  	  	  			  }
	  	  	  		  }
	  	  	  		  band_menu=1;
	  	  	  	  }
	  	  	  	  if(menu==4)
	  	  	  	  {
	  	  	  		  printf("\x1b[2J");
	  	  	  		  int band=0;
	  	  	  		  printf("\r\nIngrese el valor semilla: ");
	  	  	       	  scanf("%s", semilla);
	  	  	  	 	  printf("\r\nEl valor semilla ingresado: %s\r\n ", semilla);
	  	  	  	 	  printf("\r\nIngrese el tamaño para la realimentacion\r\n ");
	  	  	  		  scanf("%s", tags);
	  	  	      	  printf("\r\nEl tamaño: %s\r\n ", tags);
	  	  	  	  	  int numref = atoi(tags);
	  	  	  	  	  int sem;
	  	  	  	  	  if(numref == 7)
	  	  	  		  {
	  	  	  	  		  int cont=0,cant=0,max=128,p_max=0,pos=0;
	  	  	  	  		  uint8_t lfsr=atoi(semilla);
	  	  	  		  	  uint8_t bit;
	  	  	  		  	  printf("\r\n#bits término de realimentación %s\r\n", tags);
	  	  	  		  	  printf("\r\nValor inicial(semilla) %s\r\n", semilla);
	  	  	  		  	  printf("\r\n#Valores esperados %d\r\n", max);
	  	  	  		  	  while(cont<128)
	  	  	  		  	  {
	  	  	  		  	  	  bit = ((lfsr >> 0) ^ (lfsr >> 1)) & 1;
	  	  	  			  	  lfsr = (lfsr >> 1) | (bit << 6);
	  	  	  		  	  	  cont++;
	  	  	  		  	  	  if(band_impresion==1)
	  	  	  		  	  	  {
	  	  	  		  	  		  printf("\r\ndata: %x\r\n", lfsr);
	  	  	  		  	  	  }
	  	  	  		  	  	  HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	  	  	  			  	  HAL_Delay(200);
	  	  	  		  		  cant++;
	  	  	  		  	  	  if(atoi(semilla)==lfsr)
	  	  	  			  	  {
	  	  	  			  		  cont=129;
	  	  	  			  		  printf("\r\nCantidad: %d\r\n", cant);
	  	  	  			  	  }
	  	  	  		  	 	  if(0X7F ==lfsr)
	  	  	  			  	  {
	  	  	  			  		  pos=cant;
	  	  	    			  		  p_max=1;
	  	  	   			  	  }
	  	  	  			  }
	  	  	  		  	  printf("\r\n#bits término de realimentación %s\r\n", tags);
	  	  	  		  	  printf("\r\nValor inicial(semilla) %s\r\n", semilla);
	  	  	  		  	  printf("\r\nNumero de valores calculados %d\r\n", cant);
	  	  	  		  	  if(p_max=1)
	  	  	  		  	  {
	  	  	  				  printf("\r\nTuvo periodo maximo en la secuencia %d\r\n", pos);
	  	  	  		  	  }
	  	  	  		  	  else
	  	  	  		  	  {
	  	  	  		  		  printf("\r\nNo tuvo periodo maximo\r\n");
	  	  	  		  	  }
	  	  	  		  	  if(pos | 0)
	  	  	  		  	  {
	  	  	  		  		  HAL_GPIO_TogglePin(LD3_GPIO_Port,LD3_Pin);
	  	  	  		  		  HAL_Delay(5000);
	  	  	  		  	  }
	  	  	  		  	  band=1;
	  	  	  		  }
	  	  	  		  if(numref == 12)
	  	  	  		  {
	  	  	  		  	 int cont=0,cant=0,max=4096,p_max=0,pos=0;
	  	  	  		  	 uint16_t lfsr1=atoi(semilla);
	  	  	  		  	 uint16_t seed=0xFFF;
	  	  	  		  	 uint16_t bit1;
	  	  	  		  	 printf("\r\n#bits término de realimentación %s\r\n", tags);
	  	  	  		  	 printf("\r\nValor inicial(semilla) %s\r\n", semilla);
	  	  	  		  	 printf("\r\n#Valores esperados %d\r\n", max);
	  	  	  		  	 while(cont<4096)
	  	  	  		  	 {
	  	  	  		  		 bit1 = ((lfsr1 >> 0) ^ (lfsr1 >> 1)) & 1;
	  	  	  		  		 lfsr1 = (lfsr1 >> 1) | (bit1 << 11);
	  	  	  		  		 cont++;
	  	  	  		  		 if(band_impresion==1)
	  	  	  		  		 {
	  	  	  		  			 printf("\r\ndata: %x\r\n", lfsr1);
	  	  	  		  		 }
	  	  	  		  		 HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	  	  	  		  		 //HAL_Delay(200);
	  	  	  		  		 cant++;
	  	  	  		  		 if(atoi(semilla)==lfsr1)
	  	  	  		  		 {
	  	  	  		  			 cont=4097;
	  	  	  		  			 printf("\r\nCantidad: %d\r\n", cant);
	  	  	  		  		 }
	  	  	  		  		 if(0XFFF ==lfsr1)
	  	  	  		  		 {
	  	  	  		  			 pos=cant;
	  	  	  		  			 p_max=1;
	  	  	  		  		 }
	  	  	  		  	 }
	  	  	  		  	 printf("\r\n#bits término de realimentación %s\r\n", tags);
	  	  	  		  	 printf("\r\nValor inicial(semilla) %s\r\n", semilla);
	  	  	  		  	 printf("\r\nNumero de valores calculados %d\r\n", cant);
	  	  	  		  	 if(p_max=1)
	  	  	  		  	 {
	  	  	  		  		 printf("\r\nTuvo periodo maximo en la secuencia %d\r\n", pos);
	  	  	  		  	 }
	  	  	  		  	 else
	  	  	  		  	 {
	  	  	  		  		 printf("\r\nNo tuvo periodo maximo\r\n");
	  	  	  		  	 }
	  	  	  		  	 if(pos | 0)
	  	  	  		  	 {
	  	  	  		  		 HAL_GPIO_TogglePin(LD3_GPIO_Port,LD3_Pin);
	  	  	  		  		 HAL_Delay(5000);
	  	  	  		  	 }
	  	  	  		  	 band=1;
	  	  	  		  }
	  	  	  		  if(numref == 16)
	  	  	  		  {
	  	  	  			  int cont=0,cant=0,max=4096,p_max=0,pos=0;
	  	  	  			  uint16_t lfsr2=atoi(semilla);
	  	  	  			  uint16_t bit2;
	  	  	  			  printf("\r\n#bits término de realimentación %s\r\n", tags);
	  	  	  			  printf("\r\nValor inicial(semilla) %s\r\n", semilla);
	  	  	  			  printf("\r\n#Valores esperados %d\r\n", max);
	  	  	  			  while(cont<65535)
	  	  	  			  {
	  	  	  				  bit2 = ((lfsr2 >> 0) ^ (lfsr2 >> 1)) & 1;
	  	  	  				  lfsr2 = (lfsr2 >> 1) | (bit2 << 15);
	  	  	  				  cont++;
	  	  	  				  if(band_impresion==1)
	  	  	  				  {
	  	  	  					  printf("\r\ndata: %x\r\n", lfsr2);
	  	  	  				  };
	  	  	  				  HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	  	  	  				  HAL_Delay(200);
	  	  	  				  cant++;
	  	  	  				  if(atoi(semilla)==lfsr2)
	  	  	  				  {
	  	  	  					  cont=65536;
	  	  	  					  printf("\r\nCantidad: %d\r\n", cant);
	  	  	  				  }
	  	  	  				  if(0XFFFF ==lfsr2)
	  	  	  				  {
	  	  	  					  pos=cant;
	  	  	  					  p_max=1;
	  	  	  				  }
	  	  	  			  }
	  	  	  			  printf("\r\n#bits término de realimentación %s\r\n", tags);
	  	  	  			  printf("\r\nValor inicial(semilla) %s\r\n", semilla);
	  	  	  			  printf("\r\nNumero de valores calculados %d\r\n", cant);
	  	  	  			  if(p_max=1)
	  	  	  			  {
	  	  	  				  printf("\r\nTuvo periodo maximo en la secuencia %d\r\n", pos);
	  	  	  			  }
	  	  	  			  else
	  	  	  			  {
	  	  	  				  printf("\r\nNo tuvo periodo maximo\r\n");
	  	  	  			  }
	  	  	  			  if(pos | 0)
	  	  	  			  {
	  	  	  				  HAL_GPIO_TogglePin(LD3_GPIO_Port,LD3_Pin);
	  	  	  				  HAL_Delay(5000);
	  	  	  			  }
	  	  	  			  band=1;
	  	  	  		  }
	  	  	  		  if(band==0)
	  	  	  		  {
	  	  	  			  printf("\r\nTamaño no autorizado");
	  	  	  		  }
	  	  	  		  band_menu=1;
	  	  	  	  }
	  	  	  	  if(menu==5)
	  	  	  	  {
	  	  	  		  printf("\x1b[2J");
	  	  	  		  sprintf(hora,"Time: %02d:%02d:%02d\t", time.hour, time.minutes, time.seconds);
	  	  	  		  sprintf(date,"Date: %02d/%02d/20%02d\r\n", time.dayofmonth, time.month, time.year);

	  	  	  		  HAL_UART_Transmit(&huart3, (uint8_t*)hora, sizeof(hora), HAL_MAX_DELAY);
	  	  	  		  HAL_UART_Transmit(&huart3, (uint8_t*)date, sizeof(date), HAL_MAX_DELAY);
	  	  	  		  HAL_Delay(900);

	  	  	  		  band_menu=1;
	  	  	  	  }
	  	  	  	  if(menu==6)
	  	  	  	  {
	  	  	  		  printf("\x1b[2J");
	  	  	  		  printf("\r\nMenu 6 ");
	  	  	  		  band_menu=1;
	  	  	  	  }
	  	  	  	  if(menu==7)
	  	  	  	  {
	  	  	  		  printf("\x1b[2J");
	  	  	  		  printf("\r\nMenu 7 ");
	  	  	  		  band_menu=1;
	  	  	  	  }
	  	  	  	  if(menu==8)
	  	  	  	  {
	  	  	  		  printf("\x1b[2J");
	  	  	  		  printf("\r\nEscoja la opcion que crea pertinente ");
	  	  	  		  printf("\r\nSi desea activar la impresion de datos presione: 1 ");
	  	  	  		  printf("\r\nSi dese desactivar la impresion de datos presione: 2 ");
	  	  	  		  scanf("%s",impresion);
	  	  	  		  int band_datos = atoi(impresion);
	  	  	  		  if(band_datos==1)
	  	  	  		  {
	  	  	  			  printf("\r\nLa impresion de datos se encuentra activada");
	  	  	  			  HAL_Delay(2000);
	  	  	  			  band_impresion=1;
	  	  	  			  band_menu=1;
	  	  	  		  }
	  	  	  		  if(band_datos==2)
	  	  	  		  {
	  	  	  			  printf("\r\nLa impresion de datos se encuentra desactivada");
	  	  	  			  HAL_Delay(2000);
	  	  	  			  band_impresion=0;
	  	  	  			  band_menu=1;
	  	  	  		  }
	  	  	  		  band_menu=1;
	  	  	  	  }
	  	  	  	  if(band_menu==0)
	  	  	  	  {
	  	  	  		  printf("\r\nOpción no disponible");
	  	  	  	  }


	  }

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
