import { BrowserWindow, ipcMain } from "electron";

export class CommonWindowEvent {
  /**
   * 根据事件对象内的webContents属性获取对应的窗口
   * @param event 事件对象
   * @returns 窗口对象
   */
  private static getWin(event: any) {
    return BrowserWindow.fromWebContents(event.sender);
  }
  /**
   * 监听与窗口相关的事件
   * 使用事件对象获取对应的窗口，然后再对应的窗口上执行操作
   */
  public static listen() {
    ipcMain.handle("minimizeWindow", (e) => {
      this.getWin(e)?.minimize();
    });

    ipcMain.handle("maxmizeWindow", (e) => {
      this.getWin(e)?.maximize();
    });

    ipcMain.handle("unmaximizeWindow", (e) => {
      this.getWin(e)?.unmaximize();
    });

    ipcMain.handle("hideWindow", (e) => {
      this.getWin(e)?.hide();
    });

    ipcMain.handle("showWindow", (e) => {
      this.getWin(e)?.show();
    });

    ipcMain.handle("closeWindow", (e) => {
      this.getWin(e)?.close();
    });
    ipcMain.handle("resizable", (e) => {
      return this.getWin(e)?.isResizable();
    });
  }
  /**
   * 监听某个窗口的最大化状态变化事件
   * 事件发生后向对应窗口的页面发送相应的消息 以使该页面有机会改变标题栏的相应图标
   * @param win
   */
  public static windowMaximizeChange(win: BrowserWindow) {
    win.on("maximize", () => {
      win.webContents.send("windowMaximized");
    });
    win.on("unmaximize", () => {
      win.webContents.send("windowUnmaximized");
    });
  }
}
