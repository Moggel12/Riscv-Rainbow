#include <hal.h>
#include <sendfn.h>

int main(void)
{
  unsigned char bytes[] = {0xDEu, 0xADu, 0xBEu, 0xEFu};
  hal_setup(CLOCK_FAST);
  send_start();
  send_string("targetname", hal_name);
  send_unsigned("unsigned", 0xDEADBEEFu);
  send_unsignedll("llu", 0xDEADBEEFDEADBEEFllu);
  send_string("strings", "You can send \"escaped\"\n strings!");
  send_bytes("bytes", bytes, sizeof(bytes));
  send_stop();
	while (1)
		;
	return 0;
}
