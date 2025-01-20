/*
 * MIT License
 *
 * Copyright (c) 2025 KU Leuven - COSIC
 * Authors: Quinten Norga <quinten.norga@esat.kuleuven.be>
 *          Suparna Kundu <suparna.kundu@esat.kuleuven.be>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "main.h"

int main(void)
{
    // Initialize board
    clock_setup();
    gpio_setup();
    usart_setup(115200);
    flash_setup();
    rng_enable();

    char buffer[15];

    #ifdef RUN_BENCHMARK
    SCS_DEMCR |= SCS_DEMCR_TRCENA;
    DWT_CYCCNT = 0;
    DWT_CTRL |= DWT_CTRL_CYCCNTENA;

    unsigned int start_total_count, end_total_count; 
    unsigned int start_rowech_count, end_rowech_count;
    unsigned int start_backsub_count, end_backsub_count;
    unsigned int start_step1_count, end_step1_count;
    unsigned int start_step2_count, end_step2_count;
    unsigned int start_step3_count, end_step3_count;
    unsigned int start_step4_count, end_step4_count;
    #endif

    uint8_t A[M_DIM][M_DIM][N_SHARES];
    uint8_t b[M_DIM][N_SHARES];
    uint8_t x[M_DIM];

    #ifdef RUN_VERIFY
    uint8_t A_unmasked[M_DIM][M_DIM];
	uint8_t b_unmasked[M_DIM];
    uint8_t check[M_DIM];
    #endif

    for (uint32_t n_iter = 0; n_iter < N_RUNS; n_iter++)
    {
        // Initialize inputs
        int fail = 1;

        for (uint8_t i = 0; i < M_DIM; i++)
        {
            for (uint8_t j = 0; j < M_DIM; j++)
            {
                for (uint8_t k = 0; k < N_SHARES; k++)
                {
                    A[i][j][k] = random_uint8();
                }
            }
            for (uint8_t k = 0; k < N_SHARES; k++)
            {
                b[i][k] = random_uint8();
            }
        }

        sprintf(buffer, "iteration: %d", (int) n_iter);
        send_USART_str(buffer);

        /////////////////////
        // Start SecRowEch //
        /////////////////////

        #ifdef RUN_BENCHMARK
        DWT_CYCCNT = 0;
        end_step1_count = 0;
        end_step2_count = 0;
        end_step3_count = 0;
        end_step4_count = 0;
        start_total_count = DWT_CYCCNT; 
        start_rowech_count = DWT_CYCCNT;
        #endif

        while(fail == 1) 
        {
            uint8_t p[N_SHARES];
            uint8_t s[N_SHARES];
            for (uint8_t j = 0; j < M_DIM; j++) {
                
                // STEP 1
                #ifdef RUN_BENCHMARK
                start_step1_count = DWT_CYCCNT;
                #endif

                #ifdef STEP1_EARLY_STOP
                    uint8_t end = ((j + STEP1_STOP) < M_DIM) ? j + STEP1_STOP : M_DIM; 
                #else 
                    uint8_t end = M_DIM;    
                #endif
                
                for (uint8_t k = j + 1; k < end; k++) {
                    uint8_t z[N_SHARES];
                    
                    secNonzero_gf256(z, A[j][j]);
                    secNOT_gf256(z, z);

                    for (uint8_t l = j; l < M_DIM; l++) {
                        secCondAdd_gf256(A[j][l], A[j][l], A[k][l], z);
                    }
                    secCondAdd_gf256(b[j], b[j], b[k], z);
                }

                #ifdef RUN_BENCHMARK    
                end_step1_count += (DWT_CYCCNT - start_step1_count);
                #endif

                // STEP 2
                #ifdef RUN_BENCHMARK
                start_step2_count = DWT_CYCCNT;
                #endif

                uint8_t t[N_SHARES];
                uint8_t cj;

                secNonzero_gf256(t, A[j][j]);
                FullAdd_gf256(&cj, t);

                #ifdef RUN_BENCHMARK
                end_step2_count += (DWT_CYCCNT - start_step2_count);
                #endif

                if (cj == 0) {
                    fail = -1; // Fail
                    break;
                }
                else {

                    // STEP 3
                    #ifdef RUN_BENCHMARK
                    start_step3_count = DWT_CYCCNT;
                    #endif 

                    B2Minv_gf256(p, A[j][j]);

                    for (uint8_t k = j; k < M_DIM; k++) {
                        secScalarMult_gf256(A[j][k], A[j][k], p);
                    }
                    secScalarMult_gf256(b[j], b[j], p);

                    #ifdef RUN_BENCHMARK    
                    end_step3_count += (DWT_CYCCNT - start_step3_count);
                    #endif

                    // STEP 4
                    #ifdef RUN_BENCHMARK
                    start_step4_count = DWT_CYCCNT;
                    #endif

                    for (uint8_t k = j + 1; k < M_DIM; k++) {
                        strongrefresh_gf256(s, A[k][j]);
                        for (uint8_t l = j; l < M_DIM; l++) {
                            secMultSub_gf256(A[k][l], A[j][l], A[k][l], s);
                        }
                        secMultSub_gf256(b[k], b[j], b[k], s);
                    }

                    #ifdef RUN_BENCHMARK
                    end_step4_count += (DWT_CYCCNT - start_step4_count);
                    #endif
                }
            }
            if (fail == -1) {
                break;
            }
            fail = 0; // Success 
        }
        #ifdef RUN_BENCHMARK
        end_rowech_count = DWT_CYCCNT - start_rowech_count;
        #endif

        #ifdef RUN_VERIFY
        for (uint8_t i = 0; i < M_DIM; i++){
            for (uint8_t j = 0; j < M_DIM; j++){
                A_unmasked[i][j] = 0;
            }
            b_unmasked[i] = 0;
        }
        for (uint8_t i = 0; i < M_DIM; i++){
            for (uint8_t j = 0; j < M_DIM; j++){
                for (uint8_t k = 0; k < N_SHARES; k++){
                    A_unmasked[i][j] ^= A[i][j][k];
                }
            }
            for (uint8_t k = 0; k < N_SHARES; k++){
                b_unmasked[i] ^= b[i][k];
            }
        }
        #endif
        
        //////////////////////
        // Start SecBackSub //
        //////////////////////
        #ifdef RUN_BENCHMARK
        start_backsub_count = DWT_CYCCNT;
        #endif

        if (fail == 0)
        {
            for (uint8_t j = M_DIM - 1; j > 0; j--) {
                FullAdd_gf256(&x[j], b[j]);

                for (uint8_t k = 0; k < j; k++) {
                    for (uint8_t l = 0; l < N_SHARES; l++) {
                        b[k][l] ^= gf256_mul(x[j], A[k][j][l]);
                    }
                }
            }
            FullAdd_gf256(&x[0], b[0]);
        }

        #ifdef RUN_BENCHMARK
        end_backsub_count = DWT_CYCCNT - start_backsub_count;
        end_total_count = DWT_CYCCNT - start_total_count;
        
        sprintf(buffer, "total cyc: %d", end_total_count);
        send_USART_str(buffer);
        sprintf(buffer, "secRE cyc: %d", end_rowech_count);
        send_USART_str(buffer);
        sprintf(buffer, "step1 cyc: %d", end_step1_count);
        send_USART_str(buffer);
        sprintf(buffer, "step2 cyc: %d", end_step2_count);
        send_USART_str(buffer);
        sprintf(buffer, "step3 cyc: %d", end_step3_count);
        send_USART_str(buffer);
        sprintf(buffer, "step4 cyc: %d", end_step4_count);
        send_USART_str(buffer);
        sprintf(buffer, "secBS cyc: %d", end_backsub_count);
        send_USART_str(buffer);
        #endif

        #ifdef RUN_VERIFY
        for (uint8_t i = 0; i < M_DIM; i++){
            check[i] = b_unmasked[i];
            for (uint8_t j = 0; j < M_DIM; j++){
                check[i] ^= gf256_mul(A_unmasked[i][j],x[j]);
            }
        }
        for (uint8_t i = 0; i < M_DIM; i++){
            if(check[i] != 0)
            {
                sprintf(buffer, "wrong result");
                send_USART_str(buffer);
                break;
            }
            if (i == M_DIM-1)
            {
                sprintf(buffer, "correct result");
                send_USART_str(buffer);
            }
        }
        #endif

        if (fail != 0)
        {
            sprintf(buffer, "no solution");
            send_USART_str(buffer);
        }
        sprintf(buffer, "end iteration");
        send_USART_str(buffer);
    }
    
    return 0;
}
