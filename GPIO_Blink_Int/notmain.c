
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );
extern void ASMDELAY ( unsigned int );
extern void SAVECONTEXT ();

typedef void (*INTPOINT)(int);

//L4_PER 0x48000000

#define DMTIMER0_BASE 0x44E05000

#define DMTIMER0_TCLR (DMTIMER0_BASE+0x38)
#define DMTIMER0_TCRR (DMTIMER0_BASE+0x3C)
#define DMTIMER0_IRQENABLE_SET	(DMTIMER0_BASE+0x2C)
#define DMTIMER0_IRQENABLE_CLR	(DMTIMER0_BASE+0x30)
#define DMTIMER0_TSICR (DMTIMER0_BASE+0x54)
#define DMTIMER0_TMAR	(DMTIMER0_BASE+0x4C)

#define GPIO1_BASE              0x4804C000
#define GPIO1_OE                (GPIO1_BASE+0x134)
#define GPIO1_CLEARDATAOUT      (GPIO1_BASE+0x190)
#define GPIO1_SETDATAOUT        (GPIO1_BASE+0x194)

#define GPIO1_BASE      0x4804C000
#define GPIO1_OE        (GPIO1_BASE+0x134)
#define GPIO1_DATAOUT   (GPIO1_BASE+0x13C)
#define CM_PER_BASE     0x44E00000
#define CM_PER_GPIO1_CLKCTRL (CM_PER_BASE+0xAC)
#define CM_WKUP_TIMER0_CLKCTRL (CM_PER_BASE+0x10)
#define GPIO_DATA_BASE	0x000F0000

#define MAX_INTERRUPTS 128

#define TIMER0_INT_NUM		66

#define INTC_BASE	0x48200FFF
#define IRQ_SYSCONFIG		(INTC_BASE+0x10)
#define IRQ_IDLE		(INTC_BASE+0x50)
#define ILR_BASE		(INTC_BASE+0x100)
#define TIMER0_ILR		(ILR_BASE+TIMER0_INT_NUM)

/*
 * For shiggles, all the MIRn registers are listed here, in case they are needed
 * for other interrupts 
 */

#define IRQ_MIR0		(INTC_BASE+0x84)
#define IRQ_MIRSET0		(INTC_BASE+0x8C)
#define IRQ_MIRCLEAR0		(INTC_BASE+0x88)
#define IRQ_ISR_SET0		(INTC_BASE+0x90)

#define IRQ_MIR1		(INTC_BASE+0xA4)
#define IRQ_MIRSET1		(INTC_BASE+0xAC)
#define IRQ_MIRCLEAR1		(INTC_BASE+0xA8)
#define IRQ_ISR_SET1		(INTC_BASE+0xB0)

#define IRQ_MIR2		(INTC_BASE+0xC4)
#define IRQ_MIRSET2		(INTC_BASE+0xCC)
#define IRQ_MIRCLEAR2		(INTC_BASE+0xC8)
#define IRQ_ISR_SET2		(INTC_BASE+0xD0)
#define IRQ_ITR2		(INTC_BASE+0xC0)

#define IRQ_MIR3		(INTC_BASE+0xE4)
#define IRQ_MIRSET3		(INTC_BASE+0xEC)
#define IRQ_MIRCLEAR3		(INTC_BASE+0xE8)
#define IRQ_ISR_SET3		(INTC_BASE+0xF0)

volatile unsigned int numcnt = 0x00100000;

void (*funcVectors[MAX_INTERRUPTS])(void);


void timer_interrupt(){
	PUT32(DMTIMER0_TCRR, 0x00000000); //reset the timer
	PUT32(GPIO1_CLEARDATAOUT,0x01E00000);
	PUT32(GPIO1_SETDATAOUT, GPIO_DATA_BASE+numcnt);
	numcnt+=0x00200000;
}

void bank2_interrupt_handler(){
	if((GET32(IRQ_ITR2)&0x1)==0){
		timer_interrupt();
	}
}

int notmain ( void )
{
		
	PUT32(CM_PER_GPIO1_CLKCTRL, 2);
	while(1){
		if ((GET32(CM_PER_GPIO1_CLKCTRL)&0x00030000)==0) break;
	}
	PUT32(GPIO1_OE,0xFE1FFFFF);
	PUT32(DMTIMER0_TCLR,0x00000003);
	//This first block is designed to set a bunch of interrupt specific things
	//First off we set the AUTOIDLE bit (the 0 bit of the SYSCONFIG register
	//of the interrupt controller) in order to save power consumption by putting
	//a clock gate in on the controller when not in use (increases number of clock
	//cycles required to start an interrupt by 2)
	PUT32(IRQ_SYSCONFIG, 0x00000001);

	//Next we put another clock gate on, for autogating the input synchonizer, 
	//because we similarly don't need that to be really fast so we can take that 
	//out. That bit, the TURBO bit sits at bit 1.
	PUT32(IRQ_IDLE,	0x00000002);

	//Now we need to set the priority for the timer interrupt, which is the 66th
	//interrupt event in the base table. Set it to middle of the line, because we
	//don't really care about the priority b/c it should be the only priority
	//outside of resetting it. Value should be 0x07 for priority. Last two nibbles
	//should be 0x1C
	PUT32(TIMER0_ILR, 0x0000001C);

	//Next, we set our respective MIR bit. For us, that is going to be bit 66,
	//which falls into MIR2 (66/32 rounded down = 2). Then it is going to be bit
	//1, which makes it easy enough to load IRQ_MIRSET2 with a 0x1. This enables
	//the interrupt. 
	PUT32(IRQ_MIRCLEAR2, 0x00000001);

	//Set the timer interrupt description to be when it matches a given value set
	//in the TMAR register, which is next, and set to 0x4000 for 1 second 
	PUT32(DMTIMER0_IRQENABLE_SET, 0x000000001);
	PUT32(DMTIMER0_TMAR, 0x00004000);



	//Next we load the ISR_SET bank for the interrupt handler. Give it a pointer
	//to the function which will be our interrupt handler.
	unsigned int funcptr = (int)&bank2_interrupt_handler;
	PUT32(0xFFFF0000 + TIMER0_INT_NUM, funcptr);
	PUT32(0x00000000 + TIMER0_INT_NUM, funcptr);

	PUT32(DMTIMER0_TCRR, 0x00000000); //Right before we start, just reset
					  //the timer

	//now we go into an infinite loop while waiting for an interrupt.
	while(1);

    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
