#include <stdint.h>

#include <machine.h>
#include <tk/sysdef.h>
#include <tk/syslib.h>

void sio_init(void)
{
	/* NOP: do nothing. already initialized by u-boot */
}

void sio_send_frame(unsigned char const *buf, int size)
{
	while (size > 0) {
		/* Wait until there is space in the FIFO */
		while (in_w(PL011_STAT) & 0x00000020U) {
			/* NOP */
		}

		out_w(PL011_IO, *buf);
		++buf; --size;
	}
}

void sio_recv_frame(unsigned char *buf, int size)
{
	while (size > 0) {
		/* Wait until there is data in the FIFO */
		while (in_w(PL011_STAT) & 0x00000010U) {
			/* NOP */
		}

		*buf = (unsigned char)in_w(PL011_IO);
		++buf; --size;
	}
}
