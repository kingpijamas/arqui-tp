 #include "../include/sysio.h"
 // #include "../include/i386.h"

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long int uint64;

typedef struct{
 uint32 ata_type, ata_bus, ata_drive;
} ata_info;

extern ata_info pata_drives[4];

 /* The default and seemingly universal sector size for CD-ROMs. */
 #define ATAPI_SECTOR_SIZE 2048


 #define ATA_BUS_PRIMARY     0x1F0
 #define ATA_BUS_SECONDARY   0x170

 #define ATA_IRQ_PRIMARY     0x0E
 #define ATA_IRQ_SECONDARY   0x0F
 #define ATA_VECTOR_PRIMARY   0x27
 #define ATA_VECTOR_SECONDARY 0x28
 #define ATA_IRQ(bus)        (bus==ATA_BUS_PRIMARY ? ATA_IRQ_PRIMARY : ATA_IRQ_SECONDARY)
 #define ATA_VECTOR(bus)     (bus==ATA_BUS_PRIMARY ? ATA_VECTOR_PRIMARY : ATA_VECTOR_SECONDARY)

  /* The default ISA IRQ numbers of the ATA controllers. */
  #define ATA_IRQ_PRIMARY     0x0E
  #define ATA_IRQ_SECONDARY   0x0F

  /* The necessary I/O ports, indexed by "bus". */
  #define ATA_DATA(x)         (x)
  #define ATA_FEATURES(x)     (x+1)
  #define ATA_SECTOR_COUNT(x) (x+2)
  #define ATA_ADDRESS1(x)     (x+3)
  #define ATA_ADDRESS2(x)     (x+4)
  #define ATA_ADDRESS3(x)     (x+5)
  #define ATA_DRIVE_SELECT(x) (x+6)
  #define ATA_COMMAND(x)      (x+7)
  #define ATA_DCR(x)          (x+0x206)   /* device control register */

  /* valid values for "bus" */
 #define ATA_BUS_PRIMARY     0x1F0
 #define ATA_BUS_SECONDARY   0x170
  /* valid values for "drive" */
 #define ATA_DRIVE_MASTER    0xA0
 #define ATA_DRIVE_SLAVE     0xB0

 #define ATA_TYPE_NONE 0
 #define ATA_TYPE_PATA 1
 #define ATA_TYPE_PATAPI 2
 #define ATA_TYPE_SATA 3
 #define ATA_TYPE_SATAPI 4

#define NOTCD 0
#define ISCD 1
  /* ATA specifies a 400ns delay after drive switching -- often
   * implemented as 4 Alternative Status queries. */
  #define ATA_SELECT_DELAY(bus) \
{inb(ATA_DCR(bus));inb(ATA_DCR(bus));inb(ATA_DCR(bus));inb(ATA_DCR(bus));}

void ata_sreset (uint32 bus);

static inline void outw(uint16 us, uint16 usPort);
static inline void outsw (uint16 usPort, void *buf, int count);

int atapi_drive_startstop(uint32 drive,uint32 bus);