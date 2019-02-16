var path = nw.require('path');
var fs = nw.require('fs');
var basePath = path.join(window.nw.App.dataPath, "xxm");
var readFileConfig = {
    encoding: 'utf8'
}
var initData = function (name) {
    var dataStr = fs.readFileSync(path.join(basePath, name + ".data"), readFileConfig);
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