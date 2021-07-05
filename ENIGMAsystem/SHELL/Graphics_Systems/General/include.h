// this is a single include for ALL of the general graphics functions
// this file should only be included a single time from each system's
// include.h for systems that intend to support the generic graphics
// and it should be included last in case any of them have already been
// included before

#include "GSsprite.h"
#include "GSbackground.h"
#include "GSsurface.h"
#include "GScolors.h"
#include "GSblend.h"
#include "GSd3d.h"
#include "GSgpu.h"
#include "GSprimitives.h"
#include "GStiles.h"
#include "GSmodel.h"
#include "GSmatrix.h"
#include "GSfont.h"
#include "GScurves.h"
#include "GSvertex.h"
#include "GStextures.h"
#include "GSscreen.h"
#include "GSstdraw.h"
#include "actions.h"
#include "texture_atlas.h"
