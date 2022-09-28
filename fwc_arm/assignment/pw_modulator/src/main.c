/*==========================================================
Code by G V V Sharma
Apr 21, 2021,
Released under GNU/GPL
https://www.gnu.org/licenses/gpl-3.0.en.html
/*==========================================================
 *
 *    File   : main.c
 *    Purpose: main for Pygmy Seven Segment Display Driver using GPIO inputs
 *                                                          
 *=========================================================*/

#include "Fw_global_config.h"   // This defines application specific charactersitics

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "RtosTask.h"

/*    Include the generic headers required for QORC */
#include "eoss3_hal_gpio.h"
#include "eoss3_hal_rtc.h"
#include "eoss3_hal_timer.h"
#include "eoss3_hal_fpga_usbserial.h"
#include "ql_time.h"
#include "s3x_clock_hal.h"
#include "s3x_clock.h"
#include "s3x_pi.h"
#include "dbg_uart.h"

#include "cli.h"


extern const struct cli_cmd_entry my_main_menu[];


const char *SOFTWARE_VERSION_STR;


/*
 * Global variable definition
 */


extern void qf_hardwareSetup();
static void nvic_init(void);
#define GPIO_OUTPUT_MODE (1)
#define GPIO_INPUT_MODE (0)
void PyHal_GPIO_SetDir(uint8_t gpionum,uint8_t iomode);
int PyHal_GPIO_GetDir(uint8_t gpionum);
int PyHal_GPIO_Set(uint8_t gpionum, uint8_t gpioval);
int PyHal_GPIO_Get(uint8_t gpionum);

// Remember to reset flipflops before testing tmrw

#define W_PIN	4
#define X_PIN	5
#define Y_PIN	23	// changed from 15
#define Z_PIN	7
#define S_PIN	8	

#define D_PIN	25	// Probably problem pin
#define C_PIN	10
#define B_PIN	11
#define A_PIN	12
#define T_PIN	13	//red led

#define	CLK_PIN	18	//blue led 
void sevenseg_setup(void);
void sevenseg(int a, int b, int c, int d, int e, int f, int g);
void disp(int num);

int main(void)
{
    uint32_t i=0,j=0,k=0;
    uint32_t D=0,C=0,B=0,A=0,T=0;
    uint32_t W=0,X=0,Y=0,Z=0,S=0;
    uint32_t a,b,c,d,e,f,g;    
    SOFTWARE_VERSION_STR = "qorc-onion-apps/qf_hello-fpga-gpio-ctlr";
    
    qf_hardwareSetup();
    nvic_init();

    dbg_str("\n\n");
    dbg_str( "##########################\n");
    dbg_str( "Quicklogic QuickFeather FPGA GPIO CONTROLLER EXAMPLE\n");
    dbg_str( "SW Version: ");
    dbg_str( SOFTWARE_VERSION_STR );
    dbg_str( "\n" );
    dbg_str( __DATE__ " " __TIME__ "\n" );
    dbg_str( "##########################\n\n");

    dbg_str( "\n\nHello GPIO!!\n\n");	// <<<<<<<<<<<<<<<<<<<<<  Change me!

    CLI_start_task( my_main_menu );
	HAL_Delay_Init();
/***************START SOFTWARE IMPL******************
PyHal_GPIO_SetDir(X_PIN,0); //Input

PyHal_GPIO_SetDir(T_PIN,1); //Output
PyHal_GPIO_SetDir(CLK_PIN,1); //Output
PyHal_GPIO_Set(T_PIN, 0);
PyHal_GPIO_Set(CLK_PIN, 0);

while(1){
	
    PyHal_GPIO_Set(CLK_PIN, 0);
    HAL_DelayUSec(2000000);    

    PyHal_GPIO_Set(CLK_PIN, 1);
    
    sevenseg_setup();    //Sevenseg ready for display
    sevenseg(a,b,c,d,e,f,g); 
    
    HAL_DelayUSec(2000000);    
    
    W = A;
    X = B;
    Y = C;
    Z = D;
    S = PyHal_GPIO_Get(S_PIN);

    A = (X&&!Z) || (X&&!Y&&!S) || (!X&&Y&&Z&&!S);
    B = (!X&&Y&&!Z) || (!X&&Y&&S) || (!W&&!X&&!Y&&!S) || (X&&!Y&&Z&&S);
    C = (!X&&Z&&S) || (!Y&&Z&&S) || (W&&!Z&&!S) || (!W&&!X&&!Y&&S) || (!X&&Y&&!Z&&!S);
    D = (!W&&!X&&S) || (!X&&!Z&&S) || (X&&!Z&&!S) || (X&&Y&&!S) || (!W&&!Y&&Z&&S);
    T = (Y&&!Z) || (X&&Z) || (W&&!Z) || (W&&S) || (!W&&!Y&&Z);

    a=(!A&!B&!C&D)|(!A&B&!C&!D);
    b=(!A&B&!C&D)|(!A&B&C&!D);
    c=(!A&!B&C&!D);
    d=(!A&!B&!C&D)|(!A&B&!C&!D)|(!A&B&C&D);
    e=(!A&!B&!C&D)|(!A&!B&C&D)|(!A&B&!C&!D)|(!A&B&!C&D)|(!A&B&C&D)|(A&!B&!C&D);
    f=(!A&!B&!C&D)|(!A&!B&C&!D)|(!A&!B&C&D)|(!A&B&C&D);
    g=(!A&!B&!C&!D)|(!A&!B&!C&D)|(!A&B&C&D);
    
    PyHal_GPIO_Set(T_PIN, T);
}
***************END SOFTWARE IMPL******************/
	
/***************START 7474 IMPL***********************/
PyHal_GPIO_SetDir(W_PIN,0); //Input
PyHal_GPIO_SetDir(X_PIN,0); //Input
PyHal_GPIO_SetDir(Y_PIN,0); //Input
PyHal_GPIO_SetDir(Z_PIN,0); //Input
PyHal_GPIO_SetDir(S_PIN,0); //Input

PyHal_GPIO_SetDir(D_PIN,1); //Output
PyHal_GPIO_SetDir(C_PIN,1); //Output
PyHal_GPIO_SetDir(B_PIN,1); //Output
PyHal_GPIO_SetDir(A_PIN,1); //Output
PyHal_GPIO_SetDir(T_PIN,1); //Output
PyHal_GPIO_Set(T_PIN, 0);
//PyHal_GPIO_SetDir(9,1); //Input
//PyHal_GPIO_SetDir(10,1); //Input
//PyHal_GPIO_Set(9, 0);
//PyHal_GPIO_Set(10, 1);

PyHal_GPIO_SetDir(CLK_PIN,1); //Output
PyHal_GPIO_Set(CLK_PIN, 0);

while(1)
{
    A = (X&&!Z) || (X&&!Y&&!S) || (!X&&Y&&Z&&!S);
    B = (!X&&Y&&!Z) || (!X&&Y&&S) || (!W&&!X&&!Y&&!S) || (X&&!Y&&Z&&S);
    C = (!X&&Z&&S) || (!Y&&Z&&S) || (W&&!Z&&!S) || (!W&&!X&&!Y&&S) || (!X&&Y&&!Z&&!S);
    D = (!W&&!X&&S) || (!X&&!Z&&S) || (X&&!Z&&!S) || (X&&Y&&!S) || (!W&&!Y&&Z&&S);
    T = (Y&&!Z) || (X&&Z) || (W&&!Z) || (W&&S) || (!W&&!Y&&Z);

    a=(!A&!B&!C&D)|(!A&B&!C&!D);
    b=(!A&B&!C&D)|(!A&B&C&!D);
    c=(!A&!B&C&!D);
    d=(!A&!B&!C&D)|(!A&B&!C&!D)|(!A&B&C&D);
    e=(!A&!B&!C&D)|(!A&!B&C&D)|(!A&B&!C&!D)|(!A&B&!C&D)|(!A&B&C&D)|(A&!B&!C&D);
    f=(!A&!B&!C&D)|(!A&!B&C&!D)|(!A&!B&C&D)|(!A&B&C&D);
    g=(!A&!B&!C&!D)|(!A&!B&!C&D)|(!A&B&C&D);
    

    PyHal_GPIO_Set(D_PIN, D);
    PyHal_GPIO_Set(C_PIN, C);
    PyHal_GPIO_Set(B_PIN, B);
    PyHal_GPIO_Set(A_PIN, A);
    PyHal_GPIO_Set(T_PIN, T);

    PyHal_GPIO_Set(CLK_PIN, 0);
    HAL_DelayUSec(2000000);    

    PyHal_GPIO_Set(CLK_PIN, 1);
    HAL_DelayUSec(2000000);    
    
    W = PyHal_GPIO_Get(W_PIN);  
    X = PyHal_GPIO_Get(X_PIN);  
    Y = PyHal_GPIO_Get(Y_PIN);  
    Z = PyHal_GPIO_Get(Z_PIN);  
    S = PyHal_GPIO_Get(S_PIN);  

}
/***********************END 7474 IMPL********************************/
//     disp(7);  //display the number

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
    dbg_str("\n");

    while(1);
}

static void nvic_init(void)
 {
    // To initialize system, this interrupt should be triggered at main.
    // So, we will set its priority just before calling vTaskStartScheduler(), not the time of enabling each irq.
    NVIC_SetPriority(Ffe0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SpiMs_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(CfgDma_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(Uart_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FbMsg_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
 }    

void sevenseg(int a, int b, int c, int d, int e, int f, int g)

{
    //Seven Segment GPIO
    PyHal_GPIO_Set(4,a);//a
    PyHal_GPIO_Set(5,b);//b
    PyHal_GPIO_Set(6,c);//c
    PyHal_GPIO_Set(7,d);//d
    PyHal_GPIO_Set(8,e);//e
    PyHal_GPIO_Set(10,f);//f
    PyHal_GPIO_Set(11,g);//g
}

//needed for startup_EOSS3b.s asm file
void SystemInit(void)
{

}

//gpionum --> 0 --> 31 corresponding to the IO PADs
//gpioval --> 0 or 1
#define FGPIO_DIRECTION_REG (0x40024008)
#define FGPIO_OUTPUT_REG (0x40024004)
#define FGPIO_INPUT_REG (0x40024000)
//Set GPIO(=gpionum) Mode: Input(iomode = 0) or Output(iomode = 1)
//Before Set/Get GPIO value, the direction must be correctly set
void PyHal_GPIO_SetDir(uint8_t gpionum,uint8_t iomode)
{
    uint32_t tempscratch32;

    if (gpionum > 31)
        return;

    tempscratch32 = *(uint32_t*)(FGPIO_DIRECTION_REG);
    if (iomode)
        *(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 | (0x1 << gpionum);
    else
        *(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 & (~(0x1 << gpionum));

}


//Get current GPIO(=gpionum) Mode: Input(iomode = 0) or Output(iomode = 1)
int PyHal_GPIO_GetDir(uint8_t gpionum)
{
    uint32_t tempscratch32;
    int result = 0;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t*)(FGPIO_DIRECTION_REG);

    result = ((tempscratch32 & (0x1 << gpionum)) ? GPIO_OUTPUT_MODE : GPIO_INPUT_MODE);

    return result;
}

//Set GPIO(=gpionum) to 0 or 1 (= gpioval)
//The direction must be set as Output for this GPIO already
//Return value = 0, success OR -1 if error.
int PyHal_GPIO_Set(uint8_t gpionum, uint8_t gpioval)
{
    uint32_t tempscratch32;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t*)(FGPIO_DIRECTION_REG);

    //Setting Direction moved out as separate API, we will only check
    //*(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 | (0x1 << gpionum);
    if (!(tempscratch32 & (0x1 << gpionum)))
    {
        //Direction not Set to Output
        return -1;
    }
    
    tempscratch32 = *(uint32_t*)(FGPIO_OUTPUT_REG);

    if(gpioval > 0)
    {
        *(uint32_t*)(FGPIO_OUTPUT_REG) = tempscratch32 | (0x1 << gpionum);
    }
    else
    {
        *(uint32_t*)(FGPIO_OUTPUT_REG) = tempscratch32 & ~(0x1 << gpionum);
    }    

    return 0;
}
//Get GPIO(=gpionum): 0 or 1 returned (or in erros -1)
//The direction must be set as Input for this GPIO already
int PyHal_GPIO_Get(uint8_t gpionum)
{
    uint32_t tempscratch32;
    uint32_t gpioval_input;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t*)(FGPIO_INPUT_REG);
    gpioval_input = (tempscratch32 >> gpionum) & 0x1;

    return ((int)gpioval_input);
}



void sevenseg_setup(void)

{

    //Set GPIO direction
    PyHal_GPIO_SetDir(4,1);
    PyHal_GPIO_SetDir(5,1);
    PyHal_GPIO_SetDir(6,1);
    PyHal_GPIO_SetDir(7,1);    
    PyHal_GPIO_SetDir(8,1);
    PyHal_GPIO_SetDir(10,1);
    PyHal_GPIO_SetDir(11,1);    

}

void disp(int num)
{
switch(num)
{
	case 0:
	  sevenseg(0,0,0,0,0,0,1);
	break;
	case 1:
	  sevenseg(1,0,0,1,1,1,1);
	break;
	case 2:
	  sevenseg(0,0,1,0,0,1,0);
	break;
	case 3:
	  sevenseg(0,0,0,0,1,1,0);
	break;
	case 4:
	  sevenseg(1,0,0,1,1,0,0);
	break;
	case 5:
	  sevenseg(0,1,0,0,1,0,0);
	break;
	case 6:
	  sevenseg(0,1,0,0,0,0,0);
	break;
	case 7:
	  sevenseg(0,0,0,1,1,1,1);
	break;
	case 8:
	  sevenseg(0,0,0,0,0,0,0);
	break;
	case 9:
	  sevenseg(0,0,0,0,1,0,0);
	break;
	default:
	  sevenseg(0,1,1,0,0,0,0);
	break;
}
}

