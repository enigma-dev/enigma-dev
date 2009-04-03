bool collFill(collCustom& aColl,unsigned char Blend,float aX,float aY,collCustom& bColl,float bX,float bY, double aAngle, double bAngle, double axScale, double ayScale, double bxScale, double byScale){
// // // Fill body A with body B where they collide; both have rotations and scaling to account for

// //Start: Make changes such that body B is altered to keep body A as-is

//Start: Set up important variables
double aSin, aCos, bSin, bCos, 
        aOx,  aOy,  bOx,  bOy,
        aRx,  aRy,  bRx,  bRy, 
        ;

aAngle=fmod(aAngle,360); aAngle/=57.295779513082320876798154814105;//Convert from degrees (expected) to radians (what C++ needs)
bAngle=fmod(bAngle,360); bAngle/=57.295779513082320876798154814105;

aSin=sin(aAngle); bSin=sin(bAngle);
aCos=cos(aAngle); bCos=cos(bAngle);      
//End

//Start: Alter the values of the top-left-most corner of each

bX-=aX+aColl.xOff-bColl.xOff;
bY-=aY+aColl.yOff-bColl.xOff;

aX=(-aColl.xFocus)*axScale+aColl.xFocus;//Scaling the top-left corners
aY=(-aColl.yFocus)*ayScale+aColl.yFocus;//Note: aX and aY are treated as if they were initially 0     
bX=(bX-bColl.xFocus)*bxScale+bColl.xFocus;
bY=(bY-bColl.yFocus)*byScale+bColl.yFocus; 

aOx=(aColl.xOrigin-aColl.xFocus)*axScale+aColl.xFocus; bOx=(bColl.xOrigin-bColl.xFocus)*bxScale+bColl.xFocus;//Scaling the
aOy=(aColl.yOrigin-aColl.yFocus)*ayScale+aColl.yFocus; bOy=(bColl.yOrigin-bColl.yFocus)*byScale+bColl.yFocus;//Rotation Origin 

aX=(bX-xSOrigin)*aCos - (bY-ySOrigin)*aSin + xSOrigin;
bY=(bY-ySOrigin)*aCos + (bX-xSOrigin)*aSin + ySOrigin;

xSOrigin=
ySOrigin=

bX=aX; 
