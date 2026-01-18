#include "ata.h"

#define ATA_PRIMARY_IO 0x1F0
#define ATA_REG_DATA   0x00
#define ATA_REG_ERROR  0x01
#define ATA_REG_SECCNT 0x02
#define ATA_REG_LBA0   0x03
#define ATA_REG_LBA1   0x04
#define ATA_REG_LBA2   0x05
#define ATA_REG_HDDEV  0x06
#define ATA_REG_CMD    0x07
#define ATA_REG_STAT   0x07

#define ATA_CMD_READ_SECTORS  0x20
#define ATA_CMD_WRITE_SECTORS 0x30
#define ATA_CMD_CACHE_FLUSH   0xE7

#define ATA_STAT_BSY 0x80
#define ATA_STAT_DRQ 0x08
#define ATA_STAT_ERR 0x01

static inline unsigned char inb(unsigned short port) {
    unsigned char result;
    asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline void outb(unsigned short port, unsigned char data) {
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

static inline unsigned short inw(unsigned short port) {
    unsigned short result;
    asm volatile("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline void outw(unsigned short port, unsigned short data) {
    asm volatile("outw %0, %1" : : "a"(data), "Nd"(port));
}

static inline void ata_io_wait(void) {
    inb(ATA_PRIMARY_IO + ATA_REG_STAT);
    inb(ATA_PRIMARY_IO + ATA_REG_STAT);
    inb(ATA_PRIMARY_IO + ATA_REG_STAT);
    inb(ATA_PRIMARY_IO + ATA_REG_STAT);
}

static int ata_wait_not_busy(void) {
    for (unsigned int i = 0; i < 100000; i++) {
        unsigned char st = inb(ATA_PRIMARY_IO + ATA_REG_STAT);
        if ((st & ATA_STAT_BSY) == 0) {
            return 0;
        }
    }
    return -1;
}

static int ata_wait_drq(void) {
    for (unsigned int i = 0; i < 100000; i++) {
        unsigned char st = inb(ATA_PRIMARY_IO + ATA_REG_STAT);
        if (st & ATA_STAT_ERR) {
            return -1;
        }
        if (st & ATA_STAT_DRQ) {
            return 0;
        }
    }
    return -1;
}

static void ata_select_lba(unsigned int lba) {
    outb(ATA_PRIMARY_IO + ATA_REG_HDDEV, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO + ATA_REG_SECCNT, 1);
    outb(ATA_PRIMARY_IO + ATA_REG_LBA0, (unsigned char)(lba & 0xFF));
    outb(ATA_PRIMARY_IO + ATA_REG_LBA1, (unsigned char)((lba >> 8) & 0xFF));
    outb(ATA_PRIMARY_IO + ATA_REG_LBA2, (unsigned char)((lba >> 16) & 0xFF));
}

int ata_read_sector(unsigned int lba, unsigned char *buffer) {
    if (ata_wait_not_busy() != 0) {
        return -1;
    }
    ata_select_lba(lba);
    outb(ATA_PRIMARY_IO + ATA_REG_CMD, ATA_CMD_READ_SECTORS);
    ata_io_wait();

    if (ata_wait_drq() != 0) {
        return -1;
    }

    for (int i = 0; i < 256; i++) {
        unsigned short data = inw(ATA_PRIMARY_IO + ATA_REG_DATA);
        buffer[i * 2] = (unsigned char)(data & 0xFF);
        buffer[i * 2 + 1] = (unsigned char)(data >> 8);
    }
    return 0;
}

int ata_write_sector(unsigned int lba, const unsigned char *buffer) {
    if (ata_wait_not_busy() != 0) {
        return -1;
    }
    ata_select_lba(lba);
    outb(ATA_PRIMARY_IO + ATA_REG_CMD, ATA_CMD_WRITE_SECTORS);
    ata_io_wait();

    if (ata_wait_drq() != 0) {
        return -1;
    }

    for (int i = 0; i < 256; i++) {
        unsigned short data = (unsigned short)buffer[i * 2]
                            | ((unsigned short)buffer[i * 2 + 1] << 8);
        outw(ATA_PRIMARY_IO + ATA_REG_DATA, data);
    }

    outb(ATA_PRIMARY_IO + ATA_REG_CMD, ATA_CMD_CACHE_FLUSH);
    ata_io_wait();
    for (unsigned int i = 0; i < 100000; i++) {
        unsigned char st = inb(ATA_PRIMARY_IO + ATA_REG_STAT);
        if (st & ATA_STAT_ERR) {
            return -1;
        }
        if ((st & ATA_STAT_BSY) == 0) {
            return 0;
        }
    }
    return -1;
}
