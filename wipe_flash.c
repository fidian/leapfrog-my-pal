/* Immense gratitude to Robert Copeland
 * http://bobcopeland.com/blog/2011/07/my-pal-scsi/
 */
#include <stdio.h>
#include <stdint.h>
#include <scsi/sg.h>
#include <sys/ioctl.h>

static int wipe_page(int fd, uint32_t addr)
{
    uint8_t cmdblk[] = {
        0xfd,               /* access flash */
        0x20,               /* write it (0x28 = read) */
        0, 0, 0, 0,
        0x06, 0, 0x08, 0,   /* no idea what the rest is */
        0, 0, 0, 0,
        0x47, 0x50
    };
    uint8_t response_buf[4096];
    uint8_t sense_buffer[32];
    int i;

    for (i = 0; i < sizeof(response_buf); i += 1) {
        response_buf[i] = 0xff;
    }

    cmdblk[2] = (addr >> 24) & 0xff;
    cmdblk[3] = (addr >> 16) & 0xff;
    cmdblk[4] = (addr >> 8) & 0xff;
    cmdblk[5] = addr & 0xff;

    sg_io_hdr_t io_hdr = {
        .interface_id = 'S',
        .cmd_len = sizeof(cmdblk),
        .mx_sb_len = sizeof(sense_buffer),
        .dxfer_direction = SG_DXFER_FROM_DEV,
        .dxfer_len = sizeof(response_buf),
        .dxferp = response_buf,
        .cmdp = cmdblk,
        .sbp = sense_buffer,
        .timeout = 20000
    };

    ioctl(fd, SG_IO, &io_hdr);

    return 0;
}

int main(int argc, char **argv) {
    int fd;
    FILE *fp;
    unsigned int addr;

    if (argc < 2) {
        printf("Specify a device to wipe\n");
        return -1;
    }

    fp = fopen(argv[1], "wb");
    
    if (!fp) {
        printf("Error opening file: %s\n", argv[1]);
    }

    fd = fileno(fp);

    for (addr = 0x01000; addr < 0x10000; addr += 4096) {
        fprintf(stderr, "Write address 0x%04x\r", addr);
        fflush(stderr);
        wipe_page(fd, addr);
    }

    fprintf(stderr, "\nDone\n");
}
