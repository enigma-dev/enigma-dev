#include <cstdlib>
#include <cmath>
#include <vector>
#include <limits>
#include <iostream>
#include "colligma_start.h"

int main()
{
    collLinelist aColl;
    collFill(aColl,fill_rectangle,bm_normal,0,0,11,11,/*NA*/0,/*NA*/0,-10);//dont need rotations
    aColl.xOrigin=11.5;
    aColl.yOrigin=11.5;
    aColl.xFocus=11.5;
    aColl.yFocus=11.5;
    unsigned int i,ii;
    double iii;    
    for (iii=0;iii<10;iii++)
    {
    std::cout << "\n\n";
    for (ii=0;ii<24;ii++)
    {
    std::cout << "\n";
    for (i=0;i<24;i++)
    {
    std::cout<<collCheck(aColl,iii,0,i,ii,10,-1,1);
    }
    }
    }
    std::cin >> i;
    return 1;
}
#include "colligma_end.h"

