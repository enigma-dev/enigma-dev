/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Adam Domurad                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **                      
**                                                                              **
\********************************************************************************/
//Bit concerning functions
bool collbitSet(collCustom& aColl,unsigned short aX,unsigned short aY,bool aBool){
      if (aBool)
      aColl.cMask[(int)(aX>>3)+aY*(aColl.byteDim)]|=(1<<(aX&7));
      else
      aColl.cMask[(int)(aX>>3)+aY*(aColl.byteDim)]&=~(1<<(aX&7));
      return 1;
      }
      
bool collbitNegate(collCustom& aColl,unsigned short aX,unsigned short aY){
      aColl.cMask[(aX>>3)+aY*(aColl.byteDim)]^=(1<<(aX&7));
      return 1;
      }
      
//simple solid fill
bool collFill(collCustom& aColl, bool Full)
     {
     unsigned int i;
                  if (Full)
                         {
                         for (i=0;i<(unsigned int)(aColl.byteDim*aColl.yDim);i++)
                         aColl.cMask[i]=255 >>((((i+1)%aColl.byteDim)==0)*((-aColl.xDim)&7));
                         }
                  else
                         {
                         for (i=0;i<(unsigned int)(aColl.byteDim*aColl.yDim);i++)
                         aColl.cMask[i]=0;
                         }
       return 0;
     }
     //Custom Fill
//Geometry fill
bool collFill(collCustom& aColl,unsigned char Type, unsigned char Blend,float aX1,float aY1,float aX2,float aY2,float aX3,float aY3, double Angle, float xOrigin, float yOrigin){
     unsigned int  i,ii;            
     bool isInfinite[4]={0,0,0,0};            
     unsigned int minY,maxY;
     float minX,maxX;
     float Slope[4],Intersect[4], LineMin[4], LineMax[4];
     double aSin,aCos;     
     unsigned char byteRef,bitMod;
     Angle/=57.295779513082320876798154814105;
     aSin=sin(Angle);
     aCos=cos(Angle);
     if (Blend==bm_normal)
     collFill(aColl,0);
     float rotX[4],rotY[4];
     switch (Type)
     {
            ////////////////BEGIN ROTATED RECTANGLE FILL
            case fill_rectangle:
                 
                 //ROTATE ALL POINTS
     rotX[0]=aCos*(aX1-xOrigin)+aSin*(aY1-yOrigin)+xOrigin;
     rotY[0]=aCos*(aY1-yOrigin)-aSin*(aX1-xOrigin)+yOrigin;
     
     rotX[1]=aCos*(aX1-xOrigin)+aSin*(aY2-yOrigin)+xOrigin;
     rotY[1]=aCos*(aY2-yOrigin)-aSin*(aX1-xOrigin)+yOrigin;
     
     rotX[2]=aCos*(aX2-xOrigin)+aSin*(aY2-yOrigin)+xOrigin;
     rotY[2]=aCos*(aY2-yOrigin)-aSin*(aX2-xOrigin)+yOrigin;
     
     rotX[3]=aCos*(aX2-xOrigin)+aSin*(aY1-yOrigin)+xOrigin;
     rotY[3]=aCos*(aY1-yOrigin)-aSin*(aX2-xOrigin)+yOrigin;

            for (i=0;i<4;i++)
            {
            Slope[i]=rotY[i]-rotY[(i+1)&3];
            if (Slope[i]==0)
               isInfinite[i]=1;
            else
            {
                Slope[i]=(rotX[i]-rotX[(i+1)&3])/Slope[i];
                Intersect[i]=rotX[i]-(rotY[i]*Slope[i]);
            }
            LineMax[i]=std::max(rotY[i],rotY[(i+1)&3]);
            LineMin[i]=std::min(rotY[i],rotY[(i+1)&3]);
            }
            minY=(int)std::min(rotY[0],(std::min(rotY[1],std::min(rotY[2],rotY[3]))));
            maxY=(int)std::max(rotY[0],(std::max(rotY[1],std::max(rotY[2],rotY[3]))));
            for (ii=std::min(std::max((long int)minY,(long int)0),(long int)aColl.yDim);ii<=(unsigned int)std::max(std::min((long int)maxY,(long int)aColl.yDim),(long int)0);ii++)
                {
                minX=maxFloat;
                maxX=minFloat;
                ///////////LINE TESTS
                                 for (i=0;i<4;i++)
                                 {
                                 if ((ii<=LineMax[i]) && (ii>=LineMin[i]))
                                 { 
                                 if (isInfinite[i])
                                 {
                                 minX=std::min(minX,rotX[i]);
                                 maxX=std::max(maxX,rotX[i]+1);
                                 }
                                 else
                                 {
                                 aSin=(Slope[i]*(ii)+Intersect[i]);    
                                 minX=std::min(minX,(float)(aSin));
                                 maxX=std::max(maxX,(float)(aSin+1));
                                 }
                                 }
                                 }
                  aX1=ceil(std::min(std::max(minX,(float)0),(float)aColl.xDim));
                  aX2=floor(std::min(std::max(maxX,(float)0),(float)aColl.xDim));          
                  if (aX1<aX2)
                  {                              
                  #include "linear_fill.h"
                  } 
                }
            break;//////////END ROTATED RECTANGLE FILL
            
            
            ////////////////////////////TRIANGLE
            ////////////////////////////TRIANGLE
            
            ///////////////BEGIN TRIANGLE FILL
            case fill_triangle:
            
                 //ROTATE ALL POINTS
     rotX[0]=aCos*(aX1-xOrigin)-aSin*(aY1-yOrigin)+xOrigin;
     rotY[0]=aCos*(aY1-yOrigin)+aSin*(aX1-xOrigin)+yOrigin;
     
     rotX[1]=aCos*(aX2-xOrigin)-aSin*(aY2-yOrigin)+xOrigin;
     rotY[1]=aCos*(aY2-yOrigin)+aSin*(aX2-xOrigin)+yOrigin;
     
     rotX[2]=aCos*(aX3-xOrigin)-aSin*(aY3-yOrigin)+xOrigin;
     rotY[2]=aCos*(aY3-yOrigin)+aSin*(aX3-xOrigin)+yOrigin;
            
            for (i=0;i<3;i++)
            {
            Slope[i]=rotY[i]-rotY[(i+1)%3];
            if (Slope[i]==0)
               isInfinite[i]=1;
            else
            {
                Slope[i]=(rotX[i]-rotX[(i+1)%3])/Slope[i];
                Intersect[i]=rotX[i]-(rotY[i]*Slope[i]);
            }
            LineMax[i]=std::max(rotY[i],rotY[(i+1)%3]);
            LineMin[i]=std::min(rotY[i],rotY[(i+1)%3]);
            }
            minY=(int)std::min(rotY[0],(std::min(rotY[1],rotY[2])));
            maxY=(int)std::max(rotY[0],(std::max(rotY[1],rotY[2])));
            for (ii=std::min(std::max((long int)minY,(long int)0),(long int)aColl.yDim);ii<=(unsigned int)std::max(std::min((long int)maxY,(long int)aColl.yDim),(long int)0);ii++)
               {
                minX=maxFloat;
                maxX=minFloat;
                ///////////LINE TESTS
                                 for (i=0;i<3;i++)
                                 {
                                 if ((ii<=LineMax[i]) && (ii>=LineMin[i]))
                                 { 
                                 if (isInfinite[i])
                                 {
                                 minX=std::min(minX,rotX[i]);
                                 maxX=std::max(maxX,rotX[i]+1);
                                 }
                                 else
                                 {
                                 aSin=(Slope[i]*(ii)+Intersect[i]);    
                                 minX=std::min(minX,(float)(aSin));
                                 maxX=std::max(maxX,(float)(aSin+1));
                                 }                      
                                 }
                                 }
                  aX1=ceil(std::min(std::max(minX,(float)0),(float)aColl.xDim));
                  aX2=floor(std::min(std::max(maxX,(float)0),(float)aColl.xDim));      
                  if (aX1<aX2)
                  {       
                  #include "linear_fill.h"
                   }  
                  }
            break;//////////END TRIANGLE FILL
            
            ///////////////////////////////////ellipse
            ///////////////////////////////////ellipse
                        case fill_ellipse:///////////////BEGIN ELLIPSE FILL
            float W, H, T[6];
            W=(float)(aX2-aX1)/2;
            H=(float)(aY2-aY1)/2;
            rotX[0]=aX1+W;
            rotY[0]=aY1+H;
            rotX[1]=(rotX[0]-xOrigin)*aCos+(rotY[0]-yOrigin)*aSin+xOrigin;
            rotY[1]=(rotY[0]-yOrigin)*aCos-(rotX[0]-xOrigin)*aSin+yOrigin;
            rotY[2]=H*aCos*aCos+W*aSin*aSin;
            //square the two semilengths
            W+=0.5;
            H+=0.5;
            W*=W;
            H*=H;
            T[0]=aCos*aSin*(H-W);//multiply by Y
            T[1]=-W*H*aCos*aCos*aCos*aCos-/*Next term*/2*W*H*aSin*aSin*aCos*aCos-/*Next term*/W*H*aSin*aSin*aSin*aSin;//multiply by Y squared
            T[2]=W*H*H*aCos*aCos+W*W*H*aSin*aSin;
            T[3]=H*aCos*aCos+W*aSin*aSin;
            minX=-std::max(rotY[2],rotY[1]);//actually Y values
            maxX=std::max(rotY[2]+1,aColl.yDim-rotY[1]);//
            float d;           
            for (d=minX;d<=maxX;d++){
                T[4]=T[1]*d*d+T[2];//Forms the discriminant
                if (T[4]>0)
                T[4]=sqrt(T[4]);
                else
                T[4]=-1;
                if ((T[3]!=0) && (T[4]!=-1))
                {
                T[5]=T[0]*d;
                  aX1=std::min(std::max((long int)ceil(rotX[1]+(T[5]-T[4])/T[3]),(long int)0),(long int)aColl.xDim);
                  aX2=std::min(std::max((long int)floor(rotX[1]+(T[5]+T[4])/T[3]),(long int)0),(long int)aColl.xDim);
                  ii=(int)(d+rotY[1]); 
                  if (aX1<aX2)
                  {             
                  #include "linear_fill.h"
                  }
                }
            }
            break;                                    
     }
     return 0;
}
    //LINE LIST FILLS
//Geometry fill
bool collFill(collLinelist& aColl,unsigned char Type, unsigned char Blend,float aX1,float aY1,float aX2,float aY2,float aX3,float aY3, double Angle, float xOrigin, float yOrigin){
     unsigned int i,ii,offSet=0;            
     bool isInfinite[4]={0,0,0,0};            
     int minY,maxY;
     float minX,maxX;
     float Slope[4],Intersect[4], LineMin[4], LineMax[4];
     double aSin,aCos;     
     aColl.xOrigin=xOrigin;
     aColl.yOrigin=yOrigin;
     Angle/=57.295779513082320876798154814105;
     aSin=sin(Angle);
     aCos=cos(Angle);
     float rotX[4],rotY[4];
     switch (Type)
     {
            ////////////////BEGIN ROTATED RECTANGLE FILL
            case fill_rectangle:
                 
                 //ROTATE ALL POINTS
     rotX[0]=aCos*(aX1-xOrigin)+aSin*(aY1-yOrigin)+xOrigin;
     rotY[0]=aCos*(aY1-yOrigin)-aSin*(aX1-xOrigin)+yOrigin;
     
     rotX[1]=aCos*(aX1-xOrigin)+aSin*(aY2-yOrigin)+xOrigin;
     rotY[1]=aCos*(aY2-yOrigin)-aSin*(aX1-xOrigin)+yOrigin;
     
     rotX[2]=aCos*(aX2-xOrigin)+aSin*(aY2-yOrigin)+xOrigin;
     rotY[2]=aCos*(aY2-yOrigin)-aSin*(aX2-xOrigin)+yOrigin;
     
     rotX[3]=aCos*(aX2-xOrigin)+aSin*(aY1-yOrigin)+xOrigin;
     rotY[3]=aCos*(aY1-yOrigin)-aSin*(aX2-xOrigin)+yOrigin;

            for (i=0;i<4;i++)
            {
            Slope[i]=rotY[i]-rotY[(i+1)&3];
            if (Slope[i]==0)
               isInfinite[i]=1;
            else
            {
                Slope[i]=(rotX[i]-rotX[(i+1)&3])/Slope[i];
                Intersect[i]=rotX[i]-(rotY[i]*Slope[i]);
            }
            LineMax[i]=std::max(rotY[i],rotY[(i+1)&3]);
            LineMin[i]=std::min(rotY[i],rotY[(i+1)&3]);
            }
            minY=(int)std::min(rotY[0],(std::min(rotY[1],std::min(rotY[2],rotY[3]))));
            maxY=(int)std::max(rotY[0],(std::max(rotY[1],std::max(rotY[2],rotY[3]))));
            if (Blend==bm_normal)
            {
                 collResize(aColl,(unsigned int)(maxY-minY+1));
                 aColl.yOff=(int)minY;
            }
            if (Blend==bm_add)
            {
               if (aColl.yOff>(int)minY)
               {
               aColl.x1List.insert(aColl.x1List.begin(),aColl.yOff-(int)minY,0);
               aColl.x2List.insert(aColl.x2List.begin(),aColl.yOff-(int)minY,0);
               aColl.yOff=(int)minY;
               offSet=0;
               }
               else
               offSet=(int)minY-aColl.yOff;
               collResize(aColl,std::max((unsigned int)aColl.x1List.size(),(unsigned int)(maxY-minY+1)));
            }
            for (ii=minY;ii<=(unsigned int)maxY;ii++)
                {
                minX=maxFloat;
                maxX=minFloat;
                ///////////LINE TESTS
                                 for (i=0;i<4;i++)
                                 {
                                 if ((ii<=LineMax[i]) && (ii>=LineMin[i]))
                                 { 
                                 if (isInfinite[i])
                                 {
                                 minX=std::min(minX,rotX[i]);
                                 maxX=std::max(maxX,rotX[i]+1);
                                 }
                                 else
                                 {
                                 aSin=(Slope[i]*(ii)+Intersect[i]);    
                                 minX=std::min(minX,((float)aSin));
                                 maxX=std::max(maxX,((float)aSin+1));
                                 }
                                 }
                                 }
                                 
                    if ((Blend==bm_normal)||(maxX-minX)>0)
                    {aColl.x1List[(int)(ii-minY)+offSet]=minX;
                   aColl.x2List[(int)(ii-minY)+offSet]=maxX;}
                    }
            break;//////////END ROTATED RECTANGLE FILL
            
            
            ////////////////////////////TRIANGLE
            ////////////////////////////TRIANGLE
            
            ///////////////BEGIN TRIANGLE FILL
            case fill_triangle:
            
                 //ROTATE ALL POINTS
     rotX[0]=aCos*(aX1-xOrigin)-aSin*(aY1-yOrigin)+xOrigin;
     rotY[0]=aCos*(aY1-yOrigin)+aSin*(aX1-xOrigin)+yOrigin;
     
     rotX[1]=aCos*(aX2-xOrigin)-aSin*(aY2-yOrigin)+xOrigin;
     rotY[1]=aCos*(aY2-yOrigin)+aSin*(aX2-xOrigin)+yOrigin;
     
     rotX[2]=aCos*(aX3-xOrigin)-aSin*(aY3-yOrigin)+xOrigin;
     rotY[2]=aCos*(aY3-yOrigin)+aSin*(aX3-xOrigin)+yOrigin;
            
            for (i=0;i<3;i++)
            {
            Slope[i]=rotY[i]-rotY[(i+1)%3];
            if (Slope[i]==0)
               isInfinite[i]=1;
            else
            {
                Slope[i]=(rotX[i]-rotX[(i+1)%3])/Slope[i];
                Intersect[i]=rotX[i]-(rotY[i]*Slope[i]);
            }
            LineMax[i]=std::max(rotY[i],rotY[(i+1)%3]);
            LineMin[i]=std::min(rotY[i],rotY[(i+1)%3]);
            }
            minY=(int)std::min(rotY[0],(std::min(rotY[1],rotY[2])));
            maxY=(int)std::max(rotY[0],(std::max(rotY[1],rotY[2])));
            if (Blend==bm_normal)
            {
                 collResize(aColl,(unsigned int)(maxY-minY+1));
                 aColl.yOff=(int)minY;
            }

            if (Blend==bm_add)
            {
               if (aColl.yOff>(int)minY)
               {
               aColl.x1List.insert(aColl.x1List.begin(),aColl.yOff-(int)minY,0);
               aColl.x2List.insert(aColl.x2List.begin(),aColl.yOff-(int)minY,0);
               aColl.yOff=(int)minY;
               offSet=0;
               }
               else
               offSet=(int)minY-aColl.yOff;
               collResize(aColl,std::max((unsigned int)aColl.x1List.size(),(unsigned int)(maxY-minY+1)));
            }
            for (ii=minY;ii<=(unsigned int)maxY;ii++)
                {
                minX=maxFloat;
                maxX=minFloat;
                ///////////LINE TESTS
                                 for (i=0;i<3;i++)
                                 {
                                 if ((ii<=LineMax[i]) && (ii>=LineMin[i]))
                                 { 
                                 if (isInfinite[i])
                                 {
                                 minX=std::min(minX,rotX[i]);
                                 maxX=std::max(maxX,rotX[i])+1;
                                 }
                                 else
                                 {
                                 aSin=(Slope[i]*(ii)+Intersect[i]);    
                                 minX=std::min(minX,((float)aSin));
                                 maxX=std::max(maxX,((float)aSin))+1;
                                 }                    
                                 }
                                 }                    
                    if ((Blend==bm_normal)||(maxX-minX)>0)
                    {aColl.x1List[(int)(ii-minY)+offSet]=minX;
                    aColl.x2List[(int)(ii-minY)+offSet]=maxX;}
                    }
            break;//////////END TRIANGLE FILL
            
            ///////////////////////////////////ellipse
            ///////////////////////////////////ellipse
                        case fill_ellipse:///////////////BEGIN ELLIPSE FILL
            float W, H, T[6];
            W=(float)(aX2-aX1)/2;
            H=(float)(aY2-aY1)/2;
            rotX[0]=aX1+W;
            rotY[0]=aY1+H;
            rotX[1]=(rotX[0]-xOrigin)*aCos+(rotY[0]-yOrigin)*aSin+xOrigin;
            rotY[1]=(rotY[0]-yOrigin)*aCos-(rotX[0]-xOrigin)*aSin+yOrigin;
            rotY[2]=H*aCos*aCos+W*aSin*aSin;
            if (Blend==bm_normal)
            {
                 collResize(aColl,(unsigned int)(floor(rotY[2]*2+1)));
                 aColl.yOff=(int)(rotY[1]-rotY[2]);
                 i=0;
            }
            if (Blend==bm_add)
            {
               if (aColl.yOff>(int)(rotY[1]-rotY[2]))
               {
               aColl.x1List.insert(aColl.x1List.begin(),aColl.yOff-(int)(rotY[1]-rotY[2]),0);
               aColl.x2List.insert(aColl.x2List.begin(),aColl.yOff-(int)(rotY[1]-rotY[2]),0);
               aColl.yOff=(int)(rotY[1]-rotY[2]);
               i=0;
               }
               else
               {
               i=(int)(rotY[1]-rotY[2])-aColl.yOff;
               }
               collResize(aColl,std::max((unsigned int)aColl.x1List.size(),(unsigned int)(floor(rotY[2]*2+1))));
            }
            //square the two semilengths
            W+=0.5;
            H+=0.5;
            W*=W;
            H*=H;
            T[0]=aCos*aSin*(H-W);//multiply by Y
            T[1]=-W*H*aCos*aCos*aCos*aCos-/*Next term*/2*W*H*aSin*aSin*aCos*aCos-/*Next term*/W*H*aSin*aSin*aSin*aSin;//multiply by Y squared
            T[2]=W*H*H*aCos*aCos+W*W*H*aSin*aSin;
            T[3]=H*aCos*aCos+W*aSin*aSin;
            float d;
            for (d=-rotY[2];d<=rotY[2];d++)
            {
                T[4]=T[1]*d*d+T[2];//Forms the discriminant
                if (T[4]>0)
                T[4]=sqrt(T[4]);
                else
                T[4]=-1;
                if ((T[3]!=0) && (T[4]!=-1))
                {
                T[5]=T[0]*d;
                if (Blend==bm_normal || (floor((T[5]+T[4])/T[3])-ceil((T[5]-T[4])/T[3])>0))
                {
                    aColl.x1List[(int)(d+rotY[2])+i]=rotX[1]+(T[5]-T[4])/T[3];
                    aColl.x2List[(int)(d+rotY[2])+i]=rotX[1]+(T[5]+T[4])/T[3];
                }
                }
                else
                {
                if (Blend==bm_normal)
                {
                    aColl.x1List[(int)(d+rotY[2])+i]=maxFloat;
                    aColl.x2List[(int)(d+rotY[2])+i]=minFloat;
                }
                }    
            }
            break;                                    
     }
     return 0;
}
