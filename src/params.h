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

#ifndef PARAMS_H
#define PARAMS_H

/////////////////////////////////////////////////////

// Use pre-defined parameter sets, or define custom (below)
#define UOV_I
// #define UOV_III
// #define UOV_V
// #define CUSTOM

// Masking order
#define N_SHARES 2

////////////////////////////////////////////////////

#ifdef UOV_I
#define M_DIM 44 
#define STEP1_EARLY_STOP
#define STEP1_STOP 8
#endif

#ifdef UOV_III
#define M_DIM 72 
#define STEP1_EARLY_STOP
#define STEP1_STOP 8
#endif

#ifdef UOV_V
#define M_DIM 96
#define STEP1_EARLY_STOP
#define STEP1_STOP 8
#endif 

#ifdef CUSTOM
// Matrix dimension
#define M_DIM 2

// Early stop conditional additions in Step 1
// #define STEP1_EARLY_STOP
// #define STEP1_STOP 2
#endif

#endif
