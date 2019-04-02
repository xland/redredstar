chrome.runtime.onMessage.addListener(function (request, sender, sendResponse) {
    console.log(request, sender, sendResponse);
    var hostName = "com.xiangxuema.xxm";
    var port = chrome.runtime.connectNative(hostName);
    port.postMessage(request);
    port.onMessage.addListener(function (message) {
        message = JSON.stringify(message);
        alert(message);
    });
    sendResponse('我是后台，我已收到你的消息：' + JSON.stringify(request));
});