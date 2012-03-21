String convertHash(uint8_t* hash) {
  String returnString;
  int i;
  for (i=0; i<20; i++) {
    returnString += ("0123456789abcdef"[hash[i]>>4]);
    returnString += ("0123456789abcdef"[hash[i]&0xf]);  
  }  
  return returnString;
}
