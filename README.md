<h2 align="center">运行</h2>

> ### `a.exe`
>
> 快捷键 `` ALT+` `` 获取鼠标坐标或该点颜色并复制到剪切板

> ### `a.exe a.txt`
>
> 运行脚本 `a.txt`

<br>

<h2 align="center">脚本说明</h2>

1. 头部（必须写且在第一行）

```
BDO <run_times> <runEnd_wait(s)> [hotkey]
BDO 3 0 70
```

2. 打开 / 关闭游戏

```
OPEN [wait(ms)]
OPEN 800
#正数：HWND_TOPMOST => 将窗口置于最上方
#负数：HWND_BOTTOM  => 将窗口置于最下方

CLOSE
```

3. 按键

```
K <key> <keyUp_delay(ms)> [wait(ms)]
K Q 50 800

K1 <key1> <keyUp_delay(ms)> [wait(ms)]
K1 1 50 800

K2 <key> <key> <keyUp_delay(ms)> [wait(ms)]
K2 ALT Q 50 800
```

4. 鼠标左击

```
L <keyUp_delay(ms)> [wait(ms)]
L 50 800

L <x,y>/<color(r,g,b)(r,g,b)> [wait(ms)]
L 0,0 800
```

```
<x,y>           =>  0,0 / x,y
<color(r,g,b)(r,g,b)>  =>  color(255,255,255)(0,0,0)
```

5. 鼠标右击

```
R <keyUp_delay(ms)> [wait(ms)]
R 50 800

R <x,y>/<color(r,g,b)(r,g,b)> [wait(ms)]
R 0,0 800
```

6. 鼠标滚轮

```
W <x,y>/<color(r,g,b)(r,g,b)> <downMove_times> [wait(ms)]
W 0,0 3 800
```

7. 鼠标左击拖动

```
M <x,y>/<color(r,g,b)(r,g,b)> <x,y>/<color(r,g,b)(r,g,b)> [wait(ms)]
M 0,0 100,100 800
```

8. 等待时间

```
S <wait(ms)>
S 500
```

-   `run_times`：运行次数
-   `runEnd_wait(s)`：单次脚本运行完后的等待时间，当倒计时到达 `3` 秒内且游戏最小化时将弹出窗口，`0` 秒时再隐藏
-   `hotkey`：用于运行脚本的快捷按键，虚拟键码自行搜索，当设置有运行快捷键时按键 `End` 可结束脚本（正数：按键按下就执行，负数：按键抬起才执行）
-   `wait(ms)`：运行单行命令后的等待时间
-   `key`：可填按键 `0-9` `A-Z` `F1-F11` `ESC` `TAB` `ENTER` `CTRL` `SHIFT` `ALT` `SPACE` `SCRLK` `~` `,`
-   `key1`：可填按键 `0-9`（当且仅当选择物品数量时才用此命令）
-   `keyUp_delay(ms)`：按键按下到抬起之间的时间
-   `x,y`：可以填写坐标如 `100,200`，也可以直接填写 `x,y`。当填写 `x,y` 时，程序第一次运行到这行代码时，你需要按快捷键 `` ALT+` `` 获取坐标，之后的运行就自动使用这次得到的坐标
-   `color(r,g,b)(r,g,b)`：获取图片 `BDO.BMP`（自动截取的图片，保存在游戏截图文件夹内）中第一个满足条件的颜色的坐标（两点是 3 像素矩形左上与右下的顶点，匹配成功后取左上角一点为匹配到的坐标）
-   `downMove_times`：向下移动，以一个背包格子为单位

> 单行命令最后为 `0` 时可不填写  
> 所有功能均需游戏没最小化才会执行，否则直接跳过  
> 脚本以空行作为结束标记, 非匹配功能不会执行（例：`#K` 用以注释该命令）

<br>

<h2 align="center">循环语法</h2>

```
FOR <run_times>
K/K1/K2/W/L/R/M
END
```

```
FOR 180
    K R 10 800
    FOR 180
        K R 10 800
        K SPACE 10 40
        K2 ALT Q 50 800
    END
    S 800
    K R 10
END
```

<br>

<h2 align="center">脚本实列</h2>

```
BDO 100 0 -2 #右键
OPEN 800
K I 10 800
R color(84,163,100)(84,163,124) 800
K U 10 800
K I 10 800
CLOSE
```

<br>

<h2 align="center">依赖</h2>

```
dumpbin.exe /dependents BDO.exe

KERNEL32.dll
USER32.dll
GDI32.dll
SHELL32.dll
MSVCP140.dll
SHLWAPI.dll
gdiplus.dll
VCRUNTIME140_1.dll
VCRUNTIME140.dll
api-ms-win-crt-runtime-l1-1-0.dll
api-ms-win-crt-string-l1-1-0.dll
api-ms-win-crt-stdio-l1-1-0.dll
api-ms-win-crt-heap-l1-1-0.dll
api-ms-win-crt-utility-l1-1-0.dll
api-ms-win-crt-filesystem-l1-1-0.dll
api-ms-win-crt-convert-l1-1-0.dll
api-ms-win-crt-math-l1-1-0.dll
api-ms-win-crt-locale-l1-1-0.dll
```
