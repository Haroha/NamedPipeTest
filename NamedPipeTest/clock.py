import time
import datetime
import win32pipe, win32file, pywintypes


def pipe_client():
    print("pipe client")
    while True:
        try:
            handle = win32file.CreateFile(
                r'\\.\pipe\Pipe',
                win32file.GENERIC_READ | win32file.GENERIC_WRITE,
                0,
                None,
                win32file.OPEN_EXISTING,
                0,
                None
            )
            res = win32pipe.SetNamedPipeHandleState(handle, win32pipe.PIPE_READMODE_MESSAGE, None, None)
            if res == 0:
                print(f"SetNamedPipeHandleState return code: {res}")
            while True:
                win32file.WriteFile(handle, str.encode("\x1b[H"))
                win32file.WriteFile(handle, str.encode(f"{datetime.datetime.now()}"))
        except pywintypes.error as e:
            if e.args[0] == 2:
                print("no pipe, trying again in a sec")
                time.sleep(1)
            elif e.args[0] == 109:
                print("broken pipe, bye bye")
                break
            else:
                break
        except KeyboardInterrupt:
                break
        finally:
            win32file.CloseHandle(handle)


if __name__ == '__main__':
        pipe_client()