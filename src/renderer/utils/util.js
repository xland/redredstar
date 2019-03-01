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
//todo:改成过滤器
window.getSimpleTime = function(timeStamp) {
    var dateTime = new Date(timeStamp);
    var now = new Date();
    var milliseconds = now - dateTime;
    var r = null;
    if (milliseconds < 60000) {
        r = '刚刚';
    } else if (60000 <= milliseconds && milliseconds <= 3600000) {
        r = Math.round((milliseconds / 60000)) + '分钟前';
    } else if (3600000 < milliseconds && milliseconds <= 3600000 * 24) {
        r = Math.round(milliseconds / (3600000)) + '小时前';
    } else if (3600000 * 24 < milliseconds && milliseconds <= 3600000 * 24 * 26) {
        r = Math.round(milliseconds / (3600000 * 24)) + '天前';
    } else if (3600000 * 24 * 26 < milliseconds && dateTime.getFullYear() == now.getFullYear()) {
        r = dateTime.format("MM-dd hh:mm:ss");
    }else{
        r = dateTime.format("yyyy-MM-dd hh:mm:ss");
    }
    return r;
}