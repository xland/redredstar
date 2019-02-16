var siteFrame = document.getElementById("siteFrame");

siteFrame.onload = function () {
    var win = siteFrame.contentWindow;
    var titleTb = win.document.getElementById("Editor_Edit_txbTitle");
    if(!titleTb || !win.blogEditor){
        return;//没有标题和内容区域，就认定不是文章编辑页面
    }
    titleTb.value = subWin.data.article.title;
    var clipboard = nw.Clipboard.get();
    clipboard.clear();
    clipboard.set(subWin.data.article.content,"html")
    win.tinyMCE.getInstanceById('Editor_Edit_EditorBody').focus();
    document.execCommand('paste')
    // window.addEventListener('message', function(event) {
    //     var data = JSON.parse(event.data);
    //     console.log(data);
    // }, false);
    // var subWin = nw.Window.get();
    // subWin.window.postMessage("articlePageReady");
}