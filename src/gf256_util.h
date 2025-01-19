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

#ifndef GF256_UTIL_H
#define GF256_UTIL_H

#include "mask_util.h"
#include "refresh.h"
#include "gf256.h"
#include "bool_util.h"
#include "params.h"
#include <stdint.h>

void secMult_gf256(uint8_t z_out[N_SHARES], uint8_t x_in[N_SHARES], uint8_t y_in[N_SHARES]);
void secNonzero_gf256(uint8_t b[N_SHARES], uint8_t x[N_SHARES]);
void secInv_gf256(uint8_t x_inv[N_SHARES], uint8_t x[N_SHARES]);

#endif
