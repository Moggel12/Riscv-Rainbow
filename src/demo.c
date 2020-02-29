#include <hal.h>
#include <sendfn.h>

int main(void)
{
  unsigned char bytes[] = {0xDEu, 0xADu, 0xBEu, 0xEFu};
  hal_setup(CLOCK_FAST);
  send_start();
  send_string("targetname", hal_name);
  send_unsigned("u_bin", 0xDEADBEEFu, 2);
  send_unsigned("u_bin", 0, 2);
  send_unsigned("u_oct", 0xDEADBEEFu, 8);
  send_unsigned("u_oct", 0, 8);
  send_unsigned("u_dec", 0xDEADBEEFu, 10);
  send_unsigned("u_hex", 0xDEADBEEFu, 16);
  send_unsigned("u_hex", 0, 16);
  send_unsignedll("llu_bin", 0xDEADBEEFDEADBEEFllu, 2);
  send_unsignedll("llu_oct", 0xDEADBEEFDEADBEEFllu, 8);
  send_unsignedll("llu_dec", 0xDEADBEEFDEADBEEFllu, 10);
  send_unsignedll("llu_hex", 0xDEADBEEFDEADBEEFllu, 16);
  send_string("strings", "You can send \"escaped\"\n strings!");
  send_bytes("bytes", bytes, sizeof(bytes));
  send_stop();
	while (1)
		;
	return 0;
}
