const { clipboard,ipcRenderer } = require('electron');
const {
    BrowserWindow
} = require('electron').remote
document.addEventListener("DOMContentLoaded", function () {
    console.log(BrowserWindow);
    var titleTb = document.getElementById("Editor_Edit_txbTitle");
    if(!titleTb){
        return;//没有标题和内容区域，就认定不是文章编辑页面
    }
    ipcRenderer.on('message', (event, article) => {
        window.onbeforeunload = null;
        var win = BrowserWindow.fromId(article.winId);
        win.focus();
        titleTb.value = article.title;
        clipboard.writeHTML(article.content)
        tinyMCE.getInstanceById('Editor_Edit_EditorBody').focus();
        win.webContents.paste();
    })
});