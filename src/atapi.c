 #include "../include/atapi.h"
 #include "../include/kernel.h"

//Code taken from quest.bu.edu/qxr/source/kernel/include/arch/i386.h#0255
 insw (uint16 usPort, void *buf, int count)
 { asm volatile ("cld; rep insw"
                 :"=c" (count), "=D" (buf)
                 :"0" (count), "1" (buf), "d" (usPort)
                 :"memory","flags");
 }

//Code taken from quest.bu.edu/qxr/source/kernel/include/arch/i386.h#0314
 static inline void
 outsw (uint16 usPort, void *buf, int count)
 {  asm volatile ("cld; rep outsw"
                  :"=S" (buf), "=c" (count)
                  :"0" (buf), "1" (count), "d" (usPort)
                  :"flags");
  }

  static inline void
  outw(uint16 us, uint16 usPort){
    asm volatile("outl %0,%1"::"a"(us),"Nd"(usPort));
  }

 //ATA software reset mechanism, SRST (bit 2 in the Device Control Register)
 void ata_sreset (uint32 bus)
{
  //Set the SRST bit to initiate the software reset protocol
   outb (ATA_DCR (bus),0x02);
   //Clear SRST
   outb (ATA_DCR (bus),0x0);
 }

void
ata_drive_select (uint32 bus, uint32 drive)
 {
   outb (ATA_DRIVE_SELECT (bus),drive);
   ATA_SELECT_DELAY (bus);
 }

  int atapi_drive_startstop(uint32 drive,uint32 bus){
    printf("\n%s\n","entre");
   //Operation code and bit 1 of byte 4 -LoEj(eject)- as 1.
   uint8 startstop_cmd[12]={0x1B,0,0,0,2,0,0,0,0,0,0,0}; 
   uint8 allowremoval_cmd[12]={0x1E,0,0,0,0,0,0,0,0,0,0,0};
   uint8 status;
 

   // while((status=inb(ATA_COMMAND(bus))) & 0x80){
   //  rprintf("while1-busy");
   //  //BUSY
   //  // If the first bit of the status register (BUSY) isn't 0, the device is busy,
   //  // so keep looping until it isn't.
   // }

    // // _Cli();
   // while(!((status=inb(ATA_COMMAND(bus))) & 0x60)){
   //  rprintf("%x%s",status,"2");
   //  //NOT READY
   // }
   // /* DRQ or ERROR */
   // if(status & 0x1){
   //    return -1;
   // }

   ata_drive_select (bus, drive);
   //TODO reemplazar por drive
   // outb(ATA_DRIVE_SELECT(bus),0x10); //0 master, 10h slave

   outb(ATA_FEATURES(bus),0);

   /* Set Nien on device control register to 1 to skip waiting state. 
   nIEN is the second bit from the right here */
   // outb(ATA_DCR(bus),0x10); 

  /* Send the ATAPI PACKAGE command to the command register */
   outb(ATA_COMMAND(bus),0xA0);   
   //wait 400ns
   ATA_SELECT_DELAY(bus);
   while(status=inb(ATA_COMMAND(bus)) & 0x80){
   rprintf("while1");
    }
   while(!(status=inb(ATA_COMMAND(bus)) & 0x8)){
    rprintf("while2");
    //DATA TRANSFER REQUESTED
   }
   

  /* Send ATAPI/SCSI command as 6 words, to the data port */
   // outsw(ATA_DATA(bus), (uint16 *)allowremoval_cmd,6);
  outb(ATA_DATA(bus),0x1E);
  outb(ATA_DATA(bus),0);
  outb(ATA_DATA(bus),0);
  outb(ATA_DATA(bus),0);
  outb(ATA_DATA(bus),0);
  outb(ATA_DATA(bus),0);

   // inb(ATA_DCR(bus));
   while(status=inb(ATA_COMMAND(bus)) & 0x80){
   rprintf("while1-allow removal");
   //BUSY
   }
   while(!(status=inb(ATA_COMMAND(bus)) & 0x8)){
    rprintf("while2-allow removal");
    //DATA TRANSFER REQUESTED
   }

    outb(ATA_COMMAND(bus),0xA0);   
   //wait 400ns
   // ATA_SELECT_DELAY(bus);
   while(status=inb(ATA_COMMAND(bus)) & 0x80){
     rprintf("while1-2");
    }
   while(!(status=inb(ATA_COMMAND(bus)) & 0x8)){
    rprintf("while2-2");
    //DATA TRANSFER REQUESTED
   }


  /* Send ATAPI/SCSI command as 6 words, to the data port */
   // outsw(ATA_DATA(bus), (uint16 *)startstop_cmd,6);
  outb(ATA_DATA(bus),0x1B);
  outb(ATA_DATA(bus),0);
  outb(ATA_DATA(bus),2);
  outb(ATA_DATA(bus),0);
  outb(ATA_DATA(bus),0);
  outb(ATA_DATA(bus),0);  

  //wait 400ns
   // ATA_SELECT_DELAY(bus);
   // inb(ATA_DCR(bus));
   while(status=inb(ATA_COMMAND(bus)) & 0x80){
     rprintf("while 1 -BUSY- startstop_cmd");
   //BUSY
   }

   // _Sti();

   return 1;
 }

/* Use the ATA IDENTIFY command to find out what kind of drive is
* attached to the given bus/slot.  */
uint32
ata_identify (uint32 bus, uint32 drive)
 {
   uint8 status;
   uint16 buffer[256];

   
   ata_drive_select (bus, drive);

   // while(!(status=inb(ATA_COMMAND(bus)) & 0x60)){
   //  rprintf("loop0");
   // }

   outb (ATA_COMMAND (bus),0xEC);       /* Send IDENTIFY command */
   
   ATA_SELECT_DELAY (bus);
 
   status = inb (ATA_COMMAND (bus));
   if (status == 0) {
     // rprintf ("ATA bus %X drive %X does not exist\n", bus, drive);
     return NOTCD;
   }
 
   if (status & 0x1) {
     /* Drive does not support IDENTIFY.  Probably a CD-ROM. */
     goto guess_identity;
   }
 
   /* Poll the Status port (0x1F7) until bit 7 (BSY, value = 0x80)
    * clears, and bit 3 (DRQ, value = 8) sets -- or until bit 0 (ERR,
    * value = 1) sets. */
 
   while ((status = inb (ATA_COMMAND (bus))) & 0x80){
     asm volatile ("pause");
     // printf("%s\n","busy");
   }     /* BUSY */

  while (!((status = inb (ATA_COMMAND (bus))) & 0x8) && !(status & 0x01)){
     // printf("%x",status);
     asm volatile ("pause");
  }

  if (status & 0x1) {
    // rprintf ("ATA bus %X drive %X caused error.\n", bus, drive);
    goto guess_identity;
  }
 
   /* Read 256 words */
   insw (ATA_DATA (bus), buffer, 256);
 
 #ifdef DEBUG_ATA
   {
     int i, j;
 
     // DLOG ("IDENTIFY (bus: %X drive: %X) command output:", bus, drive);
     /* dump to com1 */
     for (i = 0; i < 32; i++) {
       for (j = 0; j < 8; j++) {
        rprintf ("%.4X ", buffer[i * 32 + j]);
       }
       rprintf ("\n");
     }
   }
 #endif
 
   if (buffer[83] & (1 << 10))
   // rprintf ("LBA48 mode supported.\n");
   // rprintf ("LBA48 addressable sectors: %.4X %.4X %.4X %.4X\n",
   //                buffer[100], buffer[101], buffer[102], buffer[103]);
   rprintf("not a cd");   
   return NOTCD;

  guess_identity:{
     uint8 b1, b2;
 
     b1 = inb (ATA_ADDRESS2 (bus));
     b2 = inb (ATA_ADDRESS3 (bus));
      
     // rprintf ("ata_detect: %.2X %.2X\n", b1, b2);
 
     if (b1 == 0x14 && b2 == 0xEB) {
       rprintf("is a CD");
       // rprintf("P-ATAPI detected\n");
       return ISCD;
     }

     //TODO: probar eliminar estos
     if (b1 == 0x69 && b2 == 0x96) {
      rprintf("is a CD");
       // rprintf ("S-ATAPI detected\n");
       return ISCD;
     }
     if (b1 == 0x3C && b2 == 0xC3) {
       rprintf("is a CD");
       // rprintf ("SATA detected\n");
       return ISCD;
     }
     rprintf("NOT a CD");
     return NOTCD;
   }
}