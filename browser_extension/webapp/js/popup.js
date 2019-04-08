function start(type) {
    chrome.tabs.query({
        active: true,
        currentWindow: true
    }, function (tabs) {
        chrome.tabs.sendMessage(tabs[0].id, type);
        window.close();
    });
}
window.onload = function () {
    document.querySelector(".articleBtn").addEventListener("click", function () {
        start("article__xxm")
    })
    document.querySelector(".flowerBtn").addEventListener("click", function () {
        start("flower__xxm")
    })
}