#pragma once

#include "includes.h"

// x0
#define KP_X0 0.5
#define KI_X0 0.05
#define KD_X0 0.0
// y0
#define KP_Y0 0.45
#define KI_Y0 0.04
#define KD_Y0 0.00
// x1
#define KP_X1 0.5
#define KI_X1 0.05
#define KD_X1 0.0
// y1
#define KP_Y1 0.45
#define KI_Y1 0.04
#define KD_Y1 0.001
// x2
#define KP_X2 0.5
#define KI_X2 0.05
#define KD_X2 0.0
// y2
#define KP_Y2 0.45
#define KI_Y2 0.04
#define KD_Y2 0.001
// x3
#define KP_X3 0.5
#define KI_X3 0.1
#define KD_X3 0.00
// y3
#define KP_Y3 0.5
#define KI_Y3 0.05
#define KD_Y3 0.00
// x4
#define KP_X4 0.5
#define KI_X4 0.05
#define KD_X4 0.0
// y4
#define KP_Y4 0.5
#define KI_Y4 0.03
#define KD_Y4 0.0
// x5
#define KP_X5 0.5
#define KI_X5 0.05
#define KD_X5 0.0
// y5
#define KP_Y5 0.45
#define KI_Y5 0.04
#define KD_Y5 0.001
// x9
#define KP_X9 0.5
#define KI_X9 0.05
#define KD_X9 0.0
// y9
#define KP_Y9 0.45
#define KI_Y9 0.01
#define KD_Y9 0.005
// x10
#define KP_X10 0.5
#define KI_X10 0.05
#define KD_X10 0.0
// y10
#define KP_Y10 0.45
#define KI_Y10 0.04
#define KD_Y10 0.001
// x11
#define KP_X11 0.5
#define KI_X11 0.05
#define KD_X11 0.0
// y11
#define KP_Y11 0.45
#define KI_Y11 0.04
#define KD_Y11 0.001
// x12
#define KP_X12 0.5
#define KI_X12 0.05
#define KD_X12 0.0
// y12
#define KP_Y12 0.45
#define KI_Y12 0.04
#define KD_Y12 0.001
// x13
#define KP_X13 0.5
#define KI_X13 0.1
#define KD_X13 0.00
// y13
#define KP_Y13 0.5
#define KI_Y13 0.05
#define KD_Y13 0.00
// x14
#define KP_X14 0.5
#define KI_X14 0.05
#define KD_X14 0.0
// y14
#define KP_Y14 0.5
#define KI_Y14 0.03
#define KD_Y14 0.0
// x15
#define KP_X15 0.5
#define KI_X15 0.05
#define KD_X15 0.0
// y15
#define KP_Y15 0.45
#define KI_Y15 0.04
#define KD_Y15 0.001

PID pidx0 (KP_X0, KI_X0, KD_X0);
PID pidx1 (KP_X1, KI_X1, KD_X1);
PID pidx2 (KP_X2, KI_X2, KD_X2);
PID pidx3 (KP_X3, KI_X3, KD_X3);
PID pidx4 (KP_X4, KI_X4, KD_X4);
PID pidx5 (KP_X5, KI_X5, KD_X5);
PID pidx9 (KP_X9, KI_X9, KD_X9);
PID pidx10(KP_X10, KI_X10, KD_X10);
PID pidx11(KP_X11, KI_X11, KD_X11);
PID pidx12(KP_X12, KI_X12, KD_X12);
PID pidx13(KP_X13, KI_X13, KD_X13);
PID pidx14(KP_X14, KI_X14, KD_X14);
PID pidx15(KP_X15, KI_X15, KD_X15);

PID pidy0(KP_Y0, KI_Y0, KD_Y0);
PID pidy1(KP_Y1, KI_Y1, KD_Y1);
PID pidy2(KP_Y2, KI_Y2, KD_Y2);
PID pidy3(KP_Y3, KI_Y3, KD_Y3);
PID pidy4(KP_Y4, KI_Y4, KD_Y4);
PID pidy5(KP_Y5, KI_Y5, KD_Y5);
PID pidy9(KP_Y9, KI_Y9, KD_Y9);
PID pidy10(KP_Y10, KI_Y10, KD_Y10);
PID pidy11(KP_Y11, KI_Y11, KD_Y11);
PID pidy12(KP_Y12, KI_Y12, KD_Y12);
PID pidy13(KP_Y13, KI_Y13, KD_Y13);
PID pidy14(KP_X14, KI_Y14, KD_Y14);
PID pidy15(KP_Y15, KI_Y15, KD_Y15);

