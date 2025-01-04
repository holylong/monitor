import os
import subprocess
import tkinter as tk
from tkinter import messagebox
import win32com.client
from pynput import keyboard

desktop_path = os.path.join(os.environ['USERPROFILE'], 'Desktop')
icons = [f for f in os.listdir(desktop_path) if os.path.isfile(os.path.join(desktop_path, f)) or os.path.isdir(os.path.join(desktop_path, f))]

def get_target_path(lnk_path):
    shell = win32com.client.Dispatch("WScript.Shell")
    shortcut = shell.CreateShortCut(lnk_path)
    return shortcut.Targetpath

def show_window(icons):
    root = tk.Tk()
    root.title("桌面图标管理工具")

    listbox = tk.Listbox(root)
    listbox.pack()

    for icon in icons:
        listbox.insert(tk.END, icon)

    listbox.bind("<Double-Button-1>", lambda event: open_app(listbox.get(tk.ACTIVE)))

    root.mainloop()

def open_app(app_name):
    app_path = os.path.join(desktop_path, app_name)
    if os.path.isfile(app_path):
        if app_path.endswith('.lnk'):
            target_path = get_target_path(app_path)
            subprocess.Popen([target_path])
        else:
            subprocess.Popen([app_path])
    elif os.path.isdir(app_path):
        subprocess.Popen(f'explorer {app_path}')
    else:
        messagebox.showerror("错误", f"无法找到 {app_name}")

def on_activate():
    show_window(icons)

# with keyboard.GlobalHotKeys(
#         keyboard.HotKey.parse('<alt>+n'),
#         on_activate) as h:
#     h.join()

# 监听快捷键 ALT + N
with keyboard.GlobalHotKeys('<alt>+n', on_activate) as h:
    h.join()