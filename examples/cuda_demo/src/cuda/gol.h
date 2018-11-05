#pragma once

extern "C" void testprint();
extern "C" void initGOL(int ** world, int ** d_world, int ** d_tmpWorld, int dim, uchar4 * texture);
extern "C" void stepGOL(int * world, int * d_world, int * d_tmpWorld, int dim, uchar4 * texture);