import win32gui
import win32con
import time
import ctypes


def is_admin():
    """检查程序是否以管理员权限运行"""
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False


def get_window_handles_and_titles():
    """获取所有可见窗口的句柄和标题"""
    windows = []

    def callback(hwnd, _):
        if win32gui.IsWindowVisible(hwnd) and win32gui.GetWindowText(hwnd):
            windows.append((hwnd, win32gui.GetWindowText(hwnd)))

    win32gui.EnumWindows(callback, None)
    return windows


def bring_window_to_front(hwnd):
    """
    将窗口设置到最前端并激活
    Args:
        hwnd (int): 窗口句柄
    """
    # 还原窗口（防止窗口被最小化或隐藏）
    win32gui.ShowWindow(hwnd, win32con.SW_RESTORE)
    # 设置到最前端
    win32gui.BringWindowToTop(hwnd)
    # 激活窗口
    win32gui.SetForegroundWindow(hwnd)


def switch_windows(interval=3):
    """
    循环切换并将每个窗口设置到最前端
    Args:
        interval (int): 每个窗口停留的时间（秒）
    """
    # 获取所有窗口句柄和标题
    windows = get_window_handles_and_titles()

    if not windows:
        print("未找到任何活动窗口！")
        return

    print("检测到的窗口列表:")
    for i, (_, title) in enumerate(windows, 1):
        print(f"{i}. {title}")

    print("\n开始循环切换窗口...")
    try:
        while True:
            for hwnd, title in windows:
                print(f"切换到窗口: {title}")
                bring_window_to_front(hwnd)
                time.sleep(interval)
    except KeyboardInterrupt:
        print("\n程序已终止。")


if __name__ == "__main__":
    if not is_admin():
        print("请以管理员权限运行程序！")
    else:
        switch_windows(interval=5)
