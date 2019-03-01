const electron = require('electron');
var fs = require('fs');
var path = require('path');
var readFileConfig = {
    encoding: 'utf8'
}
//数据存储目录，如果不存在就创建
var basePath = path.join(electron.remote.app.getPath('userData'), "/xxm");
if (!fs.existsSync(basePath)) {
    fs.mkdirSync(basePath);
    var userData = {
        autoSaveIntervalSeconds: 8,
        tabs: [{
            text: "我的知识",
            url: '/'
        }],
        tabIndex: 0,
        userName:'',
        phoneNum:'',
        sex:'',
        domain:[],
        createAt: new Date().getTime()
    };
    fs.writeFileSync(path.join(basePath,"u.data"), JSON.stringify(userData));
    fs.writeFileSync(path.join(basePath,"a.data"), '[]');
    fs.writeFileSync(path.join(basePath,"t.data"), '[]');
}

var initData = function (name) {
    var dataStr = fs.readFileSync(path.join(basePath, name + ".data"), {
        encoding: 'utf8'
    });
    let result = JSON.parse(dataStr);
    //todo: bug fix for compatible
    //老版本的一些问题
    if(name == "a"){
        result.forEach(item=>{
            if(!item.update){
                item.update = new Date().getTime();
            }
        })
    }
    if(name == "u"){
        if(!result.compressHeight){
            result.compressHeight = 768;
            result.compressWidth = 1024;
        }
    }
    return result;
};
const store = {
    basePath,
    readFileConfig,
    u: initData("u"),
    a: initData("a"),
    t: initData("t"),
    needSave: {
        u: false,
        a: false,
        t: false,
        c: false,
    },
    aIndex: -1,
    timer: null,
    timerGuard: 8,
    saveOneData(name) {
        var str = JSON.stringify(this[name]);
        fs.writeFileSync(path.join(this.basePath, name + ".data"), str, this.readFileConfig);
        this.needSave[name] = false;
    },
    saveContent() {
        this.needSave.c = false;
        this.a[this.aIndex].update = new Date().getTime();
        this.saveOneData("a");
        var cPath = path.join(this.basePath, this.a[this.aIndex].id + "/a.data");
        var str = window.UE.instants.ueditorInstant0.getContent();
        fs.writeFileSync(cPath, str, this.readFileConfig);
    },
    save() {
        if (this.needSave.c && this.aIndex >=0) {
            this.saveContent();
        }
        if (this.needSave.u) {
            this.saveOneData("u");
        }
        if (this.needSave.t) {
            this.saveOneData("t");
        }
        if (this.needSave.a) {
            this.saveOneData("a");
        }
    }
}
export default store