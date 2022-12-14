#ifndef __RESAMPLESUBS_H__
#define __RESAMPLESUBS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdefs.h"

#define Nhc         8
#define Na          7
#define Np          (Nhc+Na)
#define Npc         (1<<Nhc)
#define Amask       ((1<<Na)-1)
#define Pmask       ((1<<Np)-1)
#define Nh          16
#define Nb          16
#define Nhxn        16
#define Nhg         (Nh-Nhxn)
#define NLpScl      13

#define IBUFFSIZE   4096*16  // dzy
typedef struct
{
    UHWORD LpScl;               /* Unity-gain scale factor */
    UHWORD Nwing;               /* Filter table size */
    UHWORD Nmult;               /* Filter length for up-conversions */
    UHWORD Xoff;
    UHWORD Xread;
    HWORD *Imp;               /* Filter coefficients */
    HWORD *ImpD;              /* ImpD[n] = Imp[n+1]-Imp[n] */
    UWORD factor;
    UWORD dhb, dtb;
    UWORD Time;
    HWORD X1[IBUFFSIZE];
} ParameterResample;

typedef struct
{
    int     nInputSampleRate;
    int     nOutputSampleRate;
    int     nInputbufferszie;  // samples number of mono channel input buffer
    int     nOutputbuffersize;  // samples number of mono channel output buffer
    void    *pSRCHandler;
} PassSRC;

int resample_Init(PassSRC *pPassSRC, char *pGlobal);
void resample_UpdateOutSampRate(PassSRC *pPassSRC, char *pGlobal);
void resample_Process(PassSRC *pPassSRC, char *pInBuf, char *pOutBuf, int ch);

#ifdef __cplusplus
}
#endif

#endif  /* ! __RESAMPLESUBS_H__ */
