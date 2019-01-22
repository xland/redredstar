var isDev = process.versions["nw-flavor"] == "sdk"
//isProduction = false;
const winURL = isDev ? 'http://localhost:1025/':"ui/index.html";

var fs = nw.require("fs");
var path = nw.require("path");
var basePath = path.join(nw.App.dataPath, "/xxm");
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

nw.Window.open(winURL, {
    "width": 1000,
    "height": 600,
    "min_width": 1000,
    "min_height": 600,
    "frame": false
})