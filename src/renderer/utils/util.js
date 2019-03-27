Array.prototype.last = function(){
    if(this.length && this.length>0){
        return this[this.length - 1];
    }
    return null;
};
//todo:改成过滤器
Date.prototype.format = function (fmt) {
    var o = {
        "M+": this.getMonth() + 1, //月份   
        "d+": this.getDate(), //日   
        "h+": this.getHours(), //小时   
        "m+": this.getMinutes(), //分   
        "s+": this.getSeconds(), //秒   
        "q+": Math.floor((this.getMonth() + 3) / 3), //季度   
        "S": this.getMilliseconds() //毫秒   
    };
    if (/(y+)/.test(fmt))
        fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length));
    for (var k in o)
        if (new RegExp("(" + k + ")").test(fmt))
            fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[k]) : (("00" + o[k]).substr(("" + o[k]).length)));
    return fmt;
}
//todo:改成过滤器
String.prototype.trim = function () {
    return this.replace(/(^\s*)|(\s*$)/g, "");
};
//todo:改成过滤器
String.prototype.getByteLength = function() {  
    if (this == null) return 0;
    return this.replace(/[^\x00-\xff]/g,"01").length;  
};
//todo:改成过滤器
String.prototype.getParamVal = function(name) { 
    if (this == null) return null;
    var paramStr = this.split('?')[1];
    var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)", "i"); 
    var r = paramStr.match(reg); 
    if (r != null) return unescape(r[2]); 
    return null; 
}

window.xxmPost = function(url, form, cb) {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", url, true);
    xhr.withCredentials = true;
    xhr.onreadystatechange = () => {
        if (xhr.readyState == 4) {
            if (xhr.status == 200 || xhr.status == 304) {
                cb(xhr.responseText)
            } else {
                console.log("post to jna error");
            }
        } else {
            //alert("错误：可能的原因是目标网站不支持您上传的图片");
        }
    }
    xhr.send(form);
}