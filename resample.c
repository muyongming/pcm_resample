#include "stdio.h"
#include "stdint.h"
#include "resamplesubs.h"

#define APP_INPUT_BUFF_SIZE         (0x1000)
#define APP_OUTPUT_BUFF_SIZE        (APP_INPUT_BUFF_SIZE *)

#define SAMPLE_RATE_8k              (8000)
#define SAMPLE_RATE_16K             (16000)
#define SAMPLE_RATE_22K_05          (22050)
#define SAMPLE_RATE_44K_1           (44100)
#define SAMPLE_RATE_70K_100         (70100)
#define SAMPLE_RATE_96K             (96000)

#define IN_SAMPLE_RATE              (SAMPLE_RATE_16K)
#define OUT_SAMPLE_RATE             (SAMPLE_RATE_96K)

#define TRACE_ERR(c)    printf(c)

int16_t in_buff[APP_INPUT_BUFF_SIZE];
int16_t out_buff[APP_INPUT_BUFF_SIZE * OUT_SAMPLE_RATE / IN_SAMPLE_RATE];

int main(int argc, char *argv[])
{
    int cnt, n;
    FILE *fp_in = NULL, *fp_out = NULL;

    fp_in  = fopen("in.bin", "rb");
    if (!fp_in) {
        printf("aaaaab\n");
        return -1;
    }

    fp_out  = fopen("out.bin", "wb");
    if (!fp_out) {
        printf("ddddd\n");
        return -2;
    }

    ParameterResample param_resample = {};

    PassSRC pass_src = {
        .nInputSampleRate = IN_SAMPLE_RATE,
        .nOutputSampleRate = OUT_SAMPLE_RATE,
        .nInputbufferszie = APP_INPUT_BUFF_SIZE,
        .pSRCHandler = NULL,
    };

    pass_src.nOutputbuffersize = pass_src.nInputbufferszie * pass_src.nOutputSampleRate / pass_src.nInputSampleRate;

    if ((pass_src.nInputbufferszie * pass_src.nOutputSampleRate) % pass_src.nInputSampleRate) {  // 余数大于0
        printf("----------\n");
    }

    // int resample_Init(PassSRC *pPassSRC, char *pGlobal)
    if (-1 == resample_Init(&pass_src, (char *)&param_resample)) {
        TRACE_ERR("resample init failed!\n");
        return FALSE;
    }

    while (1) {
        n = fread(in_buff, 2, APP_INPUT_BUFF_SIZE, fp_in);
        resample_Process(&pass_src, (char *)in_buff, (char *)out_buff, 1);
        if (n < APP_INPUT_BUFF_SIZE) {
            printf("read:%d/%d\n", n, APP_INPUT_BUFF_SIZE);
            break;
        }

        fwrite(out_buff, 2, pass_src.nOutputbuffersize, fp_out);
    }

    fwrite(out_buff, 2, n * pass_src.nOutputSampleRate / pass_src.nInputSampleRate, fp_out);  // 写最后一帧数据

    fclose(fp_in);
    fclose(fp_out);

    printf("finish!\n");

    return 0;

}

