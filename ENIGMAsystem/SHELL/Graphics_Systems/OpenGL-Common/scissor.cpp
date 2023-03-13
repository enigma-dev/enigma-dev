namespace enigma_user {

void d3d_enable_scissor_test(bool enable) {
  if(enable==true)
     glEnable(GL_SCISSOR_TEST);
  else
     glDisable(GL_SCISSOR_TEST);
}

}
