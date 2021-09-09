#include "redis.h"
#include <iostream>

#ifdef _WIN32
#pragma comment(lib, "WS2_32.Lib")
#endif

template <typename T>
void println(T t)
{
	std::cout << t << std::endl;
}

int demo(int argc, char **argv)
{
	RedisClient rc("127.0.0.1:8000");
	if (rc.err)
	{
		println(rc.errstr);
		return 1;
	}

	char UserInputBuffer[1024];
	int index = 0;

	for (;;)
	{
		int c = getchar();
		UserInputBuffer[index++] = c;
		if (c == '\n' || c == '\r\n')
		{
			UserInputBuffer[index - 1] = '\0';
			index = 0;
			RedisReply *r = rc.command(UserInputBuffer);
			if (rc.err)
			{
				println(rc.errstr);
			}
			else
			{
				println("type is:");
				println(r->GetTypeStr());
				if (r->GetStrSize() > 0)
					println(r->GetStr());
			}
		}
	}
}
