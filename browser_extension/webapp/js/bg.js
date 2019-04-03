chrome.runtime.onMessage.addListener(function (request, sender, sendResponse) {
    console.log("event in bg:")
    var hostName = "com.xiangxuema.xxm";
    chrome.runtime.sendNativeMessage(hostName, request, function (response) {
        console.log("Received " + response);
    });
    sendResponse('bg log end');
});

// "permissions": [
//     "nativeMessaging"
//   ]

// "background": {
//     "scripts": ["js/bg.js"]
//   },
//  "update_url": "http://xiangxuema.com/chrome_extension/updates.xml",