import swal from 'sweetalert';
var electron  = require("electron");
var checkVersion = function (versionObj) {
    var arr = versionObj.version.split('.').map((v) => {
        return parseInt(v)
    });
    var curArr = electron.remote.app.getVersion().split('.').map((v) => {
        return parseInt(v);
    });
    var flag = false;
    if (arr[0] > curArr[0]) {
        flag = true;
    } else if (arr[0] == curArr[0]) {
        if (arr[1] > curArr[1]) {
            flag = true;
        } else if (arr[1] == curArr[1]) {
            if (arr[2] > curArr[2]) {
                flag = true;
            }
        }
    }
    if (flag) {
        swal({
            icon: "info",
            text: "有新版本可供升级",
            buttons: [
                false, "去升级"
            ]
        }).then((value) => {
            if (!value) return;
            var osName = electron.remote.process.platform;
            var os;
            if (osName == "darwin") {
                os = "mac";
            } else if (osName == "win32") {
                os = "win"
            } else {
                os = "linux"
            }
            var url = encodeURI(versionObj.baseurl + versionObj[os]);
            electron.remote.shell.openExternal(url);
        })
    }
};

var xhr = new XMLHttpRequest();
xhr.open("GET", 'http://xiangxuema.com/version.json', true);
xhr.onreadystatechange = () => {
    if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 304)) {
        var r = JSON.parse(xhr.responseText);
        checkVersion(r);
    }
}
xhr.send();