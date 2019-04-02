document.addEventListener('DOMContentLoaded', function () {
    document.getElementById('send').addEventListener('click', function () {
        var hostName = "com.xiangxuema.xxm";
        var port = chrome.runtime.connectNative(hostName);
        port.onMessage.addListener(function (message) {
            message = JSON.stringify(message);
            alert(message);
        });
        port.onDisconnect.addListener(function () {
            alert("已断开")
        });
        var message = {
            "text": "这是我的数据"
        };
        console.log(message);
        console.log(port);
        port.postMessage(message);
    });
})