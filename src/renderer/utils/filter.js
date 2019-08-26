import Vue from 'vue'

Vue.filter("getSimpleTime", timeStamp => {
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
    } else {
        r = dateTime.format("yyyy-MM-dd hh:mm:ss");
    }
    return r;
});