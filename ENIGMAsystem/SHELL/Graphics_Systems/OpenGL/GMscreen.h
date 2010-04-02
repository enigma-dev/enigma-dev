int screen_redraw(int dontswap)
{
    if (!view_enabled)
    {
      glViewport(0,0,window_get_width(),window_get_height());
      glLoadIdentity();
      glScaled(1,-1,1);
      glOrtho(0,room_width-1,0,room_height-1,0,1);
       
      if (background_showcolor)
      {
         int clearcolor=(int)enigma::negmod((int)background_color,16777215);
         glClearColor(__GETR(clearcolor)/255.0,__GETG(clearcolor)/255.0,__GETB(clearcolor)/255.0, 1);
         glClear(GL_COLOR_BUFFER_BIT);
      }
      
      enigma::event_current=16;
      for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
      {
         (*enigma::instance_event_iterator).second->myevent_draw(); 
      }
    }
    else for (view_current=0; view_current<7; view_current++)
    if (view_visible[(int)view_current])
    {
       int vc=(int)view_current;
       int vob=(int)view_object[vc];
       
       if (vob != -1)
       {
         if (instance_exists(vob)) 
         {
           enigma::wildclass vobr=enigma::int2object(vob);
           int vobx=*vobr.x,voby=*vobr.y;
           //int bbl=*vobr.x+*vobr.bbox_left,bbr=*vobr.x+*vobr.bbox_right,bbt=*vobr.y+*vobr.bbox_top,bbb=*vobr.y+*vobr.bbox_bottom;
           //if (bbl<view_xview[vc]+view_hbor[vc]) view_xview[vc]=bbl-view_hbor[vc];
           if (vobx<view_xview[vc]+view_hborder[vc]) view_xview[vc]=vobx-view_hborder[vc];
           if (vobx>view_xview[vc]+view_wview[vc]-view_hborder[vc]) view_xview[vc]=vobx+view_hborder[vc]-view_wview[vc];
           if (voby<view_yview[vc]+view_vborder[vc]) view_yview[vc]=voby-view_vborder[vc];
           if (voby>view_yview[vc]+view_hview[vc]-view_vborder[vc]) view_yview[vc]=voby+view_vborder[vc]-view_hview[vc];
           if (view_xview[vc]<0) view_xview[vc]=0;
           if (view_yview[vc]<0) view_yview[vc]=0;
           if (view_xview[vc]>room_width-view_wview[vc]) view_xview[vc]=room_width-view_wview[vc];
           if (view_yview[vc]>room_height-view_hview[vc]) view_yview[vc]=room_height-view_hview[vc];
         }
       }
       
       glViewport((int)view_xport[vc],(int)view_yport[vc],(int)view_wport[vc],(int)view_hport[vc]);
       glLoadIdentity();
       glScaled(1,-1,1);
       glOrtho((int)view_xview[vc],(int)view_wview[vc]+(int)view_xview[vc],(int)view_yview[vc],(int)view_hview[vc]+(int)view_yview[vc],0,1);
       
       if (background_showcolor)
       {
          int clearcolor=(int)enigma::negmod((int)background_color,16777215);
          glClearColor(__GETR(clearcolor)/255.0,__GETG(clearcolor)/255.0,__GETB(clearcolor)/255.0, 1);
          glClear(GL_COLOR_BUFFER_BIT);
       }
       
       enigma::event_current=16;
       for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
       {
          (*enigma::instance_event_iterator).second->myevent_draw(); 
       }
    }
    
    if (!dontswap) SwapBuffers(enigma::window_hDC);
    
    return 0;
}
