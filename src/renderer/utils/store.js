const electron = require('electron');
const fs = require('fs');
const path = require('path');
import db from './db';
// db.init();
//数据存储目录，如果不存在就创建
if (!fs.existsSync(db.basePath)) {
    fs.mkdirSync(db.basePath);
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
    fs.writeFileSync(path.join(db.basePath,"u.data"), JSON.stringify(userData));
    fs.writeFileSync(path.join(db.basePath,"a.data"), '[]');
    fs.writeFileSync(path.join(db.basePath,"t.data"), '[]');
}

var initData = function (name) {
    var dataStr = fs.readFileSync(path.join(db.basePath, name + ".data"), db.readConfig);
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
        if(!result.imgSize){
            result.imgSize = {};
            result.imgSize.h = 800;
            result.imgSize.w = 1300;
        }
    }
    return result;
};
const store = {
    basePath:db.basePath,
    readFileConfig:db.readConfig,
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