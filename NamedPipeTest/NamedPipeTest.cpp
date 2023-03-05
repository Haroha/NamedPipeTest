#include <Windows.h>
#include <stdio.h>
#include <thread>

using namespace std;

char buffer[1024];

void StartAndListenPipe()
{
	HANDLE hPipe;
	DWORD dwRead;


	hPipe = CreateNamedPipe(L"\\\\.\\pipe\\Pipe",
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		sizeof(buffer),
		sizeof(buffer),
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);

	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL))
			{
				/* add terminating zero */
				buffer[dwRead] = '\0';
				printf("%s", buffer);
			}
		}

		DisconnectNamedPipe(hPipe);
	}
}

int main(void)
{
	thread pipeThread(StartAndListenPipe);
	pipeThread.join();

	return 0;
}