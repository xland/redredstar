const fs = require('fs');
const path = require('path');
const {
    clipboard,
    ipcRenderer
} = require('electron');
const {
    BrowserWindow
} = require('electron').remote;
const mime = {
    "jpeg":"image/jpeg",
    "jpg":"image/jpeg",
    "png":"image/png",
    "gif":"image/gif",
    "bmp":'image/bmp'
}

var uploadImg = function (dom, file) {
    var formData = new FormData();
    formData.append('imageFile', file);
    formData.append("mimeType", file.type);
    var xhr = new XMLHttpRequest();
    xhr.open("POST", 'https://upload.cnblogs.com/imageuploader/CorsUpload', true);
    xhr.withCredentials = true;
    xhr.setRequestHeader("Cache-Control", "no-cache");
    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 304)) {
            var imgObj = JSON.parse(xhr.responseText);
            console.log(imgObj);
        }
    };
    xhr.send(formData);
}

document.addEventListener("DOMContentLoaded", function () {
    console.log(BrowserWindow);
    var titleTb = document.getElementById("Editor_Edit_txbTitle");
    if (!titleTb) {
        return; //没有标题和内容区域，就认定不是文章编辑页面
    }
    ipcRenderer.on('message', (event, article) => {
        window.onbeforeunload = null;
        titleTb.value = article.title;

        var parser = new DOMParser();
        var doc = parser.parseFromString(article.content, "text/html");
        var imgs = doc.querySelectorAll('img');
        imgs.forEach(v => {
            if (v.dataset[article.siteId]) {
                v.src = v.dataset[article.siteId]
            } else {
                var filePath = decodeURI(v.src).substr(7);
                var extname = path.extname(filePath).substr(1);
                var buffer = fs.readFileSync(filePath);
                var file = new window.File([new Uint8Array(buffer)],path.basename(filePath),{
                    type:mime[extname]
                });
                uploadImg(v, file);
            }
            Object.keys(v.dataset).forEach(ds => {
                delete v.dataset[ds];
            })
        });

        doc.body.innerHTML

        var re = new RegExp("data-img_" + article.siteId, "gi");
        content = article.content.replace(re, 'src');
        content = content.replace(/data-img_.+?=".+?"/gi, '');

        var win = BrowserWindow.fromId(article.winId);
        win.focus();
        clipboard.writeHTML(article.content)
        tinyMCE.getInstanceById('Editor_Edit_EditorBody').focus();
        win.webContents.paste();
    })
});