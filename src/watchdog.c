#include <sysheader.h>
#include <nx_wdt.h>
#include <nx_rstcon.h>

#define RESETINDEX_OF_WDT_MODULE_PRESETn	58

void watchdog_start(unsigned short wtcnt)
{
        uint32_t regvalue;
        struct nx_wdt_registerset *pwdt =
                (struct nx_wdt_registerset *)PHY_BASEADDR_WDT_MODULE;
        struct NX_RSTCON_RegisterSet *prstcon =
                (struct NX_RSTCON_RegisterSet *)PHY_BASEADDR_RSTCON_MODULE;

        regvalue = RIO32((unsigned int)&prstcon->REGRST
                        [RESETINDEX_OF_WDT_MODULE_PRESETn>>5]);
        WIO32((unsigned int)&prstcon->REGRST
                        [RESETINDEX_OF_WDT_MODULE_PRESETn>>5],
                        regvalue |
                                3<<(RESETINDEX_OF_WDT_MODULE_PRESETn & (32-1)));

        regvalue = 0xff<<8 |            /* prescaler value */
                WDT_CLOCK_DIV128<<3 |   /* division factor */
                0x1<<2;                 /* reset enable */

        WIO32((unsigned int)&pwdt->wtcon, regvalue);
        WIO32((unsigned int)&pwdt->wtcnt, wtcnt & 0xffff); /* reset cnt */
        WIO32((unsigned int)&pwdt->wtcon, regvalue | 1<<5); /* now reset */
}
