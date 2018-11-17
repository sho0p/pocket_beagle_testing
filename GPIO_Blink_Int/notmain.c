
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );
extern void ASMDELAY ( unsigned int );

//L4_PER 0x48000000

#define DMTIMER0_BASE 0x44E05000

#define DMTIMER0_TCLR (DMTIMER0_BASE+0x38)
#define DMTIMER0_TCRR (DMTIMER0_BASE+0x3C)

#define DMTIMER0_TSICR (DMTIMER0_BASE+0x54)

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

#define RTC_CLK_BASE_ADDRESS 0x10017000

int notmain ( void )
{
    
    PUT32(GPIO1_CLEARDATAOUT,0x01E00000);
    PUT32(GPIO1_SETDATAOUT,GET32(DMTIMER0_TCRR&GPIO_DATA_BASE));
	/*
    unsigned int ra;

    PUT32(CM_PER_GPIO1_CLKCTRL,2);
    while(1)
    {
        if((GET32(CM_PER_GPIO1_CLKCTRL)&0x00030000)==0) break;
    }
    PUT32(GPIO1_OE,0xFE1FFFFF); 
    PUT32(DMTIMER0_TCLR,0x00000003);
    while(1)
    {
	for(unsigned int i = 0x00100000; i <= 0x01F00000; i+=0x00200000){
        PUT32(GPIO1_CLEARDATAOUT,0x01E00000);
        PUT32(GPIO1_SETDATAOUT,GPIO_DATA_BASE+i);
        while(1)
        {
            ra=GET32(DMTIMER0_TCRR);
            if((ra&=0x4000)==0) break;
        }
        PUT32(GPIO1_CLEARDATAOUT,0x01E00000);
        PUT32(GPIO1_SETDATAOUT,GPIO_DATA_BASE+i);
        while(1)
        {
            ra=GET32(DMTIMER0_TCRR);
            if((ra&=0x4000)!=0) break;
        }
     *   PUT32(GPIO1_CLEARDATAOUT,GPIO_DATA_BASE+i);
        while(1)
        {
            ra=GET32(DMTIMER0_TCRR);
            if((ra&=0x4000)!=0) break;
        }/
	}
    }
*/
    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
