#ifdef COMMANDCOOL
int Cmd_Cool_Fan_Temp(int fan, int temp){
  if(fan<0){ return -3; }
  if(fan>4){ return -4; }

  Serial.println(" fan temp ");
  Serial.print(fan);
  Serial.print(" ");
  Serial.println(temp);

       if(fan==0){ return Cmd_Cool_Fan0_Temp(temp); }
  else if(fan==1){ return Cmd_Cool_Fan1_Temp(temp); }
  else if(fan==2){ return Cmd_Cool_Fan2_Temp(temp); }
  else if(fan==3){ return Cmd_Cool_Fan3_Temp(temp); }
  else if(fan==4){ return Cmd_Cool_Fan4_Temp(temp); }
}
#endif

