const ipc = require('electron').ipcRenderer;
ipc.on('message', (event, message) => {
    console.log(message); // logs out "Hello second window!"
})
document.addEventListener("DOMContentLoaded", function () {
    var titleTb = document.getElementById("Editor_Edit_txbTitle");
    if(!titleTb || !blogEditor){
        return;//没有标题和内容区域，就认定不是文章编辑页面
    }
    console.log("alen");
});