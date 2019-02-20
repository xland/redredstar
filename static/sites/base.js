module.exports = {
    mime: {
        "jpeg": "image/jpeg",
        "jpg": "image/jpeg",
        "png": "image/png",
        "gif": "image/gif",
        "bmp": 'image/bmp'
    },
    ajaxInjector(cb) {
        var open = window.XMLHttpRequest.prototype.open;
        window.XMLHttpRequest.prototype.open = function (method, url, async, user, pass) {
            this.addEventListener("readystatechange", function () {
                if (this.readyState === 4) {
                    var obj = JSON.parse(this.responseText);
                    cb(obj);
                }
            }, false);
            open.apply(this, arguments);
        }
    }
}