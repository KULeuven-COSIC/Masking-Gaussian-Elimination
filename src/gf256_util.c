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

#include "gf256_util.h"

void secMult_gf256(uint8_t z_out[N_SHARES], uint8_t x_in[N_SHARES], uint8_t y_in[N_SHARES]) {
    uint8_t r[N_SHARES][N_SHARES];
    uint8_t random;

	for (uint8_t i = 0; i < N_SHARES; i++)
	{
		for (uint8_t j = (i + 1); j < N_SHARES; j++)
		{
            random = random_uint8();
			r[i][j] = random;
            uint8_t xiyj;
            uint8_t xjyi;
            xiyj = gf256_mul(x_in[i], y_in[j]);
            xjyi = gf256_mul(x_in[j], y_in[i]);
			r[j][i] = r[i][j] ^ xiyj;
			r[j][i] = r[j][i] ^ xjyi;
		}
	}

	for (uint8_t i = 0; i < N_SHARES; i++)
	{
		z_out[i] = gf256_mul(x_in[i], y_in[i]);
		for (uint8_t j = 0; j < N_SHARES; j++)
		{
			if (i != j)
			{
				z_out[i] ^= r[i][j];
			}
		}
	}
}

void secNonzero_gf256(uint8_t b[N_SHARES], uint8_t x[N_SHARES]) {
    uint8_t t[N_SHARES];
    uint8_t l[N_SHARES];
    uint8_t r[N_SHARES];
    for (uint8_t i = 0; i < N_SHARES; i++) {
        t[i] = x[i];
    }

    // ITER 1 8b -> 4b
    for (uint8_t i = 0; i < N_SHARES; i++) {
        l[i] = (t[i] & 0xf0) >> 4;
        r[i] = (t[i] & 0x0f);
    }
    strongrefresh_gf256(l, l);
    secOR_gf256(t, l, r);
    
    // ITER 2 4b -> 2b
    for (uint8_t i = 0; i < N_SHARES; i++) {
        l[i] = (t[i] & 0x0c) >> 2;
        r[i] = (t[i] & 0x03);
    }
    strongrefresh_gf256(l, l);
    secOR_gf256(t, l, r);

    // ITER 3 2b -> 1b
    for (uint8_t i = 0; i < N_SHARES; i++) {
        l[i] = (t[i] & 0x02) >> 1;
        r[i] = (t[i] & 0x01);
    }
    strongrefresh_gf256(l, l);
    secOR_gf256(t, l, r);

    for (uint8_t i = 0; i < N_SHARES; i++) {
        b[i] = (t[i] & 0x01); 
    }
}

void secInv_gf256(uint8_t x_inv[N_SHARES], uint8_t x[N_SHARES]) { 
	for (uint8_t i = 0; i < N_SHARES; i++) {
		x_inv[i] = gf256_inv(x[i]);
	}
}
