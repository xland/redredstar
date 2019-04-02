const contentJs = {
    curDom: null,
    floatDom: null,
    title: null,
    content: null,
    mouseMove(e) {
        var dom = document.elementFromPoint(e.clientX, e.clientY);
        this.floatDom.style.left = e.pageX + "px";
        this.floatDom.style.top = e.pageY + "px";
        this.floatDom.style.display = "inline-block";
        if (this.curDom == dom || dom.classList.contains("ex__xxm__float__div")) {
            return;
        }
        var lastDom = document.querySelector(".ex__xxm__border");
        if (lastDom) {
            lastDom.classList.remove("ex__xxm__border");
            this.floatDom.style.display = "none";
        }
        this.curDom = dom;
        dom.classList.add("ex__xxm__border");
    },
    mouseDown(e) {
        if (this.floatDom.innerHTML == "选为知识标题") {
            this.curDom.classList.add("ex__xxm__title");
            this.title = this.curDom.innerText;
            this.createFloatDom("选为知识正文");
        } else {
            this.content = this.curDom.innerHTML;
            window.onmousemove = null;
            window.onmousedown = null;
            chrome.runtime.sendMessage({
                title:this.title,
                content:this.content
            }, function (response) {
                console.log('收到来自后台的回复：' + response);
            });
        }
    },
    createFloatDom(str) {
        this.floatDom = document.createElement('div');
        this.floatDom.classList.add("ex__xxm__float__div");
        this.floatDom.innerHTML = str;
        document.body.appendChild(this.floatDom);
    },
    keyDown(e) {
        var ctrlKey = e.ctrlKey || e.metaKey;
        if (ctrlKey && e.altKey && e.keyCode == 88) {
            this.createFloatDom("选为知识标题");
            window.onmousemove = e => this.mouseMove(e);
            window.onmousedown = e => this.mouseDown(e);
        };
    }
}
document.addEventListener('DOMContentLoaded', function () {
    document.onkeydown = function (e) {
        contentJs.keyDown(e);
    };
});