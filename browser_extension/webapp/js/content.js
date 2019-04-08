const contentJs = {
    curDom: null,
    floatDom: null,
    title: null,
    content: null,
    baseUrl: 'http://localhost:9416',
    flowerTempFlag: false,
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
    showReceiveMsg(receiveMsg) {
        var dom = document.createElement('div');
        dom.classList.add("ex__xxm__msg__div");
        if (!receiveMsg.ok) {
            dom.style.background = "red";
        }
        dom.innerHTML = receiveMsg.msg;
        document.body.appendChild(dom);
        setTimeout(() => {
            this.dispose();
        }, 1218);
    },
    post(url, data) {
        var errMsg = {
            "ok": false,
            "msg": "请检查“想学吗”是否已启动"
        };
        var xhr = new XMLHttpRequest();
        xhr.open("POST", url, true);
        xhr.setRequestHeader("Content-Type", "application/json");
        xhr.withCredentials = true;
        xhr.onreadystatechange = () => {
            if (xhr.readyState == 4) {
                if (xhr.status == 200 || xhr.status == 304) {
                    var receiveMsg = JSON.parse(xhr.responseText);
                    this.showReceiveMsg(receiveMsg)
                } else {
                    this.showReceiveMsg(errMsg)
                }
            } else {
                this.showReceiveMsg(errMsg)
            }
        }
        xhr.send(data);
    },
    postArticle() {
        var url = this.baseUrl + "/newArticleFromWebApp";
        var data = JSON.stringify({
            title: this.title,
            content: this.content,
            from_url: window.location.href
        });
        this.post(url, data);
    },
    postFlower() {
        var url = this.baseUrl + "/newFlowerFromWebApp";
        var data = JSON.stringify({
            content: window.getSelection().toString(),
            from_url: window.location.href
        });
        this.post(url, data);
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
            this.postArticle();
        }
    },
    createFloatDom(str) {
        this.floatDom = document.createElement('div');
        this.floatDom.classList.add("ex__xxm__float__div");
        this.floatDom.innerHTML = str;
        document.body.appendChild(this.floatDom);
    },
    dispose() {
        window.onmousemove = null;
        window.onmousedown = null;
        var borderDom = document.querySelector(".ex__xxm__border");
        if (borderDom) {
            borderDom.classList.remove('ex__xxm__border');
        }
        var titleDom = document.querySelector(".ex__xxm__title");
        if (titleDom) {
            titleDom.classList.remove('ex__xxm__title')
        }
        var msgDom = document.querySelector(".ex__xxm__msg__div");
        if (msgDom) {
            msgDom.remove();
        }
        var floatDoms = document.querySelectorAll(".ex__xxm__float__div");
        if (floatDoms) {
            floatDoms.forEach(v => {
                v.remove();
            })
        }
    },
    startArticle() {
        this.createFloatDom("选为知识标题");
        window.onmousemove = e => this.mouseMove(e);
        window.onmousedown = e => this.mouseDown(e);
    },
    startFlower() {
        var self = this;
        var selectEndEvent = function (event) {
            window.removeEventListener("mouseup", selectEndEvent);
            window.removeEventListener("selectstart",selectStartEvent);
            self.postFlower();
        }
        var selectMouseMove = function (e) {
            self.floatDom.style.left = e.pageX + "px";
            self.floatDom.style.top = e.pageY + "px";
            self.floatDom.style.display = "inline-block";
        }
        var selectStartEvent = function () {
            self.createFloatDom("选为思想火花");
            window.onmousemove = selectMouseMove;
            window.addEventListener("mouseup", selectEndEvent);
        };
        window.addEventListener("selectstart",selectStartEvent);
    }
}
chrome.runtime.onMessage.addListener(function (request, sender, sendResponse) {
    if (request == "article__xxm") {
        contentJs.startArticle();
    }
    if (request == "flower__xxm") {
        contentJs.startFlower();
    }
});
document.onkeydown = function (e) {
    var ctrlKey = e.ctrlKey || e.metaKey;
    if (ctrlKey && e.keyCode == 27) {
        contentJs.dispose();
    }
};