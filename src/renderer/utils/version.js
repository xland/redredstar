import swal from 'sweetalert';
var http = window.nw.require('http');
var electron = require('electron');
http.get('http://xiangxuema.com/version.json', (resp) => {
    let data = '';
    resp.on('data', (chunk) => {
        data += chunk;
    });
    resp.on('end', () => {
        var versionObj = JSON.parse(data);
        var arr = versionObj.version.split('.').map((v) => {
            return parseInt(v)
        });
        var curVersion = window.nw.App.manifest.version;
        var curArr = curVersion.split('.').map((v) => {
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
                var osName = window.nw.require("os").platform;
                var os;
                if(osName == "darwin"){
                    os = "mac";
                }else if(osName == "win32"){
                    os = "win"
                }else{
                    os = "linux"
                }
                var url = encodeURI(versionObj.baseurl + versionObj[os]);
                electron.remote.shell.openExternal(url);
            })
        }
    });
});