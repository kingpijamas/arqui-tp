 #include "../include/atapi.h"
 #include "../include/kernel.h"

// ata_info pata_drives[4];
// bool mp_ISA_PC = 0;
// extern uint32 mp_ISA_bus_id;

//  /* ata_current_task has exclusive access to ATA.  If an ATA IRQ comes
//   * in, we assume that ata_current_task is waiting on it and needs to
//   * be woken. */
// static uint16 ata_current_task = 0;


//  /* Count number of times IRQs are triggered. */
//  uint32 ata_primary_irq_count = 0, ata_secondary_irq_count = 0;
//  uint32 ata_irq_count = 0;
//  uint64 irq_turnaround = 0, irq_response = 0, irq_start = 0, irq_resp_max = 0;
//  uint64 irq_resp_min = ~0LL;

// typedef uint32 (*vector_handler) (uint8 vector);

// static vector_handlers vector_handler[256];
//   Program an entry in the table. 
//  void
//  set_vector_handler (uint8 vec, vector_handler func)
//  {
//    vector_handlers[vec] = func;
// }

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
 
 /* Note: on the primary bus, the standar set of ATA IO ports is 0x1F7 
 You will need a buffer, DMA buffer or PIO buffer.
 If it is going to be a PIO buffer, then you need to know the size of the buffer. Call this size "maxByteCount.
 It must be an unsigned word, and 0 is illegal.*/
  int atapi_drive_eject(uint32 drive,uint32 bus){
     printf("\n%s\n","entre");
   //Operation code and 10 reserved spaces
   uint8 readcapacity_cmd[12]={0x1B,0,0,0,2,0,0,0,0,0,0,0}; 
   uint8 *buffer;
   //Size of the buffer (cd sector size, take a look at this)
   // uint32 maxByteCount=ATAPI_SECTOR_SIZE;
   
   //not using DMA mode.
   bool isDMA=0;
   uint8 status;
   int size;
 
   status = inb(ATA_COMMAND(bus)); //status register
   rprintf("%x",status);

   while(status=inb(ATA_COMMAND(bus)) & 0x80){
      rprintf("%x",status);
      //BUSY
    // If the first bit of the status register (BUSY) isn't 0, the device is busy,
    // so keep looping until it isn't.
   }

   _Cli();
   rprintf("cli");

   while(!(status=inb(ATA_COMMAND(bus)) & 0x60)){
    rprintf("%x%s",status,"2");
    //NOT READY
   }

   //TODO reemplazar por drive
   outb(ATA_DRIVE_SELECT(bus),0x10); //0 master, 10h slave

   /* Set Nien on device control register to 1 to skip waiting state. 
   nIEN is the second bit from the right here */
   outb(ATA_DCR(bus),0x10); 

  /* Send the ATAPI PACKAGE command to the command register */
   outb(ATA_COMMAND(bus),0xA0);   

   //wait 400ns
   ATA_SELECT_DELAY(bus);

   rprintf("%s\n","OK1");

   while(status=inb(ATA_COMMAND(bus)) & 0x80){
   //BUSY
   }

   rprintf("%s\n","OK2");

   while(!(status=inb(ATA_COMMAND(bus)) & 0x8)){
    //DATA TRANSFER REQUESTED
   }

   rprintf("%s\n","OK3");

  /* Send ATAPI/SCSI command as 6 words, to the data port */
  outsw(ATA_DATA(bus), (uint16 *)readcapacity_cmd,6);

  inb(ATA_DCR(bus));


   while(status=inb(ATA_COMMAND(bus)) & 0x80){
   //BUSY
   }
   rprintf("%s\n","OK4");

   // while(!(status=inb(ATA_COMMAND(bus)) & 0x8)){
   //  //DATA TRANSFER REQUESTED
   // }

   rprintf("%s\n","OK5");

   _Sti();

   return 1;

//    /* Device is in the primary slave. Select the target device by setting 
//     * the master/slave bit in the Device Selector Register. 
//     * There are no other bits needed */
//    outb(bus, drive & (1<<4));
//    ATA_SELECT_DELAY(bus);
//    rprintf("%s\n","OK1");
   
//   /* Set the feature 0 for PIO */
//    outb(ATA_FEATURES(bus),isDMA);
//    rprintf("%s\n","ok2");

//   /* The Sectorcount Register and LBA Low Register are unused currently. 
//    Send maxByteCount in LBA Mid and LBA High Registers */
//    outb(ATA_ADDRESS2(bus),0x0008);
//    outb(ATA_ADDRESS3(bus),0x0008);
//    // outb(ATA_ADDRESS2(bus), (maxByteCount & 0xFF));
//    // outb(ATA_ADDRESS3(bus), (maxByteCount >> 8));
//    rprintf("%s\n","ok3");

//   /* Send the ATAPI PACKAGE command to the command register */
//    outb(ATA_COMMAND(bus),0xA0);
//    rprintf("%s\n","ok4");
 
//  // TODO REVISAR. Code for waiting an IRQ taken from wiki.osdev.org/ATAPI 
//    status= (inb(ATA_COMMAND(bus)) & 0x80);
//    rprintf("%s","status:");
//    rprintf("%x\n",status);
   
//    if(status==0){
//     rprintf("ATA bus %X drive %X does not exist\n",bus,drive);
//     return size;
//    }
   
//   while(status= (inb(ATA_COMMAND(bus)) & 0x80)){ //BUSY
//      printf("%s\n","while1");
//      asm volatile ("pause");
//   }

// _Cli();

//   //No está ocupado y no hay error
//   while(!(status= (inb(ATA_COMMAND(bus)) & 0x8)) && !(status & 0x1)){
//     asm volatile ("pause");
//     printf("%s\n","while2");
//  }

//  /* DRQ or ERROR */
//  if(status & 0x1){
//     size=-1;
//     return size;
//  }

 //  Send ATAPI/SCSI command as 6 words, to the data port 
 // outsw(ATA_DATA(bus), (uint16 *)readcapacity_cmd,6);

// _Sti();

//  /* Wait for IRQ --not implementing scheduler, so skipping this*/

//  /* Read actual size because is PIO mode.
//   * transfer size= (bytecount = LBA high << 8 | LBA mid) */
//  size= (((int) inb(ATA_ADDRESS3(bus))) << 8)|(int)(inb(ATA_ADDRESS2(bus)));

//  /* PIO transfer */
//  insw(ATA_DATA(bus),buffer,size/2);

//  status=inb(ATA_COMMAND(bus)) & 0x88;
//  while(status){
//   asm volatile ("pause");
//  }

//  return size;

 }


// void
// ata_drive_select (uint32 bus, uint32 drive)
//  {
//    outb (drive, ATA_DRIVE_SELECT (bus));
//    // ATA_SELECT_DELAY (bus);
//  }


//  /* IRQ handler for both drive controllers. */
//  static uint32
//  ata_irq_handler (uint8 vec)
//  {
//    // lock_kernel ();
//    // DLOG ("ata_irq_handler(%x) ata_current_task=%x", vec,
//          // ata_current_task);
//    if (vec == ATA_VECTOR_PRIMARY)
//      ata_primary_irq_count++;
//    else
//      ata_secondary_irq_count++;
 
//    if (irq_start != 0)
//      ata_irq_count++;
 
//    uint64 finish;
//    // RDTSC (finish);
//    if (irq_start != 0) {
//      irq_response += finish - irq_start;
//      if (irq_resp_max < finish - irq_start)
//        irq_resp_max = finish - irq_start;
//      if (irq_resp_min > finish - irq_start)
//        irq_resp_min = finish - irq_start;
//    }
 
//    // /* Unblock the task waiting for the IRQ. */
//    // if (ata_current_task)
//    //   wakeup (ata_current_task);
 
//    // unlock_kernel ();
//    return 0;
//  }


  // /* Use the ATA IDENTIFY command to find out what kind of drive is
  // * attached to the given bus/slot.  */
// uint32
// ata_identify (uint32 bus, uint32 drive)
//  {
//    uint8 status;
//    uint16 buffer[256];
 
//    ata_drive_select (bus, drive);
 
//    outb (0xEC, ATA_COMMAND (bus));       /* Send IDENTIFY command */
 
//    ATA_SELECT_DELAY (bus);
 
//    status = inb (ATA_COMMAND (bus));
//    if (status == 0) {
//      rprintf ("ATA bus %X drive %X does not exist\n", bus, drive);
//      return ATA_TYPE_NONE;
//    }
 
//    if (status & 0x1) {
//      /* Drive does not support IDENTIFY.  Probably a CD-ROM. */
//      goto guess_identity;
//    }
 
//    /* Poll the Status port (0x1F7) until bit 7 (BSY, value = 0x80)
//     * clears, and bit 3 (DRQ, value = 8) sets -- or until bit 0 (ERR,
//     * value = 1) sets. */
 
//    while ((status = inb (ATA_COMMAND (bus))) & 0x80)     /* BUSY */
//      asm volatile ("pause");
 
//    while (!((status = inb (ATA_COMMAND (bus))) & 0x8) && !(status & 0x1))
//      asm volatile ("pause");
 
//    if (status & 0x1) {
//      rprintf ("ATA bus %X drive %X caused error.\n", bus, drive);
//      goto guess_identity;
//    }
 
//    /* Read 256 words */
//    insw (ATA_DATA (bus), buffer, 256);
 
//  #ifdef DEBUG_ATA
//    {
//      int i, j;
 
//      DLOG ("IDENTIFY (bus: %X drive: %X) command output:", bus, drive);
//      /* dump to com1 */
//      for (i = 0; i < 32; i++) {
//        for (j = 0; j < 8; j++) {
//         rprintf ("%.4X ", buffer[i * 32 + j]);
//        }
//        rprintf ("\n");
//      }
//    }
//  #endif
 
//    if (buffer[83] & (1 << 10))
//      rprintf ("LBA48 mode supported.\n");
//    rprintf ("LBA48 addressable sectors: %.4X %.4X %.4X %.4X\n",
//                   buffer[100], buffer[101], buffer[102], buffer[103]);
//    return ATA_TYPE_PATA;

//      guess_identity:{
//      uint8 b1, b2;
 
//      b1 = inb (ATA_ADDRESS2 (bus));
//      b2 = inb (ATA_ADDRESS3 (bus));
 
//      rprintf ("ata_detect: %.2X %.2X\n", b1, b2);
 
//      if (b1 == 0x14 && b2 == 0xEB) {
//        rprintf("P-ATAPI detected\n");
//        return ATA_TYPE_PATAPI;
//      }
//      if (b1 == 0x69 && b2 == 0x96) {
//        rprintf ("S-ATAPI detected\n");
//        return ATA_TYPE_SATAPI;
//      }
//      if (b1 == 0x3C && b2 == 0xC3) {
//        rprintf ("SATA detected\n");
//        return ATA_TYPE_SATA;
//      }
//      return ATA_TYPE_NONE;
//    }
// }

//  //Code taken from http://wiki.osdev.org/Inline_Assembly/Examples
// static inline unsigned char inb( unsigned short port ) {
//     unsigned char ret;
//     asm volatile( "inb %1, %0"
//                   : "=a"(ret) : "Nd"(port) );
//     return ret;
// }

// //Code taken from http://wiki.osdev.org/Inline_Assembly/Examples
// static inline void outb(unsigned short port, unsigned char val){
//   asm volatile( "outb %0, %1"
//     : : "a"(val), "Nd"(port) );
// }

  // /* Use the ATAPI protocol to read a single sector from the given
  //  * bus/drive into the buffer using logical block address lba. */
  // int
  // atapi_drive_start_stop (uint32 bus, uint32 drive, uint32 lba, uint8 *buffer)
  // {
  //   /* 0xA8 is READ SECTORS command byte. */
  //   uint8 startstop_cmd[12] = { 0x1B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  //   uint8 status;
  //   int size;
  //   /* Tell the scheduler that this process is using the ATA subsystem. */
  //   // ata_grab ();
  //   /* Select drive (only the slave-bit is set) */
  //   outb (drive & (1 << 4), ATA_DRIVE_SELECT (bus));      
  //   ATA_SELECT_DELAY (bus);       /* 400ns delay */
  //   outb (0x0, ATA_FEATURES (bus));       /* PIO mode */
  //   outb (ATAPI_SECTOR_SIZE & 0xFF, ATA_ADDRESS2 (bus));
  //   outb (ATAPI_SECTOR_SIZE >> 8, ATA_ADDRESS3 (bus));
  //   outb (0xA0, ATA_COMMAND (bus));       /* ATA PACKET command */
  //   while ((status = inb (ATA_COMMAND (bus))) & 0x80)     /* BUSY */
  //     asm volatile ("pause");
  //   while (!((status = inb (ATA_COMMAND (bus))) & 0x8) && !(status & 0x1))
  //     asm volatile ("pause");
  //   /* DRQ or ERROR set */
  //   if (status & 0x1) {
  //     size = -1;
  //     goto cleanup;
  //   }
  //   startstop_cmd[9] = 1;               1 sector 
  //   startstop_cmd[2] = (lba >> 0x18) & 0xFF;   /* most sig. byte of LBA */
  //   startstop_cmd[3] = (lba >> 0x10) & 0xFF;
  //   startstop_cmd[4] = (lba >> 0x08) & 0xFF;
  //   startstop_cmd[5] = (lba >> 0x00) & 0xFF;   /* least sig. byte of LBA */
  //   /* Send ATAPI/SCSI command */
  //   outsw (ATA_DATA (bus), (uint16 *) startstop_cmd, 6);
  //   /* Wait for IRQ that says the data is ready. */
  //   // schedule ();
  //   /* Read actual size */
  //   size =
  //     (((int) inb (ATA_ADDRESS3 (bus))) << 8) |
  //     (int) (inb (ATA_ADDRESS2 (bus)));
  //   /* This example code only supports the case where the data transfer
  //    * of one sector is done in one step. */
  //   // ASSERT (size == ATAPI_SECTOR_SIZE);
  //   /* Read data. */
  //   insw (ATA_DATA (bus), buffer, size / 2);
  //   /* The controller will send another IRQ even though we've read all
  //    * the data we want.  Wait for it -- so it doesn't interfere with
  //    * subsequent operations: */
  //   // schedule ();
  //   /* Wait for BSY and DRQ to clear, indicating Command Finished */
  //   while ((status = inb (ATA_COMMAND (bus))) & 0x88) 
  //     asm volatile ("pause");
  //  cleanup:
  //   /* Exit the ATA subsystem */
  //   // ata_release ();
  //   return size;
  // }

//  /* Initialize and identify the ATA drives in the system. */
//  bool
//  ata_init (void)
//  {
//    uint32 bus, drive, i;
 
//    i = 0;
//    bus = ATA_BUS_PRIMARY;
//    drive = ATA_DRIVE_MASTER;
//    pata_drives[i].ata_type = ata_identify (bus, drive);
//    pata_drives[i].ata_bus = bus;
//    pata_drives[i].ata_drive = drive;
 
//    i = 1;
//    bus = ATA_BUS_PRIMARY;
//    drive = ATA_DRIVE_SLAVE;
//    pata_drives[i].ata_type = ata_identify (bus, drive);
//    pata_drives[i].ata_bus = bus;
//    pata_drives[i].ata_drive = drive;
 
//    i = 2;
//    bus = ATA_BUS_SECONDARY;
//    drive = ATA_DRIVE_MASTER;
//    pata_drives[i].ata_type = ata_identify (bus, drive);
//    pata_drives[i].ata_bus = bus;
//    pata_drives[i].ata_drive = drive;
 
//    i = 3;
//    bus = ATA_BUS_SECONDARY;
//    drive = ATA_DRIVE_SLAVE;
//    pata_drives[i].ata_type = ata_identify (bus, drive);
//    pata_drives[i].ata_bus = bus;
//    pata_drives[i].ata_drive = drive;
 
//    // if (mp_ISA_PC) {
//    //   set_vector_handler ((ATA_IRQ_PRIMARY - 8) + PIC2_BASE_IRQ,
//    //                       ata_irq_handler);
//    //   set_vector_handler ((ATA_IRQ_SECONDARY - 8) + PIC2_BASE_IRQ,
//    //                       ata_irq_handler);
//    // } else {
//      // IOAPIC_map_GSI (IRQ_to_GSI (mp_ISA_bus_id, ATA_IRQ_PRIMARY),
//      //                 ATA_VECTOR_PRIMARY, 0xFF00000000000800LL);
//      // IOAPIC_map_GSI (IRQ_to_GSI (mp_ISA_bus_id, ATA_IRQ_SECONDARY),
//      //                 ATA_VECTOR_SECONDARY, 0xFF00000000000800LL);
//      set_vector_handler (ATA_VECTOR_PRIMARY, ata_irq_handler);
//      set_vector_handler (ATA_VECTOR_SECONDARY, ata_irq_handler);
//    // }
 
//    return true;
// }