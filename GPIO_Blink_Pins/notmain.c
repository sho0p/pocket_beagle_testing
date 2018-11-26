
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
#define GPIO1_DATAOUT		(GPIO1_BASE+0x13C)

#define GPIO1_BASE      0x4804C000
#define GPIO1_OE        (GPIO1_BASE+0x134)
#define GPIO1_DATAOUT   (GPIO1_BASE+0x13C)
#define CM_PER_BASE     0x44E00000
#define CM_PER_GPIO1_CLKCTRL (CM_PER_BASE+0xAC)
#define CM_WKUP_TIMER0_CLKCTRL (CM_PER_BASE+0x10)
#define GPIO_DATA_BASE_EMPTY	0x00000000
#define GPIO_DATA_BASE_FULL	0x1E000000

#define GPIO_DATA_MASK		0x1E000000
#define GPIO_DATA_INC		0x02000000

void binary_cnt ( void ) 
{
	unsigned int ra = GET32(GPIO1_DATAOUT);
	PUT32(GPIO1_CLEARDATAOUT, GPIO_DATA_BASE_FULL);
	if( (ra == GPIO_DATA_BASE_FULL) )
	{
		return;
	}
	PUT32( GPIO1_SETDATAOUT, ra+=GPIO_DATA_INC);
}

int notmain ( void )
{
    unsigned int ra;

    PUT32(CM_PER_GPIO1_CLKCTRL,2);
    while(1)
    {
        if((GET32(CM_PER_GPIO1_CLKCTRL)&0x00030000)==0) break;
    }
    PUT32(GPIO1_OE,~GPIO_DATA_MASK); 
    PUT32(DMTIMER0_TCLR,0x00000003);
    PUT32(GPIO1_SETDATAOUT, 0x00000000);
    while(1)
    {
        while(1)
        {
            ra=GET32(DMTIMER0_TCRR);
            if((ra&=0x80)==0) break;
        }
	binary_cnt();	
        while(1)
        {
            ra=GET32(DMTIMER0_TCRR);
            if((ra&=0x80)!=0) break;
        }
	binary_cnt();
     /*   PUT32(GPIO1_CLEARDATAOUT,GPIO_DATA_BASE+i);
        while(1)
        {
            ra=GET32(DMTIMER0_TCRR);
            if((ra&=0x4000)!=0) break;
        }*/
	
    }
    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
