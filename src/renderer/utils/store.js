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
    return JSON.parse(dataStr);
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
    saveUAT(name){
        var str = JSON.stringify(this[name]);
        var uPath = path.join(this.basePath, name+".data");
        fs.writeFileSync(uPath, str);
    },
    saveC(){
        var str = window.UE.instants.ueditorInstant0.getContent();
        var cPath = path.join(self.basePath, self.a[self.aIndex].id + "/c.data");
        fs.writeFileSync(cPath, str);
    },
    aIndex: -1,
    timer: null,
    timerGuard: 8,
    saveOneData(name, cb) {
        var str = JSON.stringify(this[name]);
        var self = this;
        fs.writeFile(path.join(this.basePath, name + ".data"), str, this.readFileConfig, function (err) {
            self.needSave[name] = false;
            if (err) {
                console.log(err);
            }
            if(cb){
                cb();
            }
        });
    },
    saveContent(cb) {
        var self = this;
        self.needSave.c = false;
        if(self.aIndex < 0){
            if(cb){
                cb();
            }
            return;
        }
        self.a[self.aIndex].update = new Date().getTime();
        var aPath = path.join(self.basePath, self.a[self.aIndex].id + "/a.data");
        var str = window.UE.instants.ueditorInstant0.getContent();
        fs.writeFile(aPath, str, self.readFileConfig, function (err) {
            if (err) {
                console.log(err);
            }
            if(cb){
                cb();
            }
        });
    },
    save(cb) {
        var guardNum = 0;
        var selfCb = function () {
            guardNum -= 1;
            if (cb && guardNum <= 0) {
                cb();
            }
        }
        if (this.needSave.c) {
            guardNum += 1;
            this.saveContent(selfCb);
        }
        if (this.needSave.u) {
            guardNum += 1;
            this.saveOneData("u", selfCb);
        }
        if (this.needSave.t) {
            guardNum += 1;
            this.saveOneData("t", selfCb);
        }
        if (this.needSave.a) {
            guardNum += 1;
            this.saveOneData("a", selfCb);
        }
        if (guardNum == 0) {
            selfCb();
        }
    }
}
export default store